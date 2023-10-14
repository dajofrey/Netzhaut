// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "String.h"

#include "../Encodings/UTF8.h"
#include "../Common/Macros.h"

#include "../../nhcore/Util/String.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STRING ==========================================================================================

nh_encoding_String nh_encoding_initString(
    NH_ENCODING_NAME encoding, int length)
{
NH_ENCODING_BEGIN()

    nh_encoding_String String;
    String.encoding = encoding;
    String.Bytes = nh_core_initString(length);

NH_ENCODING_END(String)
}

nh_encoding_String nh_encoding_encode(
    NH_ENCODING_NAME encoding, NH_ENCODING_UTF32 *codepoints_p, unsigned long length)
{
NH_ENCODING_BEGIN()

    nh_encoding_String String;
    String.encoding = encoding;
    String.Bytes = nh_core_initString(16);

    switch (encoding) {
        case NH_ENCODING_NAME_UTF_8 : String.Bytes = nh_encoding_encodeUTF8(codepoints_p, length);
    }

NH_ENCODING_END(String)
}

NH_ENCODING_RESULT nh_encoding_append(
    nh_encoding_String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length)
{
NH_ENCODING_BEGIN()


    switch (String_p->encoding) {
        case NH_ENCODING_NAME_UTF_8 : NH_ENCODING_END(nh_encoding_appendUTF8(&String_p->Bytes, codepoints_p, length))
        default : NH_ENCODING_END(NH_ENCODING_ERROR_BAD_STATE)
    }

NH_ENCODING_END(NH_ENCODING_SUCCESS)
}

void nh_encoding_freeString(
    nh_encoding_String *String_p)
{
NH_ENCODING_BEGIN()

    nh_core_freeString(&String_p->Bytes);

NH_ENCODING_SILENT_END()
}

