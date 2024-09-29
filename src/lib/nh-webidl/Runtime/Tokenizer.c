// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Tokenizer.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-encoding/Base/UnicodeDataHelper.h"
#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

const char *NH_WEBIDL_TOKENS_PP[] = {
    "Whitespace",
    "LineTerminator",
    "Comment",
    "IdentifierName",
    "Punctuator",
    "DecimalLiteral",
    "DecimalBigIntegerLiteral",
    "NonDecimalBinaryIntegerLiteral",
    "NonDecimalOctalIntegerLiteral",
    "NonDecimalHexIntegerLiteral",
    "StringLiteral",
};

// TOKENIZER =======================================================================================

static bool nh_webidl_isDigit(
    NH_ENCODING_UTF32 codepoint, bool zero)
{
NH_WEBIDL_BEGIN()

    if (codepoint == '1' 
    ||  codepoint == '2'
    ||  codepoint == '3'
    ||  codepoint == '4'
    ||  codepoint == '5'
    ||  codepoint == '6'
    ||  codepoint == '7'
    ||  codepoint == '8'
    ||  codepoint == '9')
    {
        NH_WEBIDL_END(true)
    }

    if (zero && codepoint == '0') {NH_WEBIDL_END(true)}

NH_WEBIDL_END(false)
}

static unsigned int nh_webidl_isDigits(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    if (nh_webidl_isDigit(codepoints_p[0], true)) {
        unsigned int i = 1;
        while (i < length && (nh_webidl_isDigit(codepoints_p[i], true) || codepoints_p[i] == '_')) {
            i++;
        }
        NH_WEBIDL_END(i)
    }

NH_WEBIDL_END(0)
}

static unsigned int nh_webidl_isSignedInteger(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    int offset = 0;
    if (codepoints_p[0] == '+' || codepoints_p[0] == '-') {
        offset++;
    }
    int digits = nh_webidl_isDigits(offset == 0 ? codepoints_p : &codepoints_p[1], true);
    if (digits == 0) {NH_WEBIDL_END(0)}

NH_WEBIDL_END(offset + digits)
}

static unsigned int nh_webidl_isExponentPart(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    if (codepoints_p[0] != 'e' && codepoints_p[0] != 'E') {
        NH_WEBIDL_END(0)
    }
    int digits = nh_webidl_isSignedInteger(&codepoints_p[1], true);

NH_WEBIDL_END(digits)
}

static unsigned int nh_webidl_isDecimalIntegerLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    if (codepoints_p[0] == '0') {NH_WEBIDL_END(1)}

    if (nh_webidl_isDigit(codepoints_p[0], false)) 
    {
        int offset = 1;
        if (length > 1 && codepoints_p[1] == '_') {
            offset++;
        }
        int digits = nh_webidl_isDigits(&codepoints_p[offset], length - offset);
        if (digits == 0 && offset == 2) {NH_WEBIDL_END(0)}
        NH_WEBIDL_END(digits + offset)
    }

NH_WEBIDL_END(0)
}

static unsigned int nh_webidl_isWhiteSpace(
    NH_ENCODING_UTF32 codepoint)
{
NH_WEBIDL_BEGIN()

    if (codepoint == 0x0009  // CHARACTER TABULATION <TAB> 
    ||  codepoint == 0x000B  // LINE TABULATION <VT> 
    ||  codepoint == 0x000C  // FORM FEED (FF) 
    ||  codepoint == 0x0020) // SPACE <SP> 
    {
        NH_WEBIDL_END(1)
    }

NH_WEBIDL_END(0)
}

static unsigned int nh_webidl_isLineTerminator(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()


    if (codepoints_p[0] == 0x0A  // LINE FEED <LF> 
    ||  codepoints_p[0] == 0x0D) // CARRIAGE RETURN (CR) <CR> 
    {
        if (length > 1 && codepoints_p[0] == 0x0D && codepoints_p[1] == 0x0A) {
            NH_WEBIDL_END(2)
        }
        NH_WEBIDL_END(1)
    }

NH_WEBIDL_END(0)
}

