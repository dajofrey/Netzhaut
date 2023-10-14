// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

// INCLUDE =========================================================================================

#include "Main.h"
#include "Version.h"
#include "Header.h"
#include "Changelogs.h"
#include "Patches.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

// HELPER ==========================================================================================

static int appendToChangelog(
    NH_BYTE *path_p, NH_BYTE *commit_p)
{
    NH_BYTE *oldChangelog_p = nh_api_getFileData(path_p, NULL);
    if (!oldChangelog_p) {return 1;}

    NH_BYTE *newChangelog_p = malloc(sizeof(NH_BYTE)*(strlen(oldChangelog_p)+strlen(commit_p)+1));
    if (!newChangelog_p) {return 1;}
    memset(newChangelog_p, 0, sizeof(NH_BYTE)*(strlen(oldChangelog_p)+strlen(commit_p)+1));

    sprintf(newChangelog_p, "%s%s", commit_p, oldChangelog_p);
    if (nh_api_writeBytesToFile(path_p, newChangelog_p)) {return 1;}

    free(newChangelog_p);
    free(oldChangelog_p);

    return 0;
}

// OPTIONS =========================================================================================

static const NH_BYTE *help_p =
{
    "--help\n"
    "--tag\n"
    "--adopt <changelog path> <raw patches destination path>\n"
    "--generate_patch_pages <changelog path> <raw patches path> <pages destination path>\n"
    "--generate_html_page\n"
    "--generate_headers\n"
    "--generate_changelogs\n"
    "--show\n"
    "--show_versions\n"
};

static int help()
{
    puts("Printing help.\n");
    puts(help_p);
    return 0;
}

static int adopt(
    int argc, char **argv_pp)
{
    if (argc != 2) {return 1;}

    nh_docs_Changelog Changelog = nh_docs_parseChangelog(argv_pp[0]);
    if (!Changelog.Commits_p) {return 1;}

    // Get commit count in changelog.
    NH_BYTE command_p[255] = {0};
    NH_BYTE commitCount_p[255] = {0};
    sprintf(command_p, "git rev-list --count HEAD");
    FILE *proc_p = popen(command_p, "r");
    if (!proc_p) {return 1;}
    if (fgets(commitCount_p, 255, proc_p) == NULL) {return 1;}
    pclose(proc_p);
    int commitCount = atoi(commitCount_p);

    // Get the index of the git commit that matches the last changelog commit aka the base commit.
    int base = -1;
    for (int i = 0; i < commitCount; ++i) 
    {
        memset(command_p, 0, 255);
        sprintf(command_p, "git log --first-parent -n 1 HEAD~%d --pretty=format:%%H", i);
        proc_p = popen(command_p, "r");
        if (!proc_p) {return 1;}

        NH_BYTE id_p[255] = {0};
        if (fgets(id_p, 255, proc_p) == NULL) {return 1;}
        pclose(proc_p);

        // If true, The git commit id matches the newest changelog commit id, which means we found a common base.
        if (!strcmp(Changelog.Commits_p->id_p, id_p)) {
            base = i; 
            break;
        }
    }

    // If there is no base commit, assume new repo and use second oldest git commit.
    if (base == -1) {base = commitCount - 2;}

    // Go from base commit follow-up to newest commit and append them to changelog. Also copy patch.
    for (int i = base - 1; i >= 0; --i)
    {
        memset(command_p, 0, 255);
        sprintf(command_p, "git log --first-parent -n 1 HEAD~%d --pretty=format:%%H", i);
        proc_p = popen(command_p, "r");
        if (!proc_p) {return 1;}

        NH_BYTE id_p[255] = {0};
        if (fgets(id_p, 255, proc_p) == NULL) {return 1;}
        pclose(proc_p);

        memset(command_p, 0, 255);
        sprintf(command_p, "git tag -l --format='%(contents)' tmp_%s", id_p);
        proc_p = popen(command_p, "r");
        if (!proc_p) {return 1;}

        NH_BYTE message_p[2048] = {0};
        if (fgets(message_p, 2048, proc_p) == NULL) {return 1;}
        pclose(proc_p);
        if (appendToChangelog(argv_pp[0], message_p)) {return 1;}

        // Copy raw patch.
        memset(command_p, 0, 255);
        sprintf(command_p, "git show -m %s --pretty=format: > %s/%s.patch", id_p, argv_pp[1], id_p);
        if (system(command_p)) {return 1;}
    }

    return 0;
}

