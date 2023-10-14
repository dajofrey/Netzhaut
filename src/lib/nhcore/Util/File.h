#ifndef NH_CORE_UTIL_FILE_H
#define NH_CORE_UTIL_FILE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "List.h"

#include "../Common/Includes.h"

#include <stddef.h>

#endif

/** @addtogroup lib_nhcore_typedefs
 *  @{
 */

    typedef NH_BYTE *(*nh_core_getFileData_f)(
        const NH_BYTE* path_p, long *size_p
    );
    
    typedef NH_CORE_RESULT (*nh_core_writeBytesToFile_f)(
        NH_BYTE *filePath_p, NH_BYTE *bytes_p
    );

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_getCurrentDir(
        NH_BYTE *set_p, int size
    );

    nh_List nh_core_getDirContentPaths(
        NH_BYTE *dirPath_p
    );

    NH_CORE_RESULT nh_core_getFilePath(
        NH_BYTE *filename_p, NH_BYTE *base_p, NH_BYTE *out_p
    );

    NH_BYTE *nh_core_getFileData(
        const NH_BYTE* path_p, long *size_p
    );
    
    NH_CORE_RESULT nh_core_writeBytesToFile(
        NH_BYTE *filePath_p, NH_BYTE *bytes_p
    );

    NH_BOOL nh_fileExistsOnMachine(
        NH_BYTE *filename_p, NH_BYTE *base_p
    ); 

    NH_BOOL nh_core_isRegularFile(
        const NH_BYTE *path_p
    );

/** @} */

#endif 
