// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Source.h"
#include "Runtime.h"

#include "../Common/Macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// INIT ============================================================================================

void nh_make_initSourceContextArray(
    nh_make_SourceContextArray *Array_p)
{
NH_MAKE_BEGIN()

    Array_p->length = 0;
    Array_p->maxNameLength = 0;
    Array_p->SourceContexts_p = NULL;

NH_MAKE_SILENT_END()
}

static nh_make_SourceContext nh_make_initSourceContext(
    NH_MAKE_SOURCE_CONTEXT type)
{
NH_MAKE_BEGIN()

    nh_make_SourceContext SourceContext;

    SourceContext.name_p = NULL;
    SourceContext.type = type;
    SourceContext.path_p = NULL;
    SourceContext.compileArgs_p = NULL;
    SourceContext.linkArgs_p = NULL;
    SourceContext.outputPath_p = NULL;

    memset(&SourceContext.Version, 0, sizeof(nh_docs_Version));
 
NH_MAKE_END(SourceContext)
}

void nh_make_initSourceArray(
    nh_make_SourceArray *Array_p)
{
NH_MAKE_BEGIN()

    Array_p->length = 0;
    Array_p->Sources_p = NULL;

NH_MAKE_SILENT_END()
}

static nh_make_Source nh_make_initSource()
{
NH_MAKE_BEGIN()

    nh_make_Source Source;
    Source.Context_p = NULL;
    Source.path_p = NULL;

NH_MAKE_END(Source)
}

// INDENT ==========================================================================================

void nh_make_getIndentAfterSourceContext(
    NH_BYTE *name_p, NH_BYTE *buffer_p, int size, nh_make_SourceContextArray *Array_p)
{
NH_MAKE_BEGIN()

    memset(buffer_p, 0, size);
    int diff = name_p ? Array_p->maxNameLength - strlen(name_p) : Array_p->maxNameLength;
    for (int i = 0; i < diff; ++i) {buffer_p[i] = ' ';}

NH_MAKE_SILENT_END()
}

// ADD =============================================================================================

static NH_MAKE_BOOL nh_make_matchSourceContext(
    nh_make_SourceContext *SourceContext_p, NH_BYTE *name_p, NH_MAKE_SOURCE_CONTEXT type)
{
NH_MAKE_BEGIN()

    if (type == SourceContext_p->type) {
        if (type == NH_MAKE_SOURCE_CONTEXT_UNDEFINED && SourceContext_p->name_p == NULL && name_p == NULL) {
            NH_MAKE_END(NH_MAKE_TRUE)
        }
        if (SourceContext_p->name_p && name_p) {
            if (!strcmp(SourceContext_p->name_p, name_p)) {
                NH_MAKE_END(NH_MAKE_TRUE)
            }
        }
    }

NH_MAKE_END(NH_MAKE_FALSE)
}

nh_make_SourceContextArray *nh_make_getSourceContextArray(
    nh_make_Runtime *Runtime_p)
{
NH_MAKE_BEGIN()
NH_MAKE_END(&Runtime_p->SourceContextArray)
}

nh_make_SourceContext *nh_make_getSourceContext(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p, NH_MAKE_SOURCE_CONTEXT type)
{
NH_MAKE_BEGIN()

    for (int i = 0; i < Runtime_p->SourceContextArray.length; ++i) {
        nh_make_SourceContext *Context_p = &Runtime_p->SourceContextArray.SourceContexts_p[i];
        if (nh_make_matchSourceContext(Context_p, name_p, type)) {
            NH_MAKE_END(Context_p)
        }
    }

NH_MAKE_END(NULL)
}

