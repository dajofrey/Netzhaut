#ifndef NH_ENCODING_ENCODINGS_UTF8_H
#define NH_ENCODING_ENCODINGS_UTF8_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"
#include "../Base/String.h"

// DEFINES =========================================================================================

typedef nh_core_String nh_encoding_UTF8String;

// FUNCTIONS =======================================================================================

nh_encoding_UTF32String nh_encoding_decodeUTF8(
    unsigned char *p, unsigned long bytes, unsigned long *read_p 
);

NH_ENCODING_UTF32 nh_encoding_decodeUTF8Single(
    unsigned char *p, unsigned long bytes, unsigned long *read_p
);

nh_encoding_UTF8String nh_encoding_encodeUTF8(
    NH_ENCODING_UTF32 *codepoints_p, unsigned long length
);

int nh_encoding_encodeUTF8Single(
    NH_ENCODING_UTF32 codepoint, char *p
);

NH_API_RESULT nh_encoding_appendUTF8(
    nh_encoding_UTF8String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
);

NH_API_RESULT nh_encoding_appendUTF8Single(
    nh_encoding_UTF8String *String_p, NH_ENCODING_UTF32 codepoint
);

nh_encoding_UTF8String nh_encoding_initUTF8(
    int length
);

void nh_encoding_freeUTF8(
    nh_encoding_UTF8String *String_p
);

#endif