static unsigned int nh_webidl_isComment(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    if (codepoints_p[0] == 0x2F) // '/'
    {
        // single line comment
        if (length > 1 && codepoints_p[1] == 0x2F) 
        {
            unsigned int i = 2;
            while (i < length && !nh_webidl_isLineTerminator(&codepoints_p[i], length - i)) {
                i++;
            }
            NH_WEBIDL_END(i)
        }

        // multi line comment 
        if (length > 1 && codepoints_p[1] == 0x2A) 
        {
            unsigned int i = 2;
            while (i < length - 1 && codepoints_p[i] != 0x2A && codepoints_p[i + 1] != 0x2F) {
                i++;
            }
            NH_WEBIDL_END(i + 2)
        }
    }

NH_WEBIDL_END(0)
}

static unsigned int nh_webidl_isPunctuator(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    if (length > 3) {
        if (codepoints_p[0] == '>' && codepoints_p[1] == '>' && codepoints_p[2] == '>' && codepoints_p[3] == '=')
        {
            NH_WEBIDL_END(4)
        }
    }

    if (length > 2) {
        if ((codepoints_p[0] == '.' && codepoints_p[1] == '.' && codepoints_p[2] == '.')
        ||  (codepoints_p[0] == '=' && codepoints_p[1] == '=' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '!' && codepoints_p[1] == '=' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '>' && codepoints_p[2] == '>')
        ||  (codepoints_p[0] == '*' && codepoints_p[1] == '*' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '<' && codepoints_p[1] == '<' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '>' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '&' && codepoints_p[1] == '&' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '|' && codepoints_p[1] == '|' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '?' && codepoints_p[1] == '?' && codepoints_p[2] == '='))
        {
            NH_WEBIDL_END(3)
        }
    }

    if (length > 1) {
        if ((codepoints_p[0] == '<' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '=' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '!' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '*' && codepoints_p[1] == '*')
        ||  (codepoints_p[0] == '+' && codepoints_p[1] == '+')
        ||  (codepoints_p[0] == '-' && codepoints_p[1] == '-')
        ||  (codepoints_p[0] == '<' && codepoints_p[1] == '<')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '>')
        ||  (codepoints_p[0] == '&' && codepoints_p[1] == '&')
        ||  (codepoints_p[0] == '|' && codepoints_p[1] == '|')
        ||  (codepoints_p[0] == '?' && codepoints_p[1] == '?')
        ||  (codepoints_p[0] == '+' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '-' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '*' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '%' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '&' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '|' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '^' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '=' && codepoints_p[1] == '>'))
        {
            NH_WEBIDL_END(2)
        }
        if (codepoints_p[0] == '?' && codepoints_p[1] == '.')
        { 
            if (length > 2 && nh_webidl_isDigit(codepoints_p[2], true)) {}
            else {NH_WEBIDL_END(2)}
        }
    }

    if (codepoints_p[0] == '{'
    ||  codepoints_p[0] == '}'
    ||  codepoints_p[0] == '('
    ||  codepoints_p[0] == ')'
    ||  codepoints_p[0] == '['
    ||  codepoints_p[0] == ']'
    ||  codepoints_p[0] == '.'
    ||  codepoints_p[0] == ';'
    ||  codepoints_p[0] == ','
    ||  codepoints_p[0] == '<' 
    ||  codepoints_p[0] == '>' 
    ||  codepoints_p[0] == '+' 
    ||  codepoints_p[0] == '-' 
    ||  codepoints_p[0] == '*' 
    ||  codepoints_p[0] == '%' 
    ||  codepoints_p[0] == '&' 
    ||  codepoints_p[0] == '|' 
    ||  codepoints_p[0] == '^' 
    ||  codepoints_p[0] == '!' 
    ||  codepoints_p[0] == '~' 
    ||  codepoints_p[0] == '?' 
    ||  codepoints_p[0] == ':' 
    ||  codepoints_p[0] == '@' 
    ||  codepoints_p[0] == '=') 
    {
        NH_WEBIDL_END(1)
    }

