// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Changelog.h"
#include "Parser.h"
#include "Version.h"

#include "../../nhmake/Parser/Variables.h"
#include "../../nhmake/Parser/Tokenizer.h"
#include "../../nhmake/Core/File.h"

#include "../Common/Macros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

// PRINT ===========================================================================================

NH_DOCS_RESULT nh_docs_printChangelog(
    nh_docs_Changelog *Changelog_p)
{
NH_DOCS_BEGIN()

    for (int j = 0; j < Changelog_p->commits; ++j) 
    {
        nh_docs_Commit *Commit_p = Changelog_p->Commits_p+j;
        puts("- commit");

        printf("  | id   : \"%s\"\n", Commit_p->id_p);
        printf("  | date : \"%s\"\n", Commit_p->date_p);

        puts("  | author");
 
        printf("    | name    : \"%s\"\n", Commit_p->Author.name_p);
        printf("    | contact : \"%s\"\n", Commit_p->Author.contact_p);
 
        for (int i = 0; i < Commit_p->workloads; ++i) 
        {
            nh_docs_Workload *Workload_p = Commit_p->Workloads_p+i;
            puts("  | workload");
            printf("    | scope : \"%s\"\n", nh_docs_getScopeString(Workload_p->scope));

            for (int k = 0; k < Workload_p->libs; ++k) {
                if (k == 0) {printf("    | libs  : ");}
                printf("\"%s\" ", Workload_p->libs_pp[k]);
                if (k + 1 == Workload_p->libs) {printf("\n");};
            }
            for (int k = 0; k < Workload_p->bins; ++k) {
                if (k == 0) {printf("    | bins  : ");}
                printf("\"%s\" ", Workload_p->bins_pp[k]);
                if (k + 1 == Workload_p->bins) {printf("\n");};
            }
        }
    }

NH_DOCS_DIAGNOSTIC_END(NH_DOCS_SUCCESS)
}

// STRINGIFY =======================================================================================

