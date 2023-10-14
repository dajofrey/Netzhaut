// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

// INCLUDE =========================================================================================

#include "Changelogs.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// CHANGELOGS ======================================================================================

static NH_BYTE *writeChangelog(
    nh_docs_Changelog *Changelog_p, nh_docs_Commit *Commit_p, nh_docs_Context *Context_p, 
    NH_BYTE *data_p) 
{
    NH_BOOL init = data_p == NULL;

    int length = 4096;
    if (Commit_p->date_p) {length += strlen(Commit_p->date_p);}
    if (Commit_p->id_p) {length += strlen(Commit_p->id_p);}
    if (data_p) {length += strlen(data_p);}

    if (!data_p) {
        data_p = malloc(sizeof(NH_BYTE) * length);
        if (!data_p) {return NULL;}
        memset(data_p, 0, length);
    } else {
        data_p = realloc(data_p, strlen(data_p)+length);
        if (!data_p) {return NULL;}
        memset(data_p+strlen(data_p), 0, length);
    }

    if (init) {
        sprintf(data_p+strlen(data_p), "\\page %s %s\n\n", Context_p->name_p ? Context_p->name_p : "undefined", 
            Context_p->name_p ? Context_p->name_p : "(undefined)");
        sprintf(data_p+strlen(data_p), "<div style=\"max-width:700px;\">\n\n");
    }

    // Set version.
    nh_docs_Version Version = nh_docs_getVersion(Changelog_p, Context_p, Commit_p);
    sprintf(data_p+strlen(data_p), "\\section %s%d%d%d%d ver.%d.%d.%d.%d", 
        Context_p->name_p ? Context_p->name_p : "undefined", Version.api, 
        Version.major, Version.minor, Version.patch, Version.api, Version.major, Version.minor, Version.patch);

    // Set date.
    if (Commit_p->date_p) {sprintf(data_p+strlen(data_p), "\n\n%s", Commit_p->date_p);}

    // Set revision.
    nh_docs_Version Revision = nh_docs_getRevision(Changelog_p, Commit_p);
    sprintf(data_p+strlen(data_p), " | rev.%d.%d.%d.%d", Revision.api, Revision.major, Revision.minor, Revision.patch);

    // Set commit id.
    if (Commit_p->id_p) {
        sprintf(data_p+strlen(data_p), " | [open patch](../../patches/html/md_pages_%s.html)", Commit_p->id_p);
    }

    sprintf(data_p+strlen(data_p), "\n\n ---");
    sprintf(data_p+strlen(data_p), "<br>");

    return data_p;
}

