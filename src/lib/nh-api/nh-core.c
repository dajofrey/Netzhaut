// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-core.h"

#include "../nh-core/Loader/Loader.h"
#include "../nh-core/System/Thread.h"
#include "../nh-core/Util/File.h"
#include "../nh-core/Config/Updater.h"
#include "../nh-core/Common/Terminate.h"

#include <unistd.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DATA ============================================================================================

nh_core_Loader *LOADER_P = NULL;
static void *CORE_P  = NULL;

typedef void  *(*nh_core_initialize_f)( 
    char *path_p, char *config_p, int length 
); 

typedef int (*nh_core_run_f)( 
);
typedef void (*nh_core_dump_f)( 
    char *node_p
);
typedef void (*nh_api_setLogCallback_f)(
    void *Logger_p, nh_api_logCallback_f logCallback_f
);

// TYPEDEFS ========================================================================================

typedef void *(*nh_core_createMonitorInterface_f)(
);
typedef void (*nh_core_freeMonitorInterface_f)(
    void *Interface_p
);

// FUNCTIONS =======================================================================================

static void *nh_api_openCoreLibrary(
    char *path_p)
{
#ifdef __unix__

    char *error_p;
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

    dlclose(CORE_P);

#endif
}

static NH_API_RESULT nh_api_getExeDir(
    char *buffer_p, size_t size)
{
#ifdef __unix__

    if (readlink("/proc/self/exe", buffer_p, 1024) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, 1024) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, 1024) == -1) {return NH_API_ERROR_BAD_STATE;}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {return NH_API_ERROR_BAD_STATE;}

    buffer_p[i] = '\0'; // remove exe name
    return NH_API_SUCCESS;

#endif
}

static void *nh_api_loadCoreFunction(
    const char *functionName_p)
{
#ifdef __unix__

    char *error_p;
    dlerror(); // clear any existing error

    void *function_p = dlsym(CORE_P, functionName_p); 
    if ((error_p = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error_p);
        return NULL;
    }

    return function_p;

#endif
}

NH_API_RESULT nh_api_initialize(
    char *path_p, char *config_p, int length)
{
    if (CORE_P != NULL || LOADER_P != NULL) {return NH_API_ERROR_BAD_STATE;}

    CORE_P = nh_api_openCoreLibrary("libnh-core.so");

    bool fallback = !CORE_P && path_p != NULL;
    if (fallback) {CORE_P = nh_api_openCoreLibrary(path_p);}

    if (!CORE_P) {return NH_API_ERROR_BAD_STATE;}

    nh_core_initialize_f initialize_f = nh_api_loadCoreFunction("nh_core_initialize");
    LOADER_P = !initialize_f ? NULL : initialize_f(path_p, config_p, length);

    return LOADER_P ? NH_API_SUCCESS : NH_API_ERROR_BAD_STATE;
}
 
NH_API_RESULT nh_api_terminate()
{
    if (CORE_P == NULL || LOADER_P == NULL) {return NH_API_ERROR_BAD_STATE;}

    nh_core_terminate_f terminate_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_terminate");
    if (terminate_f) {terminate_f(LOADER_P);}

    nh_api_closeCoreLibrary();

    LOADER_P = NULL;
    CORE_P = NULL;

    return NH_API_SUCCESS;
}

int nh_api_run()
{
    nh_core_run_f run_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_run");
    return run_f ? run_f() : -1;
}

bool nh_api_keepRunning()
{
    nh_core_keepRunning_f keepRunning_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_keepRunning");
    return keepRunning_f ? keepRunning_f() : false;
}

void nh_api_setLogCallback(
    nh_api_logCallback_f logCallback_f)
{
    nh_api_setLogCallback_f setLogCallback_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_setLogCallback");
    if (setLogCallback_f != NULL) {setLogCallback_f(NULL, logCallback_f);}
    return;
}

nh_api_Workload *nh_api_getWorkload(
    void *args_p)
{
    nh_core_getWorkload_f getWorkload_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_getWorkload");
    return getWorkload_f ? getWorkload_f(args_p) : NULL;
}

char *nh_api_getFileData(
    const char* path_p, long *size_p)
{
    nh_core_getFileData_f getFileData_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_getFileData");
    return getFileData_f ? getFileData_f(path_p, size_p) : NULL;
}

NH_API_RESULT nh_api_writeBytesToFile(
    char *filePath_p, char *bytes_p)
{
    nh_core_writeBytesToFile_f writeBytesToFile_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_writeBytesToFile");
    return writeBytesToFile_f ? writeBytesToFile_f(filePath_p, bytes_p) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_registerConfig(
    char *absolutePath_p, int length)
{
    nh_core_registerConfig_f registerConfig_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_registerConfig");
    return registerConfig_f ? registerConfig_f(absolutePath_p, length) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_loadConfig(
    char *data_p, int length)
{
    nh_core_loadConfig_f loadConfig_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_loadConfig");
    return loadConfig_f ? loadConfig_f(data_p, length) : NH_API_ERROR_BAD_STATE;
}

void *nh_api_getLoader()
{
    return LOADER_P;
}

void *nh_api_dump(
    char *node_p)
{
    nh_core_dump_f dump_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_dump");
    if (dump_f) {dump_f(node_p);}
    return;
}

void *nh_api_createMonitorInterface()
{
    nh_core_createMonitorInterface_f createMonitorInterface_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_createMonitorInterface");
    return createMonitorInterface_f ? createMonitorInterface_f() : NULL;
}

void nh_api_freeMonitorInterface(
    void *Interface_p)
{
    nh_core_freeMonitorInterface_f freeMonitorInterface_f = !LOADER_P ? NULL : LOADER_P->loadSymbol_f(NH_MODULE_CORE, 0, "nh_core_freeMonitorInterface");
    freeMonitorInterface_f(Interface_p);
}

