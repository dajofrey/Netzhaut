// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Runtime.h"
#include "Thread.h"
#include "File.h"
#include "Build.h"
#include "Options.h"
#include "Utils.h"

#include "../Parser/Functions.h"
#include "../Parser/Variables.h"
#include "../Common/Macros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// VARS ============================================================================================

nh_make_Runtime NH_MAKE_DEFAULT_RUNTIME;

// PARSE ===========================================================================================

static int nh_make_getOptionLength(
    int argc, NH_BYTE **argv_pp)
{
NH_MAKE_BEGIN()

    int length = 0;

    for (int i = 0; i < argc; ++i) {
        if (i && argv_pp[i][0] == '-') {break;}
        length++;
    }

NH_MAKE_END(length)
}

static NH_MAKE_RESULT nh_make_parseArguments(
    nh_make_Runtime *Runtime_p, int argc, NH_BYTE **argv_pp)
{
NH_MAKE_BEGIN()

    for (int i = 0; i < argc;) {
        int advance = 0;
        int optionLength = nh_make_getOptionLength(argc-i, argv_pp+i);
        NH_MAKE_CHECK(nh_make_parseOption(Runtime_p, optionLength, &argv_pp[i], &advance))
        if (optionLength != advance) {
            NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)
        }
        i += advance;
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// RUN =============================================================================================

/**
 * Replace arguments that match variable names with variable content.
 */
static NH_BYTE **nh_make_processArguments(
    nh_make_VariableArray *Variables_p, NH_BYTE **args_pp, int args, int *processedArgCount_p)
{
NH_MAKE_BEGIN()

    int processedArgCount = 0;
    NH_BYTE **processedArgs_pp = malloc(sizeof(NH_BYTE*)); 
    NH_MAKE_CHECK_NULL_2(NULL, processedArgs_pp)    

    for (int i = 0; i < args; ++i) 
    {
        nh_make_Variable *Variable_p = nh_make_getVariable(Variables_p, args_pp[i]);
        if (Variable_p) {
            for (int j = 0; j < Variable_p->valueCount; ++j) {
                processedArgs_pp[processedArgCount] = malloc(strlen(Variable_p->values_pp[j]) + 1);
                NH_MAKE_CHECK_NULL_2(NULL, processedArgs_pp[processedArgCount])
                strcpy(processedArgs_pp[processedArgCount++], Variable_p->values_pp[j]);
                processedArgs_pp = realloc(processedArgs_pp, sizeof(NH_BYTE*) * (processedArgCount + 1));
                NH_MAKE_CHECK_NULL_2(NULL, processedArgs_pp)
            } 
        }
        else {
            processedArgs_pp[processedArgCount] = malloc(strlen(args_pp[i]) + 1);
            NH_MAKE_CHECK_NULL_2(NULL, processedArgs_pp[processedArgCount])
            strcpy(processedArgs_pp[processedArgCount++], args_pp[i]);
            processedArgs_pp = realloc(processedArgs_pp, sizeof(NH_BYTE*) * (processedArgCount + 1));
            NH_MAKE_CHECK_NULL_2(NULL, processedArgs_pp)
        }
    }

    *processedArgCount_p = processedArgCount;

NH_MAKE_END(processedArgs_pp)
}

static NH_MAKE_RESULT nh_make_executeRuntimeArguments(
    nh_make_Runtime *Runtime_p, NH_BYTE **args_pp, int args)
{
NH_MAKE_BEGIN()

    nh_make_operationf("Execute Runtime Arguments");

    int processedArgs = 0;
    NH_BYTE **processedArgs_pp = nh_make_processArguments(&Runtime_p->VariableArray, args_pp, args, &processedArgs);
    NH_MAKE_CHECK_NULL(processedArgs_pp)

    NH_MAKE_RESULT result = nh_make_parseArguments(Runtime_p, processedArgs, processedArgs_pp);

    for (int i = 0; i < processedArgs; ++i) {free(processedArgs_pp[i]);}
    free(processedArgs_pp);

NH_MAKE_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_executeScriptFunctions(
    nh_make_Runtime *Runtime_p, nh_make_Parser *Parser_p)
{
NH_MAKE_BEGIN()

    if (Parser_p->executed) {NH_MAKE_END(NH_MAKE_SUCCESS)}

    nh_make_Block Block;
    Block.definitions = Parser_p->definitions;
    Block.Definitions_p = Parser_p->Definitions_p;

    NH_MAKE_CHECK(nh_make_executeBlock(Runtime_p, &Block))

    Parser_p->executed = NH_MAKE_TRUE;

NH_MAKE_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_executeRuntimeScripts(
    nh_make_Runtime *Runtime_p)
{
NH_MAKE_BEGIN()

    nh_make_operationf("Execute Runtime Scripts");

    for (int i = 0; i < Runtime_p->FileArray.length; ++i) 
    {
        NH_MAKE_CHECK(nh_make_appendParser(
            &Runtime_p->ParserArray, &Runtime_p->FileArray.Files_p[i], Runtime_p->showParseTree
        ))

        nh_make_Parser *Parser_p = &Runtime_p->ParserArray.Parsers_p[Runtime_p->ParserArray.length - 1];

        if (Parser_p->executed) {continue;}
        NH_MAKE_CHECK(nh_make_executeScriptFunctions(Runtime_p, Parser_p))
        Parser_p->executed = NH_MAKE_TRUE;
    }

NH_MAKE_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_executeRuntime(
    nh_make_Runtime *Runtime_p, NH_BYTE **args_pp, int args)
{
NH_MAKE_BEGIN()

    NH_MAKE_RESULT result = NH_MAKE_SUCCESS;

    // Execute all scripts.
    result = nh_make_executeRuntimeScripts(Runtime_p);
    if (result) {goto NH_MAKE_EXECUTERUNTIME_END;}

    // Add build and test options.
    nh_make_Variable *All_p = nh_make_getVariable(&Runtime_p->VariableArray, "ALL");
    for (int j = 0; All_p && j < All_p->valueCount; ++j) {
        result = nh_make_addBuildOption(Runtime_p, All_p->values_pp[j]);
        if (result) {goto NH_MAKE_EXECUTERUNTIME_END;}
        result = nh_make_addTestOption(Runtime_p, All_p->values_pp[j]);
        if (result) {goto NH_MAKE_EXECUTERUNTIME_END;}
    }

    // Handle and execute arguments.
    result = nh_make_executeRuntimeArguments(Runtime_p, args_pp, args);

NH_MAKE_EXECUTERUNTIME_END:

    nh_make_exitMessage(result);

NH_MAKE_DIAGNOSTIC_END(result)
}

NH_MAKE_RESULT nh_make_run(
    nh_make_Runtime *Runtime_p, int argc, NH_BYTE **argv_pp)
{
NH_MAKE_BEGIN()
NH_MAKE_DIAGNOSTIC_END(nh_make_runThread(Runtime_p, argv_pp, argc))
} 

// CREATE/DESTROY ==================================================================================

static NH_MAKE_RESULT nh_make_initRuntime(
    nh_make_Runtime *Runtime_p)
{
NH_MAKE_BEGIN()

    Runtime_p->showParseTree = NH_MAKE_TRUE;
    Runtime_p->GUI = NH_MAKE_FALSE;
    Runtime_p->quiet = NH_MAKE_FALSE;

    Runtime_p->functionCallback_f      = NULL;
    Runtime_p->sourceContextCallback_f = NULL;
    Runtime_p->beforeBuildCallback_f   = NULL;
    Runtime_p->afterBuildCallback_f    = NULL;

    nh_make_initFileArray(&Runtime_p->FileArray);
    nh_make_initParserArray(&Runtime_p->ParserArray);
    nh_make_initTestArray(&Runtime_p->TestArray);
    nh_make_initSourceArray(&Runtime_p->SourceArray);
    nh_make_initSourceContextArray(&Runtime_p->SourceContextArray);
    nh_make_initVariableArray(&Runtime_p->VariableArray);

    nh_make_addDefaultSourceContext(Runtime_p);

    NH_BYTE *wrk_p = nh_make_getWorkDirectory();

    nh_make_updateVariable(&Runtime_p->VariableArray, "WRK_DIR", &wrk_p, 1);
    nh_make_updateVariable(&Runtime_p->VariableArray, "PROJ_DIR", &wrk_p, 1);
    nh_make_updateVariable(&Runtime_p->VariableArray, "LIB_DEST", &wrk_p, 1);
    nh_make_updateVariable(&Runtime_p->VariableArray, "BIN_DEST", &wrk_p, 1);

    NH_BYTE *true_p = "true";

#ifdef __APPLE__
    nh_make_updateVariable(&Runtime_p->VariableArray, "MAC", &true_p, 1);
#elif WIN32 
    nh_make_updateVariable(&Runtime_p->VariableArray, "WINDOWS", &true_p, 1);
#elif __linux__
    nh_make_updateVariable(&Runtime_p->VariableArray, "LINUX", &true_p, 1);
#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

nh_make_Runtime *nh_make_createRuntime(
    NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    NH_MAKE_CHECK_NULL_2(NULL, name_p)

    nh_make_Runtime *Runtime_p = malloc(sizeof(nh_make_Runtime));
    NH_MAKE_CHECK_NULL_2(NULL, Runtime_p)

    NH_MAKE_CHECK_2(NULL, nh_make_initRuntime(Runtime_p))

    nh_make_updateVariable(&Runtime_p->VariableArray, "NAME", &name_p, 1);

NH_MAKE_END(Runtime_p)
}

void nh_make_destroyRuntime(
    nh_make_Runtime *Runtime_p)
{
NH_MAKE_BEGIN()

    // TODO Implement.

NH_MAKE_SILENT_END()
}

// INITIALIZE/TERMINATE ============================================================================

NH_MAKE_RESULT nh_make_initialize()
{
    nh_make_initThreadPool();
    NH_MAKE_CHECK(nh_make_initRuntime(&NH_MAKE_DEFAULT_RUNTIME))
    NH_BYTE *name_p = "nhmake";
    nh_make_updateVariable(&NH_MAKE_DEFAULT_RUNTIME.VariableArray, "NAME", &name_p, 1);
}

void nh_make_terminate()
{
NH_MAKE_BEGIN()

    // TODO Implement.

NH_MAKE_SILENT_END()
}

