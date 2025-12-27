// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Tokenizer.h"

#include "../Common/Log.h"

#include "../../nh-encoding/Base/UnicodeDataHelper.h"
#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

const char *NH_ECMASCRIPT_TOKENS_PP[] = {
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

static bool nh_ecmascript_isDigit(
    NH_ENCODING_UTF32 codepoint, bool zero)
{
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
        return true;
    }

    if (zero && codepoint == '0') {return true;}

    return false;
}

static unsigned int nh_ecmascript_isDigits(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    if (nh_ecmascript_isDigit(codepoints_p[0], true)) {
        unsigned int i = 1;
        while (i < length && (nh_ecmascript_isDigit(codepoints_p[i], true) || codepoints_p[i] == '_')) {
            i++;
        }
        return i;
    }

    return 0;
}

static unsigned int nh_ecmascript_isSignedInteger(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    int offset = 0;
    if (codepoints_p[0] == '+' || codepoints_p[0] == '-') {
        offset++;
    }
    int digits = nh_ecmascript_isDigits(offset == 0 ? codepoints_p : &codepoints_p[1], true);
    if (digits == 0) {return 0;}

    return offset + digits;
}

static unsigned int nh_ecmascript_isExponentPart(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    if (codepoints_p[0] != 'e' && codepoints_p[0] != 'E') {
        return 0;
    }
    int digits = nh_ecmascript_isSignedInteger(&codepoints_p[1], true);

    return digits;
}

static unsigned int nh_ecmascript_isDecimalIntegerLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    if (codepoints_p[0] == '0') {return 1;}

    if (nh_ecmascript_isDigit(codepoints_p[0], false)) 
    {
        int offset = 1;
        if (length > 1 && codepoints_p[1] == '_') {
            offset++;
        }
        int digits = nh_ecmascript_isDigits(&codepoints_p[offset], length - offset);
        if (digits == 0 && offset == 2) {return 0;}
        return digits + offset;
    }

    return 0;
}

static unsigned int nh_ecmascript_isWhiteSpace(
    NH_ENCODING_UTF32 codepoint)
{
    if (codepoint == 0x0009  // CHARACTER TABULATION <TAB> 
    ||  codepoint == 0x000B  // LINE TABULATION <VT> 
    ||  codepoint == 0x000C  // FORM FEED (FF) 
    ||  codepoint == 0x0020) // SPACE <SP> 
    {
        return 1;
    }

    return 0;
}

static unsigned int nh_ecmascript_isLineTerminator(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    if (codepoints_p[0] == 0x0A  // LINE FEED <LF> 
    ||  codepoints_p[0] == 0x0D) // CARRIAGE RETURN (CR) <CR> 
    {
        if (length > 1 && codepoints_p[0] == 0x0D && codepoints_p[1] == 0x0A) {
            return 2;
        }
        return 1;
    }

    return 0;
}

static unsigned int nh_ecmascript_isComment(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    if (codepoints_p[0] == 0x2F) // '/'
    {
        // single line comment
        if (length > 1 && codepoints_p[1] == 0x2F) 
        {
            unsigned int i = 2;
            while (i < length && !nh_ecmascript_isLineTerminator(&codepoints_p[i], length - i)) {
                i++;
            }
            return i;
        }

        // multi line comment 
        if (length > 1 && codepoints_p[1] == 0x2A) 
        {
            unsigned int i = 2;
            while (i < length - 1 && codepoints_p[i] != 0x2A && codepoints_p[i + 1] != 0x2F) {
                i++;
            }
            return i + 2;
        }
    }

    return 0;
}

static unsigned int nh_ecmascript_isPunctuator(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    if (length > 3) {
        if (codepoints_p[0] == '>' && codepoints_p[1] == '>' && codepoints_p[2] == '>' && codepoints_p[3] == '=')
        {
            return 4;
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
            return 3;
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
            return 2;
        }
        if (codepoints_p[0] == '?' && codepoints_p[1] == '.')
        { 
            if (length > 2 && nh_ecmascript_isDigit(codepoints_p[2], true)) {}
            else {return 2;}
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
        return 1;
    }

    return 0;
}

static bool nh_ecmascript_isHexDigit(
    NH_ENCODING_UTF32 codepoint)
{
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
    ||  codepoint == 'F') {return true;}

    return false;
}

static unsigned int nh_ecmascript_isHexDigits(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    int digits = 0;

    while (length >= 1 && nh_ecmascript_isHexDigit(codepoints_p[digits])) {
        digits++;
        length--;
    }

    return digits;
}

