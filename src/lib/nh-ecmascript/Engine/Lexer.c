// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Lexer.h"

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

const char *NH_ECMASCRIPT_INPUT_ELEMENTS_PP[] = {
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

// https://tc39.es/ecma262/#sec-white-space
static unsigned int nh_ecmascript_isWhiteSpace(
    NH_ENCODING_UTF32 codepoint)
{
    if (codepoint == 0x0009  // CHARACTER TABULATION <TAB> 
    ||  codepoint == 0x000B  // LINE TABULATION <VT> 
    ||  codepoint == 0x000C  // FORM FEED (FF) 
    ||  codepoint == 0x0020  // SPACE <SP> 
    ||  codepoint == 0x00A0  // NO-BREAK SPACE <NBSP>
    ||  codepoint == 0xFEFF) // ZERO WIDTH NO-BREAK SPACE <ZWNBSP>
    {
        return 1;
    }

    return 0;
}

// https://tc39.es/ecma262/#sec-line-terminators
static unsigned int nh_ecmascript_isLineTerminator(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{

    if (codepoints_p[0] == 0x000A  // LINE FEED <LF> 
    ||  codepoints_p[0] == 0x000D  // CARRIAGE RETURN (CR) <CR> 
    ||  codepoints_p[0] == 0x2028  // LINE SEPERATOR (LS) <LS>
    ||  codepoints_p[0] == 0x2029) // PARAGRAPH SEPERATOR <PS> 
    {
        if (length > 1 && codepoints_p[0] == 0x000D && codepoints_p[1] == 0x000A) {
            return 2;
        }
        return 1;
    }

    return 0;
}

// https://tc39.es/ecma262/#sec-comments
static unsigned int nh_ecmascript_isComment(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    if (codepoints_p[0] == 0x002F) // '/'
    {
        // single line comment
        if (length > 1 && codepoints_p[1] == 0x002F) 
        {
            unsigned int i = 2;
            while (i < length && !nh_ecmascript_isLineTerminator(&codepoints_p[i], length - i)) {
                i++;
            }
            return i;
        }

        // multi line comment 
    }

    return 0;
}

// https://tc39.es/ecma262/#sec-punctuators
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
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length, NH_ECMASCRIPT_INPUT_ELEMENT *type_p)
{
// NonDecimalHexIntegerLiteral

    int digits = nh_ecmascript_isHexIntegerLiteral(codepoints_p, length);
    if (digits > 0) {
        *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL;
        return digits;
    }

// DecimalBigIntegerLiteral
    if (length > 1 && codepoints_p[0] == '0' && codepoints_p[1] == 'n') 
    {
        *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
        return 2;
    }

    if (nh_ecmascript_isDigit(codepoints_p[0], false) && length > 1) 
    {
        if (codepoints_p[1] == '_') {
            digits = nh_ecmascript_isDigits(&codepoints_p[2], length - 2);
            if (digits > 0 && length > digits + 2 && codepoints_p[digits + 2] == 'n') {
                *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
                return digits + 3;
            } 
        }

        digits = nh_ecmascript_isDigits(&codepoints_p[1], length - 1);
        if (length > digits + 1 && codepoints_p[digits + 1] == 'n') {
            *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
            return digits + 2;
        } 
    }

// DecimalLiteral
    if (codepoints_p[0] == '.')
    {
        digits = nh_ecmascript_isDigits(&codepoints_p[1], length - 1) + 1;
        if (digits > 1) {
            digits += nh_ecmascript_isExponentPart(&codepoints_p[digits], length - digits);
            *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL;
            return digits;
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
            return digits;
        }

        digits += nh_ecmascript_isExponentPart(&codepoints_p[digits], length - digits);
        return digits;
    }

    return 0;
}

// https://tc39.es/ecma262/#sec-literals-string-literals
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

static unsigned int nh_ecmascript_isRightBracePunctuator(
    NH_ENCODING_UTF32 *codepoints_p, unsigned int length)
{
    return length > 0 && codepoints_p[0] == '}' ? 1 : 0;
}

static unsigned int nh_ecmascript_getInputElement(
    nh_core_Array *InputElements_p, NH_ECMASCRIPT_GOAL_SYMBOL goalSymbol, NH_ENCODING_UTF32 *codepoints_p, 
    unsigned int length)
{
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

return count;
}

nh_core_Array nh_ecmascript_getInputElements(
    nh_encoding_UTF32String Codepoints)
{
    nh_core_Array InputElements = nh_core_initArray(sizeof(nh_ecmascript_InputElement), 64);

    int index = 0;
    while (index < Codepoints.length) 
    {
        NH_ECMASCRIPT_GOAL_SYMBOL goalSymbol = NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_DIV;

        index += nh_ecmascript_getInputElement(
            &InputElements, goalSymbol, &Codepoints.p[index], Codepoints.length - index 
        );
    }

    return InputElements;
}

// DISCARD REDUNDANT INPUT ELEMENTS ================================================================

// https://tc39.es/ecma262/#sec-lexical-and-regexp-grammars
nh_core_Array nh_ecmascript_discardRedundantInputElements(
    nh_core_Array DirtyInputElements)
{
    nh_core_Array CleanInputElements = nh_core_initArray(sizeof(nh_ecmascript_InputElement), 64);

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

    return CleanInputElements;
}

// IS NUMERIC LITERAL ==============================================================================

bool nh_ecmascript_isNumericToken(
    nh_ecmascript_InputElement *InputElement_p)
{
    if (InputElement_p->type >= NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL && InputElement_p->type <= NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL) {
        return true;
    }

    return false;
}


