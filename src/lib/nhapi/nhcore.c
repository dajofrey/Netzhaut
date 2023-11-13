// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nhcore.h"

#include "../nhcore/Loader/Loader.h"
#include "../nhcore/System/Thread.h"
#include "../nhcore/Util/File.h"
#include "../nhcore/Config/Updater.h"

#include "../nhcore/Common/Terminate.h"

#include <unistd.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DATA ============================================================================================

nh_Loader *NH_LOADER_P = NULL;
static void *core_p  = NULL;

// HELPER ==========================================================================================

static void *nh_api_openCoreLibrary(
    NH_BYTE *path_p)
{
#ifdef __unix__

    NH_BYTE *error_p;
    dlerror();

    void *dl_p = dlopen(path_p, RTLD_NOW | RTLD_GLOBAL);

    if ((error_p = dlerror()) != NULL) { 
        fprintf(stderr, "%s\n", error_p);
        return NULL;
    }

    return dl_p;

#endif
}

static void *nh_api_closeCoreLibrary()
{
#ifdef __unix__

    dlclose(core_p);

#endif
}

static NH_CORE_RESULT nh_api_getExeDir(
    NH_BYTE *buffer_p, size_t size)
{
#ifdef __unix__

    if (readlink("/proc/self/exe", buffer_p, 1024) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, 1024) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, 1024) == -1) {return NH_CORE_ERROR_BAD_STATE;}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {return NH_CORE_ERROR_BAD_STATE;}

    buffer_p[i] = '\0'; // remove exe name
    return NH_CORE_SUCCESS;

#endif
}

static void *nh_api_loadCore(
    NH_LOADER_SCOPE_E scope)
{
#ifdef __unix__

    switch (scope)
    {
        case NH_LOADER_SCOPE_LOCAL :
        {
            NH_BYTE exeDir_p[2048] = {'\0'};
            if (nh_api_getExeDir(exeDir_p, 2048) != NH_CORE_SUCCESS) {return NULL;}
            NH_BYTE path_p[2048] = {'\0'};
            sprintf(path_p, "%s/netzhaut-master/lib/libnhcore.so", exeDir_p);
            return nh_api_openCoreLibrary(path_p);
        }
        case NH_LOADER_SCOPE_LOCAL_SYSTEM :
            return nh_api_openCoreLibrary("/usr/local/lib/libnhcore.so");
        case NH_LOADER_SCOPE_SYSTEM :
            return nh_api_openCoreLibrary("libnhcore.so");
    }

#endif

    return NULL;
}

static void *nh_api_loadCoreFunction(
    const NH_BYTE *functionName_p)
{
#ifdef __unix__

    NH_BYTE *error_p;
    dlerror(); // clear any existing error

    void *function_p = dlsym(core_p, functionName_p); 
    if ((error_p = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error_p);
        return NULL;
    }

    return function_p;

#endif
}

// INITIALIZE/TERMINATE ============================================================================

NH_CORE_RESULT nh_api_initialize(
    NH_LOADER_SCOPE_E scope, NH_BYTE *path_p, NH_BYTE *config_p, int length)
{
    if (core_p != NULL || NH_LOADER_P != NULL) {return NH_CORE_ERROR_BAD_STATE;}

    core_p = nh_api_loadCore(scope);

    NH_BOOL fallback = !core_p && path_p != NULL;
    if (fallback) {core_p = nh_api_openCoreLibrary(path_p);}

    if (!core_p) {return NH_CORE_ERROR_BAD_STATE;}

    nh_core_initLoader_f initLoader_f = nh_api_loadCoreFunction("nh_core_initLoader");
    NH_LOADER_P = !initLoader_f ? NULL : initLoader_f(scope, NH_FALSE, NH_FALSE);

    return NH_LOADER_P ? NH_CORE_SUCCESS : NH_CORE_ERROR_BAD_STATE;
}
 
NH_CORE_RESULT nh_api_terminate()
{
    if (core_p == NULL || NH_LOADER_P == NULL) {return NH_CORE_ERROR_BAD_STATE;}

    nh_core_terminate_f terminate_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_terminate");
    if (terminate_f) {terminate_f(NH_LOADER_P);}

    nh_api_closeCoreLibrary();

    NH_LOADER_P = NULL;
    core_p = NULL;

    return NH_CORE_SUCCESS;
}

// OTHER ===========================================================================================

unsigned int nh_api_run()
{
    nh_core_runThreadWorkloads_f runThreadWorkloads_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_runThreadWorkloads");
    return runThreadWorkloads_f ? runThreadWorkloads_f() : 0;
}

NH_BOOL nh_api_keepRunning()
{
    nh_core_keepRunning_f keepRunning_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_keepRunning");
    return keepRunning_f ? keepRunning_f() : NH_FALSE;
}

NH_CORE_RESULT nh_api_addLogCallback(
    nh_core_logCallback_f logCallback_f)
{
    nh_core_addLogCallback_f addLogCallback_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_addLogCallback");
    return addLogCallback_f ? addLogCallback_f(logCallback_f) : NH_CORE_ERROR_BAD_STATE;
}

nh_core_Workload *nh_api_getWorkload(
    void *args_p)
{
    nh_core_getWorkload_f getWorkload_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_getWorkload");
    return getWorkload_f ? getWorkload_f(args_p) : NULL;
}

// FILE FUNCTIONS ==================================================================================

NH_BYTE *nh_api_getFileData(
    const NH_BYTE* path_p, long *size_p)
{
    nh_core_getFileData_f getFileData_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_getFileData");
    return getFileData_f ? getFileData_f(path_p, size_p) : NULL;
}

NH_CORE_RESULT nh_api_writeBytesToFile(
    NH_BYTE *filePath_p, NH_BYTE *bytes_p)
{
    nh_core_writeBytesToFile_f writeBytesToFile_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_writeBytesToFile");
    return writeBytesToFile_f ? writeBytesToFile_f(filePath_p, bytes_p) : NH_CORE_ERROR_BAD_STATE;
}

// CONFIG FUNCTIONS ================================================================================

NH_CORE_RESULT nh_api_registerConfig(
    NH_BYTE *absolutePath_p, int length)
{
    nh_core_registerConfig_f registerConfig_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_registerConfig");
    return registerConfig_f ? registerConfig_f(absolutePath_p, length) : NH_CORE_ERROR_BAD_STATE;
}


NH_CORE_RESULT nh_api_loadConfig(
    NH_BYTE *data_p, int length)
{
    nh_core_loadConfig_f loadConfig_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_loadConfig");
    return loadConfig_f ? loadConfig_f(data_p, length) : NH_CORE_ERROR_BAD_STATE;
}
