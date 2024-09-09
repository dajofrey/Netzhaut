// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "ComponentParser.h"

#include "../Interfaces/Rule.h"
#include "../Interfaces/StyleDeclaration.h"
#include "../Interfaces/StyleRule.h"
#include "../Interfaces/RuleList.h"
#include "../Interfaces/StyleSheet.h"
#include "../Interfaces/CounterStyleRule.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DECLARE =========================================================================================

typedef struct nh_css_ComponentParser {
    nh_css_ComponentValue *Components_p;
    unsigned long long current;
    unsigned long long index;
    unsigned long long length;
    NH_BOOL inBlockOrFunction;
    nh_css_Token *Token_p;
} nh_css_ComponentParser;

// COMPONENT VALUES PARSER =========================================================================

nh_css_Token *nh_css_getMirrorToken(
    nh_css_Token *Token_p)
{
NH_CSS_BEGIN()

    static nh_css_Token RightCurly;
    RightCurly.type = NH_CSS_TOKEN_RIGHT_CURLY_BRACKET;
    static nh_css_Token RightSquare;
    RightSquare.type = NH_CSS_TOKEN_RIGHT_SQUARE_BRACKET;
    static nh_css_Token RightParenthesis;
    RightParenthesis.type = NH_CSS_TOKEN_RIGHT_PARENTHESIS;

    switch (Token_p->type)
    {
        case NH_CSS_TOKEN_LEFT_CURLY_BRACKET  : NH_CSS_END(&RightCurly)
        case NH_CSS_TOKEN_LEFT_SQUARE_BRACKET : NH_CSS_END(&RightSquare)
        case NH_CSS_TOKEN_LEFT_PARENTHESIS    : NH_CSS_END(&RightParenthesis)
    }

NH_CSS_END(NULL)
}

static nh_css_Token *nh_css_getTokenFromComponentValue(
    nh_css_ComponentParser *Parser_p, unsigned long long index)
{
NH_CSS_BEGIN()

    if (!Parser_p->Components_p) {NH_CSS_END(NULL)}

    nh_css_ComponentValue *Value_p = &Parser_p->Components_p[Parser_p->current];
    nh_css_Token *Token_p = NULL;

    if (Value_p->type == NH_CSS_COMPONENT_VALUE_PRESERVED_TOKEN && index == 0) {
        Token_p = Value_p->PreservedToken.Token_p;
    }
    else if (Value_p->type == NH_CSS_COMPONENT_VALUE_SIMPLE_BLOCK) 
    {
        if (index == 0) {
            Token_p = Value_p->Block.Token_p;
            Parser_p->inBlockOrFunction = NH_TRUE;
        }
        else
        {
            nh_List Tokens = nh_css_getTokensFromComponentValues(&Value_p->Block.ComponentValues);
            Token_p = nh_core_getFromList(&Tokens, index - 1);
            nh_core_freeList(&Tokens, NH_FALSE);

            if (!Token_p && Parser_p->inBlockOrFunction) {
                Token_p = nh_css_getMirrorToken(Value_p->Block.Token_p);
                Parser_p->inBlockOrFunction = NH_FALSE;
            }
        }
    }
    else if (Value_p->type == NH_CSS_COMPONENT_VALUE_FUNCTION) 
    {
        if (index == 0) {
            Token_p = Value_p->Function.Token_p;
            Parser_p->inBlockOrFunction = NH_TRUE;
        }
        else
        {
            nh_List Tokens = nh_css_getTokensFromComponentValues(&Value_p->Function.ComponentValues);
            Token_p = nh_core_getFromList(&Tokens, index - 1);
            nh_core_freeList(&Tokens, NH_FALSE);

            if (!Token_p && Parser_p->inBlockOrFunction) {
                Token_p = nh_css_getMirrorToken(Value_p->Function.Token_p);
                Parser_p->inBlockOrFunction = NH_FALSE;
            }
        }
    }

NH_CSS_END(Token_p)
}

static nh_css_ComponentParser nh_css_initComponentParser(
    nh_css_ComponentValue *Components_p, unsigned long long length)
{
NH_CSS_BEGIN()

    nh_css_ComponentParser Parser;
    Parser.Components_p = Components_p;
    Parser.current = 0;
    Parser.index = 0;
    Parser.length = length;
    Parser.inBlockOrFunction = NH_FALSE;
    Parser.Token_p = nh_css_getTokenFromComponentValue(&Parser, 0);

NH_CSS_END(Parser)
}