NH_WEBIDL_END(0)
}

static bool nh_webidl_isHexDigit(
    NH_ENCODING_UTF32 codepoint)
{
NH_WEBIDL_BEGIN()

    if (codepoint == '0'
    ||  codepoint == '1'
    ||  codepoint == '2'
    ||  codepoint == '3'
    ||  codepoint == '4'
    ||  codepoint == '5'
    ||  codepoint == '6'
    ||  codepoint == '7'
    ||  codepoint == '8'
    ||  codepoint == '9'
    ||  codepoint == 'a'
    ||  codepoint == 'b'
    ||  codepoint == 'c'
    ||  codepoint == 'd'
    ||  codepoint == 'e'
    ||  codepoint == 'f'
    ||  codepoint == 'A'
    ||  codepoint == 'B'
    ||  codepoint == 'C'
    ||  codepoint == 'D'
    ||  codepoint == 'E'
    ||  codepoint == 'F') {NH_WEBIDL_END(true)}

NH_WEBIDL_END(false)
}

static unsigned int nh_webidl_isHexDigits(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    int digits = 0;

    while (length >= 1 && nh_webidl_isHexDigit(codepoints_p[digits])) {
        digits++;
        length--;
    }

NH_WEBIDL_END(digits)
}

static unsigned int nh_webidl_isHexIntegerLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    int digits = 0;

    if (length > 2 && codepoints_p[0] == '0' && (codepoints_p[1] == 'x' || codepoints_p[1] == 'X')) {
        digits = nh_webidl_isHexDigits(&codepoints_p[2], length - 2);
    }
    if (digits > 0) {digits += 2;}

NH_WEBIDL_END(digits)
}

// https://tc39.es/ecma262/#sec-literals-numeric-literals
static unsigned int nh_webidl_isNumericLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length, NH_WEBIDL_TOKEN *type_p)
{
NH_WEBIDL_BEGIN()

// NonDecimalHexIntegerLiteral

    int digits = nh_webidl_isHexIntegerLiteral(codepoints_p, length);
    if (digits > 0) {
        *type_p = NH_WEBIDL_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL;
        NH_WEBIDL_END(digits)
    }

// DecimalBigIntegerLiteral
    if (length > 1 && codepoints_p[0] == '0' && codepoints_p[1] == 'n') 
    {
        *type_p = NH_WEBIDL_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
        NH_WEBIDL_END(2)
    }

    if (nh_webidl_isDigit(codepoints_p[0], false) && length > 1) 
    {
        if (codepoints_p[1] == '_') {
            digits = nh_webidl_isDigits(&codepoints_p[2], length - 2);
            if (digits > 0 && length > digits + 2 && codepoints_p[digits + 2] == 'n') {
                *type_p = NH_WEBIDL_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
                NH_WEBIDL_END(digits + 3)
            } 
        }

        digits = nh_webidl_isDigits(&codepoints_p[1], length - 1);
        if (length > digits + 1 && codepoints_p[digits + 1] == 'n') {
            *type_p = NH_WEBIDL_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
            NH_WEBIDL_END(digits + 2)
        } 
    }

// DecimalLiteral
    if (codepoints_p[0] == '.')
    {
        digits = nh_webidl_isDigits(&codepoints_p[1], length - 1) + 1;
        if (digits > 1) {
            digits += nh_webidl_isExponentPart(&codepoints_p[digits], length - digits);
            *type_p = NH_WEBIDL_TOKEN_DECIMAL_LITERAL;
            NH_WEBIDL_END(digits)
        }
    }

    digits = nh_webidl_isDecimalIntegerLiteral(codepoints_p, length);
    if (digits > 0) 
    {
        *type_p = NH_WEBIDL_TOKEN_DECIMAL_LITERAL;

        if (length > digits && codepoints_p[digits] == '.') {
            digits++;
            digits += nh_webidl_isDigits(&codepoints_p[digits], length - digits);
            digits += nh_webidl_isExponentPart(&codepoints_p[digits], length - digits);
            NH_WEBIDL_END(digits)
        }

        digits += nh_webidl_isExponentPart(&codepoints_p[digits], length - digits);
        NH_WEBIDL_END(digits)
    }

NH_WEBIDL_END(0)
}

