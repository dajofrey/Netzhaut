// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Lexer.h"

#include "../Common/Macros.h"

#include "../../nh-encoding/Base/UnicodeDataHelper.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include "../../nh-core/Util/String.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DECLARE =========================================================================================

typedef enum NH_ECMASCRIPT_GOAL_SYMBOL {
    NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_DIV,
    NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_REG_EXP,
    NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_REG_EXP_OR_TEMPLATE_TAIL,
    NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_TEMPLATE_TAIL,
} NH_ECMASCRIPT_GOAL_SYMBOL;

// DATA ============================================================================================

const NH_BYTE *NH_ECMASCRIPT_INPUT_ELEMENTS_PP[] = {
    "Whitespace",
    "LineTerminator",
    "Comment",
    "TokenIdentifierName",
    "TokenPunctuator",
    "TokenDecimalLiteral",
    "TokenDecimalBigIntegerLiteral",
    "TokenNonDecimalBinaryIntegerLiteral",
    "TokenNonDecimalOctalIntegerLiteral",
    "TokenNonDecimalHexIntegerLiteral",
    "TokenStringLiteral",
    "TokenTemplate",
    "TokenDivPunctuator",
    "TokenRegularExpressionLiteral",
    "TokenRightBracePunctuator",
    "TokenTemplateSubstitutionTail",
};

// LEXER ===========================================================================================

static NH_BOOL nh_ecmascript_isDigit(
    NH_ENCODING_UTF32 codepoint, NH_BOOL zero)
{
NH_ECMASCRIPT_BEGIN()

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
        NH_ECMASCRIPT_END(NH_TRUE)
    }

    if (zero && codepoint == '0') {NH_ECMASCRIPT_END(NH_TRUE)}

NH_ECMASCRIPT_END(NH_FALSE)
}

static unsigned int nh_ecmascript_isDigits(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (nh_ecmascript_isDigit(codepoints_p[0], NH_TRUE)) {
        unsigned int i = 1;
        while (i < length && (nh_ecmascript_isDigit(codepoints_p[i], NH_TRUE) || codepoints_p[i] == '_')) {
            i++;
        }
        NH_ECMASCRIPT_END(i)
    }

NH_ECMASCRIPT_END(0)
}

static unsigned int nh_ecmascript_isSignedInteger(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    int offset = 0;
    if (codepoints_p[0] == '+' || codepoints_p[0] == '-') {
        offset++;
    }
    int digits = nh_ecmascript_isDigits(offset == 0 ? codepoints_p : &codepoints_p[1], NH_TRUE);
    if (digits == 0) {NH_ECMASCRIPT_END(0)}

NH_ECMASCRIPT_END(offset + digits)
}

static unsigned int nh_ecmascript_isExponentPart(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoints_p[0] != 'e' && codepoints_p[0] != 'E') {
        NH_ECMASCRIPT_END(0)
    }
    int digits = nh_ecmascript_isSignedInteger(&codepoints_p[1], NH_TRUE);

NH_ECMASCRIPT_END(digits)
}

