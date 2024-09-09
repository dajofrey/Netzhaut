#ifndef NH_ENCODING_H
#define NH_ENCODING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-core.h"

#endif

/** @defgroup api_nh-encoding nh-encoding
 *  @brief TODO.
 */

/** @addtogroup api_nh-encoding
 *  @{
 */

// ENUMS ============================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_ENCODING_RESULT {
        NH_ENCODING_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_ENCODING_ERROR_NULL_POINTER,
        NH_ENCODING_ERROR_BAD_STATE,
        NH_ENCODING_ERROR_MEMORY_ALLOCATION,
    } NH_ENCODING_RESULT;

    typedef enum NH_ENCODING_NAME {
        NH_ENCODING_NAME_UNDEFINED,
        NH_ENCODING_NAME_UTF_8, 
        NH_ENCODING_NAME_IBM866,
        NH_ENCODING_NAME_ISO_8859_2,
        NH_ENCODING_NAME_ISO_8859_3,
        NH_ENCODING_NAME_ISO_8859_4,
        NH_ENCODING_NAME_ISO_8859_5,
        NH_ENCODING_NAME_ISO_8859_6,
        NH_ENCODING_NAME_ISO_8859_7,
        NH_ENCODING_NAME_ISO_8859_8,
        NH_ENCODING_NAME_ISO_8859_8_I,
        NH_ENCODING_NAME_ISO_8859_10,
        NH_ENCODING_NAME_ISO_8859_13, 
        NH_ENCODING_NAME_ISO_8859_14, 
        NH_ENCODING_NAME_ISO_8859_15, 
        NH_ENCODING_NAME_ISO_8859_16, 
        NH_ENCODING_NAME_KOI8_R,
        NH_ENCODING_NAME_KOI8_U,
        NH_ENCODING_NAME_MACINTOSH,
        NH_ENCODING_NAME_WINDOWS_874, 	
        NH_ENCODING_NAME_WINDOWS_1250, 	
        NH_ENCODING_NAME_WINDOWS_1251,	
        NH_ENCODING_NAME_WINDOWS_1252, 	
        NH_ENCODING_NAME_WINDOWS_1253, 	
        NH_ENCODING_NAME_WINDOWS_1254, 	
        NH_ENCODING_NAME_WINDOWS_1255, 	
        NH_ENCODING_NAME_WINDOWS_1256, 	
        NH_ENCODING_NAME_WINDOWS_1257, 	
        NH_ENCODING_NAME_WINDOWS_1258, 	
        NH_ENCODING_NAME_X_MAC_CYRILLIC, 	
        NH_ENCODING_NAME_GBK,
        NH_ENCODING_NAME_GB18030, 	
        NH_ENCODING_NAME_BIG5,
        NH_ENCODING_NAME_EUC_JP,
        NH_ENCODING_NAME_ISO_2022_JP,
        NH_ENCODING_NAME_SHIFT_JIS,
        NH_ENCODING_NAME_EUC_KR,
        NH_ENCODING_NAME_REPLACEMENT,
        NH_ENCODING_NAME_UTF_16BE,
        NH_ENCODING_NAME_UTF_16LE,
        NH_ENCODING_NAME_X_USER_DEFINED,
    } NH_ENCODING_NAME;

// TYPEDEFS ========================================================================================

    typedef uint_least32_t NH_ENCODING_UTF32;
    typedef nh_String nh_encoding_UTF8String;

// STRUCTS =========================================================================================

    typedef struct nh_encoding_UTF32String {
        NH_ENCODING_UTF32 *p;
        unsigned long length;
        nh_Array Array;
    } nh_encoding_UTF32String;

    typedef struct nh_encoding_String {
        NH_ENCODING_NAME encoding;
        nh_String Bytes;
    } nh_encoding_String;

// FUNCTIONS =======================================================================================

    nh_encoding_UTF32String nh_api_initUTF32(
        int chunkSize
    );

    NH_ENCODING_RESULT nh_api_appendUTF32(
        nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );

    void nh_api_freeUTF32(
        nh_encoding_UTF32String *String_p
    );

/** @} */

#endif // NH_ENCODING_H