static int generateVersions(
    int argc, char **argv_pp)
{
    if (argc != 1) {return 1;}

    nh_docs_Changelog Changelog = nh_docs_parseChangelog(argv_pp[0]);

    nh_docs_ContextArray *Contexts_p = nh_docs_getContextArray(&Changelog);
    nh_docs_Context *Context_p = NULL;
    for (int i = 0; Context_p = nh_docs_getContext(Contexts_p, i); ++i) {
        if (generateVersionInAboutFile(Context_p, nh_docs_getVersion(&Changelog, Context_p, NULL))) {
            return 1;
        }
    }

    nh_docs_freeContextArray(Contexts_p);
    nh_docs_freeChangelog(&Changelog);

    return 0;
}

static int generatePatchPages(
    int argc, char **argv_pp)
{
    if (argc != 3) {return 1;}
    return createPatchPages(argv_pp[0], argv_pp[1], argv_pp[2]);
}

static int generateHTMLPage(
    int argc, char **argv_pp)
{
    if (argc != 3) {return 1;}

    NH_BYTE *contnt_p = nh_api_getFileData(argv_pp[0], NULL);
    NH_BYTE *header_p = nh_api_getFileData(argv_pp[1], NULL);
    NH_BYTE *footer_p = nh_api_getFileData(argv_pp[2], NULL);
    if (!contnt_p || !header_p || !footer_p) {return 1;}

    NH_BYTE *page_p = malloc(strlen(contnt_p)+strlen(header_p)+strlen(footer_p)+128);
    memset(page_p, 0, strlen(contnt_p)+strlen(header_p)+strlen(footer_p)+128);

    sprintf(page_p, "%s\n%s\n%s", header_p, contnt_p, footer_p);

    argv_pp[0][strlen(argv_pp[0])-3] = 0;
    nh_api_writeBytesToFile(argv_pp[0], page_p);

    return 0;
}

static int generateHeaders(
    int argc, char **argv_pp)
{
    if (argc < 1) {return 1;}

    nh_docs_Changelog Changelog = nh_docs_parseChangelog(argv_pp[0]);
 
    for (int i = 0; i < argc; ++i) {
        if (generateHeader(&Changelog, argv_pp[i])) {
            return 1;
        }
    }

    nh_docs_freeChangelog(&Changelog);

    return 0;
}

static int generateChangelogs(
    int argc, char **argv_pp)
{
    if (argc != 3) {return 1;}

    nh_docs_Changelog Changelog = nh_docs_parseChangelog(argv_pp[0]);
 
    if (generateChangelogPages(&Changelog, argv_pp[1], argv_pp[2])) {return 1;}

    nh_docs_freeChangelog(&Changelog);

    return 0;
}

static int show(
    int argc, char **argv_pp)
{
    if (argc != 1) {
        return 1;
    }

    puts("Printing changelog.\n");

    nh_docs_Changelog Changelog = nh_docs_parseChangelog(argv_pp[0]);
    nh_docs_printChangelog(&Changelog);
    nh_docs_freeChangelog(&Changelog);

    puts("");

    return 0;
}

