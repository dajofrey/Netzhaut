#ifndef NH_ECMASCRIPT_INTRINSICS_TOKENIZER_H
#define NH_ECMASCRIPT_INTRINSICS_TOKENIZER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nh-encoding/Encodings/UTF8.h"

typedef enum NH_ECMASCRIPT_TOKEN {
    NH_ECMASCRIPT_TOKEN_WHITE_SPACE = 0,
    NH_ECMASCRIPT_TOKEN_LINE_TERMINATOR,
    NH_ECMASCRIPT_TOKEN_COMMENT,
    NH_ECMASCRIPT_TOKEN_IDENTIFIER,
    NH_ECMASCRIPT_TOKEN_PUNCTUATOR,
    NH_ECMASCRIPT_TOKEN_DECIMAL_LITERAL,
    NH_ECMASCRIPT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL,
    NH_ECMASCRIPT_TOKEN_NON_DECIMAL_BINARY_INTEGER_LITERAL,
    NH_ECMASCRIPT_TOKEN_NON_DECIMAL_OCTAL_INTEGER_LITERAL,
    NH_ECMASCRIPT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL,
    NH_ECMASCRIPT_TOKEN_STRING,
} NH_ECMASCRIPT_TOKEN;

typedef struct nh_ecmascript_Token {
    NH_ECMASCRIPT_TOKEN type;
    nh_encoding_UTF8String String;
} nh_ecmascript_Token;

/** @addtogroup lib_nh-webidl_vars
 *  @{
 */

    extern const char *NH_ECMASCRIPT_TOKENS_PP[];

/** @} */

/** @addtogroup lib_nh-webidl_functions
 *  @{
 */

    nh_core_Array nh_ecmascript_tokenizeIntrinsicTemplate(
        char *logName_p, char *fragment_p
    );
    
    void nh_ecmascript_freeIntrinsicTemplateTokens(
        nh_core_Array Tokens
    );

/** @} */

#endif
