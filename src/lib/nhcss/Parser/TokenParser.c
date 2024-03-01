// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "TokenParser.h"
#include "ComponentParser.h"
#include "Tokenizer.h"

#include "../Interfaces/RuleList.h"
#include "../Interfaces/Document.h"
#include "../Interfaces/StyleSheet.h"
#include "../Interfaces/StyleSheetList.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nhcore/Util/Array.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/List.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhencoding/Encodings/UTF8.h"

#include "../../nhwebidl/Runtime/Object.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// PARSER HELPER ===================================================================================

nh_css_TokenParser nh_css_initTokenParser(
    nh_css_Token **Tokens_pp, unsigned long long length)
{
NH_CSS_BEGIN()

    nh_css_TokenParser Parser;
    Parser.Tokens_pp = Tokens_pp;
    Parser.length = length;

NH_CSS_END(Parser)
}

static void nh_css_advanceTokenParser(
    nh_css_TokenParser *Parser_p, unsigned long long advance)
{
NH_CSS_BEGIN()

    Parser_p->Tokens_pp = Parser_p->Tokens_pp + advance;
    Parser_p->length -= advance; 

NH_CSS_SILENT_END()
}

// PARSER ALGORITHMS ===============================================================================

static NH_CSS_RESULT nh_css_consumeComponentValue(
    nh_css_TokenParser *Parser_p, nh_css_ComponentValue *Value_p
);

static nh_css_SimpleBlock nh_css_consumeSimpleBlock(
    nh_css_TokenParser *Parser_p)
{
NH_CSS_BEGIN()

    nh_css_SimpleBlock Block;
    Block.type = NH_CSS_COMPONENT_VALUE_SIMPLE_BLOCK;
    Block.Token_p = Parser_p->Tokens_pp[0];
    Block.ComponentValues = nh_core_initArray(sizeof(nh_css_ComponentValue), 8);

    NH_CSS_TOKEN mirror = nh_css_getMirrorToken(Parser_p->Tokens_pp[0])->type;
    nh_css_advanceTokenParser(Parser_p, 1);

    while (Parser_p->length)
    {
        if (Parser_p->Tokens_pp[0]->type == mirror) {
            nh_css_advanceTokenParser(Parser_p, 1);
            NH_CSS_END(Block)
        }
        else if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_EOF) {
            // parse error
            NH_CSS_END(Block)
        }
        else {
            nh_css_ComponentValue ComponentValue;
            if (nh_css_consumeComponentValue(Parser_p, &ComponentValue) == NH_CSS_SUCCESS) {
                nh_css_ComponentValue *Value_p = nh_core_incrementArray(&Block.ComponentValues);
                *Value_p = ComponentValue;
            }
        }
    }

NH_CSS_END(Block)
}

static nh_css_Function nh_css_consumeFunction(
    nh_css_TokenParser *Parser_p)
{
NH_CSS_BEGIN()

    nh_css_Function Function;
    Function.type = NH_CSS_COMPONENT_VALUE_FUNCTION;
    Function.Token_p = Parser_p->Tokens_pp[0];
    Function.ComponentValues = nh_core_initArray(sizeof(nh_css_ComponentValue), 8);

    nh_css_advanceTokenParser(Parser_p, 1);

    while (Parser_p->length)
    {
        if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_RIGHT_PARENTHESIS) {
            nh_css_advanceTokenParser(Parser_p, 1);
            NH_CSS_END(Function)
        }
        else if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_EOF) {
            // parse error
            NH_CSS_END(Function)
        }
        else {
            nh_css_ComponentValue ComponentValue;
            if (nh_css_consumeComponentValue(Parser_p, &ComponentValue) == NH_CSS_SUCCESS) {
                nh_css_ComponentValue *Value_p = nh_core_incrementArray(&Function.ComponentValues);
                *Value_p = ComponentValue;
            }
        }
    }

NH_CSS_END(Function)
}