static int showVersions(
    int argc, char **argv_pp)
{
    if (argc != 1) {
        return 1;
    }

    puts("Printing versions.\n");

    nh_docs_Changelog Changelog = nh_docs_parseChangelog(argv_pp[0]);
    nh_docs_ContextArray *Array_p = nh_docs_getContextArray(&Changelog);
    nh_docs_Context *Context_p = NULL;

    for (int j = 0; Context_p = nh_docs_getContext(Array_p, j); j++) {
        nh_docs_Version Version = nh_docs_getVersion(&Changelog, Context_p, NULL);
        NH_BYTE string_p[255];
        memset(string_p, 0, 255);
        sprintf(
            string_p, "%d %s v%ld.%ld.%ld.%ld",
            Context_p->type,  
            Context_p->name_p ? Context_p->name_p : "(default)",
            Version.api, Version.major, Version.minor, Version.patch
        );
        puts(string_p);
    }

    nh_docs_freeContextArray(Array_p);
    nh_docs_freeChangelog(&Changelog);

    puts("");

    return 0;
}

static int tag(
    int argc, char **argv_pp) 
{
    int pid = getppid();
    NH_TTY_RESULT standalone = nh_api_sendCommandToShell(pid, NH_TTY_PROGRAM_TAGGER);

    if (standalone) {
        nh_tty_TTY *TTY_p = nh_api_openTTY(NULL);
        if (!TTY_p) {return 1;}
    
        if (nh_api_claimStandardIO(TTY_p)) {return 1;}
        if (nh_api_addProgram(TTY_p, NH_TTY_PROGRAM_TAGGER, NH_TRUE) != NH_TTY_SUCCESS) {return 1;}
    
        while (nh_api_getWorkload(TTY_p)) {
            if (!nh_api_run()) {usleep(10000);}
            if (!nh_api_keepRunning()) {break;}
        }
    
        nh_api_unclaimStandardIO(TTY_p);
//        nh_api_closeTTY(TTY_p);
    }
}

// MAIN ============================================================================================

static int execute(
    int argc, char **argv_pp) 
{

#define ARGS(argv) argc ? argv_pp+1 : NULL

    int args = 0;
    for (int i = 1; i < argc && argv_pp[i][0] != '-'; ++i) {
        ++args;
    }

    if (!strcmp(argv_pp[0]+2, "help")) {
        return help();
    }
    else if (!strcmp(argv_pp[0]+2, "tag")) {
        return tag(args, ARGS(argv_pp));
    }
    else if (!strcmp(argv_pp[0]+2, "adopt")) {
        return adopt(args, ARGS(argv_pp));
    }
    else if (!strcmp(argv_pp[0]+2, "generate_patch_pages")) {
        return generatePatchPages(args, ARGS(argv_pp));
    }
    else if (!strcmp(argv_pp[0]+2, "generate_html_page")) {
        return generateHTMLPage(args, ARGS(argv_pp));
    }
    else if (!strcmp(argv_pp[0]+2, "generate_headers")) {
        return generateHeaders(args, ARGS(argv_pp));
    }
    else if (!strcmp(argv_pp[0]+2, "generate_changelogs")) {
        return generateChangelogs(args, ARGS(argv_pp));
    }
    else if (!strcmp(argv_pp[0]+2, "show")) {
        return show(args, ARGS(argv_pp));
    }
    else if (!strcmp(argv_pp[0]+2, "show_versions")) {
        return showVersions(args, ARGS(argv_pp));
    }
 
    return 1;
}

int main(
    int argc, char **argv_pp)
{
    if (nh_api_initialize(NH_LOADER_SCOPE_SYSTEM, NULL, NULL, 0) != NH_CORE_SUCCESS) {return 1;}

    NH_BOOL forceHelp = NH_TRUE;

    for (int i = 1; i < argc; ++i) {
        if (argv_pp[i][0] == '-' && argv_pp[i][1] == '-') {
            if (execute(argc-i, argv_pp+i)) {return 1;}
            forceHelp = NH_FALSE;
        }
    }

    if (forceHelp) {return help();}

    return nh_api_terminate();
}
