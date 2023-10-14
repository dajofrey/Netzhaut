#ifndef NH_MAKE_PARSER_TOKENIZER_H
#define NH_MAKE_PARSER_TOKENIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Core/File.h"
#include "../Common/Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhmake_structs
 *  @{
 */

    typedef struct nh_make_Token {
        NH_MAKE_TOKEN type;
        NH_BYTE *string_p;
    } nh_make_Token;

    typedef struct nh_make_Tokenizer {
        unsigned int tokens;
        nh_make_Token *Tokens_p;
    } nh_make_Tokenizer;

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    nh_make_Tokenizer nh_make_initTokenizer(
    );

    NH_MAKE_RESULT nh_make_tokenizeFile(
        nh_make_Tokenizer *Tokenizer_p, nh_make_File *File_p 
    ); 

/** @} */

#endif // NH_MAKE_PARSER_TOKENIZER_H
