// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "URLParser.h"
#include "HostParser.h"
#include "Helper.h"
#include "PercentEncode.h"

#include "../Common/Macros.h"

#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/String.h"

#include "../../nh-core/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

typedef struct nh_url_Parser {
    NH_URL_PARSER_STATE state;
    NH_URL_PARSER_STATE stateOverride;
    NH_ENCODING_UTF32 *input_p;
    NH_ENCODING_UTF32 *pointer;
    nh_url_URL *URL_p;
    nh_url_URL *Base_p;
    nh_encoding_UTF32String Buffer;
    bool atSignSeen;
    bool insideBrackets;
    bool passwordTokenSeen;
} nh_url_Parser;

// HELPER ==========================================================================================

static bool nh_url_isFileScheme(
    NH_ENCODING_UTF32 *scheme_p)
{
NH_URL_BEGIN()
NH_URL_END(nh_encoding_compareUTF32ASCII(scheme_p, "file"))
}

static NH_API_RESULT nh_url_shortenURLPath(
    nh_url_URL *URL_p)
{
NH_URL_BEGIN()

    if (nh_url_isFileScheme(URL_p->Scheme.p) && URL_p->Path.length == 1) {	
        nh_encoding_UTF32String UTF32 = 
            nh_encoding_decodeUTF8(((nh_core_String*)URL_p->Path.p)->p, ((nh_core_String*)URL_p->Path.p)->length, NULL);
        bool skip = nh_url_isNormalizedWindowsDriveLetter(&UTF32);
        nh_encoding_freeUTF32(&UTF32);
        if (skip) {NH_URL_END(NH_API_SUCCESS)}
    }

    if (URL_p->Path.length > 0) {
        nh_core_freeString(&((nh_core_String*)URL_p->Path.p)[URL_p->Path.length-1]);
        nh_core_removeTailFromArray(&URL_p->Path, 1);
    }

NH_URL_END(NH_API_SUCCESS)
}

static bool nh_url_hasTwoRemainingASCIIHexDigits(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer != 0
    &&  nh_encoding_isASCIIHexDigit(*(Parser_p->pointer+1)) 
    &&  nh_encoding_isASCIIHexDigit(*(Parser_p->pointer+2))) {
        NH_URL_END(true)
    }

NH_URL_END(false)
}

// STATES ==========================================================================================