static NH_CSS_RESULT nh_css_consumeComponentValue(
    nh_css_TokenParser *Parser_p, nh_css_ComponentValue *Value_p)
{
NH_CSS_BEGIN()

    switch (Parser_p->Tokens_pp[0]->type)
    { 
        case NH_CSS_TOKEN_LEFT_CURLY_BRACKET  :
        case NH_CSS_TOKEN_LEFT_SQUARE_BRACKET :
        case NH_CSS_TOKEN_LEFT_PARENTHESIS    :
            Value_p->type = NH_CSS_COMPONENT_VALUE_SIMPLE_BLOCK;
            Value_p->Block = nh_css_consumeSimpleBlock(Parser_p);
            break;

        case NH_CSS_TOKEN_FUNCTION :
            Value_p->type = NH_CSS_COMPONENT_VALUE_FUNCTION;
            Value_p->Function = nh_css_consumeFunction(Parser_p);
            break;

        default :
        {
            Value_p->type = NH_CSS_COMPONENT_VALUE_PRESERVED_TOKEN;
            Value_p->PreservedToken.Token_p = Parser_p->Tokens_pp[0];
            nh_css_advanceTokenParser(Parser_p, 1);
        }
    }

NH_CSS_END(NH_CSS_SUCCESS)
}

static NH_CSS_RESULT nh_css_consumeQualifiedRule(
    nh_css_TokenParser *Parser_p, nh_css_Rule *Rule_p)
{
NH_CSS_BEGIN()

    Rule_p->type = NH_CSS_RULE_QUALIFIED;
    Rule_p->Prelude = nh_core_initArray(sizeof(nh_css_ComponentValue), 8);
    Rule_p->Block.ComponentValues = nh_core_initArray(sizeof(nh_css_ComponentValue), 8);

    while (Parser_p->length)
    {
        switch (Parser_p->Tokens_pp[0]->type)
        { 
            case NH_CSS_TOKEN_EOF :
                // parse error
                NH_CSS_DIAGNOSTIC_END(NH_CSS_ERROR_BAD_STATE)

            case NH_CSS_TOKEN_LEFT_CURLY_BRACKET :
                Rule_p->Block = nh_css_consumeSimpleBlock(Parser_p);
                NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS) 

            default :
            {
                nh_css_ComponentValue ComponentValue;
                if (nh_css_consumeComponentValue(Parser_p, &ComponentValue) == NH_CSS_SUCCESS) {
                    nh_css_ComponentValue *Value_p = nh_core_incrementArray(&Rule_p->Prelude);
                    *Value_p = ComponentValue;
                }
            }
        }
    }

NH_CSS_DIAGNOSTIC_END(NH_CSS_ERROR_BAD_STATE)
}

static NH_CSS_RESULT nh_css_consumeAtRule(
    nh_css_TokenParser *Parser_p, nh_css_Rule *Rule_p)
{
NH_CSS_BEGIN()

    Rule_p->type = NH_CSS_RULE_AT;
    Rule_p->Name_p = &Parser_p->Tokens_pp[0]->Other.Value;
    Rule_p->Prelude = nh_core_initArray(sizeof(nh_css_ComponentValue), 8);
    Rule_p->Block.ComponentValues = nh_core_initArray(sizeof(nh_css_ComponentValue), 8);

    nh_css_advanceTokenParser(Parser_p, 1);

    while (Parser_p->length)
    {
        switch (Parser_p->Tokens_pp[0]->type)
        { 
            case NH_CSS_TOKEN_SEMICOLON :
                nh_css_advanceTokenParser(Parser_p, 1);
                NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)

            case NH_CSS_TOKEN_EOF :
                // parse error
                NH_CSS_DIAGNOSTIC_END(NH_CSS_ERROR_BAD_STATE)

            case NH_CSS_TOKEN_LEFT_CURLY_BRACKET :
                Rule_p->Block = nh_css_consumeSimpleBlock(Parser_p);
                NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS) 

            default :
            {
                nh_css_ComponentValue ComponentValue;
                if (nh_css_consumeComponentValue(Parser_p, &ComponentValue) == NH_CSS_SUCCESS) {
                    nh_css_ComponentValue *Value_p = nh_core_incrementArray(&Rule_p->Prelude);
                    *Value_p = ComponentValue;
                }
            }
        }
    }

NH_CSS_DIAGNOSTIC_END(NH_CSS_ERROR_BAD_STATE)
}

