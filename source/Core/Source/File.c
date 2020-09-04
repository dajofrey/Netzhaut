// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/File.h"
#include "../Header/Macros.h"
#include "../Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdarg.h>

#ifdef __unix__
    #include <sys/stat.h>
#endif

// DECLARE =========================================================================================

static NH_RESULT Nh_getAbsolutePath(
    char *relPath_p, char *absPath_p
);
static NH_RESULT Nh_getCustomAbsolutePath(
    char *relPath_p, char *base_p, char *absPath_p
); 

// PATH ============================================================================================

NH_RESULT Nh_getFilePath(
    char *filename_p, char *base_p, char *out_p)
{
NH_BEGIN()

    struct stat buffer;   
    if (stat(filename_p, &buffer) == 0) {
        strcpy(out_p, filename_p);
        NH_END(NH_SUCCESS)
    }

    char absolute_p[2048] = {'\0'};
    Nh_getAbsolutePath(filename_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {
        strcpy(out_p, absolute_p);
        NH_END(NH_SUCCESS)
    }

    memset(absolute_p, '\0', sizeof(char) * 2048);
    Nh_getCustomAbsolutePath(filename_p, base_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {
        strcpy(out_p, absolute_p);
        NH_END(NH_SUCCESS)
    }

NH_END(NH_ERROR_BAD_STATE)
}

static NH_RESULT Nh_getAbsolutePath(
    char *relPath_p, char *absPath_p) 
{
NH_BEGIN()

#ifdef __unix__ 	
    realpath(relPath_p, absPath_p); 
#elif defined(_WIN32) || defined(WIN32)

#endif

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_getCustomAbsolutePath(
    char *relPath_p, char *base_p, char *absPath_p) 
{
NH_BEGIN()

    if (base_p == NULL) {NH_END(NH_SUCCESS)}

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

NH_END(NH_SUCCESS)
}

NH_BOOL Nh_isRegularFile(
    const char *path_p)
{
NH_BEGIN()

#ifdef __unix__
    struct stat path_stat;
    stat(path_p, &path_stat);

NH_END(S_ISREG(path_stat.st_mode) != 0 ? NH_TRUE : NH_FALSE)
#elif defined(_WIN32) || defined(WIN32)

#endif
}

// DATA ============================================================================================

void *Nh_getFileData(
    const char* path_p, long *size_p)
{
NH_BEGIN()

    FILE *fh = fopen(path_p, "rb");
    if (fh == NULL) {NH_END(NULL)}
    
    if (fseek(fh, 0L, SEEK_END) != 0) {NH_END(NULL)}

    long size = ftell(fh);

    if (fseek(fh, 0L, SEEK_SET) != 0) {NH_END(NULL)}

    if(size <= 0) {
        fclose(fh);
        NH_END(NULL)
    }

    char *data_p = (char*)Nh_allocate(((size_t)size) + 1); 
    if (data_p == NULL) {NH_END(NULL)}
    
    fread(data_p, 1, size, fh);
    fclose(fh);

    data_p[size] = '\0';
    if (size_p != NULL) {*size_p = size;}

NH_END(data_p)
}

NH_RESULT Nh_writeCharsToFile(
    char *filename_p, char *chars_p)
{
NH_BEGIN()

    NH_CHECK_NULL(chars_p, filename_p)

    FILE *f = fopen(filename_p, "w");
    NH_CHECK_NULL(f)    
    
    fprintf(f, "%s\n", chars_p);
    fclose(f);

NH_END(NH_SUCCESS)
}

bool Nh_fileExistsOnLocalHost(
    char *filename_p, char *base_p) 
{
NH_BEGIN()

    struct stat buffer;   
    if (stat(filename_p, &buffer) == 0) {NH_END(true)}

    char absolute_p[2048] = {'\0'};
    Nh_getAbsolutePath(filename_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {NH_END(true)}

    memset(absolute_p, '\0', sizeof(char) * 2048);
    Nh_getCustomAbsolutePath(filename_p, base_p, absolute_p);
    if (stat(absolute_p, &buffer) == 0) {NH_END(true)}

NH_END(false)
}

