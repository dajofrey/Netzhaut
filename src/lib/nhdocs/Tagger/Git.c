// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Git.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

// HELPER ==========================================================================================

static NH_BOOL nh_docs_isMergeCommit(
    NH_BYTE *id_p)
{
NH_DOCS_BEGIN()

    NH_BYTE result_p[255];
    NH_BYTE command_p[255];
    memset(command_p, 0, 255);
    memset(result_p, 0, 255);

    sprintf(command_p, "git show --no-patch --format=\"%%P\" %s", id_p);
    FILE *proc_p = popen(command_p, "r");
    if (!proc_p) {return 1;}
    if (fgets(result_p, 255, proc_p) == NULL) {return 1;}
    pclose(proc_p);

NH_DOCS_END(strlen(result_p)>50)
}

static NH_DOCS_RESULT nh_docs_getId(
    NH_BYTE *indexOrId_p, NH_BYTE *id_p)
{
NH_DOCS_BEGIN()

    if (strlen(indexOrId_p) == 40) {
        memset(id_p, 0, 255);
        memcpy(id_p, indexOrId_p, 40);
    } else {
        NH_BYTE command_p[255];
        memset(command_p, 0, 255);
        sprintf(command_p, "git log -a -1 HEAD~%d --pretty=format:%%H", atoi(indexOrId_p));
        FILE *proc_p = popen(command_p, "r");
        if (!proc_p) {return 1;}
        if (fgets(id_p, 255, proc_p) == NULL) {return 1;}
        pclose(proc_p);
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}
 
static nh_docs_Diff *nh_docs_getDiffs(
    int *diffs_p, NH_BYTE *indexOrId_p)
{
NH_DOCS_BEGIN()

    NH_BYTE id_p[255] = {0};
    nh_docs_getId(indexOrId_p, id_p);

    NH_BYTE command_p[1024] = {0};
    sprintf(command_p, "git show -m --name-only --pretty=format: %s", id_p);

    FILE *proc_p = popen(command_p, "r");
    NH_DOCS_CHECK_NULL_2(NULL, proc_p)

    int diffs = 0;
    nh_docs_Diff *Diffs_p = nh_core_allocate(sizeof(nh_docs_Diff)*16);
    NH_DOCS_CHECK_NULL_2(NULL, Diffs_p)

    NH_BYTE result_p[255] = {0x0};

    while (fgets(result_p, sizeof(result_p), proc_p) != NULL) {
        nh_docs_Diff *Diff_p = &Diffs_p[diffs];
        memset(Diff_p, 0, sizeof(nh_docs_Diff));

        strcpy(Diff_p->name_p, result_p);
        for (int i = 0; i < strlen(Diff_p->name_p); ++i) {
            if (Diff_p->name_p[i] == '\n') {Diff_p->name_p[i] = 0;}
        }

        // Get diff.
        sprintf(command_p, "git show -m %s --pretty=format: %s", id_p, Diff_p->name_p);

        FILE *proc2_p = popen(command_p, "r");
        NH_DOCS_CHECK_NULL_2(NULL, proc2_p)

        while (fgets(result_p, sizeof(result_p)-1, proc2_p) != NULL) {
            if (!Diff_p->lines_pp) {
                Diff_p->lines_pp = nh_core_allocate(sizeof(NH_BYTE*));
                NH_DOCS_CHECK_NULL_2(NULL, Diff_p->lines_pp)
                Diff_p->lines_pp[0] = nh_core_allocate(255);
                NH_DOCS_CHECK_NULL_2(NULL, Diff_p->lines_pp[0])
                memset(Diff_p->lines_pp[0], 0, 255);
            } else {
                Diff_p->lines_pp = nh_core_reallocate(Diff_p->lines_pp, sizeof(NH_BYTE*)*(Diff_p->lines+1));
                NH_DOCS_CHECK_NULL_2(NULL, Diff_p->lines_pp)
                Diff_p->lines_pp[Diff_p->lines] = nh_core_allocate(255);
                NH_DOCS_CHECK_NULL_2(NULL, Diff_p->lines_pp[Diff_p->lines])
                memset(Diff_p->lines_pp[Diff_p->lines], 0, 255);
            }
            strcpy(Diff_p->lines_pp[Diff_p->lines++], result_p);
        } 

        pclose(proc2_p);

        diffs++;
        Diffs_p = nh_core_reallocate(Diffs_p, sizeof(nh_docs_Diff)*(16+diffs));
        NH_DOCS_CHECK_NULL_2(NULL, Diffs_p)
    }

    pclose(proc_p);

    *diffs_p = diffs;

NH_DOCS_END(Diffs_p)
}

static nh_docs_Context *nh_docs_getContextsWithoutDiffs(
    int *contexts_p)
{
NH_DOCS_BEGIN()

    nh_make_initialize();

    nh_make_Runtime *Runtime_p = nh_make_createRuntime("nhmake");

    NH_BYTE *procDir_p = nh_make_getProcessDirectory();
    nh_make_setVariable(Runtime_p, "WRK_DIR", &procDir_p, 1);
    nh_make_setQuiet(Runtime_p, NH_MAKE_TRUE);

    nh_make_addFile(Runtime_p, "../build/.nhmake/source.nhmake");

    nh_make_run(Runtime_p, 0, NULL);
    while (nh_make_isRunning()) {nh_make_sleepMs(100);}

    nh_make_SourceContextArray *SourceContexts_p = nh_make_getSourceContextArray(Runtime_p);
    nh_docs_Context *Contexts_p = nh_core_allocate(sizeof(nh_docs_Context)*SourceContexts_p->length);
    NH_DOCS_CHECK_MEM_2(NULL, Contexts_p)

    for (int i = 0; i < SourceContexts_p->length; ++i) {
        memset(&Contexts_p[i], 0, sizeof(nh_docs_Context));
        switch (SourceContexts_p->SourceContexts_p[i].type) {
            case NH_MAKE_SOURCE_CONTEXT_BINARY :
                Contexts_p[i].type = NH_DOCS_CONTEXT_BINARY;
                break;
            case NH_MAKE_SOURCE_CONTEXT_UNDEFINED :
                Contexts_p[i].type = NH_DOCS_CONTEXT_UNDEFINED;
                break;
            default : 
                Contexts_p[i].type = NH_DOCS_CONTEXT_LIBRARY;
                break;
        }
        if (Contexts_p[i].type != NH_DOCS_CONTEXT_UNDEFINED) {
            strcpy(Contexts_p[i].name_p, SourceContexts_p->SourceContexts_p[i].name_p);
            strcpy(Contexts_p[i].path_p, SourceContexts_p->SourceContexts_p[i].path_p);
        }
    }

    *contexts_p = SourceContexts_p->length;

    nh_make_destroyRuntime(Runtime_p);
    nh_make_terminate();

NH_DOCS_END(Contexts_p)
}

nh_docs_Context *nh_docs_getContextsUsingGit(
    int *count_p, NH_BYTE *indexOrId_p)
{
NH_DOCS_BEGIN()

    int diffs = 0;
    nh_docs_Diff *Diffs_p = nh_docs_getDiffs(&diffs, indexOrId_p);
    NH_DOCS_CHECK_MEM_2(NULL, Diffs_p)

    int contexts = 0;
    nh_docs_Context *Contexts_p = nh_docs_getContextsWithoutDiffs(&contexts);
    NH_DOCS_CHECK_MEM_2(NULL, Contexts_p)

    // Add diffs to contexts.
    for (int i = 0; i < contexts; ++i) {
        nh_docs_Context *Context_p = Contexts_p+i;

        if (Context_p->type == NH_DOCS_CONTEXT_UNDEFINED) {continue;} // TODO

        for (int j = 0; j < diffs; ++j) {
            if (strstr(Diffs_p[j].name_p, Context_p->path_p)) {
                if (!Context_p->Diffs_pp) {
                    Context_p->Diffs_pp = nh_core_allocate(sizeof(nh_docs_Diff*));
                    NH_DOCS_CHECK_MEM_2(NULL, Context_p->Diffs_pp)
                } else {
                    Context_p->Diffs_pp = nh_core_reallocate(Context_p->Diffs_pp, sizeof(nh_docs_Diff*)*(Context_p->diffs+1));
                    NH_DOCS_CHECK_MEM_2(NULL, Context_p->Diffs_pp)
                }
                Context_p->Diffs_pp[Context_p->diffs++] = Diffs_p+j;
                sprintf(Diffs_p[j].name_p, Diffs_p[j].name_p + strlen(Context_p->path_p) + 1);
            }
        }

    }

    *count_p = contexts;

    // TODO Free diffs.

NH_DOCS_END(Contexts_p)
}

static NH_DOCS_RESULT nh_docs_getLine(
    NH_BYTE *command_p, NH_BYTE *line_p, int size)
{
NH_DOCS_BEGIN()

    FILE *proc_p = popen(command_p, "r");
    NH_DOCS_CHECK_MEM(proc_p)
    if (fgets(line_p, size, proc_p) == NULL) {NH_DOCS_DIAGNOSTIC_END(NH_DOCS_ERROR_BAD_STATE)}
    pclose(proc_p);

NH_DOCS_DIAGNOSTIC_END(NH_DOCS_SUCCESS)
}

nh_docs_Commit *nh_docs_createCommitUsingGit(
    NH_BYTE *indexOrId_p)
{
NH_DOCS_BEGIN()

    nh_docs_Commit *Commit_p = nh_core_allocate(sizeof(nh_docs_Commit));
    NH_DOCS_CHECK_MEM_2(NULL, Commit_p)

    memset(Commit_p, 0, sizeof(nh_docs_Commit));

    NH_BYTE id_p[255] = {0};
    nh_docs_getId(indexOrId_p, id_p);
    NH_BYTE command_p[1024] = {0};
    memset(command_p, 0, 1024);

    sprintf(command_p, "git show %s -s --pretty=format:%%H", id_p);
    NH_DOCS_CHECK_2(NULL, nh_docs_getLine(command_p, (NH_BYTE*)Commit_p->id_p, NH_DOCS_MAX_COMMIT_ID))
    sprintf(command_p, "git show %s -s --pretty=format:%%aI", id_p);
    NH_DOCS_CHECK_2(NULL, nh_docs_getLine(command_p, (NH_BYTE*)Commit_p->date_p, NH_DOCS_MAX_COMMIT_DATE))
    sprintf(command_p, "git show %s -s --pretty=format:%%an", id_p);
    NH_DOCS_CHECK_2(NULL, nh_docs_getLine(command_p, (NH_BYTE*)Commit_p->Author.name_p, NH_DOCS_MAX_AUTHOR_NAME))
    sprintf(command_p, "git show %s -s --pretty=format:%%ae", id_p);
    NH_DOCS_CHECK_2(NULL, nh_docs_getLine(command_p, (NH_BYTE*)Commit_p->Author.contact_p, NH_DOCS_MAX_AUTHOR_CONTACT))

    // Get merge ids if any.
    if (nh_docs_isMergeCommit(id_p)) {
        NH_BYTE line_p[1024] = {0};
        memset(line_p, 0, 1024);
        sprintf(command_p, "git show --no-patch --format=\"%%P\" %s", id_p);
        FILE *proc_p = popen(command_p, "r");
        NH_DOCS_CHECK_MEM_2(NULL, proc_p)
        if (fgets(line_p, 1024, proc_p) == NULL) {NH_DOCS_END(NULL)}
        pclose(proc_p);
        NH_BYTE *p = line_p;
        for (int i = 0; i < strlen(line_p); ++i) {
            if (line_p[i] == ' ' || i == strlen(line_p)-1) {
                if (!Commit_p->Merge.ids_pp) {
                    Commit_p->Merge.ids_pp = nh_core_allocate(sizeof(NH_BYTE*));
                    NH_DOCS_CHECK_MEM_2(NULL, Commit_p->Merge.ids_pp)
                } else {
                    Commit_p->Merge.ids_pp = 
                        nh_core_reallocate(Commit_p->Merge.ids_pp, sizeof(NH_BYTE*)*(Commit_p->Merge.ids+1));
                    NH_DOCS_CHECK_MEM_2(NULL, Commit_p->Merge.ids_pp)
                }
                Commit_p->Merge.ids_pp[Commit_p->Merge.ids] = nh_core_allocate(sizeof(NH_BYTE)*41);
                NH_DOCS_CHECK_MEM_2(NULL, Commit_p->Merge.ids_pp[Commit_p->Merge.ids])
                memset(Commit_p->Merge.ids_pp[Commit_p->Merge.ids], 0, 41);
                memcpy(Commit_p->Merge.ids_pp[Commit_p->Merge.ids], p, 40);
                Commit_p->Merge.ids++;
                p = line_p+i+1;
            }
        }
    }

    // Get commit message.
    sprintf(command_p, "git show %s -s --pretty=format:%%B", id_p);
    FILE *proc_p = popen(command_p, "r");
    NH_DOCS_CHECK_MEM_2(NULL, proc_p)
    
    NH_BYTE line_p[255] = {0};
    while (fgets(line_p, 255, proc_p)) {
        for (int i = 0; i < strlen(line_p); ++i) {
            if (line_p[i] == '\n') {line_p[i] = 0;}
        }

        if (!Commit_p->Message.lines_pp) {
            Commit_p->Message.lines_pp = nh_core_allocate(sizeof(NH_BYTE*));
            NH_DOCS_CHECK_MEM_2(NULL, Commit_p->Message.lines_pp)
        } else {
            Commit_p->Message.lines_pp = 
                nh_core_reallocate(Commit_p->Message.lines_pp, sizeof(NH_BYTE*)*(Commit_p->Message.lines+1));
            NH_DOCS_CHECK_MEM_2(NULL, Commit_p->Message.lines_pp)
        }
 
        if (strlen(line_p)) {
            Commit_p->Message.lines_pp[Commit_p->Message.lines] = 
                nh_core_allocate(sizeof(NH_BYTE)*strlen(line_p)+1);
            NH_DOCS_CHECK_MEM_2(NULL, Commit_p->Message.lines_pp[Commit_p->Message.lines])
            strcpy(Commit_p->Message.lines_pp[Commit_p->Message.lines], line_p);
        }
        else {Commit_p->Message.lines_pp[Commit_p->Message.lines] = NULL;}
                
        Commit_p->Message.lines++;
    }

    pclose(proc_p);

NH_DOCS_END(Commit_p)
}

