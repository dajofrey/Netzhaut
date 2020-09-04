// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Utils.h"
#include "../Header/Macros.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <pwd.h>

#ifdef __unix__
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

// DATA ============================================================================================

int NH_ERROR = 0;

// TOP DIR =========================================================================================

NH_RESULT Nh_Bld_getTopDirectory(
    char *buffer_p, size_t size)
{
NH_BEGIN()

    if (readlink("/proc/self/exe", buffer_p, 1024) == -1 
    &&  readlink("/proc/curproc/file", buffer_p, 1024) == -1
    &&  readlink("/proc/self/path/a.out", buffer_p, 1024) == -1) {NH_END(NH_ERROR_BAD_STATE)}

    int i;
    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {NH_END(NH_ERROR_BAD_STATE)}

    buffer_p[i] = '\0';

    for (i = strlen(buffer_p); i > -1 && buffer_p[i] != '/'; --i) {}
    if (i == -1) {NH_END(NH_ERROR_BAD_STATE)}

    buffer_p[i] = '\0';

NH_END(NH_SUCCESS)
}

NH_BOOL Nh_Bld_canRunCommand(
    const char *cmd) 
{
NH_BEGIN()

    if(strchr(cmd, '/')) {
        // if cmd includes a slash, no path search must be performed,
        // go straight to checking if it's executable
        return access(cmd, X_OK)==0;
    }
    const char *path = getenv("PATH");
    if(!path) {NH_END(NH_FALSE)} // something is horribly wrong...
    // we are sure we won't need a buffer any longer
    char *buf = malloc(strlen(path)+strlen(cmd)+3);
    if(!buf) {NH_END(NH_FALSE)} // actually useless, see comment
    // loop as long as we have stuff to examine in path
    for(; *path; ++path) {
        // start from the beginning of the buffer
        char *p = buf;
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
            NH_END(NH_TRUE)
        }
        // quit at last cycle
        if(!*path) break;
    }
    // not found
    free(buf);

NH_END(NH_FALSE)
}

NH_BOOL Nh_Bld_canFindSharedLib(
    const char *lib_p)
{
NH_BEGIN()

    char path_p[1024] = {'\0'};
    sprintf(path_p, "/usr/lib/lib%s.so", lib_p);

    if(access(path_p, F_OK) != -1) {NH_END(NH_TRUE)}

NH_END(NH_FALSE)
}

NH_RESULT Nh_Bld_createDirectory(
    const char *path_p)
{
NH_BEGIN()
 
    struct stat st = {0};
    if (stat(path_p, &st) == -1) {
        if (mkdir(path_p, 0755) != 0) { // 0755 aka drwxr-xr-x
            NH_END(NH_BUILD_ERROR_CANT_CREATE_DIRECTORY)
        }
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Bld_createPICObjectFileUsingGCC(
    const char *in_p, const char *out_p, char *extra_p)
{
NH_BEGIN()

    char realout_p[1024] = {'\0'};
    realpath(out_p, realout_p);
    printf("NETZHAUT: CREATE %s\n", realout_p);

    char command_p[1024] = {'\0'};

    if (extra_p != NULL) {sprintf(command_p, "gcc -fPIC %s -c %s -o %s", extra_p, in_p, out_p);}
    else {sprintf(command_p, "gcc -fPIC -c %s -o %s", in_p, out_p);}
    
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_GCC_EXECUTION_FAILED)}

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Bld_copy(
    char *in_p, char *out_p, NH_BOOL recursive, NH_BOOL toAbsolute, NH_BOOL sudo)
{
NH_BEGIN()

    char realin_p[1024] = {'\0'}, realout_p[1024] = {'\0'};
    realpath(out_p, realout_p);
    realpath(in_p, realin_p);
    printf(sudo ? "NETZHAUT: COPY %s TO %s \e[1;31mSUDO PREPENDED\e[0m\n" : "NETZHAUT: COPY %s TO %s\n", realin_p, realout_p);

#include NH_CUSTOM_CHECK

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

#include NH_DEFAULT_CHECK

    if (!Nh_Bld_canRunCommand("cp")) {NH_END(NH_BUILD_ERROR_CP_NOT_FOUND)}

#ifdef __unix__

    char command_p[32] = {'\0'};
    sprintf(command_p, sudo ? "sudo cp" : "cp");
    char fullCommand_p[1024] = {'\0'};

    if (!toAbsolute) {
        sprintf(fullCommand_p, recursive ? "%s -rp %s/%s %s/%s" : "%s -p %s/%s %s/%s", command_p, topDir_p, in_p, topDir_p, out_p);
    }
    else {
        sprintf(fullCommand_p, recursive ? "%s -rp %s/%s %s" : "%s -p %s/%s %s", command_p, topDir_p, in_p, out_p);
    }

    int status = system(fullCommand_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_CP_EXECUTION_FAILED)}

#elif defined(WIN_32)

#endif

NH_END(NH_SUCCESS)
}

const char *Nh_Bld_getHomeDirectory()
{
NH_BEGIN()

    const char *homedir_p = NULL;
    if ((homedir_p = getenv("HOME")) == NULL) {
        homedir_p = getpwuid(getuid())->pw_dir;
    }

NH_END(homedir_p)
}

