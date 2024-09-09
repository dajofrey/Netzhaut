// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Helper.h"

#include "../Common/Macros.h"
#include "../../nh-encoding/Encodings/UTF32.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// SCHEME ==========================================================================================

NH_URL_SPECIAL_SCHEME nh_url_getSpecialScheme(
    NH_ENCODING_UTF32 *scheme_p)
{
NH_URL_BEGIN()

    static NH_ENCODING_UTF32 ftp_p[4] = {102, 116, 112, 0};
    static NH_ENCODING_UTF32 file_p[5] = {102, 105, 108, 101, 0};
    static NH_ENCODING_UTF32 http_p[5] = {104, 116, 116, 112, 0};
    static NH_ENCODING_UTF32 https_p[6] = {104, 116, 116, 112, 115, 0};
    static NH_ENCODING_UTF32 ws_p[3] = {119, 115, 0};
    static NH_ENCODING_UTF32 wss_p[4] = {119, 115, 115, 0};

    if (nh_encoding_compareUTF32(scheme_p, ftp_p)) {
        NH_URL_END(NH_URL_SPECIAL_SCHEME_FTP)
    }
    if (nh_encoding_compareUTF32(scheme_p, file_p)) {
        NH_URL_END(NH_URL_SPECIAL_SCHEME_FILE)
    }
    if (nh_encoding_compareUTF32(scheme_p, http_p)) {
        NH_URL_END(NH_URL_SPECIAL_SCHEME_HTTP)
    }
    if (nh_encoding_compareUTF32(scheme_p, https_p)) {
        NH_URL_END(NH_URL_SPECIAL_SCHEME_HTTPS)
    }
    if (nh_encoding_compareUTF32(scheme_p, ws_p)) {
        NH_URL_END(NH_URL_SPECIAL_SCHEME_WS)
    }
    if (nh_encoding_compareUTF32(scheme_p, wss_p)) {
        NH_URL_END(NH_URL_SPECIAL_SCHEME_WSS)
    }

NH_URL_END(NH_URL_SPECIAL_SCHEME_UNDEFINED)
}

//NH_URL_PORT *nh_url_getDefaultPort(
//    NH_ENCODING_UTF32 *scheme_p)
//{
//NH_URL_BEGIN()
//
//    if (!scheme_p) {NH_URL_END(NULL)}
//
//    static NH_URL_PORT *ports_pp[] = {
//        21,
//        80,
//        443,
//    };
//
//         if (!nh_encoding_compareUTF32(scheme_p, SCHEME_PP[0])) {NH_URL_END(ports_pp[0])}
//    else if (!nh_encoding_compareUTF32(scheme_p, SCHEME_PP[1])) {NH_URL_END(NULL)}
//    else if (!nh_encoding_compareUTF32(scheme_p, SCHEME_PP[2])) {NH_URL_END(ports_pp[1])}
//    else if (!nh_encoding_compareUTF32(scheme_p, SCHEME_PP[3])) {NH_URL_END(ports_pp[2])}
//    else if (!nh_encoding_compareUTF32(scheme_p, SCHEME_PP[4])) {NH_URL_END(ports_pp[1])}
//    else if (!nh_encoding_compareUTF32(scheme_p, SCHEME_PP[5])) {NH_URL_END(ports_pp[2])}
//
//NH_URL_END(NULL)
//}

NH_BOOL nh_url_isURLCodepoint(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()

    if (nh_encoding_isASCIIAlphaNumeric(codepoint)) {
        NH_URL_END(NH_TRUE)
    }
    if (codepoint == 0x21 || codepoint == 0x24 || codepoint == 0x26 || codepoint == 0x27 ||
        codepoint == 0x28 || codepoint == 0x29 || codepoint == 0x2A || codepoint == 0x2B ||
        codepoint == 0x2C || codepoint == 0x2D || codepoint == 0x2E || codepoint == 0x2F || 
        codepoint == 0x3A || codepoint == 0x3B || codepoint == 0x3D || codepoint == 0x3F ||
        codepoint == 0x40 || codepoint == 0x5F || codepoint == 0x7E) {
        NH_URL_END(NH_TRUE)
    }
    if (codepoint >= 0xA0 && codepoint <= 0x10FFFD) {
        if (!nh_encoding_isSurrogate(codepoint) && !nh_encoding_isNonCharacter(codepoint)) {
            NH_URL_END(NH_TRUE)
        }
    }

NH_URL_END(NH_FALSE)
}

NH_BOOL nh_url_isForbiddenHostCodepoint(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()
NH_URL_END(
    codepoint == 0 || codepoint == 0x09 || codepoint == 0x0A || codepoint == 0x0D || codepoint == 0x20 ||
    codepoint == 0x23 || codepoint == 0x25 || codepoint == 0x2F || codepoint == 0x3A || codepoint == 0x3C ||
    codepoint == 0x3E || codepoint == 0x3F || codepoint == 0x40 || codepoint == 0x5B || codepoint == 0x5C || 
    codepoint == 0x5D || codepoint == 0x5E || codepoint == 0x7C
)}

NH_BOOL nh_url_inC0PercentEncodeSet(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()
NH_URL_END(nh_encoding_isC0Control(codepoint) || codepoint > 0x7E)
}

NH_BOOL nh_url_isWindowsDriveLetter(
    nh_encoding_UTF32String *String_p)
{
NH_URL_BEGIN()
NH_URL_END(String_p->length == 2 && nh_encoding_isASCIIAlpha(String_p->p[0]) && (String_p->p[1] == 0x3A || String_p->p[1] == 0x7C))
}

NH_BOOL nh_url_isNormalizedWindowsDriveLetter(
    nh_encoding_UTF32String *String_p)
{
NH_URL_BEGIN()
NH_URL_END(nh_url_isWindowsDriveLetter(String_p) && String_p->p[1] == 0x3A)
}

NH_BOOL nh_url_stringStartsWithWindowsDriveLetter(
    nh_encoding_UTF32String *String_p)
{
NH_URL_BEGIN()
NH_URL_END(String_p->length >= 2 && nh_encoding_isASCIIAlpha(String_p->p[0]) && (String_p->p[1] == 0x3A || String_p->p[1] == 0x7C) && (String_p->length == 2 || String_p->p[2] == 0x2F || String_p->p[2] == 0x5C || String_p->p[2] == 0x3F || String_p->p[2] == 0x23))
}