static unsigned int nh_ecmascript_isHexIntegerLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    int digits = 0;

    if (length > 2 && codepoints_p[0] == '0' && (codepoints_p[1] == 'x' || codepoints_p[1] == 'X')) {
        digits = nh_ecmascript_isHexDigits(&codepoints_p[2], length - 2);
    }
    if (digits > 0) {digits += 2;}

    return digits;
}

// https://tc39.es/ecma262/#sec-literals-numeric-literals
static unsigned int nh_ecmascript_isNumericLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length, NH_ECMASCRIPT_TOKEN *type_p)
{
// NonDecimalHexIntegerLiteral

    int digits = nh_ecmascript_isHexIntegerLiteral(codepoints_p, length);
    if (digits > 0) {
        *type_p = NH_ECMASCRIPT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL;
        return digits;
    }

// DecimalBigIntegerLiteral
    if (length > 1 && codepoints_p[0] == '0' && codepoints_p[1] == 'n') 
    {
        *type_p = NH_ECMASCRIPT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
        return 2;
    }

    if (nh_ecmascript_isDigit(codepoints_p[0], false) && length > 1) 
    {
        if (codepoints_p[1] == '_') {
            digits = nh_ecmascript_isDigits(&codepoints_p[2], length - 2);
            if (digits > 0 && length > digits + 2 && codepoints_p[digits + 2] == 'n') {
                *type_p = NH_ECMASCRIPT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
                return digits + 3;
            } 
        }

        digits = nh_ecmascript_isDigits(&codepoints_p[1], length - 1);
        if (length > digits + 1 && codepoints_p[digits + 1] == 'n') {
            *type_p = NH_ECMASCRIPT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
            return digits + 2;
        } 
    }

// DecimalLiteral
    if (codepoints_p[0] == '.')
    {
        digits = nh_ecmascript_isDigits(&codepoints_p[1], length - 1) + 1;
        if (digits > 1) {
            digits += nh_ecmascript_isExponentPart(&codepoints_p[digits], length - digits);
            *type_p = NH_ECMASCRIPT_TOKEN_DECIMAL_LITERAL;
            return digits;
        }
    }

    digits = nh_ecmascript_isDecimalIntegerLiteral(codepoints_p, length);
    if (digits > 0) 
    {
        *type_p = NH_ECMASCRIPT_TOKEN_DECIMAL_LITERAL;

        if (length > digits && codepoints_p[digits] == '.') {
            digits++;
            digits += nh_ecmascript_isDigits(&codepoints_p[digits], length - digits);
            digits += nh_ecmascript_isExponentPart(&codepoints_p[digits], length - digits);
            return digits;
        }

        digits += nh_ecmascript_isExponentPart(&codepoints_p[digits], length - digits);
        return digits;
    }

    return 0;
}

static unsigned int nh_ecmascript_isStringLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    if (codepoints_p[0] == '"') {
        int count = 1;
        while (count < length) {
            if (codepoints_p[count++] == '"') {break;}
        }
        if (count > 1) {return count;}
    }

    if (codepoints_p[0] == 39) {
        int count = 1;
        while (count < length) {
            if (codepoints_p[count++] == 39) {break;}
        }
        if (count > 1) {return count;}
    }

    return 0;
}

static unsigned int nh_ecmascript_isIdentifierStart(
    NH_ENCODING_UTF32 codepoint)
{
    if (codepoint == '$' || codepoint == '_') {return true;}
    if (nh_encoding_inIDSTART(codepoint)) {return true;}
    if (codepoint == 92) { // 92 == \
        // unicode esc seq
    }

    return false;
}

static unsigned int nh_ecmascript_isIdentifierPart(
    NH_ENCODING_UTF32 codepoint)
{
    if (codepoint == '$') {return true;}
    if (nh_encoding_inIDCONTINUE(codepoint)) {return true;}
    if (codepoint == 92) {
        // unicode esc seq
    }

    return false;
} 

// https://tc39.es/ecma262/#sec-names-and-keywords
static unsigned int nh_ecmascript_isIdentifierName(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    int index = 0;
    while (nh_ecmascript_isIdentifierStart(codepoints_p[index]) && index < length) {
        index++;
    }
    
    if (index > 0) {
        while ((nh_ecmascript_isIdentifierStart(codepoints_p[index]) || nh_ecmascript_isIdentifierPart(codepoints_p[index])) && index < length) {
            index++;
        }
    }

    return index;
}

