// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "SelectorParser.h"
#include "ComponentParser.h"
#include "Tokenizer.h"

#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
// FUNCTIONS =======================================================================================

const char *nh_css_getSelectorParseNodeName(
    nh_css_SelectorParseNode *Node_p)
{
    static const char *parseNodeNames_pp[] =
    {
        "<terminal>",
        "<selector-list>",
        "<complex-selector-list>",
        "<compound-selector-list>",
        "<simple-selector-list>",
        "<relative-selector-list>",
        "<complex-selector>",
        "<relative-selector>",
        "<compound-selector>",
        "<simple-selector>",
        "<combinator>",
        "<type-selector>",
        "<ns-prefix>",
        "<wq-name>",
        "<subclass-selector>",
        "<id-selector>",
        "<class-selector>",
        "<attribute-selector>",
        "<attr-matcher>",
        "<attr-modifier>",
        "<pseudo-class-selector>",
        "<pseudo-element-selector>",
    };
    
    return parseNodeNames_pp[Node_p->type];
}

typedef struct nh_css_SelectorParser {
    nh_css_Token **Tokens_pp;
    unsigned long long unparsed;
} nh_css_SelectorParser;

static nh_css_SelectorParser nh_css_initSelectorParser(
    nh_core_List Tokens)
{
    nh_css_SelectorParser Parser;
    Parser.Tokens_pp = (nh_css_Token**)Tokens.pp;

    return Parser;
}

static nh_css_SelectorParseNode *nh_css_allocateNonTerminal(
    NH_CSS_SELECTOR_PARSE_NODE type)
{
    nh_css_SelectorParseNode *Node_p = nh_core_allocate(sizeof(nh_css_SelectorParseNode));
    NH_CORE_CHECK_MEM_2(NULL, Node_p)

    Node_p->type = type;
    Node_p->Children = nh_core_initList(8);
    Node_p->Token_p = NULL;

    return Node_p;
}

static nh_css_SelectorParseNode *nh_css_appendTerminal(
    nh_css_SelectorParseNode *NonTerminal_p, nh_css_Token *Token_p)
{
    nh_css_SelectorParseNode *Terminal_p = nh_core_allocate(sizeof(nh_css_SelectorParseNode));
    NH_CORE_CHECK_MEM_2(NULL, Terminal_p)

    Terminal_p->type = NH_CSS_SELECTOR_PARSE_NODE_TERMINAL;
    Terminal_p->Children = nh_core_initList(8);
    Terminal_p->Token_p = Token_p;

    nh_core_appendToList(&NonTerminal_p->Children, Terminal_p);

    return Terminal_p;
}

