#ifndef NH_ENCODING_UTF8_H
#define NH_ENCODING_UTF8_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-encoding_functions
 *  @{
 */

    nh_encoding_UTF32String nh_encoding_decodeUTF8(
        NH_UNSIGNED_BYTE *p, unsigned long bytes, unsigned long *read_p 
    );

    NH_ENCODING_UTF32 nh_encoding_decodeUTF8Single(
        NH_UNSIGNED_BYTE *p, unsigned long bytes, unsigned long *read_p
    );

    nh_encoding_UTF8String nh_encoding_encodeUTF8(
        NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );
    
    int nh_encoding_encodeUTF8Single(
        NH_ENCODING_UTF32 codepoint, NH_BYTE *p
    );

    NH_ENCODING_RESULT nh_encoding_appendUTF8(
        nh_encoding_UTF8String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );

    NH_ENCODING_RESULT nh_encoding_appendUTF8Single(
        nh_encoding_UTF8String *String_p, NH_ENCODING_UTF32 codepoint
    );

    nh_encoding_UTF8String nh_encoding_initUTF8(
        int length
    );

    void nh_encoding_freeUTF8(
        nh_encoding_UTF8String *String_p
    );

/** @} */

#endif