static nh_Array nh_css_consumeRules(
    nh_css_TokenParser *Parser_p, NH_BOOL topLevel)
{
NH_CSS_BEGIN()

    nh_Array Rules = nh_core_initArray(sizeof(nh_css_Rule), 127);

    while (Parser_p->length)
    {
        switch (Parser_p->Tokens_pp[0]->type)
        { 
            case NH_CSS_TOKEN_WHITESPACE :
                nh_css_advanceTokenParser(Parser_p, 1);
                break;

            case NH_CSS_TOKEN_EOF :
                NH_CSS_END(Rules)

            case NH_CSS_TOKEN_CDO :
            case NH_CSS_TOKEN_CDC :
                if (topLevel) {break;}
                exit(0); // TODO

            case NH_CSS_TOKEN_AT_KEYWORD :
            {
                nh_css_Rule Rule;
                if (nh_css_consumeAtRule(Parser_p, &Rule) == NH_CSS_SUCCESS) {
                    nh_css_Rule *Rule_p = nh_core_incrementArray(&Rules);
                    *Rule_p = Rule;
                }
                break;
            }
            default :
            {
                nh_css_Rule Rule;
                if (nh_css_consumeQualifiedRule(Parser_p, &Rule) == NH_CSS_SUCCESS) {
                    nh_css_Rule *Rule_p = nh_core_incrementArray(&Rules);
                    *Rule_p = Rule;
                }
            }
        }
    }

NH_CSS_END(Rules)
}

