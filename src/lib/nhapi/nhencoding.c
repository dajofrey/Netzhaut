// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nhencoding.h"

#include "../nhcore/Loader/Loader.h"
#include "../nhencoding/Encodings/UTF32.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE ==========================================================================================

nh_encoding_UTF32String nh_api_initUTF32(
    int chunkSize)
{
    nh_encoding_UTF32String String;
    memset(&String, 0, sizeof(nh_encoding_UTF32String));

    nh_encoding_initUTF32_f initUTF32_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_ENCODING, 0, "nh_encoding_initUTF32");
    return initUTF32_f ? initUTF32_f(chunkSize) : String;
}

NH_ENCODING_RESULT nh_api_appendUTF32(
    nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length)
{
    nh_encoding_appendUTF32_f appendUTF32_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_ENCODING, 0, "nh_encoding_appendUTF32");
    return appendUTF32_f ? appendUTF32_f(String_p, codepoints_p, length) : NH_ENCODING_ERROR_BAD_STATE;
}

void nh_api_freeUTF32(
    nh_encoding_UTF32String *String_p)
{
    nh_encoding_freeUTF32_f freeUTF32_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_ENCODING, 0, "nh_encoding_freeUTF32");
    return freeUTF32_f ? freeUTF32_f(String_p) : NH_ENCODING_ERROR_BAD_STATE;
}

