// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "File.h"
#include "String.h"

#include "../System/Memory.h"
#include "../Common/Macros.h"

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>

#ifdef __unix__
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

// DECLARE =========================================================================================

static NH_API_RESULT nh_core_getAbsolutePath(
    char *relPath_p, char *absPath_p
);
static NH_API_RESULT nh_core_getCustomAbsolutePath(
    char *relPath_p, char *base_p, char *absPath_p
); 

// DIRECTORY =======================================================================================

NH_API_RESULT nh_core_getCurrentDir(
    char *set_p, int size)
{
NH_CORE_BEGIN()

#ifdef __unix__

   if (getcwd(set_p, size) == NULL) {NH_CORE_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}

#endif

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

nh_List nh_core_getDirContentPaths(
    char *dirPath_p)
{
NH_CORE_BEGIN()

    nh_List List = nh_core_initList(16);

#ifdef __unix__

    struct dirent *entry_p = NULL;
    char fullPath_p[1024] = {'\0'};

    DIR *dir_p = opendir(dirPath_p); 
    NH_CORE_CHECK_NULL_2(List, dir_p)
  
    while ((entry_p = readdir(dir_p)) != NULL)
    {
        if (dirPath_p[strlen(dirPath_p) - 1] != '/') {
            sprintf(fullPath_p, "%s/%s", dirPath_p, entry_p->d_name);
        } else {
            sprintf(fullPath_p, "%s%s", dirPath_p, entry_p->d_name);
        }

        char *path_p = nh_core_allocateBytes(fullPath_p);
        NH_CORE_CHECK_2(List, nh_core_appendToList(&List, path_p))
    }
  
    closedir(dir_p);    

#elif defined(_WIN32) || defined(WIN32)

#endif

NH_CORE_END(List)
}

// PATH ============================================================================================

NH_API_RESULT nh_core_getFilePath(
    char *filename_p, char *base_p, char *out_p)
{
NH_CORE_BEGIN()

    struct stat buffer;   
    if (stat(filename_p, &buffer) == 0) {
        strcpy(out_p, filename_p);
        NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
    }

    char absolute_p[2048] = {'\0'};
    nh_core_getAbsolutePath(filename_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {
        strcpy(out_p, absolute_p);
        NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
    }

    memset(absolute_p, '\0', sizeof(char) * 2048);
    nh_core_getCustomAbsolutePath(filename_p, base_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {
        strcpy(out_p, absolute_p);
        NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
    }

NH_CORE_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
}

static NH_API_RESULT nh_core_getAbsolutePath(
    char *relPath_p, char *absPath_p) 
{
NH_CORE_BEGIN()

#ifdef __unix__ 	
    realpath(relPath_p, absPath_p); 
#elif defined(_WIN32) || defined(WIN32)

#endif

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_core_getCustomAbsolutePath(
    char *relPath_p, char *base_p, char *absPath_p) 
{
NH_CORE_BEGIN()

    if (base_p == NULL) {NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)}

    char basePath_p[PATH_MAX] = {'\0'};
    strcpy(basePath_p, base_p);

#ifdef __unix__ 	
    for (int i = strlen(basePath_p); i > 0; --i) {if (basePath_p[i] == '/') {basePath_p[i + 1] = '\0'; break;}}
#elif defined(_WIN32) || defined(WIN32)

#endif

    char relPath2_p[PATH_MAX] = {'\0'};
    strcpy(relPath2_p, relPath_p);

#ifdef __unix__ 	
    int off = 0;
    while (strlen(relPath2_p) > off + 3 && relPath2_p[off] == '.' && relPath2_p[off + 1] == '.' && relPath2_p[off + 2] == '/') 
    {
        bool dirSuffix = true;
        for (int i = strlen(basePath_p); i > 0; --i) 
        {
            if (basePath_p[i] == '/' && dirSuffix == false) {basePath_p[i + 1] = '\0'; break;}
            if (basePath_p[i] == '/') {dirSuffix = false;}
        }
        off += 3;
    }
#elif defined(_WIN32) || defined(WIN32)

#endif

    strcpy(absPath_p, basePath_p);
    memcpy(absPath_p + strlen(basePath_p), relPath2_p + off, sizeof(char) * strlen(relPath2_p));

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

bool nh_core_isRegularFile(
    const char *path_p)
{
NH_CORE_BEGIN()

#ifdef __unix__
    struct stat path_stat;
    stat(path_p, &path_stat);

NH_CORE_END(S_ISREG(path_stat.st_mode) != 0 ? true : false)
#elif defined(_WIN32) || defined(WIN32)

#endif
}

// DATA ============================================================================================

char *nh_core_getFileData(
    const char* path_p, long *size_p)
{
NH_CORE_BEGIN()

    FILE *fh = fopen(path_p, "rb");
    if (fh == NULL) {NH_CORE_END(NULL)}
    
    if (fseek(fh, 0L, SEEK_END) != 0) {NH_CORE_END(NULL)}

    long size = ftell(fh);

    if (fseek(fh, 0L, SEEK_SET) != 0) {NH_CORE_END(NULL)}

    if(size <= 0) {
        fclose(fh);
        NH_CORE_END(NULL)
    }

    char *data_p = (char*)nh_core_allocate(((size_t)size) + 1); 
    if (data_p == NULL) {NH_CORE_END(NULL)}
    
    fread(data_p, 1, size, fh);
    fclose(fh);

    data_p[size] = '\0';
    if (size_p != NULL) {*size_p = size;}

NH_CORE_END(data_p)
}

NH_API_RESULT nh_core_writeBytesToFile(
    char *filePath_p, char *bytes_p)
{
NH_CORE_BEGIN()

    NH_CORE_CHECK_NULL(filePath_p)

    FILE *f = fopen(filePath_p, "w");
    NH_CORE_CHECK_NULL(f)    
    
    if (bytes_p) {fprintf(f, "%s", bytes_p);}

    fclose(f);

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

bool nh_fileExistsOnMachine(
    char *filename_p, char *base_p) 
{
NH_CORE_BEGIN()

    struct stat buffer;   
    if (stat(filename_p, &buffer) == 0) {NH_CORE_END(true)}

    char absolute_p[2048] = {'\0'};
    nh_core_getAbsolutePath(filename_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {NH_CORE_END(true)}

    memset(absolute_p, '\0', sizeof(char) * 2048);
    nh_core_getCustomAbsolutePath(filename_p, base_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {NH_CORE_END(true)}

NH_CORE_END(false)
}

