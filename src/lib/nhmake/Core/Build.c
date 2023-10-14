// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Build.h"
#include "Utils.h"

#include "../UI/Message.h"
#include "../Common/Macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>

// CREATE BINARY ===================================================================================

static NH_MAKE_RESULT nh_make_createBinary(
    nh_make_Runtime *Runtime_p, nh_make_SourceContext *SourceContext_p)
{
NH_MAKE_BEGIN()

    chdir(SourceContext_p->path_p);

    NH_BYTE binPath_p[256] = {'\0'};
    sprintf(binPath_p, "%s/%s", nh_make_getVariable(&Runtime_p->VariableArray, "BIN_DEST")->values_pp[0], SourceContext_p->name_p);
    if (SourceContext_p->outputPath_p) {
        sprintf(binPath_p, "%s/%s", SourceContext_p->outputPath_p, SourceContext_p->name_p);
    }

    int length = 0;
    for (int i = 0; i < Runtime_p->SourceArray.length; ++i) {
        nh_make_Source *Source_p = &Runtime_p->SourceArray.Sources_p[i];
        if (Source_p->Context_p == SourceContext_p) {
            length += strlen(Source_p->path_p) + 1;
        }
    }
   
    NH_BYTE *sources_p = malloc(length + 1);
    NH_MAKE_CHECK_NULL(sources_p)
    memset(sources_p, 0, length + 1);

    int offset = 0;
    for (int i = 0; i < Runtime_p->SourceArray.length; ++i) {
        nh_make_Source *Source_p = &Runtime_p->SourceArray.Sources_p[i];
        if (Source_p->Context_p == SourceContext_p) {
            sprintf(sources_p + strlen(sources_p), "%s ", Source_p->path_p);
        }
    }

    int compileArgsLength = SourceContext_p->compileArgs_p ? strlen(SourceContext_p->compileArgs_p) : 0;
    int linkArgsLength = SourceContext_p->linkArgs_p ? strlen(SourceContext_p->linkArgs_p) : 0;
    int commandLength = strlen(sources_p) + compileArgsLength + linkArgsLength + strlen(binPath_p) + 128;
    NH_BYTE *command_p = malloc(commandLength);
    NH_MAKE_CHECK_NULL(command_p)
    memset(command_p, 0, commandLength);

    NH_BYTE empty = 0;
    NH_BYTE *linkArgs_p = SourceContext_p->linkArgs_p ? SourceContext_p->linkArgs_p : &empty;
    NH_BYTE *compileArgs_p = SourceContext_p->compileArgs_p ? SourceContext_p->compileArgs_p : &empty;

    // set -no-pie because of https://stackoverflow.com/questions/41398444/gcc-creates-mime-type-application-x-sharedlib-instead-of-application-x-applicati
    sprintf(command_p, "gcc %s -o%s -no-pie %s %s", compileArgs_p, binPath_p, sources_p, linkArgs_p);
    nh_make_messagef(command_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_GCC_EXECUTION_FAILED)}

    chdir(nh_make_getVariable(&Runtime_p->VariableArray, "WRK_DIR")->values_pp[0]);

    free(command_p);
    free(sources_p);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// CREATE LIBRARY ==================================================================================