static unsigned int nh_ecmascript_isDecimalIntegerLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoints_p[0] == '0') {NH_ECMASCRIPT_END(1)}

    if (nh_ecmascript_isDigit(codepoints_p[0], NH_FALSE)) 
    {
        int offset = 1;
        if (length > 1 && codepoints_p[1] == '_') {
            offset++;
        }
        int digits = nh_ecmascript_isDigits(&codepoints_p[offset], length - offset);
        if (digits == 0 && offset == 2) {NH_ECMASCRIPT_END(0)}
        NH_ECMASCRIPT_END(digits + offset)
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-white-space
static unsigned int nh_ecmascript_isWhiteSpace(
    NH_ENCODING_UTF32 codepoint)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoint == 0x0009  // CHARACTER TABULATION <TAB> 
    ||  codepoint == 0x000B  // LINE TABULATION <VT> 
    ||  codepoint == 0x000C  // FORM FEED (FF) 
    ||  codepoint == 0x0020  // SPACE <SP> 
    ||  codepoint == 0x00A0  // NO-BREAK SPACE <NBSP>
    ||  codepoint == 0xFEFF) // ZERO WIDTH NO-BREAK SPACE <ZWNBSP>
    {
        NH_ECMASCRIPT_END(1)
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-line-terminators
static unsigned int nh_ecmascript_isLineTerminator(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()


    if (codepoints_p[0] == 0x000A  // LINE FEED <LF> 
    ||  codepoints_p[0] == 0x000D  // CARRIAGE RETURN (CR) <CR> 
    ||  codepoints_p[0] == 0x2028  // LINE SEPERATOR (LS) <LS>
    ||  codepoints_p[0] == 0x2029) // PARAGRAPH SEPERATOR <PS> 
    {
        if (length > 1 && codepoints_p[0] == 0x000D && codepoints_p[1] == 0x000A) {
            NH_ECMASCRIPT_END(2)
        }
        NH_ECMASCRIPT_END(1)
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-comments
static unsigned int nh_ecmascript_isComment(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoints_p[0] == 0x002F) // '/'
    {
        // single line comment
        if (length > 1 && codepoints_p[1] == 0x002F) 
        {
            unsigned int i = 2;
            while (i < length && !nh_ecmascript_isLineTerminator(&codepoints_p[i], length - i)) {
                i++;
            }
            NH_ECMASCRIPT_END(i)
        }

        // multi line comment 
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-punctuators
static unsigned int nh_ecmascript_isPunctuator(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (length > 3) {
        if (codepoints_p[0] == '>' && codepoints_p[1] == '>' && codepoints_p[2] == '>' && codepoints_p[3] == '=')
        {
            NH_ECMASCRIPT_END(4)
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
            NH_ECMASCRIPT_END(3)
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
            NH_ECMASCRIPT_END(2)
        }
        if (codepoints_p[0] == '?' && codepoints_p[1] == '.')
        { 
            if (length > 2 && nh_ecmascript_isDigit(codepoints_p[2], NH_TRUE)) {}
            else {NH_ECMASCRIPT_END(2)}
        }
    }

    if (codepoints_p[0] == '{'
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
    ||  codepoints_p[0] == '=') 
    {
        NH_ECMASCRIPT_END(1)
    }

NH_ECMASCRIPT_END(0)
}

static NH_BOOL nh_ecmascript_isHexDigit(
    NH_ENCODING_UTF32 codepoint)
{
NH_ECMASCRIPT_BEGIN()

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
    ||  codepoint == 'F') {NH_ECMASCRIPT_END(NH_TRUE)}

NH_ECMASCRIPT_END(NH_FALSE)
}

static unsigned int nh_ecmascript_isHexDigits(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    int digits = 0;

    while (length >= 1 && nh_ecmascript_isHexDigit(codepoints_p[digits])) {
        digits++;
        length--;
    }

NH_ECMASCRIPT_END(digits)
}

static unsigned int nh_ecmascript_isHexIntegerLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    int digits = 0;

    if (length > 2 && codepoints_p[0] == '0' && (codepoints_p[1] == 'x' || codepoints_p[1] == 'X')) {
        digits = nh_ecmascript_isHexDigits(&codepoints_p[2], length - 2);
    }
    if (digits > 0) {digits += 2;}

NH_ECMASCRIPT_END(digits)
}

// https://tc39.es/ecma262/#sec-literals-numeric-literals
static unsigned int nh_ecmascript_isNumericLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length, NH_ECMASCRIPT_INPUT_ELEMENT *type_p)
{
NH_ECMASCRIPT_BEGIN()

// NonDecimalHexIntegerLiteral

    int digits = nh_ecmascript_isHexIntegerLiteral(codepoints_p, length);
    if (digits > 0) {
        *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL;
        NH_ECMASCRIPT_END(digits)
    }

// DecimalBigIntegerLiteral
    if (length > 1 && codepoints_p[0] == '0' && codepoints_p[1] == 'n') 
    {
        *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
        NH_ECMASCRIPT_END(2)
    }

    if (nh_ecmascript_isDigit(codepoints_p[0], NH_FALSE) && length > 1) 
    {
        if (codepoints_p[1] == '_') {
            digits = nh_ecmascript_isDigits(&codepoints_p[2], length - 2);
            if (digits > 0 && length > digits + 2 && codepoints_p[digits + 2] == 'n') {
                *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
                NH_ECMASCRIPT_END(digits + 3)
            } 
        }

        digits = nh_ecmascript_isDigits(&codepoints_p[1], length - 1);
        if (length > digits + 1 && codepoints_p[digits + 1] == 'n') {
            *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
            NH_ECMASCRIPT_END(digits + 2)
        } 
    }

// DecimalLiteral
    if (codepoints_p[0] == '.')
    {
        digits = nh_ecmascript_isDigits(&codepoints_p[1], length - 1) + 1;
        if (digits > 1) {
            digits += nh_ecmascript_isExponentPart(&codepoints_p[digits], length - digits);
            *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL;
            NH_ECMASCRIPT_END(digits)
        }
    }

    digits = nh_ecmascript_isDecimalIntegerLiteral(codepoints_p, length);
    if (digits > 0) 
    {
        *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL;

        if (length > digits && codepoints_p[digits] == '.') {
            digits++;
            digits += nh_ecmascript_isDigits(&codepoints_p[digits], length - digits);
            digits += nh_ecmascript_isExponentPart(&codepoints_p[digits], length - digits);
            NH_ECMASCRIPT_END(digits)
        }

        digits += nh_ecmascript_isExponentPart(&codepoints_p[digits], length - digits);
        NH_ECMASCRIPT_END(digits)
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-literals-string-literals
static unsigned int nh_ecmascript_isStringLiteral(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoints_p[0] == '"') {
        int count = 1;
        while (count < length) {
            if (codepoints_p[count++] == '"') {break;}
        }
        if (count > 1) {NH_ECMASCRIPT_END(count)}
    }

    if (codepoints_p[0] == 39) {
        int count = 1;
        while (count < length) {
            if (codepoints_p[count++] == 39) {break;}
        }
        if (count > 1) {NH_ECMASCRIPT_END(count)}
    }

NH_ECMASCRIPT_END(0)
}

static unsigned int nh_ecmascript_isIdentifierStart(
    NH_ENCODING_UTF32 codepoint)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoint == '$' || codepoint == '_') {NH_ECMASCRIPT_END(NH_TRUE)}
    if (nh_encoding_inIDSTART(codepoint)) {NH_ECMASCRIPT_END(NH_TRUE)}
    if (codepoint == 92) { // 92 == \
        // unicode esc seq
    }

NH_ECMASCRIPT_END(NH_FALSE)
} 

