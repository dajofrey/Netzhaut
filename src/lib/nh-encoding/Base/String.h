#ifndef NH_ENCODING_STRING_H
#define NH_ENCODING_STRING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Encodings.h"

#include "../Common/Includes.h"
#include "../Encodings/UTF32.h"

#endif

typedef struct nh_encoding_String { 
    NH_ENCODING_NAME encoding; 
    nh_String Bytes; 
} nh_encoding_String;

/** @addtogroup lib_nh-encoding_functions
 *  @{
 */

    nh_encoding_String nh_encoding_encode(
        NH_ENCODING_NAME encoding, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );

    nh_encoding_String nh_encoding_initString(
        NH_ENCODING_NAME encoding, int length 
    );

    NH_API_RESULT nh_encoding_append(
        nh_encoding_String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );

    void nh_encoding_freeString(
        nh_encoding_String *String_p
    );

/** @} */

#endif
