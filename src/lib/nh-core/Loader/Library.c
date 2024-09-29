// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#define _GNU_SOURCE

#include "Library.h"
#include "Loader.h"
#include "Repository.h"

#include "../System/Thread.h"
#include "../Common/Macros.h"

#include <link.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

// INSTALL =========================================================================================

static bool nh_fileExists(
    char *filename_p)
{
NH_CORE_BEGIN()

    bool fileExists = false;

#ifdef __unix__
    fileExists = access(filename_p, F_OK) != -1 ? true : false;
#endif

NH_CORE_END(fileExists)
}

NH_API_RESULT nh_installLibrary(
    NH_MODULE_E type)
{
NH_CORE_BEGIN()

    if (!NH_LOADER.install) {NH_CORE_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}

    if (!nh_fileExists("netzhaut-master")) {
        NH_CORE_CHECK(nh_downloadnetzhaut())
        NH_CORE_CHECK(nh_core_buildnhinstaller())
    }

#ifdef __unix__

//    char command_p[2048] = {'\0'};
//    memset(command_p, '\0', sizeof(char) * 2048);
//
//    switch (NH_LOADER.scope)
//    {
//        case NH_LOADER_SCOPE_LOCAL :
//            sprintf(command_p, "./netzhaut-master/bin/installer/nhinstaller -l %s", NH_MODULE_NAMES_PP[_module]);
//            break;
//        case NH_LOADER_SCOPE_LOCAL_SYSTEM :
//            sprintf(command_p, "./netzhaut-master/bin/installer/nhinstaller -li %s", NH_MODULE_NAMES_PP[_module]);
//            break;
//        case NH_LOADER_SCOPE_SYSTEM :
//            NH_CORE_END(NH_API_ERROR_BAD_STATE)
//            break;
//    }
//
//    int status = system(command_p);
//    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_CORE_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}

#endif

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// OPEN ============================================================================================

static void *nh_core_getLibraryHandle(
    char *libPath_p)
{
NH_CORE_BEGIN()

    char *error_p;
    dlerror();

    void *dl_p = dlopen(libPath_p, RTLD_NOW | RTLD_GLOBAL);

    int try = 0;
    while (dl_p == NULL && try++ < 100) {
        if ((error_p = dlerror()) != NULL) { 
            fprintf(stderr, "%s\n", error_p);
        }
        nh_sleepMs(50);
        dl_p = dlopen(libPath_p, RTLD_NOW | RTLD_GLOBAL);
    }

    if (dl_p == NULL) {
        exit(0);
    }

NH_CORE_END(dl_p)
}

void *nh_core_loadLibrary(
    NH_MODULE_E type, char *path_p)
{
NH_CORE_BEGIN()

    void *lib_p = NULL;
    char libPath_p[255] = {0};

    if (path_p) {
        sprintf(libPath_p, "%s/lib%s.so", path_p, NH_MODULE_NAMES_PP[type]);
        lib_p = nh_core_getLibraryHandle(libPath_p);
    } else {
        sprintf(libPath_p, "lib%s.so", NH_MODULE_NAMES_PP[type]);
        lib_p = nh_core_getLibraryHandle(libPath_p);
    }

//    if (lib_p == NULL && NH_LOADER.install) {
//        if (nh_installLibrary(type) != NH_API_SUCCESS) {NH_CORE_END(NULL)}
//        NH_CORE_END(nh_core_loadLibrary(type, NULL))
//    }

NH_CORE_END(lib_p)
}

void *nh_core_loadExternalLibrary(
    char *name_p, char *path_p)
{
NH_CORE_BEGIN()

    void *lib_p = NULL;

#ifdef __unix__

    if (!path_p) {
        char libPath_p[255];
        sprintf(libPath_p, "lib%s.so", name_p);
        lib_p = nh_core_getLibraryHandle(libPath_p);
        NH_CORE_END(lib_p)
    }

    char path2_p[255];
    strcpy(path2_p, path_p);

    char *p = strtok(path2_p, ":");
    while (p != NULL && !lib_p) {
        char libPath_p[255];
        sprintf(libPath_p, "%s/lib%s.so", p, name_p);
        lib_p = nh_core_getLibraryHandle(libPath_p);
 	p = strtok(NULL, ":");
    }
    NH_CORE_END(lib_p)

#endif

NH_CORE_END(NULL)
}

void *nh_core_loadSymbolFromLibrary(
    void *lib_p, const char *name_p)
{
NH_CORE_BEGIN()

#ifdef __unix__

    char *error_p = NULL;
    dlerror(); // clear any existing error

    void *function_p = dlsym(lib_p, name_p);
    if ((error_p = dlerror()) != NULL) {NH_CORE_END(NULL)}

#endif

NH_CORE_END(function_p)
}

// CLOSE ===========================================================================================

NH_API_RESULT nh_unloadLibrary(
    void *lib_p)
{
NH_CORE_BEGIN()

#ifdef __unix__

    if (dlclose(lib_p)) {
        NH_CORE_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
    }

#endif

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

