// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Parser.h"

#include "../Common/Log.h"
#include "../Interfaces/Document.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-webidl/Runtime/String.h"
#include "../../nh-encoding/Encodings/UTF32.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// PARSE ERROR =====================================================================================

NH_API_RESULT nh_html_newParseError(
    nh_html_Parser *Parser_p, unsigned long index, NH_HTML_PARSE_ERROR type)
{
    nh_html_ParseError *Error_p = (nh_html_ParseError*)nh_core_incrementArray(&Parser_p->Errors);
    NH_CORE_CHECK_MEM(Error_p)

    Error_p->type  = type;
    Error_p->index = index;

    return NH_API_SUCCESS;
}

// PRE-PROCESS =====================================================================================

static nh_webidl_USVString nh_html_normalizeNewlines(
    nh_webidl_USVString Codepoints)
{
    nh_webidl_USVString Normalized = nh_webidl_initUSVString(Codepoints.length);
    NH_ENCODING_UTF32 lf = 0x0A;

    unsigned long index = 0;
    while (index < Codepoints.length) {
        if (index + 1 < Codepoints.length) {
            if (Codepoints.p[index] == 0x0D && Codepoints.p[index + 1] == 0x0A) {
                nh_webidl_appendToUSVString(&Normalized, &lf, 1);
                index += 2;
                continue;
            }
        }
        nh_webidl_appendToUSVString(&Normalized, &Codepoints.p[index], 1);
        index++;
    }

    index = 0;
    while (index < Normalized.length) {
        if (Normalized.p[index] == 0x0D) {Normalized.p[index] = 0x0A;}
        index++; 
    }

    return Normalized;
}

static NH_API_RESULT nh_html_checkInputStream(
    nh_html_Parser *Parser_p, nh_encoding_UTF32String InputStream)
{
    for (unsigned long i = 0; i < InputStream.length; ++i) 
    {
        if (nh_encoding_isSurrogate(InputStream.p[i])) {
            NH_CORE_CHECK(nh_html_newParseError(Parser_p, i, NH_HTML_PARSE_ERROR_SURROGATE_IN_INPUT_STREAM)) 
        }
        else if (nh_encoding_isNonCharacter(InputStream.p[i])) {
            NH_CORE_CHECK(nh_html_newParseError(Parser_p, i, NH_HTML_PARSE_ERROR_NONCHARACTER_IN_INPUT_STREAM))
        }
        else if (nh_encoding_isControl(InputStream.p[i]) && !nh_encoding_isASCIIWhitespace(InputStream.p[i]) && InputStream.p[i] != 0x00) {
            NH_CORE_CHECK(nh_html_newParseError(Parser_p, i, NH_HTML_PARSE_ERROR_CONTROL_CHARACTER_IN_INPUT_STREAM))
        }
    } 

    return NH_API_SUCCESS;
}

// PARSER ==========================================================================================

static nh_html_Parser nh_html_initParser(
    nh_webidl_Object *Document_p)
{
    nh_html_Parser Parser;

    Parser.insertionMode            = NH_HTML_INSERTION_MODE_INITIAL;
    Parser.originalInsertionMode    = Parser.insertionMode;
    Parser.framesetOk               = true;
    Parser.pause                    = false;
    Parser.stop                     = false;
    Parser.scripting                = true;
    Parser.fosterParenting          = false;
    Parser.scriptNestingLevel       = 0;
    Parser.OpenElements             = nh_core_initStack(255);
    Parser.TemplateInsertionModes   = nh_core_initStack(255);
    Parser.ActiveFormattingElements = nh_core_initList(64);
    Parser.Document_p               = Document_p;
    Parser.HeadElement_p            = NULL;
    Parser.FormElement_p            = NULL;
    Parser.Errors                   = nh_core_initArray(sizeof(nh_html_ParseError), 16);
    Parser.Token_p                  = NULL;
    Parser.Tokenizer_p              = NULL;

    return Parser;
}

NH_API_RESULT nh_html_parseDocument(
    char *logId_p, nh_encoding_UTF32String InputStream, nh_webidl_Object *Document_p)
{
    NH_CORE_CHECK_NULL(Document_p)

    nh_html_Parser Parser = nh_html_initParser(Document_p);

    NH_CORE_CHECK(nh_html_checkInputStream(&Parser, InputStream))
    nh_webidl_USVString NormalizedInputStream = nh_html_normalizeNewlines(InputStream);

    nh_html_Tokenizer Tokenizer = nh_html_initTokenizer(&Parser, NormalizedInputStream);
    Parser.Tokenizer_p = &Tokenizer;

    while (!Parser.stop) {
        NH_CORE_CHECK(nh_html_consumeNext(&Tokenizer))
    }

    if (logId_p) {
        NH_CORE_CHECK(nh_html_logDocument(logId_p, Parser.Document_p))
    }

    nh_webidl_freeUSVString(&Tokenizer.InputStream);

    return NH_API_SUCCESS;
}