static unsigned int nh_webidl_isStringLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    if (codepoints_p[0] == '"') {
        int count = 1;
        while (count < length) {
            if (codepoints_p[count++] == '"') {break;}
        }
        if (count > 1) {NH_WEBIDL_END(count)}
    }

    if (codepoints_p[0] == 39) {
        int count = 1;
        while (count < length) {
            if (codepoints_p[count++] == 39) {break;}
        }
        if (count > 1) {NH_WEBIDL_END(count)}
    }

NH_WEBIDL_END(0)
}

static unsigned int nh_webidl_isIdentifierStart(
    NH_ENCODING_UTF32 codepoint)
{
NH_WEBIDL_BEGIN()

    if (codepoint == '$' || codepoint == '_') {NH_WEBIDL_END(true)}
    if (nh_encoding_inIDSTART(codepoint)) {NH_WEBIDL_END(true)}
    if (codepoint == 92) { // 92 == \
        // unicode esc seq
    }

NH_WEBIDL_END(false)
} 

static unsigned int nh_webidl_isIdentifierPart(
    NH_ENCODING_UTF32 codepoint)
{
NH_WEBIDL_BEGIN()

    if (codepoint == '$') {NH_WEBIDL_END(true)}
    if (nh_encoding_inIDCONTINUE(codepoint)) {NH_WEBIDL_END(true)}
    if (codepoint == 92) {
        // unicode esc seq
    }

NH_WEBIDL_END(false)
} 

// https://tc39.es/ecma262/#sec-names-and-keywords
static unsigned int nh_webidl_isIdentifierName(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    int index = 0;
    while (nh_webidl_isIdentifierStart(codepoints_p[index]) && index < length) {
        index++;
    }
    
    if (index > 0) {
        while ((nh_webidl_isIdentifierStart(codepoints_p[index]) || nh_webidl_isIdentifierPart(codepoints_p[index])) && index < length) {
            index++;
        }
    }

NH_WEBIDL_END(index)
}

static unsigned int nh_webidl_getToken(
    nh_Array *Tokens_p, NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_TOKEN type;
    unsigned int count = 0;

    count = nh_webidl_isWhiteSpace(codepoints_p[0]);
    if (count) {
        type = NH_WEBIDL_TOKEN_WHITE_SPACE;
        goto DEFINE_TOKEN;
    } 

    count = nh_webidl_isLineTerminator(codepoints_p, length);
    if (count) {
        type = NH_WEBIDL_TOKEN_LINE_TERMINATOR;
        goto DEFINE_TOKEN;
    } 

    count = nh_webidl_isComment(codepoints_p, length);
    if (count) {
        type = NH_WEBIDL_TOKEN_COMMENT;
        goto DEFINE_TOKEN;
    } 

    count = nh_webidl_isPunctuator(codepoints_p, length);
    if (count) {
        type = NH_WEBIDL_TOKEN_PUNCTUATOR;
        goto DEFINE_TOKEN;
    } 

    count = nh_webidl_isNumericLiteral(codepoints_p, length, &type);
    if (count) {
        goto DEFINE_TOKEN;
    } 

    count = nh_webidl_isStringLiteral(codepoints_p, length);
    if (count) {
        type = NH_WEBIDL_TOKEN_STRING;
        goto DEFINE_TOKEN;
    } 

    count = nh_webidl_isIdentifierName(codepoints_p, length);
    if (count) {
        type = NH_WEBIDL_TOKEN_IDENTIFIER;
        goto DEFINE_TOKEN;
    } 

DEFINE_TOKEN: ;

    nh_webidl_Token *Token_p = nh_core_incrementArray(Tokens_p);
    Token_p->type = type;
    Token_p->String = nh_encoding_encodeUTF8(codepoints_p, count);

NH_WEBIDL_END(count)
}

