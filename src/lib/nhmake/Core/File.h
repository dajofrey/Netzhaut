#ifndef NH_MAKE_FILE_H
#define NH_MAKE_FILE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Parser/Variables.h"

#include <stddef.h>

#endif

/** @addtogroup lib_nhmake_structs
 *  @{
 */

    typedef struct nh_make_File {
        NH_BYTE *path_p;
        NH_BYTE *data_p;
    } nh_make_File;

    typedef struct nh_make_FileArray {
        int length;
        nh_make_File *Files_p;
    } nh_make_FileArray;

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    NH_MAKE_BOOL nh_make_canFindSharedLib(
        const NH_BYTE *lib_p
    );

    NH_MAKE_RESULT nh_make_copy(
        nh_make_VariableArray *Array_p, NH_BYTE *in_p, NH_BYTE *out_p, NH_MAKE_BOOL recursive, NH_MAKE_BOOL sudo
    );

    NH_MAKE_RESULT nh_make_createSymLink(
        NH_BYTE *filePath_p, NH_BYTE *symLinkPath_p, NH_MAKE_BOOL sudo
    );

    NH_MAKE_BOOL nh_make_fileExists(
        NH_BYTE *filename_p
    );

    void nh_make_initFileArray(
        nh_make_FileArray *Array_p
    );
    
    NH_MAKE_RESULT nh_make_appendFile(
        nh_make_FileArray *Array_p, NH_BYTE *path_p
    );

    nh_make_File nh_make_initFile(
    );

    void nh_make_freeFileData(
        nh_make_File *File_p
    );

/** @} */

#endif