static NH_MAKE_RESULT nh_make_createSharedLibraryUsingGCC(
    const NH_BYTE *objects_p, const NH_BYTE *out_p, NH_BYTE *compile_p, const NH_BYTE *link_p)
{
NH_MAKE_BEGIN()

    static NH_BYTE command_p[16384] = {'\0'};

    sprintf(command_p, "gcc -shared %s %s %s -o %s", objects_p, compile_p, link_p, out_p);
    nh_make_messagef(command_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_GCC_EXECUTION_FAILED)}

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_createLibrary(
    nh_make_Runtime *Runtime_p, nh_make_SourceContext *Context_p)
{
NH_MAKE_BEGIN()

    NH_MAKE_RESULT result = NH_MAKE_SUCCESS;

    NH_BYTE empty = 0;
    NH_BYTE *libName_p = Context_p->name_p;
    NH_BYTE *linkArgs_p = Context_p->linkArgs_p ? Context_p->linkArgs_p : &empty;
    NH_BYTE *compileArgs_p = Context_p->compileArgs_p ? Context_p->compileArgs_p : &empty;

    int api   = Context_p->Version.api; 
    int major = Context_p->Version.major; 
    int minor = Context_p->Version.minor; 
    int patch = Context_p->Version.patch;

    NH_BYTE tmp_p[255] = {0};
    sprintf(tmp_p, "/tmp/%s", nh_make_getVariable(&Runtime_p->VariableArray, "NAME")->values_pp[0]);
    chdir(tmp_p);

#if defined(__linux__) || defined(__APPLE__)

    int index = 0;
    NH_BYTE objects_p[16384];
    memset(objects_p, 0, 16384);

    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(tmp_p)) != NULL) 
    {
        while ((ent = readdir(dir)) != NULL) {
            NH_BOOL space = NH_FALSE;
            for (int i = 0; ent->d_name[i] != '\0' && ent->d_name[0] != '.'; ++i) {
                objects_p[index++] = ent->d_name[i];
                space = NH_TRUE;
            }
            if (space) {
                objects_p[index++] = ' ';
            }
        }
        closedir(dir);
    } 
    else {
        result = NH_MAKE_ERROR_CANT_OPEN_DIR;
        goto NH_MAKE_CREATELIBRARY_END;
    }

    if (!strlen(objects_p)) {
        goto NH_MAKE_CREATELIBRARY_END;
    }

    NH_BYTE *dest_p = nh_make_getVariable(&Runtime_p->VariableArray, "LIB_DEST")->values_pp[0];
    if (Context_p->outputPath_p) {dest_p = Context_p->outputPath_p;}

    NH_BYTE libPath_p[256] = {'\0'};
    sprintf(libPath_p, "%s/lib%s.so.%d.%d.%d.%d", dest_p, libName_p, api, major, minor, patch);
    NH_BYTE symPath1_p[256] = {'\0'};
    sprintf(symPath1_p, "%s/lib%s.so.%d", dest_p, libName_p, api);
    NH_BYTE symPath2_p[256] = {'\0'};
    sprintf(symPath2_p, "%s/lib%s.so", dest_p, libName_p);
  
    result = nh_make_createSharedLibraryUsingGCC(objects_p, libPath_p, compileArgs_p, linkArgs_p);
    if (result ) {
        goto NH_MAKE_CREATELIBRARY_END;
    }

    memset(libPath_p, 0, 256); 
    sprintf(libPath_p, "lib%s.so.%d.%d.%d.%d", libName_p, api, major, minor, patch);
    nh_make_createSymLink(libPath_p, symPath1_p, NH_MAKE_FALSE);
    nh_make_createSymLink(libPath_p, symPath2_p, NH_MAKE_FALSE);

#endif 

NH_MAKE_CREATELIBRARY_END:

    chdir(nh_make_getVariable(&Runtime_p->VariableArray, "WRK_DIR")->values_pp[0]);

NH_MAKE_DIAGNOSTIC_END(result)
}

// COMPILE =========================================================================================

static void nh_make_getObjectFileName(
    nh_make_Source *Source_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    NH_BYTE path_p[255] = {0};
    strcpy(path_p, Source_p->path_p);

    int index = strlen(path_p) - 1;
    while (path_p[index] != '/' && index) {index--;}

    NH_BYTE fileName_p[255] = {0};
    strcpy(fileName_p, index ? path_p + index + 1 : path_p);

    int tmp = index ? index - 1 : 0;
    while (path_p[tmp] != '/' && tmp) {tmp--;}

    if (index) {
        path_p[index] = '\0';
        sprintf(name_p, "%s%s.o", path_p + tmp, fileName_p);
    }
    else {
        sprintf(name_p, "%s.o", fileName_p);
    }

NH_MAKE_SILENT_END()
}