static nh_Array nh_webidl_getTokens(
    nh_encoding_UTF32String Codepoints)
{
NH_WEBIDL_BEGIN()

    nh_Array Tokens = nh_core_initArray(sizeof(nh_webidl_Token), 64);

    int index = 0;
    while (index < Codepoints.length) {
        index += nh_webidl_getToken(&Tokens, &Codepoints.p[index], Codepoints.length - index);
    }

NH_WEBIDL_END(Tokens)
}

// DISCARD REDUNDANT INPUT ELEMENTS ================================================================

static nh_Array nh_webidl_discardRedundantTokens(
    nh_Array DirtyTokens)
{
NH_WEBIDL_BEGIN()

    nh_Array CleanTokens = nh_core_initArray(sizeof(nh_webidl_Token), 64);

    for (int i = 0; i < DirtyTokens.length; ++i) 
    {
        nh_webidl_Token *Token_p = &((nh_webidl_Token*)DirtyTokens.p)[i];
        if (Token_p->type == NH_WEBIDL_TOKEN_WHITE_SPACE || Token_p->type == NH_WEBIDL_TOKEN_COMMENT || Token_p->type == NH_WEBIDL_TOKEN_LINE_TERMINATOR) {
            continue;
        }

        nh_webidl_Token *TokenCopy_p = nh_core_incrementArray(&CleanTokens);
        TokenCopy_p->type   = Token_p->type;
        TokenCopy_p->String = nh_core_initString(32);

        nh_core_appendToString(
            &TokenCopy_p->String, Token_p->String.p, Token_p->String.length
        );
    }

NH_WEBIDL_END(CleanTokens)
}

// TOKENIZE ========================================================================================

nh_Array nh_webidl_tokenizeFragment(
    char *logName_p, char *fragment_p)
{
NH_WEBIDL_BEGIN()

    nh_encoding_UTF32String Codepoints = nh_encoding_decodeUTF8(fragment_p, strlen(fragment_p), NULL);

    nh_Array DirtyTokens = nh_webidl_getTokens(Codepoints);
    nh_webidl_logTokens(logName_p, &DirtyTokens, true);

    nh_Array CleanTokens = nh_webidl_discardRedundantTokens(DirtyTokens);
    nh_webidl_logTokens(logName_p, &CleanTokens, false);

    nh_webidl_freeTokens(DirtyTokens);
    nh_encoding_freeUTF32(&Codepoints);

NH_WEBIDL_END(CleanTokens)
}

void nh_webidl_freeTokens(
    nh_Array Tokens)
{
NH_WEBIDL_BEGIN()

    for (int i = 0; i < Tokens.length; ++i)
    {
        nh_webidl_Token *Token_p = &((nh_webidl_Token*)Tokens.p)[i];
        nh_core_freeString(&Token_p->String);
    }

    nh_core_freeArray(&Tokens);

NH_WEBIDL_SILENT_END()
}

// IS NUMERIC LITERAL ==============================================================================

bool nh_webidl_isNumericToken(
    nh_webidl_Token *Token_p)
{
NH_WEBIDL_BEGIN()

    if (Token_p->type >= NH_WEBIDL_TOKEN_DECIMAL_LITERAL && Token_p->type <= NH_WEBIDL_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL) {
        NH_WEBIDL_END(true)
    }

NH_WEBIDL_END(false)
}

