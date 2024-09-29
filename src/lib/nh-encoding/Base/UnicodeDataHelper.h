#ifndef NH_ENCODING_UNICODE_DATA_HELPER_H
#define NH_ENCODING_UNICODE_DATA_HELPER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Encodings/UTF32.h"

#include <stdint.h>
#include <stddef.h>

#endif

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    /**
     * See https://www.unicode.org/reports/tr44/#UnicodeData.txt
     */ 
    typedef enum NH_ENCODING_UNICODE_DATA_PROPERTY {
        NH_ENCODING_UNICODE_DATA_PROPERTY_NAME = 1,
        NH_ENCODING_UNICODE_DATA_PROPERTY_GENERAL_CATEGORY,
        NH_ENCODING_UNICODE_DATA_PROPERTY_CANONICAL_COMBINING_CLASS,
        NH_ENCODING_UNICODE_DATA_PROPERTY_BIDI_CLASS,
        NH_ENCODING_UNICODE_DATA_PROPERTY_DECOMPOSITION_TYPE,
        NH_ENCODING_UNICODE_DATA_PROPERTY_DECOMPOSITION_MAPPING,
        NH_ENCODING_UNICODE_DATA_PROPERTY_NUMERIC_TYPE,
        NH_ENCODING_UNICODE_DATA_PROPERTY_NUMERIC_VALUE,
        NH_ENCODING_UNICODE_DATA_PROPERTY_BIDI_MIRRORED,
        NH_ENCODING_UNICODE_DATA_PROPERTY_UNICODE_1_NAME,
        NH_ENCODING_UNICODE_DATA_PROPERTY_ISO_COMMENT,
        NH_ENCODING_UNICODE_DATA_PROPERTY_SIMPLE_UPPERCASE_MAPPING,
        NH_ENCODING_UNICODE_DATA_PROPERTY_SIMPLE_LOWERCASE_MAPPING,
        NH_ENCODING_UNICODE_DATA_PROPERTY_SIMPLE_TITLECASE_MAPPING,
    } NH_ENCODING_UNICODE_DATA_PROPERTY;

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    const char *nh_encoding_getDescription(
        NH_ENCODING_UTF32 codepoint
    );

    bool nh_encoding_inIDSTART(
        NH_ENCODING_UTF32 codepoint
    );
    
    bool nh_encoding_inIDCONTINUE(
        NH_ENCODING_UTF32 codepoint
    );

    NH_ENCODING_UTF32 nh_encoding_getUnicodeDataSimpleLowerCase(
        NH_ENCODING_UTF32 codepoint
    );
    
    NH_ENCODING_UTF32 nh_encoding_getUnicodeDataSimpleUpperCase(
        NH_ENCODING_UTF32 codepoint
    );

/** @} */

#endif 
