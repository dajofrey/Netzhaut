// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Version.h"
#include "Changelog.h"
#include "Context.h"

#include "../Common/Macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// SCOPE ===========================================================================================

NH_DOCS_SCOPE_E nh_docs_getScopeValue(
    NH_BYTE *scope_p)
{
NH_DOCS_BEGIN()

    if (!strcmp(scope_p, "patch")) {
        NH_DOCS_END(NH_DOCS_SCOPE_PATCH)
    }
    else if (!strcmp(scope_p, "minor")) {
        NH_DOCS_END(NH_DOCS_SCOPE_MINOR)
    }
    else if (!strcmp(scope_p, "major")) {
        NH_DOCS_END(NH_DOCS_SCOPE_MAJOR)
    }
    else if (!strcmp(scope_p, "api")) {
        NH_DOCS_END(NH_DOCS_SCOPE_API)
    }

NH_DOCS_END(0)
}

NH_BYTE *nh_docs_getScopeString(
    NH_DOCS_SCOPE_E scope)
{
NH_DOCS_BEGIN()

    static NH_BYTE scopes_pp[4][6] = {
        "patch", "minor", "major", "api"
    };

NH_DOCS_END(scopes_pp[scope])
}

// VERSION =========================================================================================

static nh_docs_Version nh_docs_initVersion()
{
NH_DOCS_BEGIN()

    nh_docs_Version Version;

    Version.api = 0;
    Version.major = 0;
    Version.minor = 0;
    Version.patch = 0;

NH_DOCS_END(Version)
}

static NH_DOCS_RESULT nh_docs_incrementVersion(
    NH_DOCS_SCOPE_E scope, nh_docs_Version *Version_p)
{
NH_DOCS_BEGIN()

    switch (scope)
    {
        case NH_DOCS_SCOPE_API :
            Version_p->api++;
            Version_p->major = 0;
            Version_p->minor = 0;
            Version_p->patch = 0;
            break;
        case NH_DOCS_SCOPE_MAJOR :
            Version_p->major++;
            Version_p->minor = 0;
            Version_p->patch = 0;
            break;
        case NH_DOCS_SCOPE_MINOR :
            Version_p->minor++;
            Version_p->patch = 0;
            break;
        case NH_DOCS_SCOPE_PATCH :
            Version_p->patch++;
            break;
    }

NH_DOCS_DIAGNOSTIC_END(NH_DOCS_SUCCESS)
}

nh_docs_Version nh_docs_getVersion(
    nh_docs_Changelog *Changelog_p, nh_docs_Context *Context_p, nh_docs_Commit *LastCommit_p)
{
NH_DOCS_BEGIN()

    nh_docs_Version Version = nh_docs_initVersion();

    for (int i = Changelog_p->commits-1; i >= 0; --i) 
    {
        nh_docs_Commit *Commit_p = Changelog_p->Commits_p+i;

        for (int j = 0; j < Commit_p->workloads; ++j) 
        {
            nh_docs_Workload *Workload_p = Commit_p->Workloads_p+j;
            if (Workload_p->bins == 0 && Workload_p->libs == 0
            &&  Context_p->type == NH_DOCS_CONTEXT_UNDEFINED) {
                nh_docs_incrementVersion(Workload_p->scope, &Version);
                break;
            }

            if (!Context_p->name_p) {break;}

            for (int k = 0; k < Workload_p->libs; ++k) {
                if (!strcmp(Workload_p->libs_pp[k], Context_p->name_p) 
                &&  Context_p->type != NH_DOCS_CONTEXT_BINARY) {
                    nh_docs_incrementVersion(Workload_p->scope, &Version);
                    break;
                }
            }
            for (int k = 0; k < Workload_p->bins; ++k) {
                if (!strcmp(Workload_p->bins_pp[k], Context_p->name_p)
                &&  Context_p->type == NH_DOCS_CONTEXT_BINARY) {
                    nh_docs_incrementVersion(Workload_p->scope, &Version);
                    break;
                }
            }
        }

	if (Commit_p == LastCommit_p) {break;}
    }

NH_DOCS_END(Version)
}

// REVISION ======================================================================================== 

nh_docs_Version nh_docs_getRevision(
    nh_docs_Changelog *Changelog_p, nh_docs_Commit *LastCommit_p)
{
NH_DOCS_BEGIN()

    nh_docs_ContextArray *Array_p = nh_docs_getContextArray(Changelog_p);

    nh_docs_Version Revision = nh_docs_initVersion();
    for (long unsigned int i = 0; i < ((nh_Array*)Array_p)->length; ++i) {
        nh_docs_Version Version = 
            nh_docs_getVersion(Changelog_p, ((nh_docs_Context*)((nh_Array*)Array_p)->p)+i, LastCommit_p); 
        Revision.api += Version.api;
        Revision.major += Version.major;
        Revision.minor += Version.minor;
        Revision.patch += Version.patch;
    }

    nh_docs_freeContextArray(Array_p);

NH_DOCS_END(Revision)
}