static unsigned int nh_ecmascript_isIdentifierPart(
    NH_ENCODING_UTF32 codepoint)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoint == '$') {NH_ECMASCRIPT_END(NH_TRUE)}
    if (nh_encoding_inIDCONTINUE(codepoint)) {NH_ECMASCRIPT_END(NH_TRUE)}
    if (codepoint == 92) {
        // unicode esc seq
    }

NH_ECMASCRIPT_END(NH_FALSE)
} 

// https://tc39.es/ecma262/#sec-names-and-keywords
static unsigned int nh_ecmascript_isIdentifierName(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    int index = 0;
    while (nh_ecmascript_isIdentifierStart(codepoints_p[index]) && index < length) {
        index++;
    }
    
    if (index > 0) {
        while ((nh_ecmascript_isIdentifierStart(codepoints_p[index]) || nh_ecmascript_isIdentifierPart(codepoints_p[index])) && index < length) {
            index++;
        }
    }

NH_ECMASCRIPT_END(index)
}

static unsigned int nh_ecmascript_isRightBracePunctuator(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(length > 0 && codepoints_p[0] == '}' ? 1 : 0)
}

static unsigned int nh_ecmascript_getInputElement(
    nh_Array *InputElements_p, NH_ECMASCRIPT_GOAL_SYMBOL goalSymbol, NH_ENCODING_UTF32 *codepoints_p, 
    unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    NH_ECMASCRIPT_INPUT_ELEMENT type;
    unsigned int count = 0;

    if (goalSymbol == NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_DIV) 
    {
        count = nh_ecmascript_isWhiteSpace(codepoints_p[0]);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_WHITE_SPACE;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = nh_ecmascript_isLineTerminator(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_LINE_TERMINATOR;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = nh_ecmascript_isComment(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_COMMENT;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = nh_ecmascript_isPunctuator(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_PUNCTUATOR;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = nh_ecmascript_isNumericLiteral(codepoints_p, length, &type);
        if (count) {
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = nh_ecmascript_isStringLiteral(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_STRING_LITERAL;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = nh_ecmascript_isIdentifierName(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = nh_ecmascript_isRightBracePunctuator(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_RIGHT_BRACE_PUNCTUATOR;
            goto DEFINE_INPUT_ELEMENT;
        } 
    }

DEFINE_INPUT_ELEMENT: ;

    nh_ecmascript_InputElement *InputElement_p = nh_core_incrementArray(InputElements_p);
    InputElement_p->type = type;
    InputElement_p->String = nh_encoding_encodeUTF8(codepoints_p, count);

NH_ECMASCRIPT_END(count)
}

nh_Array nh_ecmascript_getInputElements(
    nh_encoding_UTF32String Codepoints)
{
NH_ECMASCRIPT_BEGIN()

    nh_Array InputElements = nh_core_initArray(sizeof(nh_ecmascript_InputElement), 64);

    int index = 0;
    while (index < Codepoints.length) 
    {
        NH_ECMASCRIPT_GOAL_SYMBOL goalSymbol = NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_DIV;

        index += nh_ecmascript_getInputElement(
            &InputElements, goalSymbol, &Codepoints.p[index], Codepoints.length - index 
        );
    }

NH_ECMASCRIPT_END(InputElements)
}

// DISCARD REDUNDANT INPUT ELEMENTS ================================================================

// https://tc39.es/ecma262/#sec-lexical-and-regexp-grammars
nh_Array nh_ecmascript_discardRedundantInputElements(
    nh_Array DirtyInputElements)
{
NH_ECMASCRIPT_BEGIN()

    nh_Array CleanInputElements = nh_core_initArray(sizeof(nh_ecmascript_InputElement), 64);

    for (int i = 0; i < DirtyInputElements.length; ++i) 
    {
        nh_ecmascript_InputElement *InputElement_p = &((nh_ecmascript_InputElement*)DirtyInputElements.p)[i];
        if (InputElement_p->type == NH_ECMASCRIPT_INPUT_ELEMENT_WHITE_SPACE) {
            continue;
        }

        nh_ecmascript_InputElement *InputElementCopy_p = nh_core_incrementArray(&CleanInputElements);
        InputElementCopy_p->type = InputElement_p->type;
        InputElementCopy_p->String = nh_core_initString(32);

        nh_core_appendToString(
            &InputElementCopy_p->String, InputElement_p->String.p, InputElement_p->String.length
        );
    }

NH_ECMASCRIPT_END(CleanInputElements)
}

// IS NUMERIC LITERAL ==============================================================================

NH_BOOL nh_ecmascript_isNumericToken(
    nh_ecmascript_InputElement *InputElement_p)
{
NH_ECMASCRIPT_BEGIN()

    if (InputElement_p->type >= NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL && InputElement_p->type <= NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL) {
        NH_ECMASCRIPT_END(NH_TRUE)
    }

NH_ECMASCRIPT_END(NH_FALSE)
}