static NH_BYTE *writeChangelogIndex(
    nh_docs_Changelog *Changelog_p, nh_docs_ContextArray *ContextArray_p)
{
    int length = 8192;

    NH_BYTE *data_p = malloc(sizeof(NH_BYTE)*length);
    if (!data_p) {return NULL;}
    memset(data_p, 0, length);
    sprintf(data_p, "<div style=\"max-width:700px;\">\n\n");

    for (int i = 0; i < Changelog_p->commits; ++i) 
    {
        nh_docs_Commit *Commit_p = Changelog_p->Commits_p+i;

        data_p = realloc(data_p, strlen(data_p)+length);
        if (!data_p) {return NULL;}
        memset(data_p+strlen(data_p), 0, length); 

        nh_docs_Version Revision = nh_docs_getRevision(Changelog_p, Commit_p);
        sprintf(data_p+strlen(data_p), "<h1>rev.%d.%d.%d.%d</h1>\n\n", Revision.api, Revision.major, Revision.minor, Revision.patch);

        if (Commit_p->date_p) {sprintf(data_p+strlen(data_p), "%s ", Commit_p->date_p);}
        if (Commit_p->id_p) {
            sprintf(data_p+strlen(data_p), 
                " | [open patch](../../patches/html/md_pages_%s.html)", Commit_p->id_p);
        }
        sprintf(data_p+strlen(data_p), "\n\n ---\n");

        int links = 0;

        for (int j = 0; j < Commit_p->workloads; ++j) 
        {
            nh_docs_Workload *Workload_p = Commit_p->Workloads_p+j;
            NH_BOOL libs = NH_FALSE;

            for (int k = 0; k < Workload_p->libs; ++k) {
                nh_docs_Context *Context_p = 
                    nh_docs_getContextFromDescription(ContextArray_p, Workload_p->libs_pp[k], NH_DOCS_CONTEXT_LIBRARY);
                if (!Context_p) {continue;}
                if (links) {sprintf(data_p+strlen(data_p), ", ");}
                nh_docs_Version Version = nh_docs_getVersion(Changelog_p, Context_p, Commit_p);
                sprintf(data_p+strlen(data_p), "[%s](md_pages_%s_lib.html#%s%d%d%d%d)", 
                    Workload_p->libs_pp[k], Workload_p->libs_pp[k], Workload_p->libs_pp[k], 
                    Version.api, Version.major, Version.minor, Version.patch);
                links++;
            }
            for (int k = 0; k < Workload_p->bins; ++k) {
                if (libs) {sprintf(data_p+strlen(data_p), ", ");} 
                nh_docs_Context *Context_p = 
                    nh_docs_getContextFromDescription(ContextArray_p, Workload_p->bins_pp[k], NH_DOCS_CONTEXT_BINARY);
                if (!Context_p) {continue;}
                if (links) {sprintf(data_p+strlen(data_p), ", ");}
                nh_docs_Version Version = nh_docs_getVersion(Changelog_p, Context_p, Commit_p);
                sprintf(data_p+strlen(data_p), "[%s](md_pages_%s_bin.html#%s%d%d%d%d)",
                    Workload_p->bins_pp[k], Workload_p->bins_pp[k], Workload_p->bins_pp[k],
                    Version.api, Version.major, Version.minor, Version.patch);
                links++;
            }
        }

//       if (!links) {
//            nh_docs_Context *Context_p = 
//                nh_docs_getSourceContext(Changelog_p, NULL, 0);
//            nh_docs_Version Version = nh_docs_getVersion(Changelog_p, Context_p, Commit_p);
//            sprintf(data_p+strlen(data_p), "[(undefined)](md_pages_undefined.html#undefined%d%d%d%d)", 
//                Version.api, Version.major, Version.minor, Version.patch);
//        }

        sprintf(data_p+strlen(data_p), "<br>");
//        sprintf(data_p+strlen(data_p), "%s<br><br>", Commit_p->summary_p);
    }

    sprintf(data_p+strlen(data_p), "\n</div>");

    return data_p;
}

int generateChangelogPages(
    nh_docs_Changelog *Changelog_p, NH_BYTE *main_p, NH_BYTE *pages_p)
{
    nh_docs_ContextArray *ContextArray_p = nh_docs_getContextArray(Changelog_p);
    if (!ContextArray_p) {return 1;}

    NH_BYTE *data_p = writeChangelogIndex(Changelog_p, ContextArray_p);
    if (!data_p) {return 1;}

    nh_api_writeBytesToFile(main_p, data_p);
    free(data_p);
    data_p = NULL;
    int i = 0;

    for (nh_docs_Context *Context_p = NULL; Context_p = nh_docs_getContext(ContextArray_p, i); ++i)
    {
        nh_docs_ChangelogPart Part = 
            nh_docs_getChangelogForContext(Changelog_p, *Context_p);
        data_p = NULL;

        for (int j = 0; j < Part.commits; ++j) {
            data_p = writeChangelog(Changelog_p, Part.Commits_pp[j], Context_p, data_p);
            if (!data_p) {return 1;}
        }

	nh_docs_freeChangelogPart(&Part);

        if (data_p) {sprintf(data_p+strlen(data_p), "</div>");}

        NH_BYTE path_p[255];
        memset(path_p, 0, 255);

        if (Context_p->name_p) {
            sprintf(path_p, "%s/%s_%s.md", pages_p, Context_p->name_p, 
                Context_p->type == NH_DOCS_CONTEXT_BINARY ? "bin" : "lib");
        }
        else {sprintf(path_p, "%s/undefined.md", pages_p);}

        if (data_p) {
            nh_api_writeBytesToFile(path_p, data_p);
            free(data_p);
        }
    }

    nh_docs_freeContextArray(ContextArray_p);

    return 0;
}

