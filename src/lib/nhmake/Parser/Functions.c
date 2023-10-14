// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Functions.h"
#include "Variables.h"

#include "../Core/Build.h"
#include "../Core/Utils.h"
#include "../UI/Message.h"
#include "../Common/Macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/types.h> 
#include <sys/stat.h>

// HELPER ==========================================================================================

static NH_MAKE_BOOL nh_make_caseInsensitiveMatch(
    NH_BYTE *str1_p, NH_BYTE *str2_p)
{
NH_MAKE_BEGIN()

    if (strlen(str1_p) != strlen(str2_p)) {NH_MAKE_END(NH_MAKE_FALSE)}

    for (int i = 0; i < strlen(str1_p); ++i) {
        int d = tolower((unsigned char)str1_p[i]) - tolower((unsigned char)str2_p[i]);
        if (d != 0) {
            NH_MAKE_END(NH_MAKE_FALSE)
        }
    }
   
NH_MAKE_END(NH_MAKE_TRUE)
}

// EXECUTE =========================================================================================

static NH_MAKE_RESULT nh_make_executeCopyFunction(
    nh_make_Runtime *Runtime_p, nh_make_Function *Function_p)
{
NH_MAKE_BEGIN()

    if (Function_p->arguments <= 0 || Function_p->arguments > 4) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)}

    switch(Function_p->arguments)
    {
        case 2 :
            nh_make_copy(&Runtime_p->VariableArray, Function_p->arguments_pp[0], Function_p->arguments_pp[1], NH_MAKE_FALSE, NH_MAKE_FALSE);
            break;
        case 3 :
            nh_make_copy(&Runtime_p->VariableArray, Function_p->arguments_pp[0], Function_p->arguments_pp[1], !strcmp(Function_p->arguments_pp[2], "true") ? NH_MAKE_TRUE : NH_MAKE_FALSE, NH_MAKE_FALSE);
            break;
        case 4 :
            nh_make_copy(&Runtime_p->VariableArray, Function_p->arguments_pp[0], Function_p->arguments_pp[1], !strcmp(Function_p->arguments_pp[2], "true") ? NH_MAKE_TRUE : NH_MAKE_FALSE, !strcmp(Function_p->arguments_pp[3], "true") ? NH_MAKE_TRUE : NH_MAKE_FALSE);
            break;
    }

NH_MAKE_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_executeChdirFunction(
    nh_make_Runtime *Runtime_p, nh_make_Function *Function_p)
{
NH_MAKE_BEGIN()

    if (Function_p->arguments == 0) {
        nh_make_updateVariable(&Runtime_p->VariableArray, "WRK_DIR", nh_make_getVariable(&Runtime_p->VariableArray, "PROJ_DIR")->values_pp, 1);
    }
    else {
        nh_make_updateVariable(&Runtime_p->VariableArray, "WRK_DIR", Function_p->arguments_pp, 1);
    }

    nh_make_messagef("chdir %s", nh_make_getVariable(&Runtime_p->VariableArray, "WRK_DIR")->values_pp[0]);

NH_MAKE_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_executeMkdirrFunction(
    nh_make_Function *Function_p)
{
NH_MAKE_BEGIN()

    if (Function_p->arguments != 1) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)}

    NH_BYTE tmp[256];
    NH_BYTE *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",Function_p->arguments_pp[0]);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);

    nh_make_messagef("mkdirr %s", Function_p->arguments_pp[0]);

NH_MAKE_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_executeMkdirFunction(
    nh_make_Function *Function_p)
{
NH_MAKE_BEGIN()

    if (Function_p->arguments != 1) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)}

    struct stat st = {0};
    if (stat(Function_p->arguments_pp[0], &st) == -1) {
        int err = mkdir(Function_p->arguments_pp[0], 0755);  // 0755 -> drwxr-xr-x
        if (err != 0) {
            NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_CANT_CREATE_DIRECTORY)
        }
    }

    nh_make_messagef("mkdir %s", Function_p->arguments_pp[0]);

NH_MAKE_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_executeSystemFunction(
    nh_make_Function *Function_p)
{
NH_MAKE_BEGIN()

    if (Function_p->arguments != 1) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)}
    nh_make_messagef("system %s", Function_p->arguments_pp[0]);
    system(Function_p->arguments_pp[0]);