static NH_API_RESULT nh_url_parseSchemeStart(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (nh_encoding_isASCIIAlpha(*Parser_p->pointer)) {
        nh_encoding_appendUTF32Codepoint(&Parser_p->Buffer, tolower((char)*Parser_p->pointer));
        Parser_p->state = NH_URL_PARSER_STATE_SCHEME;
    }
    else if (!Parser_p->stateOverride) {
        Parser_p->state = NH_URL_PARSER_STATE_NO_SCHEME;
        Parser_p->pointer--;
    }
    else {
        // val err
        NH_URL_END(NH_API_ERROR_BAD_STATE)
    }
 
NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseScheme(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (nh_encoding_isASCIIAlphaNumeric(*Parser_p->pointer)
    ||  *Parser_p->pointer == 0x2B
    ||  *Parser_p->pointer == 0x2D
    ||  *Parser_p->pointer == 0x2E) {
        nh_encoding_appendUTF32Codepoint(&Parser_p->Buffer, tolower((char)*Parser_p->pointer));
    }
    else if (*Parser_p->pointer == 0x3A) 
    {
        if (Parser_p->stateOverride) {
//            if (nh_url_isSpecialScheme(Parser_p->URL_p->Scheme.p) && !nh_url_isSpecialScheme(Parser_p->Buffer.p)) {
//                // return
//            }
//            if (!nh_url_isSpecialScheme(Parser_p->URL_p->Scheme.p) && nh_url_isSpecialScheme(Parser_p->Buffer.p)) {
//                // return
//            }
//            // TODO
        }

        nh_encoding_appendUTF32(&Parser_p->URL_p->Scheme, Parser_p->Buffer.p, Parser_p->Buffer.length);
        if (Parser_p->stateOverride) {
            // TODO
        }

        nh_encoding_freeUTF32(&Parser_p->Buffer);

        if (nh_url_isFileScheme(Parser_p->URL_p->Scheme.p)) {
            if (*Parser_p->pointer && *Parser_p->pointer != '/' && *(Parser_p->pointer+1) != '/') {
                // val err
            } 
            Parser_p->state = NH_URL_PARSER_STATE_FILE;
        }
        else if (nh_url_getSpecialScheme(Parser_p->URL_p->Scheme.p) && Parser_p->Base_p && nh_encoding_compareUTF32(Parser_p->URL_p->Scheme.p, Parser_p->Base_p->Scheme.p)) {
            Parser_p->state = NH_URL_PARSER_STATE_SPECIAL_RELATIVE_OR_AUTHORITY;
        }
        else if (nh_url_getSpecialScheme(Parser_p->URL_p->Scheme.p)) {
            Parser_p->state = NH_URL_PARSER_STATE_SPECIAL_AUTHORITY_SLASHES;
        }
        else if (*Parser_p->pointer == 0x2F) {
            Parser_p->state = NH_URL_PARSER_STATE_PATH_OR_AUTHORITY;
            Parser_p->pointer++;
        }
        else {
            Parser_p->URL_p->cannotBeABaseURL = true;
            Parser_p->state = NH_URL_PARSER_STATE_CANNOT_BE_A_BASE_URL_PATH;
        }
    }
    else if (!Parser_p->stateOverride) {
        nh_encoding_freeUTF32(&Parser_p->Buffer);
        Parser_p->state = NH_URL_PARSER_STATE_NO_SCHEME;
        Parser_p->pointer = Parser_p->input_p;
    }
    else {
        // val err
        NH_URL_END(NH_API_ERROR_BAD_STATE)
    }
 
NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseNoScheme(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (!Parser_p->Base_p || (Parser_p->Base_p->cannotBeABaseURL && *Parser_p->pointer != 0x23)) {
        // val err
        NH_URL_END(NH_API_ERROR_BAD_STATE)
    }
    else if (Parser_p->Base_p->cannotBeABaseURL && *Parser_p->pointer == 0x23) {
        // TODO
    }
    else if (!nh_url_isFileScheme(Parser_p->Base_p->Scheme.p)) {
        Parser_p->state = NH_URL_PARSER_STATE_RELATIVE;
        Parser_p->pointer--;
    }
    else {
        Parser_p->state = NH_URL_PARSER_STATE_FILE;
        Parser_p->pointer--;
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseSpecialRelativeOrAuthority(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer == 0x2F && *(Parser_p->pointer+1) == 0x2F) {
        Parser_p->state = NH_URL_PARSER_STATE_SPECIAL_AUTHORITY_IGNORE_SLASHES;
        Parser_p->pointer++;
    }
    else {
        // val err
        Parser_p->state = NH_URL_PARSER_STATE_RELATIVE;
        Parser_p->pointer--;
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parsePathOrAuthority(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer == 0x2F) {
        Parser_p->state = NH_URL_PARSER_STATE_AUTHORITY;
    }
    else {
        // val err
        Parser_p->state = NH_URL_PARSER_STATE_PATH;
        Parser_p->pointer--;
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseRelative(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

//    // TODO
//
//    if (*Parser_p->pointer == 0x2F) {
//        Parser_p->state = NH_URL_PARSER_STATE_RELATIVE_SLASH;
//    }
//    else if ( && *Parser_p->pointer == 0x5C) {
//        // val err
//        Parser_p->state = NH_URL_PARSER_STATE_RELATIVE_SLASH;
//    }
//    else {
//        // TODO
//    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseRelativeSlash(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

//    if (nh_url_isSpecial(Parser_p->URL_p) && (*Parser_p->pointer == 0x2F || *Parser_p->pointer == 0x5C)) {
//        if (*Parser_p->pointer == 0x5C) {
//            // val err
//        }
//        Parser_p->state = NH_URL_PARSER_STATE_SPECIAL_AUTHORITY_IGNORE_SLASHES;
//    }
//    else if (*Parser_p->pointer == 0x2F) {
//        Parser_p->state = NH_URL_PARSER_STATE_AUTHORITY;
//    }
//    else {
//        // TODO
//    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseSpecialAuthoritySlashes(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer == 0x2F && *(Parser_p->pointer+1) == 0x2F) {
        Parser_p->state = NH_URL_PARSER_STATE_SPECIAL_AUTHORITY_IGNORE_SLASHES;
        Parser_p->pointer++;
    }
    else {
        // val err
        Parser_p->state = NH_URL_PARSER_STATE_SPECIAL_AUTHORITY_IGNORE_SLASHES;
        Parser_p->pointer--;
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseSpecialAuthorityIgnoreSlashes(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer != 0x2F && *Parser_p->pointer != 0x5C) {
        Parser_p->state = NH_URL_PARSER_STATE_AUTHORITY;
        Parser_p->pointer--;
    }
    else {
        // val err
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseAuthority(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer == 0x40) {
        // val err
        // TODO
    }
    else if (*Parser_p->pointer == 0 || *Parser_p->pointer == 0x2F || *Parser_p->pointer == 0x3F || *Parser_p->pointer == 0x23) {
        if (Parser_p->atSignSeen && Parser_p->Buffer.length == 0) {
            // val err
            NH_URL_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
        }
        Parser_p->pointer -= Parser_p->Buffer.length+1;
        nh_encoding_freeUTF32(&Parser_p->Buffer);
        Parser_p->state = NH_URL_PARSER_STATE_HOST;
    }
    else {
        nh_encoding_appendUTF32Codepoint(&Parser_p->Buffer, *Parser_p->pointer);
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseHostName(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (Parser_p->stateOverride) {
        Parser_p->state = NH_URL_PARSER_STATE_FILE_HOST;
        // TODO
    }
    else if (*Parser_p->pointer == 0x3A && !Parser_p->insideBrackets) {
        if (Parser_p->Buffer.length == 0) {
            // val err
            NH_URL_END(NH_API_ERROR_BAD_STATE)
        }
        if (Parser_p->stateOverride && Parser_p->stateOverride == NH_URL_PARSER_STATE_HOSTNAME) {
            NH_URL_END(NH_API_SUCCESS)
        }
        // TODO
    }
    else if (*Parser_p->pointer == 0 || *Parser_p->pointer == 0x2F || *Parser_p->pointer == 0x3F || *Parser_p->pointer == 0x23) {
        Parser_p->pointer--;
        if (nh_url_getSpecialScheme(Parser_p->URL_p->Scheme.p) && Parser_p->Buffer.length == 0) {
            // val err
            NH_URL_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
        }
        else if (Parser_p->stateOverride && Parser_p->Buffer.length == 0) {
            // TODO
        }
        Parser_p->URL_p->Host_p = (nh_url_Host*)nh_core_allocate(sizeof(nh_url_Host));
        NH_CORE_CHECK_MEM(Parser_p->URL_p->Host_p)
        NH_CORE_CHECK(nh_url_parseHost(Parser_p->Buffer, true, Parser_p->URL_p->Host_p))
        nh_encoding_freeUTF32(&Parser_p->Buffer);
        Parser_p->state = NH_URL_PARSER_STATE_PATH_START;
        if (Parser_p->stateOverride) {
            // TODO
        }
    }
    else {
        if (*Parser_p->pointer == 0x5B) {Parser_p->insideBrackets = true;}
        if (*Parser_p->pointer == 0x5D) {Parser_p->insideBrackets = false;}
        nh_encoding_appendUTF32Codepoint(&Parser_p->Buffer, *Parser_p->pointer);
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parsePort(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (nh_encoding_isASCIIDigit(*Parser_p->pointer)) {
        nh_encoding_appendUTF32Codepoint(&Parser_p->Buffer, *Parser_p->pointer);
    }
    else if (*Parser_p->pointer == 0 || *Parser_p->pointer == 0x2F || *Parser_p->pointer == 0x3F || *Parser_p->pointer == 0x23) {
        // TODO
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseFile(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer == 0x2F || *Parser_p->pointer == 0x5C) {
        if (*Parser_p->pointer == 0x5C) {
            // val err
        }
        Parser_p->state = NH_URL_PARSER_STATE_FILE_SLASH;
    }
    else if (Parser_p->Base_p && nh_url_isFileScheme(Parser_p->Base_p->Scheme.p)) {
        // TODO
    }
    else {
        Parser_p->state = NH_URL_PARSER_STATE_PATH;
        Parser_p->pointer--;
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseFileSlash(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer == 0x2F || *Parser_p->pointer == 0x5C) {
        if (*Parser_p->pointer == 0x5C) {
            // val err
        }
        Parser_p->state = NH_URL_PARSER_STATE_FILE_HOST;
    }
    else {
        if (Parser_p->Base_p && nh_url_isFileScheme(Parser_p->Base_p->Scheme.p)) {
            // TODO
        }
        Parser_p->state = NH_URL_PARSER_STATE_PATH;
        Parser_p->pointer--;
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseFileHost(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer == 0 || *Parser_p->pointer == 0x2F || *Parser_p->pointer == 0x5C || *Parser_p->pointer == 0x3F || *Parser_p->pointer == 0x23) {
        Parser_p->pointer--;
        if (!Parser_p->stateOverride && Parser_p->Buffer.length == 2 && nh_encoding_isASCIIAlpha(Parser_p->Buffer.p[0]) 
        && (Parser_p->Buffer.p[1] == 0x3A || Parser_p->Buffer.p[1] == 0x7C)) {
            Parser_p->state = NH_URL_PARSER_STATE_PATH;
        }
        else if (Parser_p->Buffer.length == 0) {
            if (Parser_p->stateOverride) {NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)}
            Parser_p->state = NH_URL_PARSER_STATE_PATH_START;
        }
        else {

        }
    }
    else {
        nh_encoding_appendUTF32Codepoint(&Parser_p->Buffer, *Parser_p->pointer);
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parsePathStart(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (nh_url_getSpecialScheme(Parser_p->URL_p->Scheme.p)) {
        if (*Parser_p->pointer == 0x5C) {
            // val err
        }
        Parser_p->state = NH_URL_PARSER_STATE_PATH;
        if (*Parser_p->pointer != 0x2F && *Parser_p->pointer != 0x5C) {
            Parser_p->pointer--;
        }
    }
    else if (!Parser_p->stateOverride && *Parser_p->pointer == 0x3F) {
        if (Parser_p->URL_p->Query_p) {NH_URL_END(NH_API_ERROR_BAD_STATE)}
        Parser_p->URL_p->Query_p = (nh_core_String*)nh_core_allocate(sizeof(nh_core_String));
        NH_CORE_CHECK_MEM(Parser_p->URL_p->Query_p)
        *Parser_p->URL_p->Query_p = nh_core_initString(16);
        Parser_p->state = NH_URL_PARSER_STATE_QUERY;
    }
    else if (!Parser_p->stateOverride && *Parser_p->pointer == 0x23) {
        if (Parser_p->URL_p->Fragment_p) {NH_URL_END(NH_API_ERROR_BAD_STATE)}
        Parser_p->URL_p->Fragment_p = (nh_core_String*)nh_core_allocate(sizeof(nh_core_String));
        NH_CORE_CHECK_MEM(Parser_p->URL_p->Fragment_p)
        *Parser_p->URL_p->Fragment_p = nh_core_initString(16);
        Parser_p->state = NH_URL_PARSER_STATE_FRAGMENT;
    }
    else if (*Parser_p->pointer != 0) {
        Parser_p->state = NH_URL_PARSER_STATE_PATH;
        if (*Parser_p->pointer != 0x2F) {
            Parser_p->pointer--;
        }
    }
    else if (Parser_p->stateOverride && !Parser_p->URL_p->Host_p) {
        // TODO
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parsePath(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if ((*Parser_p->pointer == 0 || *Parser_p->pointer == 0x2F)
    ||  (nh_url_getSpecialScheme(Parser_p->URL_p->Scheme.p) && *Parser_p->pointer == 0x5C)
    ||  (!Parser_p->stateOverride && (*Parser_p->pointer == 0x3F || *Parser_p->pointer == 0x23))) {
        if (nh_url_getSpecialScheme(Parser_p->URL_p->Scheme.p) && *Parser_p->pointer == 0x5C) {
            // val err
        }
        if (Parser_p->Buffer.length == 2 && Parser_p->Buffer.p[0] == '.' && Parser_p->Buffer.p[1] == '.') {
            nh_url_shortenURLPath(Parser_p->URL_p);
            // TODO
        }
        else if (Parser_p->Buffer.length == 1 && Parser_p->Buffer.p[0] == '.' && *Parser_p->pointer != 0x2F && nh_url_getSpecialScheme(Parser_p->URL_p->Scheme.p) && *Parser_p->pointer != 0x5C) {
            // TODO
        }
        else if (Parser_p->Buffer.length != 1 || Parser_p->Buffer.p[0] != '.') {
            if (nh_url_isFileScheme(Parser_p->URL_p->Scheme.p) && Parser_p->URL_p->Path.length == 0 && nh_url_isWindowsDriveLetter(&Parser_p->Buffer)) {
                Parser_p->Buffer.p[1] = 0x3A;
            }
            nh_core_String *Part_p = (nh_core_String*)nh_core_incrementArray(&Parser_p->URL_p->Path);
            NH_CORE_CHECK_MEM(Part_p)
            *Part_p = nh_core_initString(Parser_p->Buffer.length+1);
            for (int i = 0; i < Parser_p->Buffer.length; ++i) {
                if (Parser_p->Buffer.p[i] >= 128) {NH_URL_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}
                nh_core_appendByteToString(Part_p, (char)Parser_p->Buffer.p[i]);
            }
        }

        nh_encoding_freeUTF32(&Parser_p->Buffer);

        if (*Parser_p->pointer == 0x3F) {
            if (Parser_p->URL_p->Query_p) {NH_URL_END(NH_API_ERROR_BAD_STATE)}
            Parser_p->URL_p->Query_p = (nh_core_String*)nh_core_allocate(sizeof(nh_core_String));
            NH_CORE_CHECK_MEM(Parser_p->URL_p->Query_p)
            *Parser_p->URL_p->Query_p = nh_core_initString(16);
            Parser_p->state = NH_URL_PARSER_STATE_QUERY;
        }
        if (*Parser_p->pointer == 0x23) {
            if (Parser_p->URL_p->Fragment_p) {NH_URL_END(NH_API_ERROR_BAD_STATE)}
            Parser_p->URL_p->Fragment_p = (nh_core_String*)nh_core_allocate(sizeof(nh_core_String));
            NH_CORE_CHECK_MEM(Parser_p->URL_p->Fragment_p)
            *Parser_p->URL_p->Fragment_p = nh_core_initString(16);
            Parser_p->state = NH_URL_PARSER_STATE_FRAGMENT;
        }
    }
    else {
        if (!nh_url_isURLCodepoint(*Parser_p->pointer) && *Parser_p->pointer != 0x25) {
            // val err
        }
        if (*Parser_p->pointer == 0x25 && nh_encoding_isASCIIHexDigit(*(Parser_p->pointer+1)) 
        &&  nh_encoding_isASCIIHexDigit(*(Parser_p->pointer+2))) {
            // val err
        }
        nh_core_String String = nh_url_percentEncodeCodepointAfterEncodingUTF8(
            *Parser_p->pointer, NH_URL_PERCENT_ENCODE_SET_PATH
        );
        nh_encoding_UTF32String Decoded = nh_encoding_decodeUTF8(String.p, String.length, NULL);
        nh_encoding_appendUTF32(&Parser_p->Buffer, Decoded.p, Decoded.length);
        nh_encoding_freeUTF32(&Decoded);
        nh_core_freeString(&String);
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseCannotBeABaseURLPath(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer == 0x3F) {
        if (Parser_p->URL_p->Query_p) {
            NH_URL_END(NH_API_ERROR_BAD_STATE)
        }
        Parser_p->URL_p->Query_p = (nh_core_String*)nh_core_allocate(sizeof(nh_core_String));
        NH_CORE_CHECK_MEM(Parser_p->URL_p->Query_p)
        *Parser_p->URL_p->Query_p = nh_core_initString(16);
        Parser_p->state = NH_URL_PARSER_STATE_QUERY;
    }
    else if (*Parser_p->pointer == 0x23) {
        if (Parser_p->URL_p->Fragment_p) {
            NH_URL_END(NH_API_ERROR_BAD_STATE)
        }
        Parser_p->URL_p->Fragment_p = (nh_core_String*)nh_core_allocate(sizeof(nh_core_String));
        NH_CORE_CHECK_MEM(Parser_p->URL_p->Fragment_p)
        *Parser_p->URL_p->Fragment_p = nh_core_initString(16);
        Parser_p->state = NH_URL_PARSER_STATE_FRAGMENT;
    }
    else {
        if (*Parser_p->pointer != 0 && !nh_url_isURLCodepoint(*Parser_p->pointer) && *Parser_p->pointer != 0x25) {
            // val err
        }
        if (*Parser_p->pointer == 0x25 && nh_encoding_isASCIIHexDigit(*(Parser_p->pointer+1)) 
        &&  nh_encoding_isASCIIHexDigit(*(Parser_p->pointer+2))) {
            // val err
        }
    }

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseQuery(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    // TODO

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_url_parseFragment(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    if (*Parser_p->pointer) {
        if (!nh_url_isURLCodepoint(*Parser_p->pointer) && *Parser_p->pointer != 0x25) {
            // val err
        }
        if (*Parser_p->pointer == 0x25 && !nh_url_hasTwoRemainingASCIIHexDigits(Parser_p)) {
            // val err
        }
        nh_core_String String = nh_url_percentEncodeCodepointAfterEncodingUTF8(
            *Parser_p->pointer, NH_URL_PERCENT_ENCODE_SET_FRAGMENT
        );
        nh_core_appendToString(Parser_p->URL_p->Fragment_p, String.p, String.length);
        nh_core_freeString(&String);
    } 

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// STATE MACHINE ===================================================================================

static NH_API_RESULT nh_url_runStateMachine(
    nh_url_Parser *Parser_p)
{
NH_URL_BEGIN()

    switch (Parser_p->state)
    {
        case NH_URL_PARSER_STATE_SCHEME_START                     : NH_URL_END(nh_url_parseSchemeStart(Parser_p))
        case NH_URL_PARSER_STATE_SCHEME                           : NH_URL_END(nh_url_parseScheme(Parser_p))
        case NH_URL_PARSER_STATE_NO_SCHEME                        : NH_URL_END(nh_url_parseNoScheme(Parser_p))
        case NH_URL_PARSER_STATE_SPECIAL_RELATIVE_OR_AUTHORITY    : NH_URL_END(nh_url_parseSpecialRelativeOrAuthority(Parser_p))
        case NH_URL_PARSER_STATE_PATH_OR_AUTHORITY                : NH_URL_END(nh_url_parsePathOrAuthority(Parser_p))
        case NH_URL_PARSER_STATE_RELATIVE                         : NH_URL_END(nh_url_parseRelative(Parser_p))
        case NH_URL_PARSER_STATE_RELATIVE_SLASH                   : NH_URL_END(nh_url_parseRelativeSlash(Parser_p))
        case NH_URL_PARSER_STATE_SPECIAL_AUTHORITY_SLASHES        : NH_URL_END(nh_url_parseSpecialAuthoritySlashes(Parser_p))
        case NH_URL_PARSER_STATE_SPECIAL_AUTHORITY_IGNORE_SLASHES : NH_URL_END(nh_url_parseSpecialAuthorityIgnoreSlashes(Parser_p))
        case NH_URL_PARSER_STATE_AUTHORITY                        : NH_URL_END(nh_url_parseAuthority(Parser_p))
        case NH_URL_PARSER_STATE_HOST                             :
        case NH_URL_PARSER_STATE_HOSTNAME                         : NH_URL_END(nh_url_parseHostName(Parser_p))
        case NH_URL_PARSER_STATE_PORT                             : NH_URL_END(nh_url_parsePort(Parser_p))
        case NH_URL_PARSER_STATE_FILE                             : NH_URL_END(nh_url_parseFile(Parser_p))
        case NH_URL_PARSER_STATE_FILE_SLASH                       : NH_URL_END(nh_url_parseFileSlash(Parser_p))
        case NH_URL_PARSER_STATE_FILE_HOST                        : NH_URL_END(nh_url_parseFileHost(Parser_p))
        case NH_URL_PARSER_STATE_PATH_START                       : NH_URL_END(nh_url_parsePathStart(Parser_p))
        case NH_URL_PARSER_STATE_PATH                             : NH_URL_END(nh_url_parsePath(Parser_p))
        case NH_URL_PARSER_STATE_CANNOT_BE_A_BASE_URL_PATH        : NH_URL_END(nh_url_parseCannotBeABaseURLPath(Parser_p))
        case NH_URL_PARSER_STATE_QUERY                            : NH_URL_END(nh_url_parseQuery(Parser_p))
        case NH_URL_PARSER_STATE_FRAGMENT                         : NH_URL_END(nh_url_parseFragment(Parser_p))
    }

NH_URL_END(NH_API_ERROR_BAD_STATE)
}

// PARSE ===========================================================================================

static nh_url_URL *nh_url_initURL(
    nh_url_URL *URL_p)
{
NH_URL_BEGIN()

    URL_p->Scheme = nh_encoding_initUTF32(16); 
    URL_p->UserName = nh_encoding_initUTF32(16);
    URL_p->Password = nh_encoding_initUTF32(16);
    URL_p->Host_p = NULL; 
    URL_p->port_p = NULL;
    URL_p->Path = nh_core_initArray(sizeof(nh_core_String), 8);
    URL_p->Query_p = NULL;
    URL_p->Fragment_p = NULL;
    URL_p->cannotBeABaseURL = false;

NH_URL_END(URL_p)
}

nh_url_URL *nh_url_parseBasicURL(
    nh_encoding_UTF32String Input, nh_url_URL *Base_p, nh_url_URL *URL_p, NH_URL_PARSER_STATE stateOverride)
{
NH_URL_BEGIN()

    nh_url_Parser Parser;
    Parser.URL_p = URL_p ? nh_url_initURL(URL_p) : NULL;
    Parser.Base_p = Base_p;

    if (!Parser.URL_p) 
    {
        nh_url_URL *URL_p = (nh_url_URL*)nh_core_allocate(sizeof(nh_url_URL));
        NH_CORE_CHECK_NULL_2(NULL, URL_p)

        Parser.URL_p = nh_url_initURL(URL_p);

        while (Input.length && (nh_encoding_isC0Control(Input.p[0]) || Input.p[0] == 0x20)) {
            // val err
            nh_encoding_removeUTF32(&Input, 0, 1);
        }

        while (Input.length && (nh_encoding_isC0Control(Input.p[Input.length-1]) || Input.p[Input.length-1] == 0x20)) {
            // val err
            nh_encoding_removeUTF32Tail(&Input, 1);
        }
    }

    for (int i = 0; i < Input.length; ++i) {
        if (Input.p[i] == 0x09 || Input.p[i] == 0x0A || Input.p[i] == 0x0D) {
            // val err
            nh_encoding_removeUTF32(&Input, i, 1);
            i = 0;
        }
    }

    nh_encoding_appendUTF32Codepoint(&Input, 0); // make sure there is a EOF codepoint

    Parser.state = stateOverride ? stateOverride : NH_URL_PARSER_STATE_SCHEME_START;
    Parser.stateOverride = stateOverride ? stateOverride : NH_URL_PARSER_STATE_UNDEFINED;
    Parser.Buffer = nh_encoding_initUTF32(16);
    Parser.atSignSeen = false;
    Parser.insideBrackets = false;
    Parser.passwordTokenSeen = false;
    Parser.pointer = Input.p;
    Parser.input_p = Input.p;

    while (1) {
        nh_url_runStateMachine(&Parser);
        if (*Parser.pointer == 0) {break;}
        Parser.pointer++;
    }

NH_URL_END(Parser.URL_p)
}

nh_url_URL *nh_url_runURLParser(
    nh_encoding_UTF32String Input, nh_url_URL *Base_p, nh_url_URL *URL_p)

{
NH_URL_BEGIN()

    URL_p = nh_url_parseBasicURL(Input, Base_p, NULL, NH_URL_PARSER_STATE_UNDEFINED);
    if (!URL_p) {NH_URL_END(NULL)}

    if (URL_p->Scheme.p && nh_encoding_compareUTF32ASCII(URL_p->Scheme.p, "blob")) {
        // TODO
    }

NH_URL_END(URL_p)
}