static nh_css_SelectorParseNode *nh_css_parseCombinator(
    nh_css_SelectorParser *Parser_p)
{
    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_DELIM) 
    {
        if (Parser_p->Tokens_pp[0]->Delim.value == '>' || Parser_p->Tokens_pp[0]->Delim.value == '+' || Parser_p->Tokens_pp[0]->Delim.value == '~') 
        {
            nh_css_SelectorParseNode *Combinator_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_COMBINATOR);
            NH_CORE_CHECK_MEM_2(NULL, Combinator_p)
            NH_CORE_CHECK_NULL_2(NULL, nh_css_appendTerminal(Combinator_p, Parser_p->Tokens_pp[0]))
            Parser_p->Tokens_pp = &Parser_p->Tokens_pp[1];
            return Combinator_p;
        }
    }

    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_WHITESPACE) 
    {
        nh_css_SelectorParseNode *Combinator_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_COMBINATOR);
        NH_CORE_CHECK_MEM_2(NULL, Combinator_p)
        NH_CORE_CHECK_NULL_2(NULL, nh_css_appendTerminal(Combinator_p, Parser_p->Tokens_pp[0]))
        Parser_p->Tokens_pp = &Parser_p->Tokens_pp[1];
        return Combinator_p;
    }
 
    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseNsPrefix(
    nh_css_SelectorParser *Parser_p)
{
    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_EOF) {return NULL;}

    nh_css_SelectorParser Local = *Parser_p;

    nh_css_Token *Prev_p = NULL;
    if (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_IDENT) {
        Prev_p = Local.Tokens_pp[0];
        Local.Tokens_pp = &Local.Tokens_pp[1];
    }
    else if (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_DELIM && Local.Tokens_pp[0]->Delim.value == '*') {
        Prev_p = Local.Tokens_pp[0];
        Local.Tokens_pp = &Local.Tokens_pp[1];
    }

    if (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_DELIM && Local.Tokens_pp[0]->Delim.value == '|') 
    {
        nh_css_SelectorParseNode *NsPrefix_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_NS_PREFIX);
        NH_CORE_CHECK_MEM_2(NULL, NsPrefix_p)     
        if (Prev_p) {NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(NsPrefix_p, Prev_p))}
        NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(NsPrefix_p, Local.Tokens_pp[0]))
        Local.Tokens_pp = &Local.Tokens_pp[1];
        *Parser_p = Local;
        return NsPrefix_p;
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseWqName(
    nh_css_SelectorParser *Parser_p)
{
    nh_css_SelectorParser Local = *Parser_p;
    nh_css_SelectorParseNode *NsPrefix_p = nh_css_parseNsPrefix(&Local);

    if (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_IDENT) 
    {
        nh_css_SelectorParseNode *WqName_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_WQ_NAME);
        NH_CORE_CHECK_MEM_2(NULL, WqName_p)     

        if (NsPrefix_p) {nh_core_appendToList(&WqName_p->Children, NsPrefix_p);}
        NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(WqName_p, Local.Tokens_pp[0]))

        Local.Tokens_pp = &Local.Tokens_pp[1];
        *Parser_p = Local;

        return WqName_p;
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseAttrMatcher(
    nh_css_SelectorParser *Parser_p)
{
    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_DELIM) 
    {
        if (Parser_p->Tokens_pp[0]->Delim.value == '~' 
        ||  Parser_p->Tokens_pp[0]->Delim.value == '|' 
        ||  Parser_p->Tokens_pp[0]->Delim.value == '^'
        ||  Parser_p->Tokens_pp[0]->Delim.value == '$' 
        ||  Parser_p->Tokens_pp[0]->Delim.value == '*')
        {
            if (Parser_p->Tokens_pp[1]->type == NH_CSS_TOKEN_DELIM && Parser_p->Tokens_pp[1]->Delim.value == '=') 
            {
                nh_css_SelectorParseNode *AttrMatcher_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_ATTR_MATCHER);
                NH_CORE_CHECK_MEM_2(NULL, AttrMatcher_p)
                NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(AttrMatcher_p, Parser_p->Tokens_pp[0]))
                NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(AttrMatcher_p, Parser_p->Tokens_pp[1]))
                Parser_p->Tokens_pp = &Parser_p->Tokens_pp[2];
                return AttrMatcher_p;
            }
        }
        if (Parser_p->Tokens_pp[0]->Delim.value == '=') 
        {
            nh_css_SelectorParseNode *AttrMatcher_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_ATTR_MATCHER);
            NH_CORE_CHECK_MEM_2(NULL, AttrMatcher_p)
            NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(AttrMatcher_p, Parser_p->Tokens_pp[0]))
            Parser_p->Tokens_pp = &Parser_p->Tokens_pp[1];
            return AttrMatcher_p;
        }
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseAttrModifier(
    nh_css_SelectorParser *Parser_p)
{
    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_IDENT && Parser_p->Tokens_pp[0]->Other.Value.length == 1) 
    {
        nh_css_SelectorParseNode *AttrModifier_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_ATTR_MODIFIER);
        NH_CORE_CHECK_MEM_2(NULL, AttrModifier_p)
        NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(AttrModifier_p, Parser_p->Tokens_pp[0]))
        Parser_p->Tokens_pp = &Parser_p->Tokens_pp[1];
        return AttrModifier_p;
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseAttributeSelector(
    nh_css_SelectorParser *Parser_p)
{
    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_LEFT_SQUARE_BRACKET) 
    {
        nh_css_SelectorParser Local = *Parser_p;
        Local.Tokens_pp = &Local.Tokens_pp[1];

        nh_css_SelectorParseNode *WqName_p = nh_css_parseWqName(&Local);
        if (WqName_p) 
        {
            if (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_RIGHT_SQUARE_BRACKET) 
            {
                nh_css_SelectorParseNode *AttributeSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_ATTRIBUTE_SELECTOR);
                NH_CORE_CHECK_MEM_2(NULL, AttributeSelector_p)
                NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(AttributeSelector_p, Parser_p->Tokens_pp[0]))
                nh_core_appendToList(&AttributeSelector_p->Children, WqName_p);
                NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(AttributeSelector_p, Local.Tokens_pp[0]))
                Parser_p->Tokens_pp = &Local.Tokens_pp[1];
                return AttributeSelector_p;
            }

            nh_css_SelectorParseNode *AttrMatcher_p = nh_css_parseAttrMatcher(&Local);
            if (AttrMatcher_p) 
            {
                if (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_STRING || Local.Tokens_pp[0]->type == NH_CSS_TOKEN_IDENT) 
                {
                    nh_css_Token *StringOrIdent_p = Local.Tokens_pp[0];
                    Local.Tokens_pp = &Local.Tokens_pp[1];

                    while (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_WHITESPACE) {Local.Tokens_pp = &Local.Tokens_pp[1];}
                    nh_css_SelectorParseNode *AttrModifier_p = nh_css_parseAttrModifier(&Local);

                    if (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_RIGHT_SQUARE_BRACKET) 
                    {
                        nh_css_SelectorParseNode *AttributeSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_ATTRIBUTE_SELECTOR);
                        NH_CORE_CHECK_MEM_2(NULL, AttributeSelector_p)
                        NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(AttributeSelector_p, Parser_p->Tokens_pp[0]))
                        nh_core_appendToList(&AttributeSelector_p->Children, WqName_p);
                        nh_core_appendToList(&AttributeSelector_p->Children, AttrMatcher_p);
                        NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(AttributeSelector_p, StringOrIdent_p))
                        if (AttrModifier_p) {
                            nh_core_appendToList(&AttributeSelector_p->Children, AttrModifier_p);
                        }
                        NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(AttributeSelector_p, Local.Tokens_pp[0]))
                        Parser_p->Tokens_pp = &Local.Tokens_pp[1];
                        return AttributeSelector_p;
                    }
                }
            }
        }
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parsePseudoClassSelector(
    nh_css_SelectorParser *Parser_p)
{
    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_COLON) 
    {
        if (Parser_p->Tokens_pp[1]->type == NH_CSS_TOKEN_IDENT) 
        {
            nh_css_SelectorParseNode *PseudoClassSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_CLASS_SELECTOR);
            NH_CORE_CHECK_MEM_2(NULL, PseudoClassSelector_p)
            NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(PseudoClassSelector_p, Parser_p->Tokens_pp[0]))
            NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(PseudoClassSelector_p, Parser_p->Tokens_pp[1]))
            Parser_p->Tokens_pp = &Parser_p->Tokens_pp[2];
            return PseudoClassSelector_p;
        }
        else if (Parser_p->Tokens_pp[1]->type == NH_CSS_TOKEN_FUNCTION)
        {
            nh_css_SelectorParser Local;
            Local.Tokens_pp = &Parser_p->Tokens_pp[2];

            bool valid = true;
            while (Local.Tokens_pp[0]->type != NH_CSS_TOKEN_RIGHT_PARENTHESIS) {
                Local.Tokens_pp = &Local.Tokens_pp[1];
                if (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_EOF) {
                    valid = false;
                    break;
                } 
            }

            if (valid) 
            {
                nh_css_SelectorParseNode *PseudoClassSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_CLASS_SELECTOR);
                NH_CORE_CHECK_MEM_2(NULL, PseudoClassSelector_p)
                NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(PseudoClassSelector_p, Parser_p->Tokens_pp[0]))
                NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(PseudoClassSelector_p, Parser_p->Tokens_pp[1]))
                Parser_p->Tokens_pp = &Parser_p->Tokens_pp[2];
                while (Parser_p->Tokens_pp[0]->type != NH_CSS_TOKEN_RIGHT_PARENTHESIS) {
                    NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(PseudoClassSelector_p, Parser_p->Tokens_pp[0]))
                    Parser_p->Tokens_pp = &Parser_p->Tokens_pp[1];
                }
                NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(PseudoClassSelector_p, Parser_p->Tokens_pp[0]))
                Parser_p->Tokens_pp = &Parser_p->Tokens_pp[1];
                return PseudoClassSelector_p;
            }
        }
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parsePseudoElementSelector(
    nh_css_SelectorParser *Parser_p)
{
    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_COLON) 
    {
        nh_css_SelectorParser Local = *Parser_p;
        Local.Tokens_pp = &Local.Tokens_pp[1];

        nh_css_SelectorParseNode *PseudoClassSelector_p = nh_css_parsePseudoClassSelector(&Local);

        if (PseudoClassSelector_p) 
        {
            nh_css_SelectorParseNode *PseudoElementSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_ELEMENT_SELECTOR);
            NH_CORE_CHECK_MEM_2(NULL, PseudoElementSelector_p)
            NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(PseudoElementSelector_p, Parser_p->Tokens_pp[0]))
            nh_core_appendToList(&PseudoElementSelector_p->Children, PseudoClassSelector_p);
            *Parser_p = Local;
            return PseudoElementSelector_p;
        }
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseTypeSelector(
    nh_css_SelectorParser *Parser_p)
{
    nh_css_SelectorParser Local = *Parser_p;
    nh_css_SelectorParseNode *WqName_p = nh_css_parseWqName(&Local);

    if (WqName_p) 
    {
        nh_css_SelectorParseNode *TypeSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_TYPE_SELECTOR);
        NH_CORE_CHECK_MEM_2(NULL, TypeSelector_p)
        nh_core_appendToList(&TypeSelector_p->Children, WqName_p);
        *Parser_p = Local;
        return TypeSelector_p;
    }
    else {
        nh_css_SelectorParseNode *NsPrefix_p = nh_css_parseNsPrefix(&Local);
        if (Local.Tokens_pp[0]->type == NH_CSS_TOKEN_DELIM && Local.Tokens_pp[0]->Delim.value == '*') 
        {
            nh_css_SelectorParseNode *TypeSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_TYPE_SELECTOR);
            NH_CORE_CHECK_MEM_2(NULL, TypeSelector_p)
            if (NsPrefix_p) {
                nh_core_appendToList(&TypeSelector_p->Children, NsPrefix_p);
            }
            NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(TypeSelector_p, Local.Tokens_pp[0]))
            Local.Tokens_pp = &Local.Tokens_pp[1];
            *Parser_p = Local;
            return TypeSelector_p;
        }
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseClassSelector(
    nh_css_SelectorParser *Parser_p)
{
    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_DELIM && Parser_p->Tokens_pp[0]->Delim.value == '.') 
    {
        if (Parser_p->Tokens_pp[1]->type == NH_CSS_TOKEN_IDENT)
        {
            nh_css_SelectorParseNode *ClassSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_CLASS_SELECTOR);
            NH_CORE_CHECK_MEM_2(NULL, ClassSelector_p)
            NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(ClassSelector_p, Parser_p->Tokens_pp[0]))
            NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(ClassSelector_p, Parser_p->Tokens_pp[1]))
            Parser_p->Tokens_pp = &Parser_p->Tokens_pp[2];
            return ClassSelector_p;
        }
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseIdSelector(
    nh_css_SelectorParser *Parser_p)
{
    if (Parser_p->Tokens_pp[0]->type == NH_CSS_TOKEN_HASH) 
    {
        nh_css_SelectorParseNode *IdSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_ID_SELECTOR);
        NH_CORE_CHECK_MEM_2(NULL, IdSelector_p)
        NH_CORE_CHECK_MEM_2(NULL, nh_css_appendTerminal(IdSelector_p, Parser_p->Tokens_pp[0]))
        Parser_p->Tokens_pp = &Parser_p->Tokens_pp[1];
        return IdSelector_p;
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseSubclassSelector(
    nh_css_SelectorParser *Parser_p)
{
    nh_css_SelectorParseNode *IdSelector_p = nh_css_parseIdSelector(Parser_p);

    if (IdSelector_p) {
        nh_css_SelectorParseNode *SubclassSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_SUBCLASS_SELECTOR);
        NH_CORE_CHECK_MEM_2(NULL, SubclassSelector_p)
        nh_core_appendToList(&SubclassSelector_p->Children, IdSelector_p);
        return SubclassSelector_p;
    }

    nh_css_SelectorParseNode *ClassSelector_p = nh_css_parseClassSelector(Parser_p);

    if (ClassSelector_p) {
        nh_css_SelectorParseNode *SubclassSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_SUBCLASS_SELECTOR);
        NH_CORE_CHECK_MEM_2(NULL, SubclassSelector_p)
        nh_core_appendToList(&SubclassSelector_p->Children, ClassSelector_p);
        return SubclassSelector_p;
    }

    nh_css_SelectorParseNode *AttributeSelector_p = nh_css_parseAttributeSelector(Parser_p);

    if (AttributeSelector_p) {
        nh_css_SelectorParseNode *SubclassSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_SUBCLASS_SELECTOR);
        NH_CORE_CHECK_MEM_2(NULL, SubclassSelector_p)
        nh_core_appendToList(&SubclassSelector_p->Children, AttributeSelector_p);
        return SubclassSelector_p;
    }

    nh_css_SelectorParseNode *PseudoClassSelector_p = nh_css_parsePseudoClassSelector(Parser_p);

    if (PseudoClassSelector_p) {
        nh_css_SelectorParseNode *SubclassSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_SUBCLASS_SELECTOR);
        NH_CORE_CHECK_MEM_2(NULL, SubclassSelector_p)
        nh_core_appendToList(&SubclassSelector_p->Children, PseudoClassSelector_p);
        return SubclassSelector_p;
    }

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseCompoundSelector(
    nh_css_SelectorParser *Parser_p)
{
    nh_css_SelectorParser Local = *Parser_p;

    nh_css_SelectorParseNode *TypeSelector_p = nh_css_parseTypeSelector(&Local);

    nh_core_List SubclassSelectors = nh_core_initList(8);

    while (Local.Tokens_pp[0]->type != NH_CSS_TOKEN_EOF) {
        nh_css_SelectorParseNode *SubclassSelector_p = nh_css_parseSubclassSelector(&Local);
        if (!SubclassSelector_p) {break;}
        nh_core_appendToList(&SubclassSelectors, SubclassSelector_p);
    }

    nh_core_List PseudoSelectors = nh_core_initList(8);

    while (Local.Tokens_pp[0]->type != NH_CSS_TOKEN_EOF) 
    {
        nh_css_SelectorParseNode *PseudoElementSelector_p = nh_css_parsePseudoElementSelector(&Local);
        if (!PseudoElementSelector_p) {break;}

        nh_core_appendToList(&PseudoSelectors, PseudoElementSelector_p);

        while (Local.Tokens_pp[0]->type != NH_CSS_TOKEN_EOF)
        {
            nh_css_SelectorParseNode *PseudoClassSelector_p = nh_css_parsePseudoClassSelector(&Local);
            if (!PseudoClassSelector_p) {break;}
            nh_core_appendToList(&PseudoSelectors, PseudoClassSelector_p);
        }
    }

    if (TypeSelector_p || SubclassSelectors.size || PseudoSelectors.size) 
    {
         nh_css_SelectorParseNode *CompoundSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_COMPOUND_SELECTOR);
         NH_CORE_CHECK_MEM_2(NULL, CompoundSelector_p)     

         if (TypeSelector_p) {
             nh_core_appendToList(&CompoundSelector_p->Children, TypeSelector_p);
         }
         for (int i = 0; i < SubclassSelectors.size; ++i) {
             nh_core_appendToList(&CompoundSelector_p->Children, SubclassSelectors.pp[i]);
         }
         for (int i = 0; i < PseudoSelectors.size; ++i) {
             nh_core_appendToList(&CompoundSelector_p->Children, PseudoSelectors.pp[i]);
         }

         nh_core_freeList(&SubclassSelectors, false);
         nh_core_freeList(&PseudoSelectors, false);

         *Parser_p = Local;
         return CompoundSelector_p;
    }

    nh_core_freeList(&SubclassSelectors, false);
    nh_core_freeList(&PseudoSelectors, false);

    return NULL;
}