static NH_CSS_RESULT nh_css_consumeDeclaration(
    nh_css_TokenParser *Parser_p, nh_css_Declaration *Declaration_p)
{
NH_CSS_BEGIN()

    if (Parser_p->Tokens_pp[0]->type != NH_CSS_TOKEN_IDENT) {
        NH_CSS_DIAGNOSTIC_END(NH_CSS_ERROR_BAD_STATE)
    }

    Declaration_p->Name = nh_encoding_encodeUTF8(Parser_p->Tokens_pp[0]->Other.Value.p, Parser_p->Tokens_pp[0]->Other.Value.length);
    Declaration_p->ComponentValues = nh_core_initArray(sizeof(nh_css_ComponentValue), 8);

    nh_css_advanceTokenParser(Parser_p, 1);

    while (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_WHITESPACE) {
        nh_css_advanceTokenParser(Parser_p, 1);
    }

    if (Parser_p->Tokens_pp[0]->type != NH_CSS_TOKEN_COLON) {
        NH_CSS_DIAGNOSTIC_END(NH_CSS_ERROR_BAD_STATE)
    }

    nh_css_advanceTokenParser(Parser_p, 1);

    while (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_WHITESPACE) {
        nh_css_advanceTokenParser(Parser_p, 1);
    }

    while (Parser_p->length)
    {
        nh_css_ComponentValue Value;
        if (nh_css_consumeComponentValue(Parser_p, &Value) == NH_CSS_SUCCESS) {
            nh_css_ComponentValue *Value_p = nh_core_incrementArray(&Declaration_p->ComponentValues);
            *Value_p = Value;
        }
    }

    // TODO

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

static nh_css_Declaration nh_css_initDeclaration()
{
    nh_css_Declaration Declaration;
    Declaration.important = NH_FALSE;
    return Declaration;
}

static nh_Array nh_css_consumeDeclarations(
    nh_css_TokenParser *Parser_p)
{
    nh_Array Declarations = nh_core_initArray(sizeof(nh_css_Declaration), 8);

    while (Parser_p->length)
    {
        switch (Parser_p->Tokens_pp[0]->type)
        {
            case NH_CSS_TOKEN_WHITESPACE :
            case NH_CSS_TOKEN_SEMICOLON  : 
                break;

            case NH_CSS_TOKEN_EOF :
                return Declarations;

            case NH_CSS_TOKEN_AT_KEYWORD :
                break; // TODO

            case NH_CSS_TOKEN_IDENT :
            {
                nh_List Tmp = nh_core_initList(8);
                nh_core_appendToList(&Tmp, Parser_p->Tokens_pp[0]);
                nh_css_advanceTokenParser(Parser_p, 1);

                while (Parser_p->length > 0 && Parser_p->Tokens_pp[0]->type != NH_CSS_TOKEN_SEMICOLON && Parser_p->Tokens_pp[0]->type != NH_CSS_TOKEN_EOF) 
                {
                    nh_css_ComponentValue Value;

                    if (nh_css_consumeComponentValue(Parser_p, &Value) == NH_CSS_SUCCESS) 
                    {
                        nh_Array Array;
                        Array.length = 1;
                        Array.p = (NH_BYTE*)&Value;

                        nh_List Tokens = nh_css_getTokensFromComponentValues(&Array);
                        for (int i = 0; i < Tokens.size; ++i) {nh_core_appendToList(&Tmp, Tokens.pp[i]);}

                        nh_core_freeList(&Tokens, NH_FALSE);
                    }
                }

                nh_css_TokenParser DeclarationParser = nh_css_initTokenParser((nh_css_Token**)Tmp.pp, Tmp.size);
                nh_css_Declaration Declaration = nh_css_initDeclaration();

                if (nh_css_consumeDeclaration(&DeclarationParser, &Declaration) == NH_CSS_SUCCESS) {
                    nh_css_Declaration *Declaration_p = nh_core_incrementArray(&Declarations);
                    *Declaration_p = Declaration;
                }

                nh_core_freeList(&Tmp, NH_FALSE);
                if (Parser_p->length == 0) {return Declarations;}

                break;
            }

            default :
                exit(0); // TODO
        }

        nh_css_advanceTokenParser(Parser_p, 1);
    }

    return Declarations;
}

// ENTRY POINTS ====================================================================================

nh_css_StyleSheetObject *nh_css_parseStyleSheet(
    nh_css_TokenParser *Parser_p, nh_css_DocumentObject *Document_p)
{
    nh_css_StyleSheetObject *StyleSheet_p = (nh_css_StyleSheetObject*)nh_webidl_createObject("CSS", "CSSStyleSheet");
    NH_CSS_CHECK_NULL_2(NULL, StyleSheet_p)

    nh_Array Rules = nh_css_consumeRules(Parser_p, NH_TRUE);
    NH_CSS_CHECK_2(NULL, nh_css_logRules(StyleSheet_p, &Rules))

    if (!nh_css_getRuleList(StyleSheet_p)) {return NULL;}

    NH_CSS_CHECK_2(NULL, nh_css_parseRules(&Rules, nh_css_getRuleList(StyleSheet_p)))
    NH_CSS_CHECK_2(NULL, nh_css_logRuleObjects(StyleSheet_p, nh_css_getRuleListData(nh_css_getRuleList(StyleSheet_p))))

    if (Document_p) {
        nh_css_StyleSheetListObject *StyleSheetList_p = nh_css_getStyleSheetList(Document_p);
        nh_css_appendToStyleSheetList(StyleSheetList_p, StyleSheet_p);
    }

    nh_core_freeArray(&Rules);

    return StyleSheet_p;
}

nh_Array nh_css_parseDeclarations(
    nh_css_TokenParser *Parser_p)
{
    return nh_css_consumeDeclarations(Parser_p);
}

NH_CSS_RESULT nh_css_parseComponentValue(
    nh_css_TokenParser *Parser_p, nh_css_ComponentValue *Value_p)
{
    while (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_WHITESPACE) {
        nh_css_advanceTokenParser(Parser_p, 1);
    }

    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_EOF) {
        // syntax error
        return NH_CSS_ERROR_BAD_STATE;
    }

    nh_css_ComponentValue Value;
    if (nh_css_consumeComponentValue(Parser_p, &Value) != NH_CSS_SUCCESS) {
        return NH_CSS_ERROR_BAD_STATE;
    }

    while (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_WHITESPACE) {
        nh_css_advanceTokenParser(Parser_p, 1);
    }

    if (Parser_p->Tokens_pp[0]->type != NH_CSS_TOKEN_EOF) {
        // syntax error
        return NH_CSS_ERROR_BAD_STATE;
    }

    *Value_p = Value;

    return NH_CSS_SUCCESS;
}

nh_Array nh_css_parseComponentValues(
    nh_css_TokenParser *Parser_p)
{
    nh_Array ComponentValues = nh_core_initArray(sizeof(nh_css_ComponentValue), 1);

    while (Parser_p->length && Parser_p->Tokens_pp[0]->type != NH_CSS_TOKEN_EOF)
    {
        nh_css_ComponentValue Value;
        if (nh_css_parseComponentValue(Parser_p, &Value) == NH_CSS_SUCCESS) {
            nh_css_ComponentValue *Value_p = nh_core_incrementArray(&ComponentValues);
            *Value_p = Value;
        }
        else {break;}
    }

    return ComponentValues;
}

