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

#if defined(__linux__)
    #include <link.h>
#elif defined(__APPLE__)
    #include <dlfcn.h>
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

// FUNCTIONS =======================================================================================

static bool nh_fileExists(
    char *filename_p)
{
    bool fileExists = false;

#ifdef __unix__
    fileExists = access(filename_p, F_OK) != -1 ? true : false;
#endif

    return fileExists;
}

NH_API_RESULT nh_installLibrary(
    NH_MODULE_E type)
{
    if (!NH_LOADER.install) {return NH_API_ERROR_BAD_STATE;}

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
//            return NH_API_ERROR_BAD_STATE;
//            break;
//    }
//
//    int status = system(command_p);
//    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {return NH_API_ERROR_BAD_STATE;}

#endif

    return NH_API_SUCCESS;
}

static void *nh_core_getLibraryHandle(
    char *libPath_p)
{
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

    return dl_p;
}

void *nh_core_loadLibrary(
    NH_MODULE_E type, char *path_p)
{
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
//        if (nh_installLibrary(type) != NH_API_SUCCESS) {return NULL;}
//        return nh_core_loadLibrary(type, NULL);
//    }

    return lib_p;
}

void *nh_core_loadExternalLibrary(
    char *name_p, char *path_p)
{
    void *lib_p = NULL;

#ifdef __unix__

    if (!path_p) {
        char libPath_p[255];
        sprintf(libPath_p, "lib%s.so", name_p);
        lib_p = nh_core_getLibraryHandle(libPath_p);
        return lib_p;
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
    return lib_p;

#endif

    return NULL;
}

void *nh_core_loadSymbolFromLibrary(
    void *lib_p, const char *name_p)
{
#if defined(__unix__) || defined(__APPLE__)

    char *error_p = NULL;
    dlerror(); // clear any existing error

    void *function_p = dlsym(lib_p, name_p);
    if ((error_p = dlerror()) != NULL) {return NULL;}

#endif

    return function_p;
}

NH_API_RESULT nh_unloadLibrary(
    void *lib_p)
{
#if defined(__unix__) || defined(__APPLE__)

    if (dlclose(lib_p)) {
        return NH_API_ERROR_BAD_STATE;
    }

#endif

    return NH_API_SUCCESS;
}
