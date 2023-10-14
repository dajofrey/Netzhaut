// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "File.h"
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

// GENERAL FILE RELATED OPERATIONS =================================================================

NH_MAKE_BOOL nh_make_canFindSharedLib(
    const NH_BYTE *lib_p)
{
NH_MAKE_BEGIN()

    NH_BYTE path_p[1024] = {'\0'};
    sprintf(path_p, "/usr/lib/lib%s.so", lib_p);

    if(access(path_p, F_OK) != -1) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_TRUE)}

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_FALSE)
}

NH_MAKE_RESULT nh_make_copy(
    nh_make_VariableArray *Array_p, NH_BYTE *in_p, NH_BYTE *out_p, NH_MAKE_BOOL recursive, NH_MAKE_BOOL sudo)
{
NH_MAKE_BEGIN()

    NH_BYTE realin_p[1024] = {'\0'}, realout_p[1024] = {'\0'};
    realpath(out_p, realout_p);
    realpath(in_p, realin_p);
    nh_make_messagef(sudo ? "COPY %s TO %s \e[1;31mSUDO PREPENDED\e[0m" : "COPY %s TO %s", realin_p, realout_p);

    if (!nh_make_canRunCommand("cp")) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_CP_NOT_FOUND)}

#if defined(__linux__) || defined(__APPLE__)

    NH_BYTE command_p[32] = {'\0'};
    sprintf(command_p, sudo ? "sudo cp" : "cp");
    NH_BYTE fullCommand_p[1024] = {'\0'};

    if (out_p[0] != '/') {
        sprintf(fullCommand_p, recursive ? "%s -rp %s %s" : "%s -p %s %s", command_p, in_p, out_p);
    }
    else {
        sprintf(fullCommand_p, recursive ? "%s -rp %s %s" : "%s -p %s %s", command_p, in_p, out_p);
    }

    int status = system(fullCommand_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_CP_EXECUTION_FAILED)}

#elif defined(WIN_32)

#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_createSymLink(
    NH_BYTE *filePath_p, NH_BYTE *symLinkPath_p, NH_MAKE_BOOL sudo)
{
NH_MAKE_BEGIN()

#if defined(__linux__) || defined(__APPLE__)

    if (!nh_make_canRunCommand("ln")) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_CP_NOT_FOUND)}

    NH_BYTE command_p[64] = {'\0'};
    sprintf(command_p, sudo ? "sudo ln -s" : "ln -s");

    NH_BYTE fullCommand_p[1024] = {'\0'};
    sprintf(fullCommand_p, "%s %s %s", command_p, filePath_p, symLinkPath_p);

    int status = system(fullCommand_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_CP_EXECUTION_FAILED)}

#elif defined(WIN_32)

#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_BOOL nh_make_fileExists(
    NH_BYTE *filename_p)
{
NH_MAKE_BEGIN()
NH_MAKE_END(access(filename_p, F_OK) != -1 ? NH_MAKE_TRUE : NH_MAKE_FALSE)
}

// FILE STRUCTURE ==================================================================================

nh_make_File nh_make_initFile()
{
NH_MAKE_BEGIN()

    nh_make_File File;
    File.path_p = NULL;
    File.data_p = NULL;

NH_MAKE_END(File)
}

void nh_make_freeFileData(
    nh_make_File *File_p)
{
NH_MAKE_BEGIN()

    free(File_p->path_p);
    free(File_p->data_p);

NH_MAKE_SILENT_END()
}

static NH_BYTE *nh_make_getFileData(
    const NH_BYTE* path_p, long *size_p)
{
NH_MAKE_BEGIN()

    FILE *fh = fopen(path_p, "rb");
    if (fh == NULL) {NH_MAKE_END(NULL)}
    
    if (fseek(fh, 0, SEEK_END) != 0) {NH_MAKE_END(NULL)}
    long size = ftell(fh);
    rewind(fh);

    if(size <= 0) {
        fclose(fh);
        NH_MAKE_END(NULL)
    }

    NH_BYTE *data_p = malloc(size+1); 
    if (data_p == NULL) {NH_MAKE_END(NULL)}
    
    memset(data_p, 0, size+1);
    fread(data_p, 1, size, fh);
    fclose(fh);

    data_p[size] = 0;
    if (size_p != NULL) {*size_p = size;}

NH_MAKE_END(data_p)
}

static NH_MAKE_RESULT nh_make_setFileData(
    nh_make_File *File_p, NH_BYTE *path_p)
{
NH_MAKE_BEGIN()

    if (!File_p || !path_p) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)}

    File_p->data_p = nh_make_getFileData(path_p, NULL);
    NH_MAKE_CHECK_NULL(File_p->data_p)

    File_p->path_p = malloc(strlen(path_p) + 1);
    NH_MAKE_CHECK_NULL(File_p->path_p)

    strcpy(File_p->path_p, path_p);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// FILE STRUCTURE ARRAY ============================================================================

void nh_make_initFileArray(
    nh_make_FileArray *Array_p)
{
NH_MAKE_BEGIN()

    Array_p->length = 0;
    Array_p->Files_p = NULL;

NH_MAKE_SILENT_END()
}

NH_MAKE_RESULT nh_make_appendFile(
    nh_make_FileArray *Array_p, NH_BYTE *path_p)
{
NH_MAKE_BEGIN()

    if (!Array_p->length) {
        Array_p->Files_p = malloc(sizeof(nh_make_File));
        NH_MAKE_CHECK_NULL(Array_p->Files_p)       
    }
    else {
        Array_p->Files_p = realloc(Array_p->Files_p, sizeof(nh_make_File) * (Array_p->length + 1));
        NH_MAKE_CHECK_NULL(Array_p->Files_p)       
    }

    NH_MAKE_CHECK(nh_make_setFileData(Array_p->Files_p+Array_p->length, path_p))

    Array_p->length++;

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