NH_MAKE_RESULT nh_make_addDefaultSourceContext(
    nh_make_Runtime *Runtime_p)
{
NH_MAKE_BEGIN()

    nh_make_SourceContextArray *Array_p = &Runtime_p->SourceContextArray;

    if (!Array_p->SourceContexts_p) {
        Array_p->SourceContexts_p = malloc(sizeof(nh_make_SourceContext));
        NH_MAKE_CHECK_NULL(Array_p->SourceContexts_p)
    }
    else {
        Array_p->SourceContexts_p = realloc(Array_p->SourceContexts_p, sizeof(nh_make_SourceContext) * (Array_p->length + 1));
        NH_MAKE_CHECK_NULL(Array_p->SourceContexts_p)
    }

    nh_make_SourceContext *SourceContext_p = &Array_p->SourceContexts_p[Array_p->length];
    *SourceContext_p = nh_make_initSourceContext(NH_MAKE_SOURCE_CONTEXT_UNDEFINED);

    Array_p->length++;

NH_MAKE_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_addSourceContext(
    nh_make_Runtime *Runtime_p, nh_make_Function *Function_p, NH_MAKE_SOURCE_CONTEXT type, int offset)
{
NH_MAKE_BEGIN()

    nh_make_SourceContextArray *Array_p = &Runtime_p->SourceContextArray;

    if (Function_p->arguments < 1) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)}

    if (!Array_p->SourceContexts_p) {
        Array_p->SourceContexts_p = malloc(sizeof(nh_make_SourceContext));
        NH_MAKE_CHECK_NULL(Array_p->SourceContexts_p)
    }
    else {
        Array_p->SourceContexts_p = realloc(Array_p->SourceContexts_p, sizeof(nh_make_SourceContext) * (Array_p->length + 1));
        NH_MAKE_CHECK_NULL(Array_p->SourceContexts_p)
    }

    nh_make_SourceContext *SourceContext_p = &Array_p->SourceContexts_p[Array_p->length];
    *SourceContext_p = nh_make_initSourceContext(type);

    int index = offset;

    SourceContext_p->name_p = malloc(strlen(Function_p->arguments_pp[index]) + 1);
    NH_MAKE_CHECK_NULL(SourceContext_p->name_p)
    strcpy(SourceContext_p->name_p, Function_p->arguments_pp[index++]);

    if (Function_p->arguments > index) {
        SourceContext_p->path_p = malloc(strlen(Function_p->arguments_pp[index]) + 1);
        NH_MAKE_CHECK_NULL(SourceContext_p->path_p)
        strcpy(SourceContext_p->path_p, Function_p->arguments_pp[index++]);
    }

    if (Function_p->arguments > index) {
        SourceContext_p->outputPath_p = malloc(strlen(Function_p->arguments_pp[index]) + 1);
        NH_MAKE_CHECK_NULL(SourceContext_p->outputPath_p)
        strcpy(SourceContext_p->outputPath_p, Function_p->arguments_pp[index++]);
    }

    if (Function_p->arguments > index) {
        SourceContext_p->Version.major = strtol(Function_p->arguments_pp[index++], NULL, 10);
    }

    if (Function_p->arguments > index) {
        SourceContext_p->Version.minor = strtol(Function_p->arguments_pp[index++], NULL, 10);
    }

    if (Function_p->arguments > index) {
        SourceContext_p->Version.patch = strtol(Function_p->arguments_pp[index++], NULL, 10);
    }

    if (strlen(SourceContext_p->name_p) > Array_p->maxNameLength) {
        Array_p->maxNameLength = strlen(SourceContext_p->name_p);
    }

    Array_p->length++;

    NH_MAKE_CHECK(nh_make_appendToVariable(&Runtime_p->VariableArray, "ALL", &Function_p->arguments_pp[offset], 1))

    if (type == NH_MAKE_SOURCE_CONTEXT_BINARY) {
        NH_MAKE_CHECK(nh_make_appendToVariable(&Runtime_p->VariableArray, "BINS", &Function_p->arguments_pp[offset], 1))
    }
    else {
        NH_MAKE_CHECK(nh_make_appendToVariable(&Runtime_p->VariableArray, "LIBS", &Function_p->arguments_pp[offset], 1))
    }

    switch (type)
    {
        case NH_MAKE_SOURCE_CONTEXT_BINARY :
            nh_make_messagef("Add binary [%s]", SourceContext_p->name_p);
            break;
        case NH_MAKE_SOURCE_CONTEXT_SHARED_LIBRARY :
            nh_make_messagef("Add shared library [%s]", SourceContext_p->name_p);
            break;
        case NH_MAKE_SOURCE_CONTEXT_STATIC_LIBRARY :
            nh_make_messagef("Add static library [%s]", SourceContext_p->name_p);
            break;
    }

NH_MAKE_END(NH_MAKE_SUCCESS)
}

// ARGUMENTS =======================================================================================

