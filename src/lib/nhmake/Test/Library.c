// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Library.h"

#include "../Common/Macros.h"

#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#if defined(__linux__) || defined(__APPLE__)
    #include <sys/wait.h>
#endif

// TEST ============================================================================================

void *nh_make_openLibrary(
    NH_BYTE *path_p)
{
#if defined(__linux__) || defined(__APPLE__)

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

void nh_make_closeLibrary(
    void *lib_p)
{
#if defined(__linux__) || defined(__APPLE__)

    dlclose(lib_p);

#endif
}

NH_MAKE_RESULT nh_make_getExeDir(
    NH_BYTE *buffer_p, size_t size)
{
#if defined(__linux__)
    if (readlink("/proc/self/exe", buffer_p, size) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, size) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, size) == -1) {NH_MAKE_END(NH_MAKE_ERROR_BAD_STATE)}
#elif defined(__APPLE__)
    size = 0;
    _NSGetExecutablePath(NULL, &size);
    _NSGetExecutablePath(buffer_p, &size);

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {return NH_MAKE_ERROR_BAD_STATE;}

    buffer_p[i] = '\0'; // remove exe name
    return NH_MAKE_SUCCESS;

#endif
}

void *nh_make_loadSymbol(
    void *lib_p, const NH_BYTE *name_p)
{
#if defined(__linux__) || defined(__APPLE__)

    NH_BYTE *error_p;
    dlerror(); // clear any existing error

    void *function_p = dlsym(lib_p, name_p); 
    if ((error_p = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error_p);
        return NULL;
    }

    return function_p;

#endif
}

