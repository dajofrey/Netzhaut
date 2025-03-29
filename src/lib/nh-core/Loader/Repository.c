// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#define _GNU_SOURCE

#include "Repository.h"

#ifdef __unix__
    #include <link.h>
#endif

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// FUNCTIONS =======================================================================================

static bool nh_canRunCommand(
    const char *cmd) 
{
#ifdef __unix__

    if(strchr(cmd, '/')) {
        return access(cmd, X_OK)==0;;
    }
    const char *path = getenv("PATH");
    if(!path) {return false;}
    char *buf = malloc(strlen(path)+strlen(cmd)+3);
    if(!buf) {return false;}
    for(; *path; ++path) {
        char *p = buf;
        for(; *path && *path!=':'; ++path,++p) {
            *p = *path;
        }
        if(p==buf) *p++='.';
        if(p[-1]!='/') *p++='/';
        strcpy(p, cmd);
        if(access(buf, X_OK)==0) {
            free(buf);
            return true;
        }
        if(!*path) break;
    }
    free(buf);

#endif

    return false;
}

NH_API_RESULT nh_core_getExeDir(
    char *buffer_p, unsigned int size)
{
#ifdef __unix__

    if (readlink("/proc/self/exe", buffer_p, 1024) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, 1024) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, 1024) == -1) {return NH_API_ERROR_BAD_STATE;}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {return NH_API_ERROR_BAD_STATE;}

    buffer_p[i] = '\0'; // remove exe name

#endif

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_downloadnetzhaut()
{
#ifdef __unix__

    if (!nh_canRunCommand("wget"))  {return NH_API_ERROR_BAD_STATE;}
    if (!nh_canRunCommand("unzip")) {return NH_API_ERROR_BAD_STATE;}

    char exeDir_p[2048] = {'\0'};
    if (nh_core_getExeDir(exeDir_p, 2048) != NH_API_SUCCESS) {return NH_API_ERROR_BAD_STATE;}

    char zip_p[2048] = {'\0'};
    sprintf(zip_p, "%s%s", exeDir_p, "netzhaut.zip");

    char command_p[2048] = {'\0'};
    sprintf(command_p, "wget -O %s https://github.com/dajofrey/netzhaut/archive/master.zip", zip_p);
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {return NH_API_ERROR_BAD_STATE;}

    memset(command_p, '\0', sizeof(char) * 2048);
    sprintf(command_p, "unzip -o %s -d %s", zip_p, exeDir_p);
    status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {return NH_API_ERROR_BAD_STATE;}

    remove(zip_p);

#endif

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_buildnhinstaller()
{
#ifdef __unix__

    char command_p[2048] = {'\0'};
    memset(command_p, '\0', sizeof(char) * 2048);
    sprintf(command_p, "./netzhaut-master/build/build-installer.sh");
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {return NH_API_ERROR_BAD_STATE;}

#endif

    return NH_API_SUCCESS;
}
