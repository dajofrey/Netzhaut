// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "PercentEncode.h"

#include "../Common/Macros.h"
#include "../../nh-core/Util/String.h"

#include "../../nh-encoding/Base/String.h"
#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// PERCENT ENCODE ==================================================================================

void nh_url_percentEncodeByte(
    char byte, nh_core_String *Output_p)
{
NH_URL_BEGIN()

    nh_core_appendByteToString(Output_p, 0x25);

    char hex_p[8];
    memset(hex_p, 0, 8);

    sprintf(hex_p, "%02X", byte);

    nh_core_appendToString(Output_p, hex_p, strlen(hex_p));

NH_URL_END()
}

nh_core_String nh_url_percentEncodeAfterEncoding(
    NH_ENCODING_NAME encoding, nh_encoding_UTF32String Input, NH_URL_PERCENT_ENCODE_SET percentEncodeSet, 
    bool spaceAsPlus)
{
NH_URL_BEGIN()

    nh_core_String Output = nh_core_initString(128);

    int potentialError = 0;
    nh_encoding_String EncodeOutput = nh_encoding_encode(encoding, Input.p, Input.length);

    for (int i = 0; i < EncodeOutput.Bytes.length; ++i) 
    {
        if (spaceAsPlus && EncodeOutput.Bytes.p[i] == 0x20) {
            nh_core_appendByteToString(&Output, 0x2B);
            continue;
        }
        NH_ENCODING_UTF32 isomorph = EncodeOutput.Bytes.p[i]; 
        if (!nh_url_inPercentEncodeSet(percentEncodeSet, isomorph)) {
            nh_core_appendByteToString(&Output, EncodeOutput.Bytes.p[i]);
        }
        else {
            nh_url_percentEncodeByte(EncodeOutput.Bytes.p[i], &Output);
        }
    }

    nh_encoding_freeString(&EncodeOutput);

NH_URL_END(Output)
}

nh_core_String nh_url_percentEncodeAfterEncodingUTF8(
    nh_encoding_UTF32String Input, NH_URL_PERCENT_ENCODE_SET percentEncodeSet)
{
NH_URL_BEGIN()
NH_URL_END(nh_url_percentEncodeAfterEncoding(NH_ENCODING_NAME_UTF_8, Input, percentEncodeSet, false))
}

nh_core_String nh_url_percentEncodeCodepointAfterEncodingUTF8(
    NH_ENCODING_UTF32 codepoint, NH_URL_PERCENT_ENCODE_SET percentEncodeSet)
{
NH_URL_BEGIN()

    nh_encoding_UTF32String Input = nh_encoding_initUTF32(1);
    nh_encoding_appendUTF32(&Input, &codepoint, 1);

    nh_core_String Output = nh_url_percentEncodeAfterEncoding(NH_ENCODING_NAME_UTF_8, Input, percentEncodeSet, false);

    nh_encoding_freeUTF32(&Input);

NH_URL_END(Output)
}

// PERCENT DECODE ==================================================================================

nh_core_String nh_url_percentDecode(
    nh_core_String Input)
{
NH_URL_BEGIN()

    nh_core_String Output = nh_core_initString(128);

    for (int i = 0; i < Input.length; ++i)
    {
        if (Input.p[i] != 0x25) {
            nh_core_appendByteToString(&Output, Input.p[i]);
        }
        else if (i+2<Input.length && !nh_encoding_isASCIIHexDigit(Input.p[i])) {
            nh_core_appendByteToString(&Output, Input.p[i]);
        }
        else {
            char replace = 0;
            if (i+3<Input.length) {replace = Input.p[i+3];}
            int number = (int)strtol(&Input.p[i+1], NULL, 16);
            nh_core_appendByteToString(&Output, number);
            Input.p[i+3] = replace;
            i += 2;
        }
    }

NH_URL_END(Output)
}

nh_core_String nh_url_percentDecodeUTF8(
    NH_ENCODING_UTF32 *codepoints_p, int length)
{
NH_URL_BEGIN()

    nh_core_String String = nh_encoding_encodeUTF8(codepoints_p, length);
    nh_core_String Output = nh_url_percentDecode(String);
    nh_core_freeString(&String);

NH_URL_END(Output)
}

// PERCENT ENCODE SET ==============================================================================

static bool nh_url_inC0ControlPercentEncodeSet(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()
NH_URL_END(nh_encoding_isC0Control(codepoint) || codepoint > 0x7E)
}

static bool nh_url_inFragmentPercentEncodeSet(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()
NH_URL_END(nh_url_inC0ControlPercentEncodeSet(codepoint) || codepoint == 0x20 || codepoint == 0x22 || codepoint == 0x3C || codepoint == 0x3E || codepoint == 0x60)
}

static bool nh_url_inQueryPercentEncodeSet(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()
NH_URL_END(nh_url_inC0ControlPercentEncodeSet(codepoint) || codepoint == 0x20 || codepoint == 0x22 || codepoint == 0x23 || codepoint == 0x3C || codepoint == 0x3E)
}

static bool nh_url_inSpecialQueryPercentEncodeSet(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()
NH_URL_END(nh_url_inQueryPercentEncodeSet(codepoint) || codepoint == 0x27)
}

static bool nh_url_inPathPercentEncodeSet(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()
NH_URL_END(nh_url_inQueryPercentEncodeSet(codepoint) || codepoint == 0x3F || codepoint == 0x60 || codepoint == 0x7B || codepoint == 0x7D)
}

static bool nh_url_inUserinfoPercentEncodeSet(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()
NH_URL_END(nh_url_inPathPercentEncodeSet(codepoint) || codepoint == 0x2F || codepoint == 0x3A || codepoint == 0x3B || codepoint == 0x3D || codepoint == 0x40 || (codepoint >= 0x5B && codepoint <= 0x5E) || codepoint == 0x7C)
}

static bool nh_url_inComponentPercentEncodeSet(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()
NH_URL_END(nh_url_inUserinfoPercentEncodeSet(codepoint) || (codepoint >= 0x24 && codepoint <= 0x26) || codepoint == 0x2B || codepoint == 0x2C)
}

bool nh_url_inPercentEncodeSet(
    NH_URL_PERCENT_ENCODE_SET encodeSet, NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()

    switch (encodeSet)
    {
        case NH_URL_PERCENT_ENCODE_SET_C0_CONTROL    : NH_URL_END(nh_url_inC0ControlPercentEncodeSet(codepoint)) 
        case NH_URL_PERCENT_ENCODE_SET_FRAGMENT      : NH_URL_END(nh_url_inFragmentPercentEncodeSet(codepoint))
        case NH_URL_PERCENT_ENCODE_SET_QUERY         : NH_URL_END(nh_url_inQueryPercentEncodeSet(codepoint))
        case NH_URL_PERCENT_ENCODE_SET_SPECIAL_QUERY : NH_URL_END(nh_url_inSpecialQueryPercentEncodeSet(codepoint))
        case NH_URL_PERCENT_ENCODE_SET_PATH          : NH_URL_END(nh_url_inPathPercentEncodeSet(codepoint))
        case NH_URL_PERCENT_ENCODE_SET_USERINFO      : NH_URL_END(nh_url_inUserinfoPercentEncodeSet(codepoint))
        case NH_URL_PERCENT_ENCODE_SET_COMPONENT     : NH_URL_END(nh_url_inComponentPercentEncodeSet(codepoint))
    }

NH_URL_END(false)
}

