#ifndef NH_ENCODING_UTF32_H
#define NH_ENCODING_UTF32_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nh-core/Util/List.h"

#include <stdint.h>
#include <stddef.h>

#endif

typedef NH_API_UTF32 NH_ENCODING_UTF32; 

typedef struct nh_encoding_UTF32String { 
    NH_ENCODING_UTF32 *p; 
    unsigned long length; 
    nh_core_Array Array; 
} nh_encoding_UTF32String; 

/** @addtogroup lib_nh-core_typedefs
 *  @{
 */

    typedef nh_encoding_UTF32String (*nh_encoding_initUTF32_f)(
        int chunkSize
    );

    typedef NH_API_RESULT (*nh_encoding_appendUTF32_f)(
        nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );

    typedef void (*nh_encoding_freeUTF32_f)(
        nh_encoding_UTF32String *String_p
    );

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

// STRING

    nh_encoding_UTF32String nh_encoding_initUTF32(
        int chunkSize
    );
    
    NH_API_RESULT nh_encoding_appendUTF32(
        nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );
    
    NH_API_RESULT nh_encoding_appendUTF32Codepoint(
        nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 codepoint
    );

    NH_API_RESULT nh_encoding_appendUTF8ToUTF32(
        nh_encoding_UTF32String *String_p, unsigned char *bytes_p, unsigned long length
    );

    NH_ENCODING_UTF32 *nh_encoding_incrementUTF32(
        nh_encoding_UTF32String *String_p
    );

    void nh_encoding_freeUTF32(
        nh_encoding_UTF32String *String_p
    );

    unsigned int nh_encoding_getUTF32Length( 
        NH_ENCODING_UTF32 *codepoints_p
    );

    NH_API_RESULT nh_encoding_removeUTF32Tail(
        nh_encoding_UTF32String *String_p, unsigned long count
    );

    NH_API_RESULT nh_encoding_removeUTF32(
        nh_encoding_UTF32String *String_p, int index, unsigned int count
    );
    
    NH_API_RESULT nh_encoding_insertUTF32(
        nh_encoding_UTF32String *String_p, int index, NH_ENCODING_UTF32 *codepoints_p, int length
    );

    nh_core_List nh_encoding_splitUTF32(
        nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 delimiter
    );

    void nh_encoding_replaceNonCharacters(
        nh_encoding_UTF32String *String_p
    );

    nh_encoding_UTF32String nh_encoding_replaceNonCharactersExpressively(
        nh_encoding_UTF32String *String_p
    );

    void nh_encoding_printUTF32( 
        nh_encoding_UTF32String *String_p
    ); 

// CHECKS 

    bool nh_encoding_isASCIIWhitespace(
        NH_ENCODING_UTF32 codepoint
    );

    bool nh_encoding_isASCIIUpperAlpha(
        NH_ENCODING_UTF32 codepoint
    );
    
    bool nh_encoding_isASCIILowerAlpha(
        NH_ENCODING_UTF32 codepoint
    );
    
    bool nh_encoding_isASCIIAlpha(
        NH_ENCODING_UTF32 codepoint
    );
    
    bool nh_encoding_isASCIIDigit(
        NH_ENCODING_UTF32 codepoint
    );
    
    bool nh_encoding_isASCIIDigit2(
        NH_ENCODING_UTF32 codepoint
    );
 
    bool nh_encoding_isASCIIHexDigit(
        NH_ENCODING_UTF32 codepoint
    );

    bool nh_encoding_isASCIIAlphaNumeric(
        NH_ENCODING_UTF32 codepoint
    );
    
    bool nh_encoding_isASCIICaseInsensitiveMatch(
        char *str1_p, char *str2_p
    );

    bool nh_encoding_isASCIICodepoint(
        NH_ENCODING_UTF32 codepoint
    );

    bool nh_encoding_isSurrogate(
        NH_ENCODING_UTF32 codepoint
    );
    
    bool nh_encoding_isScalarValue(
        NH_ENCODING_UTF32 codepoint
    );
    
    bool nh_encoding_isNonCharacter(
        NH_ENCODING_UTF32 codepoint
    );

    bool nh_encoding_isC0Control(
        NH_ENCODING_UTF32 codepoint
    );
    
    bool nh_encoding_isControl(
        NH_ENCODING_UTF32 codepoint
    );

// MATCH

    nh_encoding_UTF32String *nh_encoding_getBestMatch(
        nh_core_List *Strings_p, nh_encoding_UTF32String *String_p
    );

// COMPARE

    bool nh_encoding_compareUTF32(
        NH_ENCODING_UTF32 *p1, NH_ENCODING_UTF32 *p2
    );

    bool nh_encoding_compareUTF32ASCII(
        NH_ENCODING_UTF32 *utf32_p, char *ascii_p
    );

// DIGITS

    int nh_encoding_hexDigitToNumber(
        NH_ENCODING_UTF32 codepoint
    ); 

    long nh_encoding_digitsToNumber(
        NH_ENCODING_UTF32 *codepoints_p, int length, int base
    ); 

    int nh_encoding_integerToUTF32(
        int number, NH_ENCODING_UTF32 *codepoints_p, int maxLength
    );

/** @} */

#endif 
