#ifndef NH_ECMASCRIPT_LEXER_H
#define NH_ECMASCRIPT_LEXER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Type.h"

#endif

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
    } nh_ecmascript_InputElement;

/** @} */

/** @addtogroup lib_nh-ecmascript_vars
 *  @{
 */

    extern const char *NH_ECMASCRIPT_INPUT_ELEMENTS_PP[];

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    nh_core_Array nh_ecmascript_getInputElements(
        nh_encoding_UTF32String Codepoints
    );

    nh_core_Array nh_ecmascript_discardRedundantInputElements(
        nh_core_Array DirtyInputElements
    );

    bool nh_ecmascript_isNumericToken(
        nh_ecmascript_InputElement *InputElement_p
    );

/** @} */

#endif