static NH_MAKE_RESULT nh_make_createPICObjectFileUsingGCC(
    const NH_BYTE *in_p, const NH_BYTE *out_p, NH_BYTE *compileArgs_p)
{
NH_MAKE_BEGIN()

    NH_BYTE realout_p[1024] = {'\0'};
    realpath(out_p, realout_p);

    NH_BYTE command_p[1024] = {'\0'};

    if (compileArgs_p != NULL) {sprintf(command_p, "gcc -fPIC %s -c %s -o %s", compileArgs_p, in_p, out_p);}
    else {sprintf(command_p, "gcc -fPIC -c %s -o %s", in_p, out_p);}

    nh_make_messagef(command_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_GCC_EXECUTION_FAILED)}

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_compileFiles(
    nh_make_Runtime *Runtime_p, nh_make_SourceContext *Context_p)
{
NH_MAKE_BEGIN()

    chdir(Context_p->path_p);

    for (int i = 0; i < Runtime_p->SourceArray.length; ++i) 
    {
        nh_make_Source *Source_p = &Runtime_p->SourceArray.Sources_p[i];

        if (Source_p->Context_p == Context_p) 
        {
            NH_BYTE tmp_p[255] = {0};
            NH_BYTE fileName_p[255] = {0};
            nh_make_getObjectFileName(Source_p, fileName_p);

            sprintf(tmp_p, "/tmp/%s/%s", nh_make_getVariable(&Runtime_p->VariableArray, "NAME")->values_pp[0], fileName_p);
            NH_BYTE empty = 0;

            if (Context_p->type == NH_MAKE_SOURCE_CONTEXT_SHARED_LIBRARY) {
                NH_MAKE_CHECK(nh_make_createPICObjectFileUsingGCC(
                        Source_p->path_p, tmp_p, Context_p->compileArgs_p ? Context_p->compileArgs_p : &empty
                ))
            }
        }
    }

    chdir(nh_make_getVariable(&Runtime_p->VariableArray, "WRK_DIR")->values_pp[0]);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// INSTALL =========================================================================================

//static NH_MAKE_RESULT nh_make_installLibrary(
//    NH_BYTE *libName_p, int major, int minor, int patch)
//{
//NH_MAKE_BEGIN()
//
//    NH_BYTE wrkDir_p[2048] = {'\0'};
//    NH_MAKE_CHECK_NULL(NH_MAKE_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))
//
//    NH_BYTE projDir_p[2048] = {'\0'};
//    NH_MAKE_CHECK(NH_MAKE_ERROR_GET_PROJECT_DIRECTORY, nh_make_getProjectDir(projDir_p, 2048))
//
//    chdir(projDir_p);
//
//    NH_BYTE libPath_p[512] = {'\0'};
//    sprintf(libPath_p, "lib/lib%s.so.%d.%d.%d", libName_p, major, minor, patch);
//
//    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy(libPath_p, "/usr/local/lib", NH_MAKE_FALSE, NH_MAKE_TRUE))
//
//    chdir("/usr/local/lib");
//
//    NH_BYTE cpyPath_p[256] = {'\0'};
//    sprintf(cpyPath_p, "lib%s.so.%d.%d.%d", libName_p, major, minor, patch);
//    NH_BYTE symPath1_p[256] = {'\0'};
//    sprintf(symPath1_p, "lib%s.so.%d", libName_p, major);
//    NH_BYTE symPath2_p[256] = {'\0'};
//    sprintf(symPath2_p, "lib%s.so", libName_p);
//
//    nh_make_createSymLink(cpyPath_p, symPath1_p, NH_MAKE_TRUE);
//    nh_make_createSymLink(cpyPath_p, symPath2_p, NH_MAKE_TRUE);
//
//    chdir(wrkDir_p);
//
//NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
//}

// BUILD ===========================================================================================

static NH_MAKE_RESULT nh_make_buildSourceContext(
    nh_make_Runtime *Runtime_p, nh_make_SourceContext *Context_p) 
{
NH_MAKE_BEGIN()

    NH_BYTE empty = 0;
    NH_BYTE tmp_p[255];
    sprintf(tmp_p, "/tmp/%s", nh_make_getVariable(&Runtime_p->VariableArray, "NAME")->values_pp[0]);

    // Make sure that we start with a fresh/empty directory.
    nh_make_removeDir(tmp_p);
    NH_MAKE_CHECK(nh_make_createDir(tmp_p))

    if (Runtime_p->beforeBuildCallback_f) {
        NH_MAKE_CHECK(Runtime_p->beforeBuildCallback_f(Runtime_p, Context_p))
    }

    if (Context_p->type == NH_MAKE_SOURCE_CONTEXT_BINARY) {
        NH_MAKE_CHECK(nh_make_createBinary(Runtime_p, Context_p))
    }
    else {
        NH_MAKE_CHECK(nh_make_compileFiles(Runtime_p, Context_p))
        NH_MAKE_CHECK(nh_make_createLibrary(Runtime_p, Context_p))
    }

    if (Runtime_p->afterBuildCallback_f) {
        NH_MAKE_CHECK(Runtime_p->afterBuildCallback_f(Runtime_p, Context_p))
    }

//    if (install) {
//        NH_MAKE_CHECK(NH_MAKE_ERROR_BAD_STATE, nh_make_installLibrary(
//            Library_p->name_p, 0, 0, 0 
//        ))
//    }

    NH_MAKE_CHECK(nh_make_removeDir(tmp_p))

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_build(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    for (int i = 0; i < Runtime_p->SourceContextArray.length; ++i) {
        nh_make_SourceContext *SourceContext_p = &Runtime_p->SourceContextArray.SourceContexts_p[i];
        if (SourceContext_p->name_p && !strcmp(SourceContext_p->name_p, name_p)) {
            nh_make_messagef(SourceContext_p->type == NH_MAKE_SOURCE_CONTEXT_BINARY ? "-- BINARY --" : "-- LIBRARY --");
            nh_make_messagef("");
            NH_MAKE_CHECK(nh_make_buildSourceContext(Runtime_p, SourceContext_p))
        }
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

