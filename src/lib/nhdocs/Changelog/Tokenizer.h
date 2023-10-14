#ifndef NH_DOCS_CHANGELOG_TOKENIZER_H
#define NH_DOCS_CHANGELOG_TOKENIZER_H

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

    typedef enum NH_DOCS_TOKEN { 
        NH_DOCS_TOKEN_UNDEFINED, 
        NH_DOCS_TOKEN_COMMA, 
        NH_DOCS_TOKEN_IDENTIFIER, 
        NH_DOCS_TOKEN_STRING, 
        NH_DOCS_TOKEN_CURLY_BRACKET_RIGHT, 
        NH_DOCS_TOKEN_CURLY_BRACKET_LEFT, 
        NH_DOCS_TOKEN_ROUND_BRACKET_RIGHT, 
        NH_DOCS_TOKEN_ROUND_BRACKET_LEFT, 
        NH_DOCS_TOKEN_ANGLE_BRACKET_RIGHT, 
        NH_DOCS_TOKEN_ANGLE_BRACKET_LEFT, 
        NH_DOCS_TOKEN_HYPHEN_MINUS, 
        NH_DOCS_TOKEN_COLON,
        NH_DOCS_TOKEN_EOF, 
    } NH_DOCS_TOKEN; 

/** @} */

/** @addtogroup lib_nhcore_structs
 *  @{
 */

    typedef struct nh_docs_Token {
        NH_DOCS_TOKEN type;
        NH_BYTE *string_p;
    } nh_docs_Token;

    typedef struct nh_docs_Tokenizer {
        unsigned int tokens;
        nh_docs_Token *Tokens_p;
    } nh_docs_Tokenizer;

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_DOCS_RESULT nh_docs_tokenize(
        nh_docs_Tokenizer *Tokenizer_p, NH_BYTE *data_p, int length 
    ); 

    void nh_docs_freeTokenizer(
        nh_docs_Tokenizer *Tokenizer_p
    );

/** @} */

#endif // NH_DOCS_CHANGELOG_TOKENIZER_H