static NH_DOCS_RESULT nh_docs_stringifyAuthor(
    nh_docs_Author *Author_p, NH_BYTE *changelog_p)
{
NH_DOCS_BEGIN()

    sprintf(changelog_p+strlen(changelog_p), "author{");
    sprintf(changelog_p+strlen(changelog_p), "name:\\\"%s\\\"", Author_p->name_p);
    sprintf(changelog_p+strlen(changelog_p), "contact:\\\"%s\\\"", Author_p->contact_p);
    strcpy(changelog_p+strlen(changelog_p), "}");
 
NH_DOCS_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_stringifyMessage(
    nh_docs_Message *Message_p, NH_BYTE *changelog_p)
{
NH_DOCS_BEGIN()

    for (int i = 0; i < Message_p->lines; ++i) {
        if (i) {strcpy(changelog_p+strlen(changelog_p), ",");}
        else {strcpy(changelog_p+strlen(changelog_p), "message:");}
        if (Message_p->lines_pp[i] != NULL) {
            sprintf(changelog_p+strlen(changelog_p), "\\\"%s\\\"", Message_p->lines_pp[i]);
        } else {
            sprintf(changelog_p+strlen(changelog_p), "\\\"\\\"");
        }
    }
 
NH_DOCS_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_stringifyMerge(
    nh_docs_Merge *Merge_p, NH_BYTE *changelog_p)
{
NH_DOCS_BEGIN()

    for (int i = 0; i < Merge_p->ids; ++i) {
        if (i) {strcpy(changelog_p+strlen(changelog_p), ",");}
        else {strcpy(changelog_p+strlen(changelog_p), "merge:");}
        if (Merge_p->ids_pp[i] != NULL) {
            sprintf(changelog_p+strlen(changelog_p), "\\\"%s\\\"", Merge_p->ids_pp[i]);
        } else {
            sprintf(changelog_p+strlen(changelog_p), "\\\"\\\"");
        }
    }
 
NH_DOCS_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_stringifyWorkload(
    nh_docs_Workload *Workload_p, NH_BYTE *changelog_p)
{
NH_DOCS_BEGIN()

    sprintf(changelog_p+strlen(changelog_p), "workload{");
    sprintf(changelog_p+strlen(changelog_p), "scope:\\\"%s\\\"", nh_docs_getScopeString(Workload_p->scope));

    for (int i = 0; i < Workload_p->libs; ++i) {
        if (i) {strcpy(changelog_p+strlen(changelog_p), ",");}
        else {strcpy(changelog_p+strlen(changelog_p), "lib:");}
        sprintf(changelog_p+strlen(changelog_p), "\\\"%s\\\"", Workload_p->libs_pp[i]);
    }
    for (int i = 0; i < Workload_p->bins; ++i) {
        if (i) {strcpy(changelog_p+strlen(changelog_p), ",");}
        else {strcpy(changelog_p+strlen(changelog_p), "bin:");}
        sprintf(changelog_p+strlen(changelog_p), "\\\"%s\\\"", Workload_p->bins_pp[i]);
    }

    NH_DOCS_CHECK(nh_docs_stringifyMessage(&Workload_p->Message, changelog_p))
 
    strcpy(changelog_p+strlen(changelog_p), "}");

NH_DOCS_END(NH_DOCS_SUCCESS)
}

NH_DOCS_RESULT nh_docs_stringifyChangelog(
    nh_docs_Changelog *Changelog_p, NH_BYTE *result_p, int length)
{
NH_DOCS_BEGIN()

    memset(result_p, 0, length);

    for (int j = 0; j < Changelog_p->commits; ++j) 
    {
        sprintf(result_p+strlen(result_p), "commit{");
        sprintf(result_p+strlen(result_p), "date:\\\"%s\\\"", Changelog_p->Commits_p[j].date_p);
        sprintf(result_p+strlen(result_p), "id:\\\"%s\\\"", Changelog_p->Commits_p[j].id_p);

        NH_DOCS_CHECK(nh_docs_stringifyAuthor(&Changelog_p->Commits_p[j].Author, result_p))
        NH_DOCS_CHECK(nh_docs_stringifyMerge(&Changelog_p->Commits_p[j].Merge, result_p))
        NH_DOCS_CHECK(nh_docs_stringifyMessage(&Changelog_p->Commits_p[j].Message, result_p))

        for (int i = 0; i < Changelog_p->Commits_p[j].workloads; ++i) {
            NH_DOCS_CHECK(nh_docs_stringifyWorkload(Changelog_p->Commits_p[j].Workloads_p+i, result_p))
        }

        strcpy(result_p+strlen(result_p), "}");
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}

NH_DOCS_RESULT nh_docs_stringifyCommit(
    nh_docs_Commit *Commit_p, NH_BYTE *commit_p, int length)
{
NH_DOCS_BEGIN()

    nh_docs_Changelog Changelog;
    Changelog.Commits_p = Commit_p;
    Changelog.commits = 1;

NH_DOCS_END(nh_docs_stringifyChangelog(&Changelog, commit_p, length))
}

// CHANGELOG =======================================================================================

NH_DOCS_RESULT nh_docs_freeChangelog(
    nh_docs_Changelog *Changelog_p)
{
NH_DOCS_BEGIN()

    // TODO Implement.

NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)
}

nh_docs_Changelog nh_docs_parseChangelog(
    NH_BYTE *path_p)
{
NH_DOCS_BEGIN()

    nh_docs_Changelog Changelog;
    Changelog.Commits_p = NULL;
    Changelog.commits = 0;

    nh_docs_CommitNode *Root_p = nh_docs_parseCommits(path_p);
    if (!Root_p) {NH_DOCS_END(Changelog)}

    int commits = 0;
    nh_docs_Commit *Commits_p = nh_docs_getCommits(Root_p, &commits);
    if (!Commits_p) {
        // TODO Fix memory leak.
        NH_DOCS_END(Changelog)
    }

    Changelog.Commits_p = Commits_p;
    Changelog.commits = commits;

NH_DOCS_END(Changelog)
}

// CHANGELOG PART ==================================================================================

static NH_DOCS_RESULT nh_docs_addToChangelogPart(
    nh_docs_Commit *Commit_p, nh_docs_ChangelogPart *Part_p)
{
NH_DOCS_BEGIN()

    if (!Part_p->Commits_pp) {
        Part_p->Commits_pp = malloc(sizeof(nh_docs_Commit*));
        if (!Part_p->Commits_pp) {return NH_DOCS_ERROR_BAD_STATE;}
        Part_p->Commits_pp[0] = Commit_p;
	Part_p->commits++;
    }
    else {
        Part_p->Commits_pp = realloc(Part_p->Commits_pp, sizeof(nh_docs_Commit*)*(Part_p->commits+1));
        if (!Part_p->Commits_pp) {return NH_DOCS_ERROR_BAD_STATE;}
        Part_p->Commits_pp[Part_p->commits] = Commit_p;
        Part_p->commits++;
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}

nh_docs_ChangelogPart nh_docs_getChangelogForContext(
   nh_docs_Changelog *Changelog_p, nh_docs_Context Context)
{
NH_DOCS_BEGIN()

    nh_docs_ChangelogPart Part;
    Part.Commits_pp = NULL;
    Part.commits = 0;

    for (int i = 0; i < Changelog_p->commits; ++i) {
        for (int j = 0; j < Changelog_p->Commits_p[i].workloads; ++j) {
            if (!Context.name_p) {
                if (!Changelog_p->Commits_p[i].Workloads_p[j].libs 
                &&  !Changelog_p->Commits_p[i].Workloads_p[j].bins) {
                    nh_docs_addToChangelogPart(Changelog_p->Commits_p+i, &Part);
                    break;
                }
                continue;
            }
            for (int k = 0; k < Changelog_p->Commits_p[i].Workloads_p[j].libs; ++k) {
                if (!strcmp(Changelog_p->Commits_p[i].Workloads_p[j].libs_pp[k], Context.name_p) 
                &&  Context.type != NH_DOCS_CONTEXT_BINARY) {
                    nh_docs_addToChangelogPart(Changelog_p->Commits_p+i, &Part);
                    break;
                }
            }
            for (int k = 0; k < Changelog_p->Commits_p[i].Workloads_p[j].bins; ++k) {
                if (!strcmp(Changelog_p->Commits_p[i].Workloads_p[j].bins_pp[k], Context.name_p) 
                &&  Context.type == NH_DOCS_CONTEXT_BINARY) {
                    nh_docs_addToChangelogPart(Changelog_p->Commits_p+i, &Part);
                    break;
                }
            }
        }
    }

NH_DOCS_END(Part)
}

NH_DOCS_RESULT nh_docs_freeChangelogPart(
    nh_docs_ChangelogPart *Part_p)
{
NH_DOCS_BEGIN()

    if (Part_p->Commits_pp) {
        free(Part_p->Commits_pp);
    }
    Part_p->commits = 0;

NH_DOCS_END(NH_DOCS_SUCCESS)
}

