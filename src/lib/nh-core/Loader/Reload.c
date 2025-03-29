// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#define _GNU_SOURCE

#include "Reload.h"
#include "Loader.h"

#if defined(__unix__)
    #include <link.h>
#endif

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// FUNCTIONS =======================================================================================

char *nh_core_lastModified(
    void *lib_p) 
{
    char *time_p = malloc(sizeof(char) * 127);  // Allocate memory for the time string
    if (time_p == NULL) {
        return NULL;
    }
    memset(time_p, '\0', 127);  // Clear the memory

#if defined(__unix__) || defined(__APPLE__)
    // Get the filename of the shared library (so or dylib)
    const char *lib_name = NULL;

    // On Linux, use dlinfo to retrieve the library name
#ifdef __linux__
    struct link_map *info_p;
    dlinfo(lib_p, RTLD_DI_LINKMAP, &info_p);
    lib_name = info_p->l_name;
#elif defined(__APPLE__)
    // On macOS, we need to extract the file name manually using dladdr
    Dl_info info;
    if (dladdr(lib_p, &info) != 0) {
        lib_name = info.dli_fname;
    }
#endif

    if (lib_name == NULL) {
        free(time_p);
        return NULL;
    }

    // Use stat to get the file's last modified time
    struct stat attrib;
    if (stat(lib_name, &attrib) == -1) {
        free(time_p);
        return NULL;
    }

    // Convert time_t to broken-down time representation
    struct tm *tm = localtime(&attrib.st_mtime);

    // Format time as "day.month.year hour:minute:seconds"
    strftime(time_p, 127, "%d.%m.%Y %H:%M:%S", tm);
#endif

    return time_p;
}

//// INIT UPDATER ====================================================================================
//
//typedef struct nh_core_loader_ModuleUpdater {
//    double intervalInSeconds;
//    nh_core_SystemTime LastUpdate;
//    bool offline;
//} nh_core_loader_ModuleUpdater;
//
//void *nh_core_initModuleUpdater(
//    void *args_p) 
//{
//    nh_core_loader_ModuleUpdater *Updater_p = nh_core_allocate(sizeof(nh_core_loader_ModuleUpdater));
//    Updater_p->intervalInSeconds = 2.0;
//    Updater_p->LastUpdate = nh_core_getSystemTime();
//    Updater_p->offline = true;
//
//return Updater_p;
//return NULL;
//}
//
//// RUN UPDATER =====================================================================================
//
//typedef NH_API_RESULT (*nh_core_startProcess_f)(
//    int argc, char **argv_pp
//); 
//
//// TODO multithreading
//static nh_core_List nh_killLibDependentForks(
//    char *libName_p)
//{
//    nh_core_List Functions = nh_core_initList(1);
//
//    for (int i = 0; i < NH_MAX_FORKS; ++i) 
//    {
//        nh_Process *Proc_p = &NH_PROCESS_POOL.Forks_p[i];
//        if (Proc_p->id != 0) 
//        {
//            char *libName2_p = nh_core_writeToProcess(Proc_p, "NH_IPC_ID", 10, true);
//            if (libName2_p != NULL && !strcmp(libName_p, libName2_p)) {
//                char *funcName_p = nh_core_writeToProcess(Proc_p, "NH_IPC_UPDATE", 14, true);
//                if (funcName_p != NULL) {
//                    nh_core_appendToList(&Functions, funcName_p);
//                    // wait until fork has terminated
//                    while (Proc_p->id != 0) {
//                        nh_core_checkForks();
//                    }
//                }
//            }
//            free(libName2_p);
//        }
//    }
//
//return Functions;
//}
//
//NH_SIGNAL nh_core_runModuleUpdater(
//    void *args_p)
//{
//    nh_core_loader_ModuleUpdater *Updater_p = args_p;
//    if (nh_core_getSystemTimeDiffInSeconds(Updater_p->LastUpdate, nh_core_getSystemTime()) < Updater_p->intervalInSeconds) {
//        return NH_SIGNAL_OK;
//    }
//
//    for (char *libName_p; libName_p = nh_core_getModifiedLib();)
//    {
//        nh_core_List Functions = nh_killLibDependentForks(libName_p);
//
//        NH_LOADER_CHECK(nh_unload(libName_p))
//        NH_LOADER_CHECK(nh_core_load(libName_p, false))
//
//        for (int i = 0; i < Functions.size; ++i) {
//            NH_LOADER_CHECK(((nh_core_startProcess_f)nh_core_loadSymbol(libName_p, Functions.pp[i]))(0, NULL))
//        }
//    
//        nh_core_freeList(&Functions, true);
//        nh_core_free(libName_p);
//    }
//
//    Updater_p->LastUpdate = nh_core_getSystemTime();
//
//return NH_SIGNAL_OK;
//}
//