static void nh_css_advanceComponentParser(
    nh_css_ComponentParser *Parser_p)
{
NH_CSS_BEGIN()

    Parser_p->index++;
    Parser_p->Token_p = nh_css_getTokenFromComponentValue(Parser_p, Parser_p->index);

    if (!Parser_p->Token_p) 
    {
        Parser_p->current++;
        Parser_p->length--;
        Parser_p->index = 0;

        if (Parser_p->length) {
            Parser_p->Token_p = nh_css_getTokenFromComponentValue(Parser_p, Parser_p->index);
        }
    }

NH_CSS_SILENT_END()
}

nh_List nh_css_getTokensFromComponentValues(
    nh_Array *ComponentValues_p)
{
NH_CSS_BEGIN()

    nh_List Tokens = nh_core_initList(255); 
    nh_css_ComponentParser Parser = 
        nh_css_initComponentParser((nh_css_ComponentValue*)ComponentValues_p->p, ComponentValues_p->length);

    while (Parser.length) {
        nh_core_appendToList(&Tokens, Parser.Token_p);
        nh_css_advanceComponentParser(&Parser);
    }

NH_CSS_END(Tokens)
}

// QUALIFIED RULES =================================================================================

static nh_Array nh_css_getDeclarations(
    nh_css_Rule *Rule_p)
{
NH_CSS_BEGIN()

    nh_List Tokens = nh_css_getTokensFromComponentValues(&Rule_p->Block.ComponentValues);
    nh_css_TokenParser Parser = nh_css_initTokenParser((nh_css_Token**)Tokens.pp, Tokens.size);
    
    nh_css_Token EndToken;
    EndToken.type = NH_CSS_TOKEN_EOF;
    nh_core_appendToList(&Tokens, &EndToken);

    nh_Array Declarations = nh_css_parseDeclarations(&Parser);

    nh_core_freeList(&Tokens, NH_FALSE);

NH_CSS_END(Declarations)
}

static NH_CSS_RESULT nh_css_parseQualifiedRule(
    nh_css_Rule *Rule_p, nh_css_RuleListObject *RuleList_p)
{
NH_CSS_BEGIN()

    nh_css_SelectorParseNode *Selectors_p = nh_css_parseSelectorList(Rule_p->Prelude);
    NH_CSS_CHECK_NULL(Selectors_p)

    nh_Array Declarations = nh_css_getDeclarations(Rule_p);

    nh_css_StyleDeclarationObject *StyleDeclaration_p = nh_css_createStyleDeclaration(Declarations);
    NH_CSS_CHECK_NULL(StyleDeclaration_p)
    nh_css_StyleRuleObject *StyleRule_p = nh_css_createStyleRule(Selectors_p, StyleDeclaration_p);
    NH_CSS_CHECK_NULL(StyleRule_p)

    nh_css_setRuleData(nh_css_getRule((nh_webidl_Object*)StyleRule_p), *Rule_p);
    nh_css_appendToRuleList(RuleList_p, (nh_webidl_Object*)StyleRule_p);

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

// AT RULES ========================================================================================

static NH_CSS_RESULT nh_css_parseAtRule(
    nh_css_Rule *Rule_p, nh_css_RuleListObject *RuleList_p)
{
NH_CSS_BEGIN()

    if (Rule_p->Prelude.length <= 0) {
        NH_CSS_DIAGNOSTIC_END(NH_CSS_ERROR_BAD_STATE)
    }

    nh_encoding_UTF8String Name = nh_encoding_encodeUTF8(Rule_p->Name_p->p, Rule_p->Name_p->length);

    nh_webidl_Object *Object_p = NULL;
    if (!strcmp(Name.p, "counter-style")) {
        nh_Array Declarations = nh_css_getDeclarations(Rule_p);
        Object_p = (nh_webidl_Object*)nh_css_createCounterStyleRule(Declarations);
    }

    if (Object_p) {
        nh_css_appendToRuleList(RuleList_p, Object_p);
        nh_css_setRuleData(nh_css_getRule(Object_p), *Rule_p);
    }

    nh_encoding_freeUTF8(&Name);

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

// PARSE ===========================================================================================

NH_CSS_RESULT nh_css_parseRules(
    nh_Array *Rules_p, nh_css_RuleListObject *RuleList_p)
{
NH_CSS_BEGIN()

    for (int i = 0; i < Rules_p->length; ++i) 
    {
        nh_css_Rule *Rule_p = &((nh_css_Rule*)Rules_p->p)[i];
        switch (Rule_p->type)
        {
            case NH_CSS_RULE_QUALIFIED :
                NH_CSS_CHECK(nh_css_parseQualifiedRule(Rule_p, RuleList_p))
                break;
            case NH_CSS_RULE_AT :
                NH_CSS_CHECK(nh_css_parseAtRule(Rule_p, RuleList_p))
                break;
        } 
    }

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

