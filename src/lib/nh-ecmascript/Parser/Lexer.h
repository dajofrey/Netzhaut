#ifndef NH_ECMASCRIPT_PARSER_LEXER_H
#define NH_ECMASCRIPT_PARSER_LEXER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Engine/Type.h"

/** @addtogroup lib_nh-ecmascript_enums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_INPUT_ELEMENT {
        NH_ECMASCRIPT_INPUT_ELEMENT_WHITE_SPACE = 0,
        NH_ECMASCRIPT_INPUT_ELEMENT_LINE_TERMINATOR,
        NH_ECMASCRIPT_INPUT_ELEMENT_COMMENT,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_PUNCTUATOR,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_BINARY_INTEGER_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_OCTAL_INTEGER_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_STRING_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_TEMPLATE,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DIV_PUNCTUATOR,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_REGULAR_EXPRESSION_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_RIGHT_BRACE_PUNCTUATOR,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_TEMPLATE_SUBSTITUTION_TAIL,
    } NH_ECMASCRIPT_INPUT_ELEMENT;

/** @} */

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_InputElement {
        NH_ECMASCRIPT_INPUT_ELEMENT type;
        nh_encoding_UTF8String String;
        bool hasLineTerminatorBefore;
    } nh_ecmascript_InputElement;

/** @} */

/** @addtogroup lib_nh-ecmascript_vars
 *  @{
 */

    extern const char *NH_ECMASCRIPT_INPUT_ELEMENTS_PP[];

/** @} */

NH_API_RESULT nh_ecmascript_runLexer(
    void *Script_p, nh_encoding_UTF32String *UnicodeCodepoints_p,
    nh_core_Array *DirtyInputElements_p, nh_core_Array *CleanInputElements_p
);


bool nh_ecmascript_isNumericToken(
    nh_ecmascript_InputElement *InputElement_p
);

#endif
