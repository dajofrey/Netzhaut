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

static NH_BOOL nh_fileExists(
    NH_BYTE *filename_p)
{
NH_CORE_BEGIN()

    NH_BOOL fileExists = NH_FALSE;

#ifdef __unix__
    fileExists = access(filename_p, F_OK) != -1 ? NH_TRUE : NH_FALSE;
#endif

NH_CORE_END(fileExists)
}

NH_CORE_RESULT nh_installLibrary(
    NH_MODULE_E _module)
{
NH_CORE_BEGIN()

    if (!NH_LOADER.install) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}

    if (!nh_fileExists("netzhaut-master")) {
        NH_CORE_CHECK(nh_downloadnetzhaut())
        NH_CORE_CHECK(nh_core_buildnhinstaller())
    }

#ifdef __unix__

    NH_BYTE command_p[2048] = {'\0'};
    memset(command_p, '\0', sizeof(NH_BYTE) * 2048);

    switch (NH_LOADER.scope)
    {
        case NH_LOADER_SCOPE_LOCAL :
            sprintf(command_p, "./netzhaut-master/bin/installer/nhinstaller -l %s", NH_MODULE_NAMES_PP[_module]);
            break;
        case NH_LOADER_SCOPE_LOCAL_SYSTEM :
            sprintf(command_p, "./netzhaut-master/bin/installer/nhinstaller -li %s", NH_MODULE_NAMES_PP[_module]);
            break;
        case NH_LOADER_SCOPE_SYSTEM :
            NH_CORE_END(NH_CORE_ERROR_BAD_STATE)
            break;
    }

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}

#endif

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

// OPEN ============================================================================================

static void *nh_core_getLibraryHandle(
    NH_BYTE *libPath_p)
{
NH_CORE_BEGIN()

    NH_BYTE *error_p;
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
    NH_MODULE_E _module, int major)
{
NH_CORE_BEGIN()

#ifdef __unix__

    void *lib_p = NULL;

    switch (NH_LOADER.scope)
    {
        case NH_LOADER_SCOPE_LOCAL :
        {
            NH_BYTE exeDir_p[2048] = {'\0'};
            if (nh_core_getExeDir(exeDir_p, 2048) != NH_CORE_SUCCESS) {NH_CORE_END(NULL)}
            NH_BYTE libPath_p[2048] = {'\0'};

            if (major == -1) {
                sprintf(libPath_p, "%s/netzhaut-master/lib/lib%s.so", exeDir_p, NH_MODULE_NAMES_PP[_module]);
            }
            else {
                sprintf(libPath_p, "%s/netzhaut-master/lib/lib%s.so.%d", exeDir_p, NH_MODULE_NAMES_PP[_module], major);
            }

            lib_p = nh_core_getLibraryHandle(libPath_p);
        }

        case NH_LOADER_SCOPE_LOCAL_SYSTEM :
        {
            NH_BYTE libPath_p[255] = {'\0'};

            if (major == -1) {
                sprintf(libPath_p, "/usr/local/lib/lib%s.so", NH_MODULE_NAMES_PP[_module]);
            }
            else {
                sprintf(libPath_p, "/usr/local/lib/lib%s.so.%d", NH_MODULE_NAMES_PP[_module], major);
            }

            lib_p = nh_core_getLibraryHandle(libPath_p);
        }

        case NH_LOADER_SCOPE_SYSTEM :
        {
            NH_BYTE libPath_p[255] = {'\0'};

            if (major == -1) {
                sprintf(libPath_p, "lib%s.so", NH_MODULE_NAMES_PP[_module]);
            }
            else {
                sprintf(libPath_p, "lib%s.so.%d", NH_MODULE_NAMES_PP[_module], major);
            }

            lib_p = nh_core_getLibraryHandle(libPath_p);
        }
    }

#endif

    if (lib_p == NULL && NH_LOADER.install) {
        if (nh_installLibrary(_module) != NH_CORE_SUCCESS) {NH_CORE_END(NULL)}
        NH_CORE_END(nh_core_loadLibrary(_module, major))
    }

NH_CORE_END(lib_p)
}

void *nh_core_loadSymbolFromLibrary(
    void *lib_p, const NH_BYTE *name_p)
{
NH_CORE_BEGIN()

#ifdef __unix__

    NH_BYTE *error_p = NULL;
    dlerror(); // clear any existing error

    void *function_p = dlsym(lib_p, name_p);
    if ((error_p = dlerror()) != NULL) {NH_CORE_END(NULL)}

#endif

NH_CORE_END(function_p)
}

// CLOSE ===========================================================================================

NH_CORE_RESULT nh_unloadLibrary(
    void *lib_p)
{
NH_CORE_BEGIN()

#ifdef __unix__

    if (dlclose(lib_p)) {
        NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)
    }

#endif

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

