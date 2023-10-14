#ifndef NH_ENCODING_UTF32_H
#define NH_ENCODING_UTF32_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include <stdint.h>
#include <stddef.h>

#endif

/** @addtogroup lib_nhcore_typedefs
 *  @{
 */

    typedef nh_encoding_UTF32String (*nh_encoding_initUTF32_f)(
        int chunkSize
    );

    typedef NH_ENCODING_RESULT (*nh_encoding_appendUTF32_f)(
        nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );

    typedef void (*nh_encoding_freeUTF32_f)(
        nh_encoding_UTF32String *String_p
    );

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

// STRING

    nh_encoding_UTF32String nh_encoding_initUTF32(
        int chunkSize
    );
    
    NH_ENCODING_RESULT nh_encoding_appendUTF32(
        nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );
    
    NH_ENCODING_RESULT nh_encoding_appendUTF32Codepoint(
        nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 codepoint
    );

    NH_ENCODING_RESULT nh_encoding_appendUTF8ToUTF32(
        nh_encoding_UTF32String *String_p, NH_UNSIGNED_BYTE *bytes_p, unsigned long length
    );

    NH_ENCODING_UTF32 *nh_encoding_incrementUTF32(
        nh_encoding_UTF32String *String_p
    );

    void nh_encoding_freeUTF32(
        nh_encoding_UTF32String *String_p
    );

    NH_ENCODING_RESULT nh_encoding_removeUTF32Tail(
        nh_encoding_UTF32String *String_p, unsigned long count
    );

    NH_ENCODING_RESULT nh_encoding_removeUTF32(
        nh_encoding_UTF32String *String_p, int index, unsigned int count
    );
    
    NH_ENCODING_RESULT nh_encoding_insertUTF32(
        nh_encoding_UTF32String *String_p, int index, NH_ENCODING_UTF32 *codepoints_p, int length
    );

    nh_List nh_encoding_splitUTF32(
        nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 delimiter
    );

    void nh_encoding_replaceNonCharacters(
        nh_encoding_UTF32String *String_p
    );

    nh_encoding_UTF32String nh_encoding_replaceNonCharactersExpressively(
        nh_encoding_UTF32String *String_p
    );

// CHECKS 

    NH_BOOL nh_encoding_isASCIIWhitespace(
        NH_ENCODING_UTF32 codepoint
    );

    NH_BOOL nh_encoding_isASCIIUpperAlpha(
        NH_ENCODING_UTF32 codepoint
    );
    
    NH_BOOL nh_encoding_isASCIILowerAlpha(
        NH_ENCODING_UTF32 codepoint
    );
    
    NH_BOOL nh_encoding_isASCIIAlpha(
        NH_ENCODING_UTF32 codepoint
    );
    
    NH_BOOL nh_encoding_isASCIIDigit(
        NH_ENCODING_UTF32 codepoint
    );
    
    NH_BOOL nh_encoding_isASCIIDigit2(
        NH_ENCODING_UTF32 codepoint
    );
 
    NH_BOOL nh_encoding_isASCIIHexDigit(
        NH_ENCODING_UTF32 codepoint
    );

    NH_BOOL nh_encoding_isASCIIAlphaNumeric(
        NH_ENCODING_UTF32 codepoint
    );
    
    NH_BOOL nh_encoding_isASCIICaseInsensitiveMatch(
        NH_BYTE *str1_p, NH_BYTE *str2_p
    );

    NH_BOOL nh_encoding_isASCIICodepoint(
        NH_ENCODING_UTF32 codepoint
    );

    NH_BOOL nh_encoding_isSurrogate(
        NH_ENCODING_UTF32 codepoint
    );
    
    NH_BOOL nh_encoding_isScalarValue(
        NH_ENCODING_UTF32 codepoint
    );
    
    NH_BOOL nh_encoding_isNonCharacter(
        NH_ENCODING_UTF32 codepoint
    );

    NH_BOOL nh_encoding_isC0Control(
        NH_ENCODING_UTF32 codepoint
    );
    
    NH_BOOL nh_encoding_isControl(
        NH_ENCODING_UTF32 codepoint
    );

// MATCH

    nh_encoding_UTF32String *nh_encoding_getBestMatch(
        nh_List *Strings_p, nh_encoding_UTF32String *String_p
    );

// COMPARE

    NH_BOOL nh_encoding_compareUTF32(
        NH_ENCODING_UTF32 *p1, NH_ENCODING_UTF32 *p2
    );

    NH_BOOL nh_encoding_compareUTF32ASCII(
        NH_ENCODING_UTF32 *utf32_p, NH_BYTE *ascii_p
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
