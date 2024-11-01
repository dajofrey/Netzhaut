// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"

#include "../Interfaces/StyleDeclaration.h"
#include "../Interfaces/StyleRule.h"
#include "../Main/FormattingContext.h"
#include "../Parser/ComponentParser.h"

#include "../../nh-encoding/Encodings/UTF8.h"
#include "../../nh-core/System/Logger.h"

#include <stdio.h>
#include <string.h>

// DEFINES =========================================================================================

#define NH_CSS_MAX_MESSAGE 2047
#define NH_CSS_MAX_TOKEN 1023
#define NH_CSS_MAX_INDENT 255

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_css_stringifyToken(
    nh_css_Token *Token_p, char *token_p)
{
    if (Token_p->type == NH_CSS_TOKEN_IDENT 
    ||  Token_p->type == NH_CSS_TOKEN_FUNCTION 
    ||  Token_p->type == NH_CSS_TOKEN_AT_KEYWORD 
    ||  Token_p->type == NH_CSS_TOKEN_HASH 
    ||  Token_p->type == NH_CSS_TOKEN_STRING 
    ||  Token_p->type == NH_CSS_TOKEN_URL) 
    {
        nh_encoding_UTF8String UTF8 = nh_encoding_encodeUTF8(Token_p->Other.Value.p, Token_p->Other.Value.length);
        sprintf(token_p, "%s %s", nh_css_getTokenName(Token_p->type), UTF8.p);
        nh_encoding_freeUTF8(&UTF8);
    }
    else if (Token_p->type == NH_CSS_TOKEN_DELIM) {
        sprintf(token_p, "%s %c", nh_css_getTokenName(Token_p->type), Token_p->Delim.value);
    }
    else if (Token_p->type == NH_CSS_TOKEN_PERCENTAGE) {
        sprintf(token_p, "%s %lf", nh_css_getTokenName(Token_p->type), Token_p->Percentage.value);
    }
    else if (Token_p->type == NH_CSS_TOKEN_NUMBER) {
        sprintf(token_p, "%s %lf", nh_css_getTokenName(Token_p->type), Token_p->Number.value);
    }
    else if (Token_p->type == NH_CSS_TOKEN_DIMENSION) {
        nh_encoding_UTF8String Unit = nh_encoding_encodeUTF8(Token_p->Dimension.Unit.p, Token_p->Dimension.Unit.length);
        sprintf(token_p, "%s %lf%s", nh_css_getTokenName(Token_p->type), Token_p->Dimension.value, Unit.p);
        nh_encoding_freeUTF8(&Unit);
    }
    else {
        sprintf(token_p, "%s", nh_css_getTokenName(Token_p->type)); 
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_logTokens(
    nh_webidl_Object *CSSStyleSheet_p, nh_core_Array *Tokens_p)
{
    char message_p[NH_CSS_MAX_MESSAGE] = {'\0'};
    char node_p[255] = {'\0'};
    sprintf(node_p, "nh-css:Sheet:%p:Tokens", CSSStyleSheet_p);

    for (int i = 0; i < Tokens_p->length; ++i)
    {
        nh_css_Token *Token_p = &((nh_css_Token*)Tokens_p->p)[i];
        NH_CORE_CHECK(nh_css_stringifyToken(Token_p, message_p))

        nh_core_sendLogMessage(node_p, NULL, message_p);

        memset(message_p, 0, NH_CSS_MAX_MESSAGE);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_logComponentValue(
    char *node_p, nh_css_ComponentValue *Value_p, char *token_p, char *message_p, char *indent_p, int depth)
{
    for (int i = 0; i < depth * 2; ++i) {indent_p[i] = ' ';}

    if (Value_p->type == NH_CSS_COMPONENT_VALUE_PRESERVED_TOKEN) {
        nh_css_stringifyToken(Value_p->PreservedToken.Token_p, token_p);
        sprintf(message_p, "%s%s", indent_p, token_p);
        nh_core_sendLogMessage(node_p, NULL, message_p);
    }
    else if (Value_p->type == NH_CSS_COMPONENT_VALUE_SIMPLE_BLOCK) 
    {
        nh_css_stringifyToken(Value_p->Block.Token_p, token_p);
        sprintf(message_p, "%s%s", indent_p, token_p);
        nh_core_sendLogMessage(node_p, NULL, message_p);
        memset(token_p, 0, NH_CSS_MAX_TOKEN);
        memset(message_p, 0, NH_CSS_MAX_MESSAGE);

        for (int i = 0; i < Value_p->Block.ComponentValues.length; ++i) {
            NH_CORE_CHECK(nh_css_logComponentValue(node_p, &((nh_css_ComponentValue*)Value_p->Block.ComponentValues.p)[i], token_p, message_p, indent_p, depth + 1))
        }

        for (int i = 0; i < depth * 2; ++i) {indent_p[i] = ' ';}

        nh_css_stringifyToken(nh_css_getMirrorToken(Value_p->Block.Token_p), token_p);
        sprintf(message_p, "%s%s", indent_p, token_p);
        nh_core_sendLogMessage(node_p, NULL, message_p);
        memset(token_p, 0, NH_CSS_MAX_TOKEN);
        memset(message_p, 0, NH_CSS_MAX_MESSAGE);
    }
    else if (Value_p->type == NH_CSS_COMPONENT_VALUE_FUNCTION) 
    {
        nh_encoding_UTF8String Name = nh_encoding_encodeUTF8(Value_p->Function.Token_p->Other.Value.p, Value_p->Function.Token_p->Other.Value.length);
        sprintf(message_p, "%s%s", indent_p, Name.p);
        nh_core_sendLogMessage(node_p, NULL, message_p);
        memset(message_p, 0, NH_CSS_MAX_MESSAGE);
        nh_encoding_freeUTF8(&Name);

        for (int i = 0; i < Value_p->Function.ComponentValues.length; ++i) {
            NH_CORE_CHECK(nh_css_logComponentValue(node_p, &((nh_css_ComponentValue*)Value_p->Function.ComponentValues.p)[i], token_p, message_p, indent_p, depth + 1))
        }
    }

    memset(message_p, 0, NH_CSS_MAX_MESSAGE);
    memset(token_p, 0, NH_CSS_MAX_TOKEN);
    memset(indent_p, 0, NH_CSS_MAX_INDENT);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_logRules(
    nh_webidl_Object *CSSStyleSheet_p, nh_core_Array *Rules_p)
{
    char message_p[NH_CSS_MAX_MESSAGE] = {'\0'};
    char token_p[NH_CSS_MAX_TOKEN] = {'\0'};
    char indent_p[NH_CSS_MAX_INDENT] = {'\0'};

    char node_p[255] = {'\0'};
    sprintf(node_p, "nh-css:Sheet:%p:Rules", CSSStyleSheet_p);

    for (int i = 0; i < Rules_p->length; ++i)
    {
        nh_css_Rule *Rule_p = &((nh_css_Rule*)Rules_p->p)[i];

        if (i < Rules_p->length) {
            if (i > 0) {
                nh_core_sendLogMessage(node_p, NULL, " ");
            }
            if (Rule_p->type == NH_CSS_RULE_AT)
            {
                nh_encoding_UTF8String Name = nh_encoding_encodeUTF8(Rule_p->Name_p->p, Rule_p->Name_p->length);
                sprintf(message_p, "@%s Rule %d", Name.p, i);
                nh_core_sendLogMessage(node_p, NULL, message_p);
                nh_encoding_freeUTF8(&Name);
            } 
            else {
                sprintf(message_p, "Qualified Rule %d", i);
                nh_core_sendLogMessage(node_p, NULL, message_p);
            }
            memset(message_p, 0, NH_CSS_MAX_MESSAGE);
        }

        if (Rule_p->Prelude.length){nh_core_sendLogMessage(node_p, NULL, "  Prelude");}
        for (int k = 0; k < Rule_p->Prelude.length; ++k) {
            NH_CORE_CHECK(nh_css_logComponentValue(node_p, &((nh_css_ComponentValue*)Rule_p->Prelude.p)[k], token_p, message_p, indent_p, 4))
        }

        if (Rule_p->Block.ComponentValues.length){nh_core_sendLogMessage(node_p, NULL, "  Block");}
        for (int k = 0; k < Rule_p->Block.ComponentValues.length; ++k) {
            NH_CORE_CHECK(nh_css_logComponentValue(node_p, &((nh_css_ComponentValue*)Rule_p->Block.ComponentValues.p)[k], token_p, message_p, indent_p, 4))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_logSelectorParseNode(
    char *node_p, nh_css_SelectorParseNode *Node_p, int depth, char *message_p, char *indent_p)
{
    for (int i = 0; i < depth * 2; ++i) {indent_p[i] = ' ';}

    if (Node_p->Token_p != NULL) {
        char token_p[NH_CSS_MAX_TOKEN] = {'\0'};
        NH_CORE_CHECK(nh_css_stringifyToken(Node_p->Token_p, token_p))
        sprintf(message_p, "%s%s", indent_p, token_p);
    }
    else {
        const char *name_p = nh_css_getSelectorParseNodeName(Node_p);
        sprintf(message_p, "%s%s", indent_p, name_p);
    }

    nh_core_sendLogMessage(node_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);
    memset(indent_p, 0, NH_CSS_MAX_INDENT);

    for (int i = 0; i < Node_p->Children.size; ++i) {
        NH_CORE_CHECK(nh_css_logSelectorParseNode(node_p, Node_p->Children.pp[i], depth + 1, message_p, indent_p))
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_logStyleRule(
    char *node_p, char *message_p, char *indent_p, nh_webidl_Object *Rule_p)
{
    char token_p[NH_CSS_MAX_TOKEN] = {'\0'};
    nh_core_Array *Declarations_p = nh_css_getStyleDeclarationData(nh_css_getStyleRuleDeclaration(Rule_p));

    NH_CORE_CHECK(nh_css_logSelectorParseNode(node_p, nh_css_getStyleRuleSelectors(Rule_p), 1, message_p, indent_p))

    for (int i = 0; i < Declarations_p->length; ++i) 
    {
        nh_css_Declaration *Declaration_p = &((nh_css_Declaration*)Declarations_p->p)[i];

        sprintf(message_p, "  %s", Declaration_p->Name.p);
        nh_core_sendLogMessage(node_p, NULL, message_p);

        memset(message_p, 0, NH_CSS_MAX_MESSAGE);
        memset(token_p, 0, NH_CSS_MAX_TOKEN);

        for (int k = 0; k < Declaration_p->ComponentValues.length; ++k) {
            NH_CORE_CHECK(nh_css_logComponentValue(node_p, &((nh_css_ComponentValue*)Declaration_p->ComponentValues.p)[k], token_p, message_p, indent_p, 2))
        }
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_logObjects(
    nh_webidl_Object *CSSStyleSheet_p, nh_core_List *Rules_p)
{
    char message_p[NH_CSS_MAX_MESSAGE] = {'\0'};
    char indent_p[NH_CSS_MAX_INDENT] = {'\0'};

    char node_p[255] = {'\0'};
    sprintf(node_p, "nh-css:Sheet:%p:Objects", CSSStyleSheet_p);

    for (int i = 0; i < Rules_p->size; ++i)
    {
        nh_webidl_Object *Rule_p = Rules_p->pp[i];

        if (i > 0) {nh_core_sendLogMessage(node_p, NULL, " ");}

        sprintf(message_p, "%s %d", Rule_p->Interface_p->name_p, i);
        nh_core_sendLogMessage(node_p, NULL, message_p);
        memset(message_p, 0, NH_CSS_MAX_MESSAGE);

        if (nh_css_getStyleRule(Rule_p)) {
            NH_CORE_CHECK(nh_css_logStyleRule(node_p, message_p, indent_p, nh_css_getStyleRule(Rule_p)))
        }

        memset(message_p, 0, NH_CSS_MAX_MESSAGE);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_logDeclaredValue(
    char *node_p, char *message_p, char *indent_p, char *token_p, nh_css_DeclaredValue *Value_p)
{
    nh_core_sendLogMessage(node_p, NULL, "  {");

    sprintf(message_p, "    direct: %d", Value_p->direct);
    nh_core_sendLogMessage(node_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);

    sprintf(message_p, "    origin: %d", Value_p->direct);
    nh_core_sendLogMessage(node_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);

    nh_core_sendLogMessage(node_p, NULL, "    value:");

    for (int i = 0; i < Value_p->Declaration_p->ComponentValues.length; ++i) {
        NH_CORE_CHECK(nh_css_logComponentValue(node_p, &((nh_css_ComponentValue*)Value_p->Declaration_p->ComponentValues.p)[i], token_p, message_p, indent_p, 6))
    }

    nh_core_sendLogMessage(node_p, NULL, "  }");

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_logFilter(
    nh_css_LogContext *LogContext_p, nh_css_Filter *Filter_p)
{
    char message_p[NH_CSS_MAX_MESSAGE] = {'\0'};
    char node_p[1024] = {'\0'};
    char token_p[NH_CSS_MAX_TOKEN] = {'\0'};
    char indent_p[NH_CSS_MAX_INDENT] = {'\0'};

    sprintf(node_p, "%s%s:%s->Filter", LogContext_p->topLevelId_p, LogContext_p->nodeId_p, LogContext_p->indent_p);

    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i)
    {
        nh_core_List *List_p = Filter_p->DeclaredValueLists.pp[i];

        if (List_p) {
            nh_core_sendLogMessage(node_p, NULL, (char*)NH_CSS_PROPERTY_NAMES_PP[i]);
            for (int j = 0; j < List_p->size; ++j) {
                NH_CORE_CHECK(nh_css_logDeclaredValue(node_p, message_p, indent_p, token_p, List_p->pp[j]))
            }
        }
    }

    return NH_API_SUCCESS;
}

// LOG SPECIFIED/COMPUTED VALUES ===================================================================

static NH_API_RESULT nh_css_logValue(
    char *node_p, char *message_p, nh_css_Value *Value_p)
{
    if (!Value_p->Common.Origin_p) {
        sprintf(message_p, "  origin: initial");
    }
    else if (Value_p->Common.Origin_p->direct) {
        sprintf(message_p, "  origin: style-attribute");
    }
    else {
        sprintf(message_p, "  origin: %s", NH_CSS_DECLARATION_ORIGIN_NAMES_PP[Value_p->Common.Origin_p->origin]);
    }

    nh_core_sendLogMessage(node_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);

    sprintf(message_p, "  scope: %d", Value_p->Common.scope);
    nh_core_sendLogMessage(node_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);

    while (Value_p)
    {
        sprintf(message_p, "    type: %s", NH_CSS_TYPE_NAMES_PP[Value_p->Common.type]);
        nh_core_sendLogMessage(node_p, NULL, message_p);
        memset(message_p, 0, NH_CSS_MAX_MESSAGE);
    
        if (Value_p->Common.type != NH_CSS_VALUE_UNDEFINED)
        {
            if (nh_css_isStringValue(Value_p)) {
                sprintf(message_p, "    value: \"%s\"", Value_p->String.p);
            }
            else {
                sprintf(message_p, "    value: %lf", Value_p->number);
            }
            nh_core_sendLogMessage(node_p, NULL, message_p);
            memset(message_p, 0, NH_CSS_MAX_MESSAGE);
        }

        Value_p = Value_p->Common.Next_p; 
    }

    nh_core_sendLogMessage(node_p, NULL, " ");

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_logSpecifiedValues(
    nh_css_LogContext *LogContext_p, nh_core_Array *SpecifiedValues_p)
{
    char message_p[NH_CSS_MAX_MESSAGE] = {'\0'};
    char node_p[1024] = {'\0'};
    char token_p[NH_CSS_MAX_TOKEN] = {'\0'};
    char indent_p[NH_CSS_MAX_INDENT] = {'\0'};

    sprintf(node_p, "%s%s:%s->Specified", LogContext_p->topLevelId_p, LogContext_p->nodeId_p, LogContext_p->indent_p);

    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i)
    {
        nh_css_Value *Value_p = &((nh_css_Value*)SpecifiedValues_p->p)[i];
        nh_core_sendLogMessage(node_p, NULL, (char*)NH_CSS_PROPERTY_NAMES_PP[i]);
        NH_CORE_CHECK(nh_css_logValue(node_p, message_p, Value_p))
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_logComputedValues(
    nh_css_LogContext *LogContext_p, nh_core_List *ComputedValues_p)
{
    char message_p[NH_CSS_MAX_MESSAGE] = {'\0'};
    char node_p[1024] = {'\0'};
    char token_p[NH_CSS_MAX_TOKEN] = {'\0'};
    char indent_p[NH_CSS_MAX_INDENT] = {'\0'};

    sprintf(node_p, "%s%s:%s->Computed", LogContext_p->topLevelId_p, LogContext_p->nodeId_p, LogContext_p->indent_p);

    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) {
        nh_core_sendLogMessage(node_p, NULL, (char*)NH_CSS_PROPERTY_NAMES_PP[i]);
        if (ComputedValues_p->pp[i]) {
            NH_CORE_CHECK(nh_css_logValue(node_p, message_p, ComputedValues_p->pp[i]))
        }
    }

    return NH_API_SUCCESS;
}

// SOURCE TREE ========================================================================================

static NH_API_RESULT nh_css_logSource(
    char *node_p, nh_css_Source *Source_p, char *message_p, char *indent_p, int depth)
{
    for (int i = 0; i < depth; ++i) {indent_p[i] = ' ';}

    if (Source_p->type == NH_CSS_SOURCE_TEXT_NODE) {
        sprintf(message_p, "%s%s", indent_p, "TextNode");
    }
    else {
        sprintf(message_p, "%s%s", indent_p, "Element");
    }

    if (Source_p->pseudo > NH_CSS_PSEUDO_ELEMENT_UNDEFINED) {
        switch (Source_p->pseudo) {
            case NH_CSS_PSEUDO_ELEMENT_MARKER :
                sprintf(message_p + strlen(message_p), " [Pseudo-Element:Marker]");
                break;
        }
    }

    nh_core_sendLogMessage(node_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);

    if (Source_p->type == NH_CSS_SOURCE_ELEMENT) {
        for (int i = 0; i < Source_p->Element.Children.size; ++i) {
            NH_CORE_CHECK(nh_css_logSource(node_p, Source_p->Element.Children.pp[i], message_p, indent_p, depth + 2))
        }
    }

    memset(indent_p, 0, NH_CSS_MAX_INDENT);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_logSourceTree(
    nh_css_Canvas *Canvas_p, nh_css_Source *Root_p)
{
    char message_p[NH_CSS_MAX_MESSAGE] = {'\0'};
    char indent_p[NH_CSS_MAX_INDENT] = {'\0'};
    char node_p[1024] = {'\0'};

    sprintf(node_p, "nh-css:Canvas:%p:SourceTree", Canvas_p);

    NH_CORE_CHECK(nh_css_logSource(node_p, Root_p, message_p, indent_p, 0))

    return NH_API_SUCCESS;
}

// BOX TREE ========================================================================================

static NH_API_RESULT nh_css_getBoxTreeNodeInfo(
    nh_css_BoxTreeNode *Node_p, char *info_p)
{
    if (Node_p->FormattingContexts_pp[0] && Node_p->FormattingContexts_pp[1]) {
        sprintf(info_p, Node_p->Source_p ? "%s %p (%s %p, %s %p)" : "%s %p <anonymous> (%s %p, %s %p)", nh_css_getBoxTreeNodeName(Node_p->type), Node_p, nh_css_inPrimaryFormattingContextName(Node_p->FormattingContexts_pp[0]->type), Node_p->FormattingContexts_pp[0], nh_css_inPrimaryFormattingContextName(Node_p->FormattingContexts_pp[1]->type), Node_p->FormattingContexts_pp[1]);
    }
    else if (Node_p->FormattingContexts_pp[0]) {
        sprintf(info_p, Node_p->Source_p ? "%s %p (%s %p)" : "%s %p <anonymous> (%s %p)", nh_css_getBoxTreeNodeName(Node_p->type), Node_p, nh_css_inPrimaryFormattingContextName(Node_p->FormattingContexts_pp[0]->type), Node_p->FormattingContexts_pp[0]);
    }
    else {
        sprintf(info_p, Node_p->Source_p ? "%s %p" : "%s %p <anonymous>", nh_css_getBoxTreeNodeName(Node_p->type), Node_p);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_logBoxTreeNode(
    char *node_p, nh_css_BoxTreeNode *Node_p, char *message_p, char *indent_p, int depth)
{
    for (int i = 0; i < depth; ++i) {indent_p[i] = ' ';}

    char info_p[512] = {0};
    NH_CORE_CHECK(nh_css_getBoxTreeNodeInfo(Node_p, info_p))

    if (Node_p->type == NH_CSS_BOX_TREE_NODE_TEXT_RUN) {
        nh_encoding_UTF8String UTF8 = nh_encoding_encodeUTF8(Node_p->Source_p->TextNode.String.p, Node_p->Source_p->TextNode.String.length);
        sprintf(message_p, "%s%s %s", indent_p, info_p, UTF8.p);
        nh_encoding_freeUTF8(&UTF8);
    }
    else {
        sprintf(message_p, "%s%s", indent_p, info_p);
    }

    nh_core_sendLogMessage(node_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);

//            nh_encoding_UTF8String UTF8 = nh_encoding_encodeUTF8(Fragment_p->text_p, Fragment_p->textLength);
//            sprintf(message_p, "%sText     : %s", indent_p, UTF8.p);
//            nh_core_sendLogMessage(node_p, NULL, message_p);
//            memset(message_p, 0, NH_CSS_MAX_MESSAGE);
//            nh_encoding_freeUTF8(&UTF8);

    for (int i = 0; i < Node_p->Children.size; ++i) {
        NH_CORE_CHECK(nh_css_logBoxTreeNode(node_p, Node_p->Children.pp[i], message_p, indent_p, depth + 2))

//        sprintf(message_p, "%sValues   : margin-top:%d,margin-right:%d,margin-bottom:%d,margin-left:%d,border-top-width:%d,border-right-width:%d,border-bottom-width:%d,border-left-width:%d,padding-top:%d,padding-right:%d,padding-bottom:%d,padding-left:%d,background-color:(%.2f,%.2f,%.2f,%.2f)", indent_p, Fragment_p->Values.marginTop, Fragment_p->Values.marginRight, Fragment_p->Values.marginBottom, Fragment_p->Values.marginLeft, Fragment_p->Values.borderTop, Fragment_p->Values.borderRight, Fragment_p->Values.borderBottom, Fragment_p->Values.borderLeft, Fragment_p->Values.paddingTop, Fragment_p->Values.paddingRight, Fragment_p->Values.paddingBottom, Fragment_p->Values.paddingLeft, Fragment_p->Values.BackgroundColor.r,  Fragment_p->Values.BackgroundColor.g,  Fragment_p->Values.BackgroundColor.b,  Fragment_p->Values.BackgroundColor.a);
//        nh_core_sendLogMessage(node_p, NULL, message_p);
//        memset(message_p, 0, NH_CSS_MAX_MESSAGE);
    }

    memset(indent_p, 0, NH_CSS_MAX_INDENT);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_logBoxTree(
    nh_css_Canvas *Canvas_p, nh_css_BoxTree Tree)
{
    char message_p[NH_CSS_MAX_MESSAGE] = {'\0'};
    char indent_p[NH_CSS_MAX_INDENT] = {'\0'};
    char node_p[1024] = {'\0'};

    sprintf(node_p, "nh-css:Canvas:%p:BoxTree", Canvas_p);

    NH_CORE_CHECK(nh_css_logBoxTreeNode(node_p, Tree.Root_p, message_p, indent_p, 0))

    return NH_API_SUCCESS;
}

// FRAGMENT TREE ===================================================================================

static NH_API_RESULT nh_css_logFragment(
    char *logId_p, nh_css_Fragment *Fragment_p, char *message_p, char *indent_p, int depth)
{
    for (int i = 0; i < depth; ++i) {indent_p[i] = ' ';}

    if (Fragment_p->Node_p->type == NH_CSS_BOX_TREE_NODE_ROOT_INLINE) {
        sprintf(message_p, "%sLINE BOX START", indent_p);
        nh_core_sendLogMessage(logId_p, NULL, message_p);
        memset(message_p, 0, NH_CSS_MAX_MESSAGE);
    }

    char node_p[512] = {0};
    NH_CORE_CHECK(nh_css_getBoxTreeNodeInfo(Fragment_p->Node_p, node_p))

    sprintf(message_p, Fragment_p->type == NH_CSS_FRAGMENT_TEXT ? "%sText Fragment %p - %s" : "%sBox Fragment %p - %s", indent_p, Fragment_p, node_p);
    nh_core_sendLogMessage(logId_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);

    sprintf(message_p, "%sPosition : x:%d,y:%d,z:%f", indent_p, Fragment_p->Block.Position.x, Fragment_p->Block.Position.y, Fragment_p->Block.depth);
    nh_core_sendLogMessage(logId_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);

    sprintf(message_p, "%sSize     : width:%d,height:%d", indent_p, Fragment_p->Block.Size.width, Fragment_p->Block.Size.height);
    nh_core_sendLogMessage(logId_p, NULL, message_p);
    memset(message_p, 0, NH_CSS_MAX_MESSAGE);

    if (Fragment_p->type == NH_CSS_FRAGMENT_BOX) {
        sprintf(
            message_p, 
            "%sValues   : margin-top:%d,margin-right:%d,margin-bottom:%d,margin-left:%d,border-top-width:%d,border-right-width:%d,border-bottom-width:%d,border-left-width:%d,padding-top:%d,padding-right:%d,padding-bottom:%d,padding-left:%d,background-color:(%.2f,%.2f,%.2f,%.2f),text-align-all:%d", 
            indent_p, 
            Fragment_p->Box.Values.marginTop,
            Fragment_p->Box.Values.marginRight,
            Fragment_p->Box.Values.marginBottom,
            Fragment_p->Box.Values.marginLeft,
            Fragment_p->Box.Values.borderTop,
            Fragment_p->Box.Values.borderRight,
            Fragment_p->Box.Values.borderBottom,
            Fragment_p->Box.Values.borderLeft,
            Fragment_p->Box.Values.paddingTop,
            Fragment_p->Box.Values.paddingRight,
            Fragment_p->Box.Values.paddingBottom,
            Fragment_p->Box.Values.paddingLeft,
            Fragment_p->Box.Values.BackgroundColor.r,
            Fragment_p->Box.Values.BackgroundColor.g,
            Fragment_p->Box.Values.BackgroundColor.b,
            Fragment_p->Box.Values.BackgroundColor.a,
            Fragment_p->Box.Values.textAlignAll
        );
        nh_core_sendLogMessage(logId_p, NULL, message_p);
        memset(message_p, 0, NH_CSS_MAX_MESSAGE);
    }

    if (Fragment_p->type == NH_CSS_FRAGMENT_TEXT) {
        nh_encoding_UTF8String UTF8 = nh_encoding_encodeUTF8(Fragment_p->Text.text_p, Fragment_p->Text.length);

        sprintf(message_p, "%sText     : %s", indent_p, UTF8.p);
        nh_core_sendLogMessage(logId_p, NULL, message_p);
        memset(message_p, 0, NH_CSS_MAX_MESSAGE);

        sprintf(message_p, "%sValues   : color:(%.2f,%.2f,%.2f,%.2f)", 
            indent_p,
            Fragment_p->Text.Values.Color.r,
            Fragment_p->Text.Values.Color.g,
            Fragment_p->Text.Values.Color.b,
            Fragment_p->Text.Values.Color.a
        );
        nh_core_sendLogMessage(logId_p, NULL, message_p);
        memset(message_p, 0, NH_CSS_MAX_MESSAGE);

        nh_encoding_freeUTF8(&UTF8);
    }

    for (int i = 0; i < Fragment_p->Children.size; ++i) {
        NH_CORE_CHECK(nh_css_logFragment(logId_p, Fragment_p->Children.pp[i], message_p, indent_p, depth + 2))
    }

    memset(indent_p, 0, NH_CSS_MAX_INDENT);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_logFragmentTree(
    nh_css_Canvas *Canvas_p, nh_css_FragmentTree Tree)
{
    char message_p[NH_CSS_MAX_MESSAGE] = {'\0'};
    char indent_p[NH_CSS_MAX_INDENT] = {'\0'};
    char tmp_p[1024] = {'\0'};

    sprintf(tmp_p, "nh-css:Canvas:%p:FragmentTree", Canvas_p);

    NH_CORE_CHECK(nh_css_logFragment(tmp_p, Tree.Root_p, message_p, indent_p, 0))

    return NH_API_SUCCESS;
}
