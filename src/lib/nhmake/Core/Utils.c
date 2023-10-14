// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Utils.h"

#include "../UI/Message.h"
#include "../Common/Macros.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <pwd.h>

#if defined(__linux__) || defined(__APPLE__)
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

#if defined(__APPLE__)
    #include <mach-o/dyld.h>
#endif

// DIRECTORY =======================================================================================

static NH_BYTE *procDir_p = NULL;
static NH_BYTE *wrkDir_p = NULL;

NH_BYTE *nh_make_getProcessDirectory()
{
NH_MAKE_BEGIN()
    if (procDir_p) {NH_MAKE_END(procDir_p)}

    int size = 1024;
    NH_BYTE buffer_p[1024] = {0};

#if defined(__linux__)
    if (readlink("/proc/self/exe", buffer_p, size) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, size) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, size) == -1) {NH_MAKE_END(NULL)}
#elif defined(__APPLE__)
    size = 0;
    _NSGetExecutablePath(NULL, &size);
    _NSGetExecutablePath(buffer_p, &size);
#endif

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {NH_MAKE_END(NULL)}
    buffer_p[i] = '\0'; // remove exe name

    procDir_p = malloc(strlen(buffer_p) + 1);
    NH_MAKE_CHECK_NULL_2(NULL, procDir_p)
    sprintf(procDir_p, buffer_p);

NH_MAKE_END(procDir_p)
}

NH_BYTE *nh_make_getWorkDirectory()
{
NH_MAKE_BEGIN()

    if (wrkDir_p) {NH_MAKE_END(wrkDir_p)}

    int size = 1024;
    NH_BYTE buffer_p[1024] = {0};
    getcwd(buffer_p, size);

    wrkDir_p = malloc(strlen(buffer_p) + 1);
    NH_MAKE_CHECK_NULL_2(NULL, wrkDir_p)
    sprintf(wrkDir_p, buffer_p);

NH_MAKE_END(wrkDir_p)
}

const NH_BYTE *nh_make_getHomeDir()
{
NH_MAKE_BEGIN()

    const NH_BYTE *homedir_p = NULL;
    if ((homedir_p = getenv("HOME")) == NULL) {
        homedir_p = getpwuid(getuid())->pw_dir;
    }

NH_MAKE_END(homedir_p)
}

NH_MAKE_RESULT nh_make_createDir(
    NH_BYTE *path_p)
{
NH_MAKE_BEGIN()

    nh_make_messagef("CREATE DIR %s", path_p);
 
    struct stat st = {0};
    if (stat(path_p, &st) == -1) {
        int err = mkdir(path_p, 0755);  // 0755 aka drwxr-xr-x
        if (err) {
            NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_CANT_CREATE_DIRECTORY)
        }
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_removeDir(
    NH_BYTE *path_p)
{
NH_MAKE_BEGIN()

    nh_make_messagef("REMOVE DIR %s", path_p);
 
    NH_BYTE command_p[255];
    sprintf(command_p, "rm -r %s", path_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_CP_EXECUTION_FAILED)}

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// COMMAND =========================================================================================

NH_MAKE_BOOL nh_make_canRunCommand(
    const NH_BYTE *cmd) 
{
NH_MAKE_BEGIN()

    if(strchr(cmd, '/')) {
        // if cmd includes a slash, no path search must be performed,
        // go straight to checking if it's executable
        return access(cmd, X_OK)==0;
    }
    const NH_BYTE *path = getenv("PATH");
    if(!path) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_FALSE)} // something is horribly wrong...
    // we are sure we won't need a buffer any longer
    NH_BYTE *buf = malloc(strlen(path)+strlen(cmd)+3);
    if(!buf) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_FALSE)} // actually useless, see comment
    // loop as long as we have stuff to examine in path
    for(; *path; ++path) {
        // start from the beginning of the buffer
        NH_BYTE *p = buf;
        // copy in buf the current path element
        for(; *path && *path!=':'; ++path,++p) {
            *p = *path;
        }
        // empty path entries are treated like "."
        if(p==buf) *p++='.';
        // slash and command name
        if(p[-1]!='/') *p++='/';
        strcpy(p, cmd);
        // check if we can execute it
        if(access(buf, X_OK)==0) {
            free(buf);
            NH_MAKE_DIAGNOSTIC_END(NH_MAKE_TRUE)
        }
        // quit at last cycle
        if(!*path) break;
    }
    // not found
    free(buf);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_FALSE)
}

