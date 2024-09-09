// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#define _GNU_SOURCE

#include "Repository.h"

#include "../Common/Macros.h"

#include <link.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// DOWNLOAD REPOSITORY =============================================================================

static NH_BOOL nh_canRunCommand(
    const NH_BYTE *cmd) 
{
NH_CORE_BEGIN()

#ifdef __unix__

    if(strchr(cmd, '/')) {
        NH_CORE_END(access(cmd, X_OK)==0);
    }
    const NH_BYTE *path = getenv("PATH");
    if(!path) {NH_CORE_END(NH_FALSE)}
    NH_BYTE *buf = malloc(strlen(path)+strlen(cmd)+3);
    if(!buf) {NH_CORE_END(NH_FALSE)}
    for(; *path; ++path) {
        NH_BYTE *p = buf;
        for(; *path && *path!=':'; ++path,++p) {
            *p = *path;
        }
        if(p==buf) *p++='.';
        if(p[-1]!='/') *p++='/';
        strcpy(p, cmd);
        if(access(buf, X_OK)==0) {
            free(buf);
            NH_CORE_END(NH_TRUE)
        }
        if(!*path) break;
    }
    free(buf);

#endif

NH_CORE_END(NH_FALSE)
}

NH_CORE_RESULT nh_core_getExeDir(
    NH_BYTE *buffer_p, unsigned int size)
{
NH_CORE_BEGIN()

#ifdef __unix__

    if (readlink("/proc/self/exe", buffer_p, 1024) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, 1024) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, 1024) == -1) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}

    buffer_p[i] = '\0'; // remove exe name

#endif

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_downloadnetzhaut()
{
NH_CORE_BEGIN()

#ifdef __unix__

    if (!nh_canRunCommand("wget"))  {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}
    if (!nh_canRunCommand("unzip")) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}

    NH_BYTE exeDir_p[2048] = {'\0'};
    if (nh_core_getExeDir(exeDir_p, 2048) != NH_CORE_SUCCESS) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}

    NH_BYTE zip_p[2048] = {'\0'};
    sprintf(zip_p, "%s%s", exeDir_p, "netzhaut.zip");

    NH_BYTE command_p[2048] = {'\0'};
    sprintf(command_p, "wget -O %s https://github.com/dajofrey/netzhaut/archive/master.zip", zip_p);
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}

    memset(command_p, '\0', sizeof(NH_BYTE) * 2048);
    sprintf(command_p, "unzip -o %s -d %s", zip_p, exeDir_p);
    status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}

    remove(zip_p);

#endif

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

// BUILD INSTALLER =================================================================================

NH_CORE_RESULT nh_core_buildnhinstaller()
{
NH_CORE_BEGIN()

#ifdef __unix__

    NH_BYTE command_p[2048] = {'\0'};
    memset(command_p, '\0', sizeof(NH_BYTE) * 2048);
    sprintf(command_p, "./netzhaut-master/build/build-installer.sh");
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}

#endif

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}
