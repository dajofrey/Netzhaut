// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Match.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/List.h"

#include "../../nhdom/Interfaces/Element.h"
#include "../../nhdom/Interfaces/Attr.h"

#include "../../nhencoding/Encodings/UTF8.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
// MATCH ===========================================================================================

static NH_BOOL nh_css_matchTypeSelector(
    nh_css_SelectorParseNode *TypeSelector_p, nh_dom_Element *Element_p)
{
NH_CSS_BEGIN()

    NH_BOOL match = NH_FALSE;

    if (((nh_css_SelectorParseNode*)TypeSelector_p->Children.pp[0])->type == NH_CSS_SELECTOR_PARSE_NODE_WQ_NAME) 
    {
        nh_css_SelectorParseNode *WqName_p = TypeSelector_p->Children.pp[0];

        if (WqName_p->Children.size == 1) 
        {
            nh_css_Token *IdentTok_p = ((nh_css_SelectorParseNode*)WqName_p->Children.pp[0])->Token_p;
            nh_encoding_UTF8String Identifier = nh_encoding_encodeUTF8(IdentTok_p->Other.Value.p, IdentTok_p->Other.Value.length);
            nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(Element_p);
            if (!strcmp(LocalName_p->p, Identifier.p)) {match = NH_TRUE;}
            nh_encoding_freeUTF8(&Identifier);
        }
    }
 
NH_CSS_END(match)
}

static NH_BOOL nh_css_matchIdSelector(
    nh_css_SelectorParseNode *IdSelector_p, nh_dom_Element *Element_p)
{
NH_CSS_BEGIN()

    NH_BOOL match = NH_FALSE;

    nh_css_Token *HashTok_p = ((nh_css_SelectorParseNode*)IdSelector_p->Children.pp[0])->Token_p;
    nh_encoding_UTF8String Identifier = nh_encoding_encodeUTF8(HashTok_p->Hash.Value.p, HashTok_p->Hash.Value.length);

    nh_dom_Attr *Attr_p = nh_dom_getAttrByLocalName(Element_p, "id");
    if (Attr_p) {
        nh_webidl_DOMString *Value_p = nh_dom_getAttrValue(Attr_p);
        if (!strcmp(Value_p->p, Identifier.p)) {
            match = NH_TRUE;
        }
    }

    nh_encoding_freeUTF8(&Identifier);
 
NH_CSS_END(match)
}

static NH_BOOL nh_css_matchClassSelector(
    nh_css_SelectorParseNode *ClassSelector_p, nh_dom_Element *Element_p)
{
NH_CSS_BEGIN()

    NH_BOOL match = NH_FALSE;

    nh_css_Token *IdentTok_p = ((nh_css_SelectorParseNode*)ClassSelector_p->Children.pp[1])->Token_p;
    nh_encoding_UTF8String Identifier = nh_encoding_encodeUTF8(IdentTok_p->Other.Value.p, IdentTok_p->Other.Value.length);

    nh_dom_Attr *Attr_p = nh_dom_getAttrByLocalName(Element_p, "class");
    if (Attr_p) {
        nh_webidl_DOMString *Value_p = nh_dom_getAttrValue(Attr_p);
        if (!strcmp(Value_p->p, Identifier.p)) {
            match = NH_TRUE;
        }
    }

    nh_encoding_freeUTF8(&Identifier);
 
NH_CSS_END(match)
}

static NH_BOOL nh_css_matchSubclassSelector(
    nh_css_SelectorParseNode *SubclassSelector_p, nh_dom_Element *Element_p)
{
NH_CSS_BEGIN()

    NH_BOOL match = NH_FALSE;

    switch (((nh_css_SelectorParseNode*)SubclassSelector_p->Children.pp[0])->type)
    {
        case NH_CSS_SELECTOR_PARSE_NODE_CLASS_SELECTOR :
            match = nh_css_matchClassSelector(SubclassSelector_p->Children.pp[0], Element_p);
            break;
        case NH_CSS_SELECTOR_PARSE_NODE_ID_SELECTOR :
            match = nh_css_matchIdSelector(SubclassSelector_p->Children.pp[0], Element_p);
            break;
    }
 
NH_CSS_END(match)
}

static NH_BOOL nh_css_matchCompoundSelector(
    nh_css_SelectorParseNode *CompoundSelector_p, nh_dom_Element *Element_p)
{
NH_CSS_BEGIN()

// TODO

    NH_BOOL match = NH_FALSE;

    for (int i = 0; i < CompoundSelector_p->Children.size; ++i)
    {
        switch (((nh_css_SelectorParseNode*)CompoundSelector_p->Children.pp[i])->type)
        {
            case NH_CSS_SELECTOR_PARSE_NODE_TYPE_SELECTOR :
                match = nh_css_matchTypeSelector(CompoundSelector_p->Children.pp[i], Element_p);
                break;
            case NH_CSS_SELECTOR_PARSE_NODE_SUBCLASS_SELECTOR :
                match = nh_css_matchSubclassSelector(CompoundSelector_p->Children.pp[i], Element_p);
                break;
        }
    }

NH_CSS_END(match)
}

static NH_BOOL nh_css_matchComplexSelector(
    nh_css_SelectorParseNode *ComplexSelector_p, nh_dom_Element *Element_p)
{
NH_CSS_BEGIN()

// TODO

    nh_css_SelectorParseNode *CompoundSelector_p = ComplexSelector_p->Children.pp[0];
    if (nh_css_matchCompoundSelector(CompoundSelector_p, Element_p)) {NH_CSS_END(NH_TRUE)}

NH_CSS_END(NH_FALSE)
}

static NH_BOOL nh_css_matchComplexSelectorList(
    nh_css_SelectorParseNode *ComplexSelectorList_p, nh_dom_Element *Element_p)
{
NH_CSS_BEGIN()

    for (int i = 0; i < ComplexSelectorList_p->Children.size; ++i) 
    {
        nh_css_SelectorParseNode *ComplexSelector_p = ComplexSelectorList_p->Children.pp[i];
        NH_BOOL match = nh_css_matchComplexSelector(ComplexSelector_p, Element_p);
        if (match) {NH_CSS_END(NH_TRUE)}
    }

NH_CSS_END(NH_FALSE)
}

NH_BOOL nh_css_matchSelectors(
    nh_css_SelectorParseNode *Selectors_p, nh_dom_Element *Element_p)
{
NH_CSS_BEGIN()

    if (Selectors_p->type != NH_CSS_SELECTOR_PARSE_NODE_SELECTOR_LIST) {NH_CSS_END(NH_FALSE)}
    if (Selectors_p->Children.size != 1) {NH_CSS_END(NH_FALSE)}

    nh_css_SelectorParseNode *ComplexSelectorList_p = Selectors_p->Children.pp[0];

NH_CSS_END(nh_css_matchComplexSelectorList(ComplexSelectorList_p, Element_p))
}

