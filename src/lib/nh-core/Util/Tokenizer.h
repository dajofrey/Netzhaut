#ifndef NH_CORE_UTIL_TOKENIZER_H
#define NH_CORE_UTIL_TOKENIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "File.h"
#include "../Common/Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nh-core_enums
 *  @{
 */

    typedef enum NH_TOKEN { 
        NH_TOKEN_UNDEFINED, 
        NH_TOKEN_COMMA, 
        NH_TOKEN_IDENTIFIER, 
        NH_TOKEN_STRING, 
        NH_TOKEN_CURLY_BRACKET_RIGHT, 
        NH_TOKEN_CURLY_BRACKET_LEFT, 
        NH_TOKEN_ROUND_BRACKET_RIGHT, 
        NH_TOKEN_ROUND_BRACKET_LEFT, 
        NH_TOKEN_ANGLE_BRACKET_RIGHT, 
        NH_TOKEN_ANGLE_BRACKET_LEFT, 
        NH_TOKEN_HYPHEN_MINUS, 
        NH_TOKEN_COLON,
        NH_TOKEN_SEMICOLON,
        NH_TOKEN_EOF, 
    } NH_TOKEN; 

/** @} */

/** @addtogroup lib_nh-core_structs
 *  @{
 */

    typedef struct nh_Token {
        NH_TOKEN type;
        char *string_p;
    } nh_Token;

    typedef struct nh_Tokenizer {
        unsigned int tokens;
        nh_Token *Tokens_p;
    } nh_Tokenizer;

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_API_RESULT nh_core_tokenize(
        nh_Tokenizer *Tokenizer_p, char *data_p, int length 
    ); 

    void nh_core_freeTokenizer(
        nh_Tokenizer *Tokenizer_p
    );

/** @} */

#endif // NH_CORE_UTIL_TOKENIZER_H