NH_MAKE_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_executeFunction(
    nh_make_Runtime *Runtime_p, nh_make_Function *Function_p) 
{
NH_MAKE_BEGIN()

    NH_BYTE **arguments_pp = malloc(sizeof(NH_BYTE*) * Function_p->arguments);
    memcpy(arguments_pp, Function_p->arguments_pp, sizeof(NH_BYTE*) * Function_p->arguments);

    for (int i = 0; i < Function_p->arguments; ++i) {
        Function_p->arguments_pp[i] = nh_make_substituteVariables(&Runtime_p->VariableArray, arguments_pp[i]);
        NH_MAKE_CHECK_NULL(Function_p->arguments_pp[i])
    }
    NH_MAKE_RESULT result = NH_MAKE_SUCCESS;

    if (nh_make_caseInsensitiveMatch(Function_p->name_p, "lib")) {
        if (!strcmp(Function_p->arguments_pp[0], "SHARED")) {
            result = nh_make_addSourceContext(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_SHARED_LIBRARY, 1);
        }
        else if (!strcmp(Function_p->arguments_pp[0], "STATIC")) {
            result = nh_make_addSourceContext(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_STATIC_LIBRARY, 1);
        }
        else {
            result = nh_make_addSourceContext(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_SHARED_LIBRARY, 0);
        }
        if (!result && Runtime_p->sourceContextCallback_f) {
            result = Runtime_p->sourceContextCallback_f(
                Runtime_p, &Runtime_p->SourceContextArray.SourceContexts_p[Runtime_p->SourceContextArray.length - 1]
            );
        }
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "bin")) {
        result = nh_make_addSourceContext(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_BINARY, 0);
        if (!result && Runtime_p->sourceContextCallback_f) {
            result = Runtime_p->sourceContextCallback_f(
                Runtime_p, &Runtime_p->SourceContextArray.SourceContexts_p[Runtime_p->SourceContextArray.length - 1]
            );
        }
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "compile")) {
        result = nh_make_addCompileArguments(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_UNDEFINED);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "compile_lib")) {
        result = nh_make_addCompileArguments(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_SHARED_LIBRARY);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "compile_bin")) {
        result = nh_make_addCompileArguments(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_BINARY);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "link")) {
        result = nh_make_addLinkArguments(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_UNDEFINED);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "link_lib")) {
        result = nh_make_addLinkArguments(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_SHARED_LIBRARY);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "link_bin")) {
        result = nh_make_addLinkArguments(Runtime_p, Function_p, NH_MAKE_SOURCE_CONTEXT_BINARY);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "source")) { 
        result = nh_make_addSource(Runtime_p, &Runtime_p->SourceArray, Function_p, NH_MAKE_SOURCE_CONTEXT_UNDEFINED);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "source_lib")) {
        result = nh_make_addSource(Runtime_p, &Runtime_p->SourceArray, Function_p, NH_MAKE_SOURCE_CONTEXT_SHARED_LIBRARY);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "source_bin")) {
        result = nh_make_addSource(Runtime_p, &Runtime_p->SourceArray, Function_p, NH_MAKE_SOURCE_CONTEXT_BINARY);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "test")) {
        result = nh_make_addTest(Runtime_p, &Runtime_p->TestArray, Function_p);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "copy")) {
        result = nh_make_executeCopyFunction(Runtime_p, Function_p);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "build")) {
        result = nh_make_build(Runtime_p, Function_p->arguments_pp[0]);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "chdir")) {
        result = nh_make_executeChdirFunction(Runtime_p, Function_p);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "mkdir")) {
        result = nh_make_executeMkdirFunction(Function_p);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "mkdirr")) {
        result = nh_make_executeMkdirrFunction(Function_p);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "system")) {
        result = nh_make_executeSystemFunction(Function_p);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "system")) {
        result = nh_make_executeSystemFunction(Function_p);
    }
    else if (nh_make_caseInsensitiveMatch(Function_p->name_p, "set")) {
        result = nh_make_updateVariable(
            &Runtime_p->VariableArray, Function_p->arguments_pp[0], Function_p->arguments_pp + 1, 
            Function_p->arguments - 1
        );
    }

    if (!result && Runtime_p->functionCallback_f) {
        result = Runtime_p->functionCallback_f(Runtime_p, Function_p);
    }

    for (int i = 0; i < Function_p->arguments; ++i) {
        free(Function_p->arguments_pp[i]);
    }

    memcpy(Function_p->arguments_pp, arguments_pp, sizeof(NH_BYTE*) * Function_p->arguments);
    free(arguments_pp);

NH_MAKE_DIAGNOSTIC_END(result)
}

NH_MAKE_BOOL nh_make_compareIf(
    nh_make_Runtime *Runtime_p, nh_make_If *If_p, NH_MAKE_BOOL b)
{
NH_MAKE_BEGIN()

    nh_make_Variable *Var_p = nh_make_getVariable(&Runtime_p->VariableArray, If_p->string_p);
    if (Var_p && Var_p->valueCount > 0) {
        if (!strcmp(*Var_p->values_pp, "true") && b == NH_MAKE_TRUE) {NH_MAKE_END(NH_MAKE_TRUE)}
        if (!strcmp(*Var_p->values_pp, "false") && b == NH_MAKE_FALSE) {NH_MAKE_END(NH_MAKE_TRUE)}
    }

NH_MAKE_END(NH_MAKE_FALSE)
}

NH_MAKE_RESULT nh_make_executeIf(
    nh_make_Runtime *Runtime_p, nh_make_If *If_p) 
{
NH_MAKE_BEGIN()

    nh_make_Variable *Var_p = nh_make_getVariable(&Runtime_p->VariableArray, If_p->string_p);
    if (Var_p && Var_p->valueCount > 0 && !strcmp(*Var_p->values_pp, "true")) {
        NH_MAKE_CHECK(nh_make_executeBlock(Runtime_p, &If_p->Block_p->Block))
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// EXECUTE BLOCK ===================================================================================

NH_MAKE_RESULT nh_make_executeBlock(
    nh_make_Runtime *Runtime_p, nh_make_Block *Block_p)
{
NH_MAKE_BEGIN()

    for (int d = 0; d < Block_p->definitions; ++d) {
        switch (Block_p->Definitions_p[d].type) {
            case NH_MAKE_DEFINITION_FUNCTION :
                NH_MAKE_CHECK(nh_make_executeFunction(Runtime_p, &Block_p->Definitions_p[d].Function))
            case NH_MAKE_DEFINITION_IF :
                NH_MAKE_CHECK(nh_make_executeIf(Runtime_p, &Block_p->Definitions_p[d].If))
        }
    }

NH_MAKE_END(NH_MAKE_SUCCESS)
}

