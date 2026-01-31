// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// FILE DESCRIPTION ================================================================================

/**
 * https://www.w3.org/TR/selectors-4/
 */

// INCLUDES ========================================================================================

#include "Match.h"

#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/Attr.h"

#include "../../nh-encoding/Encodings/UTF8.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
// FUNCTIONS =======================================================================================

static bool nh_css_matchTypeSelector(
    nh_css_SelectorParseNode *TypeSelector_p, nh_ecmascript_Object *DOMElement_p)
{
    bool match = false;

    if (((nh_css_SelectorParseNode*)TypeSelector_p->Children.pp[0])->type == NH_CSS_SELECTOR_PARSE_NODE_WQ_NAME) 
    {
        nh_css_SelectorParseNode *WqName_p = TypeSelector_p->Children.pp[0];

        if (WqName_p->Children.size == 1) 
        {
            nh_css_Token *IdentTok_p = ((nh_css_SelectorParseNode*)WqName_p->Children.pp[0])->Token_p;
            nh_encoding_UTF8String Identifier = nh_encoding_encodeUTF8(IdentTok_p->Other.Value.p, IdentTok_p->Other.Value.length);
            nh_encoding_UTF8String *LocalName_p = nh_dom_getLocalName(DOMElement_p);
            if (!strcmp(LocalName_p->p, Identifier.p)) {match = true;}
            nh_encoding_freeUTF8(&Identifier);
        }
    }
 
    if (((nh_css_SelectorParseNode*)TypeSelector_p->Children.pp[0])->type == NH_CSS_SELECTOR_PARSE_NODE_NS_PREFIX) 
    {
        // TODO
    }

    if (((nh_css_SelectorParseNode*)TypeSelector_p->Children.pp[0])->Token_p)
    {
        nh_css_Token *Token_p = ((nh_css_SelectorParseNode*)TypeSelector_p->Children.pp[0])->Token_p;
        if (Token_p->type == NH_CSS_TOKEN_DELIM && Token_p->Delim.value == 0x2A) {
            // universal selector (*)
           match = true;
        } 
    }

    return match;
}

static bool nh_css_matchIdSelector(
    nh_css_SelectorParseNode *IdSelector_p, nh_ecmascript_Object *DOMElement_p)
{
    bool match = false;

    nh_css_Token *HashTok_p = ((nh_css_SelectorParseNode*)IdSelector_p->Children.pp[0])->Token_p;
    nh_encoding_UTF8String Identifier = nh_encoding_encodeUTF8(HashTok_p->Hash.Value.p, HashTok_p->Hash.Value.length);

    nh_ecmascript_Object *Attr_p = nh_dom_getAttrByLocalName(DOMElement_p, "id");
    if (Attr_p) {
        nh_encoding_UTF8String *Value_p = nh_dom_getAttrValue(Attr_p);
        if (!strcmp(Value_p->p, Identifier.p)) {
            match = true;
        }
    }

    nh_encoding_freeUTF8(&Identifier);
 
    return match;
}

static bool nh_css_matchClassSelector(
    nh_css_SelectorParseNode *ClassSelector_p, nh_ecmascript_Object *DOMElement_p)
{
    bool match = false;

    nh_css_Token *IdentTok_p = ((nh_css_SelectorParseNode*)ClassSelector_p->Children.pp[1])->Token_p;
    nh_encoding_UTF8String Identifier = nh_encoding_encodeUTF8(IdentTok_p->Other.Value.p, IdentTok_p->Other.Value.length);

    nh_ecmascript_Object *Attr_p = nh_dom_getAttrByLocalName(DOMElement_p, "class");
    if (Attr_p) {
        nh_encoding_UTF8String *Value_p = nh_dom_getAttrValue(Attr_p);
        if (!strcmp(Value_p->p, Identifier.p)) {
            match = true;
        }
    }

    nh_encoding_freeUTF8(&Identifier);
 
    return match;
}

static bool nh_css_matchSubclassSelector(
    nh_css_SelectorParseNode *SubclassSelector_p, nh_ecmascript_Object *DOMElement_p)
{
    bool match = false;

    switch (((nh_css_SelectorParseNode*)SubclassSelector_p->Children.pp[0])->type)
    {
        case NH_CSS_SELECTOR_PARSE_NODE_CLASS_SELECTOR :
            match = nh_css_matchClassSelector(SubclassSelector_p->Children.pp[0], DOMElement_p);
            break;
        case NH_CSS_SELECTOR_PARSE_NODE_ID_SELECTOR :
            match = nh_css_matchIdSelector(SubclassSelector_p->Children.pp[0], DOMElement_p);
            break;
    }
 
    return match;
}

static bool nh_css_matchCompoundSelector(
    nh_css_SelectorParseNode *CompoundSelector_p, nh_ecmascript_Object *DOMElement_p)
{
    bool match = false;

    for (int i = 0; i < CompoundSelector_p->Children.size; ++i)
    {
        switch (((nh_css_SelectorParseNode*)CompoundSelector_p->Children.pp[i])->type)
        {
            case NH_CSS_SELECTOR_PARSE_NODE_TYPE_SELECTOR :
                match = nh_css_matchTypeSelector(CompoundSelector_p->Children.pp[i], DOMElement_p);
                break;
            case NH_CSS_SELECTOR_PARSE_NODE_SUBCLASS_SELECTOR :
                match = nh_css_matchSubclassSelector(CompoundSelector_p->Children.pp[i], DOMElement_p);
                break;
            case NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_ELEMENT_SELECTOR :
                puts("TODO NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_ELEMENT_SELECTOR"); 
                exit(0);
                break;
            case NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_CLASS_SELECTOR :
                puts("TODO NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_CLASS_SELECTOR"); 
                exit(0);
                break;
        }

        if (!match) {break;}
    }

    return match;
}

static bool nh_css_matchComplexSelector(
    nh_css_SelectorParseNode *ComplexSelector_p, nh_ecmascript_Object *DOMElement_p)
{
// TODO

    nh_css_SelectorParseNode *CompoundSelector_p = ComplexSelector_p->Children.pp[0];
    if (nh_css_matchCompoundSelector(CompoundSelector_p, DOMElement_p)) {return true;}

    return false;
}

static bool nh_css_matchComplexSelectorList(
    nh_css_SelectorParseNode *ComplexSelectorList_p, nh_ecmascript_Object *DOMElement_p)
{
    for (int i = 0; i < ComplexSelectorList_p->Children.size; ++i) 
    {
        nh_css_SelectorParseNode *ComplexSelector_p = ComplexSelectorList_p->Children.pp[i];
        bool match = nh_css_matchComplexSelector(ComplexSelector_p, DOMElement_p);
        if (match) {return true;}
    }

    return false;
}

bool nh_css_matchSelectors(
    nh_css_SelectorParseNode *Selectors_p, nh_ecmascript_Object *DOMElement_p)
{
    if (Selectors_p->type != NH_CSS_SELECTOR_PARSE_NODE_SELECTOR_LIST) {return false;}
    if (Selectors_p->Children.size != 1) {return false;}

    nh_css_SelectorParseNode *ComplexSelectorList_p = Selectors_p->Children.pp[0];

    return nh_css_matchComplexSelectorList(ComplexSelectorList_p, DOMElement_p);
}
