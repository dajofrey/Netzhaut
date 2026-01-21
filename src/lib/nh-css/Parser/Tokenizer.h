#ifndef NH_CSS_TOKENIZER_H
#define NH_CSS_TOKENIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_enums
 *  @{
 */

    typedef enum NH_CSS_TOKEN {
        NH_CSS_TOKEN_IDENT = 0, 
        NH_CSS_TOKEN_FUNCTION,
        NH_CSS_TOKEN_AT_KEYWORD,
        NH_CSS_TOKEN_HASH,
        NH_CSS_TOKEN_STRING,
        NH_CSS_TOKEN_BAD_STRING,
        NH_CSS_TOKEN_URL,
        NH_CSS_TOKEN_BAD_URL,
        NH_CSS_TOKEN_DELIM,
        NH_CSS_TOKEN_NUMBER, 
        NH_CSS_TOKEN_PERCENTAGE, 
        NH_CSS_TOKEN_DIMENSION, 
        NH_CSS_TOKEN_WHITESPACE, 
        NH_CSS_TOKEN_CDO, 
        NH_CSS_TOKEN_CDC, 
        NH_CSS_TOKEN_COLON, 
        NH_CSS_TOKEN_SEMICOLON, 
        NH_CSS_TOKEN_COMMA, 
        NH_CSS_TOKEN_LEFT_SQUARE_BRACKET, 
        NH_CSS_TOKEN_RIGHT_SQUARE_BRACKET, 
        NH_CSS_TOKEN_LEFT_PARENTHESIS, 
        NH_CSS_TOKEN_RIGHT_PARENTHESIS, 
        NH_CSS_TOKEN_LEFT_CURLY_BRACKET, 
        NH_CSS_TOKEN_RIGHT_CURLY_BRACKET,
        NH_CSS_TOKEN_EOF,
    } NH_CSS_TOKEN;

/** @} */

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_OtherToken {
        NH_CSS_TOKEN type;
        nh_encoding_UTF32String Value;
    } nh_css_OtherToken;

    typedef struct nh_css_HashToken {
        NH_CSS_TOKEN type;
        nh_encoding_UTF32String Value;
        bool unrestricted;
    } nh_css_HashToken;

    typedef struct nh_css_DelimToken {
        NH_CSS_TOKEN type;
        NH_ENCODING_UTF32 value;
    } nh_css_DelimToken;

    typedef struct nh_css_PercentageToken {
        NH_CSS_TOKEN type;
        double value;
    } nh_css_PercentageToken;

    typedef struct nh_css_NumberToken {
        NH_CSS_TOKEN type;
        double value;
        bool isInteger;
    } nh_css_NumberToken;

    typedef struct nh_css_DimensionToken {
        NH_CSS_TOKEN type;
        double value;
        bool isInteger;
        nh_encoding_UTF32String Unit;
    } nh_css_DimensionToken;

    typedef union nh_css_Token {
        NH_CSS_TOKEN type;
        nh_css_HashToken Hash;
        nh_css_DelimToken Delim;
        nh_css_PercentageToken Percentage;
        nh_css_NumberToken Number;
        nh_css_DimensionToken Dimension;
        nh_css_OtherToken Other;
    } nh_css_Token;

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    const char *nh_css_getTokenName(
        NH_CSS_TOKEN token
    );

    nh_core_Array nh_css_tokenize(
        nh_encoding_UTF32String *String_p
    );

/** @} */

#endif
