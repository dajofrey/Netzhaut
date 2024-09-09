#ifndef NH_CSS_TOKEN_PARSER_H
#define NH_CSS_TOKEN_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Tokenizer.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_enums
 *  @{
 */

    typedef enum NH_CSS_COMPONENT_VALUE {
        NH_CSS_COMPONENT_VALUE_PRESERVED_TOKEN,
        NH_CSS_COMPONENT_VALUE_FUNCTION,
        NH_CSS_COMPONENT_VALUE_SIMPLE_BLOCK,
    } NH_CSS_COMPONENT_VALUE;

    typedef enum NH_CSS_RULE {
        NH_CSS_RULE_QUALIFIED,
        NH_CSS_RULE_AT,
    } NH_CSS_RULE;

/** @} */

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_TokenParser {
        nh_css_Token **Tokens_pp;
        unsigned long long length;
    } nh_css_TokenParser;

    typedef struct nh_css_SimpleBlock {
        NH_CSS_COMPONENT_VALUE type;
        nh_css_Token *Token_p;
        nh_Array ComponentValues;
    } nh_css_SimpleBlock;

    typedef struct nh_css_Function {
        NH_CSS_COMPONENT_VALUE type;
        nh_css_Token *Token_p;
        nh_Array ComponentValues;
    } nh_css_Function;

    typedef struct nh_css_PreservedToken {
        NH_CSS_COMPONENT_VALUE type;
        nh_css_Token *Token_p;
    } nh_css_PreservedToken;

    typedef union nh_css_ComponentValue {
        NH_CSS_COMPONENT_VALUE type;
        nh_css_PreservedToken PreservedToken;
        nh_css_SimpleBlock Block;
        nh_css_Function Function;
    } nh_css_ComponentValue;

    typedef struct nh_css_Declaration {
        nh_encoding_UTF8String Name;
        nh_Array ComponentValues;
        NH_BOOL important;
    } nh_css_Declaration;

    typedef struct nh_css_Rule {
        NH_CSS_RULE type;
        nh_encoding_UTF32String *Name_p;
        nh_Array Prelude;
        nh_css_SimpleBlock Block;
    } nh_css_Rule;

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_css_TokenParser nh_css_initTokenParser(
        nh_css_Token **Tokens_pp, unsigned long long length
    );
    
    nh_css_StyleSheetObject *nh_css_parseStyleSheet(
        nh_css_TokenParser *Parser_p, nh_css_DocumentObject *Document_p
    );

    nh_Array nh_css_parseDeclarations(
        nh_css_TokenParser *Parser_p
    );
    
    NH_CSS_RESULT nh_css_parseComponentValue(
        nh_css_TokenParser *Parser_p, nh_css_ComponentValue *Value_p
    );
    
    nh_Array nh_css_parseComponentValues(
        nh_css_TokenParser *Parser_p
    );

/** @} */

#endif
