// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Context.h"

#include "../Common/Macros.h"

#include "../../nhcore/Util/Array.h"
#include "../../nhcore/System/Memory.h"

#include <string.h>

// FUNCTIONS =======================================================================================

nh_docs_Context *nh_docs_getContextFromDescription(
    nh_docs_ContextArray *Array_p, NH_BYTE *name_p, NH_DOCS_CONTEXT_E type)
{
NH_DOCS_BEGIN()

    for (long unsigned int i = 0; i < ((nh_Array*)Array_p)->length; ++i) {
        if (!strcmp(((nh_docs_Context*)((nh_Array*)Array_p)->p)[i].name_p, name_p) 
        && ((nh_docs_Context*)((nh_Array*)Array_p)->p)[i].type == type) {
            NH_DOCS_END(((nh_docs_Context*)((nh_Array*)Array_p)->p)+i)
        }
    }

NH_DOCS_END(NULL)
}

nh_docs_Context *nh_docs_getContext(
    nh_docs_ContextArray *Array_p, long unsigned int index)
{
NH_DOCS_BEGIN()

    if (index > ((nh_Array*)Array_p)->length) {
        NH_DOCS_END(NULL)
    }

NH_DOCS_END(((nh_docs_Context*)((nh_Array*)Array_p)->p)+index)
}

static NH_DOCS_RESULT nh_docs_appendToContextArray(
    nh_Array *Array_p, NH_BYTE *name_p, NH_DOCS_CONTEXT_E type)
{
NH_DOCS_BEGIN()

    nh_docs_Context Context;
    memset(&Context, 0, sizeof(nh_docs_Context));

    Context.type = type;
    memcpy(Context.name_p, name_p, strlen(name_p));

    nh_core_appendToArray(Array_p, &Context, 1);

NH_DOCS_END(NH_DOCS_SUCCESS)
}

nh_docs_ContextArray *nh_docs_getContextArray(
    nh_docs_Changelog *Changelog_p)
{
NH_DOCS_BEGIN()

    nh_Array *Contexts_p = nh_core_allocate(sizeof(nh_Array));
    *Contexts_p = nh_core_initArray(sizeof(nh_docs_Context), 8);

    for (int j = 0; j < Changelog_p->commits; ++j) 
    {
        nh_docs_Commit *Commit_p = Changelog_p->Commits_p+j;

        for (int i = 0; i < Commit_p->workloads; ++i) 
        {
            nh_docs_Workload *Workload_p = Commit_p->Workloads_p+i;

            for (int k = 0; k < Workload_p->libs; ++k) {
                if (nh_docs_getContextFromDescription((nh_docs_ContextArray*)Contexts_p, 
                    Workload_p->libs_pp[k], NH_DOCS_CONTEXT_LIBRARY)) {continue;}
                nh_docs_appendToContextArray(Contexts_p, Workload_p->libs_pp[k], NH_DOCS_CONTEXT_LIBRARY);
            }
            for (int k = 0; k < Workload_p->bins; ++k) {
                if (nh_docs_getContextFromDescription((nh_docs_ContextArray*)Contexts_p, 
                    Workload_p->bins_pp[k], NH_DOCS_CONTEXT_BINARY)) {continue;}
                nh_docs_appendToContextArray(Contexts_p, Workload_p->bins_pp[k], NH_DOCS_CONTEXT_BINARY);
            }
        }
    }

NH_DOCS_END((nh_docs_ContextArray*)Contexts_p)
}

NH_DOCS_RESULT nh_docs_freeContextArray(
    nh_docs_ContextArray *ContextArray_p)
{
NH_DOCS_BEGIN()

    nh_core_freeArray((nh_Array*)ContextArray_p);
    nh_core_free(ContextArray_p);

NH_DOCS_DIAGNOSTIC_END(NH_DOCS_SUCCESS)
}

