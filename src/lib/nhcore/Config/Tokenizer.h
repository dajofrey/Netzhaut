#ifndef NH_CORE_CONFIG_TOKENIZER_H
#define NH_CORE_CONFIG_TOKENIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhcore_enums
 *  @{
 */

    typedef enum NH_CONFIG_TOKEN_E { 
        NH_CONFIG_TOKEN_UNDEFINED, 
        NH_CONFIG_TOKEN_COMMA, 
        NH_CONFIG_TOKEN_IDENTIFIER, 
        NH_CONFIG_TOKEN_STRING, 
        NH_CONFIG_TOKEN_CURLY_BRACKET_RIGHT, 
        NH_CONFIG_TOKEN_CURLY_BRACKET_LEFT, 
        NH_CONFIG_TOKEN_ROUND_BRACKET_RIGHT, 
        NH_CONFIG_TOKEN_ROUND_BRACKET_LEFT, 
        NH_CONFIG_TOKEN_ANGLE_BRACKET_RIGHT, 
        NH_CONFIG_TOKEN_ANGLE_BRACKET_LEFT, 
        NH_CONFIG_TOKEN_HYPHEN_MINUS, 
        NH_CONFIG_TOKEN_COLON,
        NH_CONFIG_TOKEN_SEMICOLON,
        NH_CONFIG_TOKEN_EOF, 
    } NH_CONFIG_TOKEN_E; 

/** @} */

/** @addtogroup lib_nhcore_structs
 *  @{
 */

    typedef struct nh_ConfigToken {
        NH_CONFIG_TOKEN_E type;
        NH_BYTE *string_p;
    } nh_ConfigToken;

    typedef struct nh_ConfigTokenizer {
        unsigned int tokens;
        nh_ConfigToken *Tokens_p;
    } nh_ConfigTokenizer;

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_tokenizeConfig(
        nh_ConfigTokenizer *Tokenizer_p, NH_BYTE *data_p, int length 
    ); 

    void nh_core_freeConfigTokenizer(
        nh_ConfigTokenizer *Tokenizer_p
    );

/** @} */

#endif // NH_CORE_UTIL_TOKENIZER_H
