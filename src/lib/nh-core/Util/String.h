#ifndef NH_CORE_STRING_H
#define NH_CORE_STRING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Array.h"

#include "../Common/Includes.h"

#include <stdint.h>
#include <stddef.h>

#endif

/** @addtogroup lib_nh-core_enums
 *  @{
 */

    typedef enum NH_ESCAPE {
        NH_ESCAPE_START      = 1,
        NH_ESCAPE_CSI        = 2,
        NH_ESCAPE_STR        = 4,  /* OSC, PM, APC */
        NH_ESCAPE_ALTCHARSET = 8,
        NH_ESCAPE_STR_END    = 16, /* a final string was encountered */
        NH_ESCAPE_TEST       = 32, /* Enter in test mode */
        NH_ESCAPE_UTF8       = 64,
        NH_ESCAPE_DCS        = 128,
    } NH_ESCAPE;

/** @} */

/** @addtogroup lib_nh-core_structs
 *  @{
 */

    typedef struct nh_DataStorageUnits {
        long long gigabytes;
        long long megabytes;
        long long kilobytes;
        long long bytes;
    } nh_DataStorageUnits;

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

// HUMANIZE
    nh_DataStorageUnits nh_humanizeBytes(
        long long bytes
    );

// BYTES
    NH_BYTE *nh_core_allocateBytes(
        NH_BYTE *bytes_p
    );

// LEVENSHTEIN
    int nh_levenshteinDistance(
        NH_BYTE *s1_p, NH_BYTE *s2_p, NH_BOOL caseSensitive
    );

// STRING
    nh_String nh_core_initString(
        int chunkSize
    );

    NH_CORE_RESULT nh_core_appendByteToString(
        nh_String *String_p, NH_BYTE byte
    );

    NH_CORE_RESULT nh_core_appendByteToStringRepeatedly(
        nh_String *String_p, NH_BYTE byte, int count
    );

    NH_CORE_RESULT nh_core_appendToString(
        nh_String *String_p, NH_BYTE *bytes_p, int length
    );

    NH_CORE_RESULT nh_core_appendFormatToString(
        nh_String *String_p, NH_BYTE *format_p, ...
    );
    
    NH_CORE_RESULT nh_core_removeFromString(
        nh_String *String_p, int index, unsigned int count
    );

    NH_CORE_RESULT nh_core_removeTailFromString(
        nh_String *String_p, unsigned int count
    );

    void nh_core_freeString(
        nh_String *String_p
    );

/** @} */

#endif 