static nh_css_SelectorParseNode *nh_css_parseComplexSelector(
    nh_css_SelectorParser *Parser_p)
{
    nh_css_SelectorParseNode *CompoundSelector_p = nh_css_parseCompoundSelector(Parser_p);
    NH_CORE_CHECK_NULL_2(NULL, CompoundSelector_p)

    nh_css_SelectorParseNode *ComplexSelector_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_COMPLEX_SELECTOR);
    NH_CORE_CHECK_MEM_2(NULL, ComplexSelector_p)

    nh_core_appendToList(&ComplexSelector_p->Children, CompoundSelector_p);
    nh_css_SelectorParser Local = *Parser_p;

    while (Local.Tokens_pp[0]->type != NH_CSS_TOKEN_EOF)
    {
        nh_css_SelectorParseNode *Combinator_p = nh_css_parseCombinator(&Local);
        nh_css_SelectorParseNode *CompoundSelector_p = nh_css_parseCompoundSelector(&Local);

        if (CompoundSelector_p) 
        {
            if (Combinator_p) {
                nh_core_appendToList(&ComplexSelector_p->Children, Combinator_p);
            }
            nh_core_appendToList(&ComplexSelector_p->Children, CompoundSelector_p);
        }
        else {break;}
    }

    *Parser_p = Local;

    return ComplexSelector_p;
}