NH_MAKE_RESULT nh_make_addCompileArguments(
    nh_make_Runtime *Runtime_p, nh_make_Function *Function_p, NH_MAKE_SOURCE_CONTEXT type)
{
NH_MAKE_BEGIN()

    if (Function_p->arguments < 2) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)}

    nh_make_SourceContext *Context_p = nh_make_getSourceContext(Runtime_p, Function_p->arguments_pp[0], type);
    if (!Context_p) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_LIBRARY_NOT_FOUND)}

    Context_p->compileArgs_p = malloc(strlen(Function_p->arguments_pp[1]) + 1);
    NH_MAKE_CHECK_NULL(Context_p->compileArgs_p)
    sprintf(Context_p->compileArgs_p, Function_p->arguments_pp[1]);

    NH_BYTE offset_p[64];
    nh_make_getIndentAfterSourceContext(Context_p->name_p, offset_p, 64, &Runtime_p->SourceContextArray);
    nh_make_messagef("[%s]%s Set compile arguments \"%s\"", Context_p->name_p, offset_p, Context_p->compileArgs_p);

NH_MAKE_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_addLinkArguments(
    nh_make_Runtime *Runtime_p, nh_make_Function *Function_p, NH_MAKE_SOURCE_CONTEXT type)
{
NH_MAKE_BEGIN()

    if (Function_p->arguments != 2) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)}

    nh_make_SourceContext *Context_p = nh_make_getSourceContext(Runtime_p, Function_p->arguments_pp[0], type);
    if (!Context_p) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_LIBRARY_NOT_FOUND)}

    Context_p->linkArgs_p = malloc(strlen(Function_p->arguments_pp[1]) + 1);
    NH_MAKE_CHECK_NULL(Context_p->linkArgs_p)
    sprintf(Context_p->linkArgs_p, Function_p->arguments_pp[1]);

    NH_BYTE offset_p[64];
    nh_make_getIndentAfterSourceContext(Context_p->name_p, offset_p, 64, &Runtime_p->SourceContextArray);
    nh_make_messagef("[%s]%s Set link arguments \"%s\"", Context_p->name_p, offset_p, Context_p->linkArgs_p);

NH_MAKE_END(NH_MAKE_SUCCESS)
}

// SOURCES =========================================================================================

NH_MAKE_RESULT nh_make_addSource(
    nh_make_Runtime *Runtime_p, nh_make_SourceArray *SourceArray_p, nh_make_Function *Function_p, 
    NH_MAKE_SOURCE_CONTEXT type)
{
NH_MAKE_BEGIN()

    nh_make_SourceContext *Context_p = NULL;

    for (int i = 0; i < Function_p->arguments; ++i) 
    {
        if (Function_p->argumentTypes_p[i] == NH_MAKE_TOKEN_IDENTIFIER) {
            Context_p = nh_make_getSourceContext(Runtime_p, Function_p->arguments_pp[i], type);
        }
        else {

            if (!Context_p) {
                if (!Runtime_p->SourceContextArray.length) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)}
                Context_p = Runtime_p->SourceContextArray.SourceContexts_p;
            }

            if (!SourceArray_p->Sources_p) {
                SourceArray_p->Sources_p = malloc(sizeof(nh_make_Source));
                NH_MAKE_CHECK_NULL(SourceArray_p->Sources_p)
            }
            else {
                SourceArray_p->Sources_p = realloc(SourceArray_p->Sources_p, sizeof(nh_make_Source) * (SourceArray_p->length + 1));
                NH_MAKE_CHECK_NULL(SourceArray_p->Sources_p)
            }
        
            nh_make_Source *Source_p = &SourceArray_p->Sources_p[SourceArray_p->length];
            *Source_p = nh_make_initSource();

            Source_p->Context_p = Context_p;
            Source_p->path_p = malloc(strlen(Function_p->arguments_pp[i]) + 1);
            NH_MAKE_CHECK_NULL(Source_p->path_p)
            sprintf(Source_p->path_p, Function_p->arguments_pp[i]);
        
            SourceArray_p->length++;

            NH_BYTE offset_p[64];
            nh_make_getIndentAfterSourceContext(Context_p->name_p, offset_p, 64, &Runtime_p->SourceContextArray);
            nh_make_messagef("[%s]%s Add source file (%d) \"%s\"", Context_p->name_p, offset_p, Context_p->type, Source_p->path_p);
        }
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

