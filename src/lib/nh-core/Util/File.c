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

#if defined(__unix__) || defined(__APPLE__)
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

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_core_getCurrentDir(
    char *set_p, int size)
{
#ifdef __unix__

   if (getcwd(set_p, size) == NULL) {return NH_API_ERROR_BAD_STATE;}

#endif

   return NH_API_SUCCESS;
}

nh_core_List nh_core_getDirContentPaths(
    char *dirPath_p)
{
    nh_core_List List = nh_core_initList(16);

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

    return List;
}

// PATH ============================================================================================

NH_API_RESULT nh_core_getFilePath(
    char *filename_p, char *base_p, char *out_p)
{
    struct stat buffer;   
    if (stat(filename_p, &buffer) == 0) {
        strcpy(out_p, filename_p);
        return NH_API_SUCCESS;
    }

    char absolute_p[2048] = {'\0'};
    nh_core_getAbsolutePath(filename_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {
        strcpy(out_p, absolute_p);
        return NH_API_SUCCESS;
    }

    memset(absolute_p, '\0', sizeof(char) * 2048);
    nh_core_getCustomAbsolutePath(filename_p, base_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {
        strcpy(out_p, absolute_p);
        return NH_API_SUCCESS;
    }

    return NH_API_ERROR_BAD_STATE;
}

static NH_API_RESULT nh_core_getAbsolutePath(
    char *relPath_p, char *absPath_p) 
{
#ifdef __unix__ 	
    realpath(relPath_p, absPath_p); 
#elif defined(_WIN32) || defined(WIN32)

#endif

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_core_getCustomAbsolutePath(
    char *relPath_p, char *base_p, char *absPath_p) 
{
    if (base_p == NULL) {return NH_API_SUCCESS;}

    char basePath_p[PATH_MAX] = {'\0'};
    strcpy(basePath_p, base_p);

#if defined(__unix__) || defined(__APPLE__)
    for (int i = strlen(basePath_p); i > 0; --i) {if (basePath_p[i] == '/') {basePath_p[i + 1] = '\0'; break;}}
#elif defined(_WIN32) || defined(WIN32)

#endif

    char relPath2_p[PATH_MAX] = {'\0'};
    strcpy(relPath2_p, relPath_p);

#if defined(__unix__) || defined(__APPLE__)
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

    return NH_API_SUCCESS;
}

bool nh_core_isRegularFile(
    const char *path_p)
{
#if defined(__unix__) || defined(__APPLE__)
    struct stat path_stat;
    stat(path_p, &path_stat);
    return S_ISREG(path_stat.st_mode) != 0 ? true : false;
#elif defined(_WIN32) || defined(WIN32)

#endif
}

// DATA ============================================================================================

char *nh_core_getFileData(
    const char* path_p, long *size_p)
{
    FILE *fh = fopen(path_p, "rb");
    if (fh == NULL) {return NULL;}
    
    if (fseek(fh, 0L, SEEK_END) != 0) {return NULL;}

    long size = ftell(fh);

    if (fseek(fh, 0L, SEEK_SET) != 0) {return NULL;}

    if(size <= 0) {
        fclose(fh);
        return NULL;
    }

    char *data_p = (char*)nh_core_allocate(((size_t)size) + 1); 
    if (data_p == NULL) {return NULL;}
    
    fread(data_p, 1, size, fh);
    fclose(fh);

    data_p[size] = '\0';
    if (size_p != NULL) {*size_p = size;}

    return data_p;
}

NH_API_RESULT nh_core_writeBytesToFile(
    char *filePath_p, char *bytes_p)
{
    NH_CORE_CHECK_NULL(filePath_p)

    FILE *f = fopen(filePath_p, "w");
    NH_CORE_CHECK_NULL(f)    
    
    if (bytes_p) {fprintf(f, "%s", bytes_p);}

    fclose(f);

    return NH_API_SUCCESS;
}

bool nh_fileExistsOnMachine(
    char *filename_p, char *base_p) 
{
    struct stat buffer;   
    if (stat(filename_p, &buffer) == 0) {return true;}

    char absolute_p[2048] = {'\0'};
    nh_core_getAbsolutePath(filename_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {return true;}

    memset(absolute_p, '\0', sizeof(char) * 2048);
    nh_core_getCustomAbsolutePath(filename_p, base_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {return true;}

    return false;
}
