// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "String.h"

#include "../Encodings/UTF8.h"
#include "../../nh-core/Util/String.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_encoding_String nh_encoding_initString(
    NH_ENCODING_NAME encoding, int length)
{
    nh_encoding_String String;
    String.encoding = encoding;
    String.Bytes = nh_core_initString(length);

    return String;
}

nh_encoding_String nh_encoding_encode(
    NH_ENCODING_NAME encoding, NH_ENCODING_UTF32 *codepoints_p, unsigned long length)
{
    nh_encoding_String String;
    String.encoding = encoding;
    String.Bytes = nh_core_initString(16);

    switch (encoding) {
        case NH_ENCODING_NAME_UTF_8 : String.Bytes = nh_encoding_encodeUTF8(codepoints_p, length);
    }

    return String;
}

NH_API_RESULT nh_encoding_append(
    nh_encoding_String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length)
{

    switch (String_p->encoding) {
        case NH_ENCODING_NAME_UTF_8 : return nh_encoding_appendUTF8(&String_p->Bytes, codepoints_p, length);
        default : return NH_API_ERROR_BAD_STATE;
    }

    return NH_API_SUCCESS;
}

void nh_encoding_freeString(
    nh_encoding_String *String_p)
{
    nh_core_freeString(&String_p->Bytes);
    return;
}

