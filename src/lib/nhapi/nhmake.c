// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nhmake.h"
#include "nhcore.h"

#include "../nhcore/Loader/Loader.h"

#include "../nhmake/Core/Runtime.h"
#include "../nhmake/Core/File.h"
#include "../nhmake/Core/Utils.h"
#include "../nhmake/Core/Thread.h"
#include "../nhmake/Core/Configure.h"
#include "../nhmake/Core/Source.h"
#include "../nhmake/Parser/Variables.h"
#include "../nhmake/UI/Message.h"

#include <unistd.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

NH_MAKE_RESULT nh_make_initialize()
{
    nh_make_initialize_f initialize_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_initialize");
    return initialize_f ? initialize_f() : NH_MAKE_ERROR_BAD_STATE;
}

void nh_make_terminate()
{
    nh_make_terminate_f terminate_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_terminate");
    if (terminate_f) {terminate_f();}
}

nh_make_Runtime *nh_make_createRuntime(
    NH_BYTE *name_p)
{
    nh_make_createRuntime_f createRuntime_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_createRuntime");
    return createRuntime_f ? createRuntime_f(name_p) : NULL;
}

void nh_make_destroyRuntime(
    nh_make_Runtime *Runtime_p)
{
    nh_make_destroyRuntime_f destroyRuntime_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_destroyRuntime");
    if (destroyRuntime_f) {destroyRuntime_f(Runtime_p);}
}

NH_MAKE_RESULT nh_make_run(
    nh_make_Runtime *Runtime_p, int argc, NH_BYTE **argv_pp)
{
    nh_make_run_f run_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_run");
    return run_f ? run_f(Runtime_p, argc, argv_pp) : NH_MAKE_ERROR_BAD_STATE;
}

NH_MAKE_RESULT nh_make_addFile(
    nh_make_Runtime *Runtime_p, NH_BYTE *path_p)
{
    nh_make_addFile_f addFile_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_addFile");
    return addFile_f ? addFile_f(Runtime_p, path_p) : NH_MAKE_ERROR_BAD_STATE;
}

void nh_make_setFunctionCallback(
    nh_make_Runtime *Runtime_p, nh_make_functionCallback_f functionCallback_f)
{
    nh_make_setFunctionCallback_f setFunctionCallback_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_setFunctionCallback");
    if (setFunctionCallback_f) {setFunctionCallback_f(Runtime_p, functionCallback_f);}
}

void nh_make_setSourceContextCallback(
    nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f)
{
    nh_make_setSourceContextCallback_f setSourceContextCallback_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_setSourceContextCallback");
    if (setSourceContextCallback_f) {setSourceContextCallback_f(Runtime_p, sourceContextCallback_f);}
}

void nh_make_setBeforeBuildCallback(
    nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f)
{
    nh_make_setBeforeBuildCallback_f setBeforeBuildCallback_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_setBeforeBuildCallback");
    if (setBeforeBuildCallback_f) {setBeforeBuildCallback_f(Runtime_p, sourceContextCallback_f);}
}

void nh_make_setAfterBuildCallback(
    nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f)
{
    nh_make_setAfterBuildCallback_f setAfterBuildCallback_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_setAfterBuildCallback");
    if (setAfterBuildCallback_f) {setAfterBuildCallback_f(Runtime_p, sourceContextCallback_f);}
}

nh_make_SourceContextArray *nh_make_getSourceContextArray(
    nh_make_Runtime *Runtime_p)
{
    nh_make_getSourceContextArray_f getSourceContextArray_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_getSourceContextArray");
    return getSourceContextArray_f ? getSourceContextArray_f(Runtime_p) : NULL;
}

nh_make_SourceContext *nh_make_getSourceContext(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p, NH_MAKE_SOURCE_CONTEXT type)
{
    nh_make_getSourceContext_f getSourceContext_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_getSourceContext");
    return getSourceContext_f ? getSourceContext_f(Runtime_p, name_p, type) : NULL;
}

void nh_make_setQuiet(
    nh_make_Runtime *Runtime_p, NH_MAKE_BOOL quiet)
{
    nh_make_setQuiet_f setQuiet_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_setQuiet");
    if (setQuiet_f) {setQuiet_f(Runtime_p, quiet);}
}

void nh_make_setShowParseTree(
    nh_make_Runtime *Runtime_p, NH_MAKE_BOOL showParseTree)
{
    nh_make_setShowParseTree_f setShowParseTree_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_setShowParseTree");
    if (setShowParseTree_f) {setShowParseTree_f(Runtime_p, showParseTree);}
}

NH_MAKE_RESULT nh_make_setVariable(
    nh_make_Runtime *Runtime_p, NH_BYTE *variables_p, NH_BYTE **values_pp, int valueCount)
{
    nh_make_setVariable_f setVariable_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_setVariable");
    return setVariable_f ? setVariable_f(Runtime_p, variables_p, values_pp, valueCount) : NH_MAKE_ERROR_BAD_STATE;
}

nh_make_ValueArray nh_make_getVariableValues(
    nh_make_Runtime *Runtime_p, NH_BYTE *variable_p)
{
    nh_make_getVariableValues_f getVariableValues_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_getVariableValues");
    nh_make_ValueArray Array;
    Array.values_pp = NULL;
    Array.length = 0;
    return getVariableValues_f ? getVariableValues_f(Runtime_p, variable_p) : Array;
}

int nh_make_isRunning()
{
    nh_make_isRunning_f isRunning_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_isRunning");
    return isRunning_f ? isRunning_f() : -1;
}

NH_MAKE_RESULT nh_make_sleepMs(
    int milliseconds)
{
    nh_make_sleepMs_f sleepMs_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_sleepMs");
    return sleepMs_f ? sleepMs_f(milliseconds) : NH_MAKE_ERROR_BAD_STATE;
}

NH_BYTE *nh_make_getProcessDirectory()
{
    nh_make_getProcessDirectory_f getProcessDirectory_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_getProcessDirectory");
    return getProcessDirectory_f ? getProcessDirectory_f() : NULL;
}

NH_MAKE_RESULT nh_make_messagef(
    NH_BYTE *format_p, ...)
{
    nh_make_messagef_f messagef_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_MAKE, 0, "nh_make_messagef");
    return messagef_f ? messagef_f(format_p) : NH_MAKE_ERROR_BAD_STATE;
}

