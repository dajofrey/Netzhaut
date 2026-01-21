#ifndef NH_WEBIDL_TOKENIZER_H
#define NH_WEBIDL_TOKENIZER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

/** @addtogroup lib_nh-webidl_enums
 *  @{
 */

    typedef enum NH_WEBIDL_TOKEN {
        NH_WEBIDL_TOKEN_WHITE_SPACE = 0,
        NH_WEBIDL_TOKEN_LINE_TERMINATOR,
        NH_WEBIDL_TOKEN_COMMENT,
        NH_WEBIDL_TOKEN_IDENTIFIER,
        NH_WEBIDL_TOKEN_PUNCTUATOR,
        NH_WEBIDL_TOKEN_DECIMAL_LITERAL,
        NH_WEBIDL_TOKEN_DECIMAL_BIG_INTEGER_LITERAL,
        NH_WEBIDL_TOKEN_NON_DECIMAL_BINARY_INTEGER_LITERAL,
        NH_WEBIDL_TOKEN_NON_DECIMAL_OCTAL_INTEGER_LITERAL,
        NH_WEBIDL_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL,
        NH_WEBIDL_TOKEN_STRING,
    } NH_WEBIDL_TOKEN;

/** @} */

/** @addtogroup lib_nh-webidl_structs
 *  @{
 */

    typedef struct nh_webidl_Token {
        NH_WEBIDL_TOKEN type;
        nh_encoding_UTF8String String;
    } nh_webidl_Token;

/** @} */

/** @addtogroup lib_nh-webidl_vars
 *  @{
 */

    extern const char *NH_WEBIDL_TOKENS_PP[];

/** @} */

/** @addtogroup lib_nh-webidl_functions
 *  @{
 */

    nh_core_Array nh_webidl_tokenizeFragment(
        char *logName_p, char *fragment_p
    );
    
    void nh_webidl_freeTokens(
        nh_core_Array Tokens
    );

    bool nh_webidl_isNumericToken(
        nh_webidl_Token *Token_p
    );

/** @} */

#endif