static unsigned int nh_ecmascript_getToken(
    nh_core_Array *Tokens_p, NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    NH_ECMASCRIPT_TOKEN type;
    unsigned int count = 0;

    count = nh_ecmascript_isWhiteSpace(codepoints_p[0]);
    if (count) {
        type = NH_ECMASCRIPT_TOKEN_WHITE_SPACE;
        goto DEFINE_TOKEN;
    } 

    count = nh_ecmascript_isLineTerminator(codepoints_p, length);
    if (count) {
        type = NH_ECMASCRIPT_TOKEN_LINE_TERMINATOR;
        goto DEFINE_TOKEN;
    } 

    count = nh_ecmascript_isComment(codepoints_p, length);
    if (count) {
        type = NH_ECMASCRIPT_TOKEN_COMMENT;
        goto DEFINE_TOKEN;
    } 

    count = nh_ecmascript_isPunctuator(codepoints_p, length);
    if (count) {
        type = NH_ECMASCRIPT_TOKEN_PUNCTUATOR;
        goto DEFINE_TOKEN;
    } 

    count = nh_ecmascript_isNumericLiteral(codepoints_p, length, &type);
    if (count) {
        goto DEFINE_TOKEN;
    } 

    count = nh_ecmascript_isStringLiteral(codepoints_p, length);
    if (count) {
        type = NH_ECMASCRIPT_TOKEN_STRING;
        goto DEFINE_TOKEN;
    } 

    count = nh_ecmascript_isIdentifierName(codepoints_p, length);
    if (count) {
        type = NH_ECMASCRIPT_TOKEN_IDENTIFIER;
        goto DEFINE_TOKEN;
    } 

DEFINE_TOKEN: ;

    nh_ecmascript_Token *Token_p = (nh_ecmascript_Token*)nh_core_incrementArray(Tokens_p);
    Token_p->type = type;
    Token_p->String = nh_encoding_encodeUTF8(codepoints_p, count);

    return count;
}

static nh_core_Array nh_ecmascript_getTokens(
    nh_encoding_UTF32String Codepoints)
{
    nh_core_Array Tokens = nh_core_initArray(sizeof(nh_ecmascript_Token), 64);

    int index = 0;
    while (index < Codepoints.length) {
        index += nh_ecmascript_getToken(&Tokens, &Codepoints.p[index], Codepoints.length - index);
    }

    return Tokens;
}

// DISCARD REDUNDANT INPUT ELEMENTS ================================================================

static nh_core_Array nh_ecmascript_discardRedundantTokens(
    nh_core_Array DirtyTokens)
{
    nh_core_Array CleanTokens = nh_core_initArray(sizeof(nh_ecmascript_Token), 64);

    for (int i = 0; i < DirtyTokens.length; ++i) 
    {
        nh_ecmascript_Token *Token_p = &((nh_ecmascript_Token*)DirtyTokens.p)[i];
        if (Token_p->type == NH_ECMASCRIPT_TOKEN_WHITE_SPACE || Token_p->type == NH_ECMASCRIPT_TOKEN_COMMENT || Token_p->type == NH_ECMASCRIPT_TOKEN_LINE_TERMINATOR) {
            continue;
        }

        nh_ecmascript_Token *TokenCopy_p = (nh_ecmascript_Token*)nh_core_incrementArray(&CleanTokens);
        TokenCopy_p->type   = Token_p->type;
        TokenCopy_p->String = nh_core_initString(32);

        nh_core_appendToString(
            &TokenCopy_p->String, Token_p->String.p, Token_p->String.length
        );
    }

    return CleanTokens;
}

// TOKENIZE ========================================================================================

nh_core_Array nh_ecmascript_tokenizeIntrinsic(
    char *logName_p, char *fragment_p)
{
    nh_encoding_UTF32String Codepoints = nh_encoding_decodeUTF8(fragment_p, strlen(fragment_p), NULL);

    nh_core_Array DirtyTokens = nh_ecmascript_getTokens(Codepoints);
//    nh_ecmascript_logTokens(logName_p, &DirtyTokens, true);

    nh_core_Array CleanTokens = nh_ecmascript_discardRedundantTokens(DirtyTokens);
//    nh_ecmascript_logTokens(logName_p, &CleanTokens, false);

//    nh_ecmascript_freeTokens(DirtyTokens);
    nh_encoding_freeUTF32(&Codepoints);

    return CleanTokens;
}

void nh_ecmascript_freeIntrinsicTokens(
    nh_core_Array Tokens)
{
    for (int i = 0; i < Tokens.length; ++i)
    {
        nh_ecmascript_Token *Token_p = &((nh_ecmascript_Token*)Tokens.p)[i];
        nh_core_freeString(&Token_p->String);
    }

    nh_core_freeArray(&Tokens);

    return;
}