static nh_css_SelectorParseNode *nh_css_parseComplexSelectorList(
    nh_css_SelectorParser *Parser_p)
{
    nh_css_SelectorParseNode *ComplexSelectorList_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_COMPLEX_SELECTOR_LIST);
    NH_CORE_CHECK_MEM_2(NULL, ComplexSelectorList_p)

    while (*Parser_p->Tokens_pp && Parser_p->Tokens_pp[0]->type != NH_CSS_TOKEN_EOF)
    {
        nh_css_SelectorParseNode *ComplexSelector_p = nh_css_parseComplexSelector(Parser_p);
        if (ComplexSelector_p) { 
            nh_core_appendToList(&ComplexSelectorList_p->Children, ComplexSelector_p);
        }
        else if (*Parser_p->Tokens_pp && Parser_p->Tokens_pp[0]->type != NH_CSS_TOKEN_EOF) {Parser_p->Tokens_pp = &Parser_p->Tokens_pp[1];}
    }

    return ComplexSelectorList_p;
}

nh_css_SelectorParseNode *nh_css_parseSelectorList(
    nh_core_Array ComponentValues)
{
    nh_core_List Tokens = nh_css_getTokensFromComponentValues(&ComponentValues);

    nh_css_Token EndToken;
    EndToken.type = NH_CSS_TOKEN_EOF;
    nh_core_appendToList(&Tokens, &EndToken);

    nh_css_SelectorParser Parser = nh_css_initSelectorParser(Tokens);

    nh_css_SelectorParseNode *SelectorList_p = nh_css_allocateNonTerminal(NH_CSS_SELECTOR_PARSE_NODE_SELECTOR_LIST);
    NH_CORE_CHECK_MEM_2(NULL, SelectorList_p)

    nh_css_SelectorParseNode *ComplexSelectorList_p = nh_css_parseComplexSelectorList(&Parser);
    NH_CORE_CHECK_MEM_2(NULL, ComplexSelectorList_p)

    nh_core_appendToList(&SelectorList_p->Children, ComplexSelectorList_p);

    nh_core_freeList(&Tokens, false);

    return SelectorList_p;
}
