#ifndef NH_CORE_UTIL_FILE_H
#define NH_CORE_UTIL_FILE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "List.h"
#include "../Common/Includes.h"

#include <stddef.h>

/** @addtogroup lib_nh-core_typedefs
 *  @{
 */

    typedef char *(*nh_core_getFileData_f)(
        const char* path_p, long *size_p
    );
    
    typedef NH_API_RESULT (*nh_core_writeBytesToFile_f)(
        char *filePath_p, char *bytes_p
    );

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_API_RESULT nh_core_getCurrentDir(
        char *set_p, int size
    );

    nh_core_List nh_core_getDirContentPaths(
        char *dirPath_p
    );

    NH_API_RESULT nh_core_getFilePath(
        char *filename_p, char *base_p, char *out_p
    );

    char *nh_core_getFileData(
        const char* path_p, long *size_p
    );
    
    NH_API_RESULT nh_core_writeBytesToFile(
        char *filePath_p, char *bytes_p
    );

    bool nh_fileExistsOnMachine(
        char *filename_p, char *base_p
    ); 

    bool nh_core_isRegularFile(
        const char *path_p
    );

/** @} */

#endif 
