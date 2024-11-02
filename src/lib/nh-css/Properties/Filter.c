// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Filter.h"
#include "Properties.h"
#include "Match.h"

#include "../Interfaces/StyleSheetList.h"
#include "../Interfaces/StyleSheet.h"
#include "../Interfaces/RuleList.h"
#include "../Interfaces/StyleRule.h"
#include "../Interfaces/StyleDeclaration.h"

#include "../Common/IndexMap.h"
#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-dom/Interfaces/Attr.h"
#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/NamedNodeMap.h"
#include "../../nh-dom/Interfaces/Node.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
	
// DATA ============================================================================================

const char *NH_CSS_DECLARATION_ORIGIN_NAMES_PP[] =
{
    "author",
    "user",
    "useragent",
    "animation",
    "transition",
};

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_css_insertCandidate(
    nh_css_Filter *Filter_p, nh_css_Candidate *Value_p, NH_CSS_PROPERTY property)
{
    nh_core_List *List_p = Filter_p->CandidateLists.pp[property];
    if (!List_p) {
        List_p = nh_core_incrementArray(&Filter_p->CandidateListsArray);
        *List_p = nh_core_initList(8);
        Filter_p->CandidateLists.pp[property] = List_p;
    }
    nh_core_appendToList(List_p, Value_p);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_insertOrDropCandidate(
    nh_css_Filter *Filter_p, nh_css_Declaration *Declaration_p, void *CSSStyleRule_p)
{
    unsigned int *property_p = nh_core_getFromHashMap(&NH_CSS_INDEXMAP.Properties, Declaration_p->Name.p);

    if (property_p) 
    {
        nh_css_Candidate *Value_p = nh_core_incrementArray(&Filter_p->Candidates);
        NH_CORE_CHECK_MEM(Value_p)

        Value_p->Declaration_p = Declaration_p;
        Value_p->CSSStyleRule_p = CSSStyleRule_p;

        nh_css_insertCandidate(Filter_p, Value_p, *property_p);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_filterFromStyleSheet(
    nh_css_Filter *Filter_p, nh_dom_Element *Element_p, nh_webidl_Object *CSSStyleSheet_p)
{
    nh_core_List *Rules_p = nh_css_getRuleList(CSSStyleSheet_p)->internal_p;

    for (int i = 0; i < Rules_p->size; ++i) {
        nh_webidl_Object *CSSStyleRule_p = nh_css_getCSSStyleRule(Rules_p->pp[i]);
        if (CSSStyleRule_p && nh_css_matchSelectors(nh_css_getCSSStyleRuleSelectors(CSSStyleRule_p), Element_p)) {
            // Selector match successful, we found a rule that potentially applies to the element.
            nh_core_Array *Array_p = 
                nh_css_getStyleDeclarationData(nh_css_getStyleRuleDeclaration(CSSStyleRule_p));
            for (int j = 0; j < Array_p->length; ++j) {
                nh_css_Declaration *Declaration_p = &((nh_css_Declaration*)Array_p->p)[j];
                nh_css_insertOrDropCandidate(Filter_p, Declaration_p, CSSStyleRule_p);
            }
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_filterFromDefaultStyleSheet(
    nh_css_Filter *Filter_p, nh_dom_Element *Element_p)
{
    int oldLength = Filter_p->Candidates.length;
    nh_css_filterFromStyleSheet(Filter_p, Element_p, NH_CSS_DEFAULT_STYLE_SHEET_P);

    for (int i = oldLength; i < Filter_p->Candidates.length; ++i) {
        nh_css_Candidate *Value_p = &((nh_css_Candidate*)Filter_p->Candidates.p)[i];
        Value_p->origin = NH_CSS_DECLARATION_ORIGIN_USER_AGENT;
        Value_p->direct = false;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_filterFromAuthorStyleSheet(
    nh_css_Filter *Filter_p, nh_dom_Element *Element_p, nh_css_StyleSheetObject *StyleSheet_p)
{
    int oldLength = Filter_p->Candidates.length;
    nh_css_filterFromStyleSheet(Filter_p, Element_p, StyleSheet_p);

    for (int i = oldLength; i < Filter_p->Candidates.length; ++i) {
        nh_css_Candidate *Value_p = &((nh_css_Candidate*)Filter_p->Candidates.p)[i];
        Value_p->origin = NH_CSS_DECLARATION_ORIGIN_AUTHOR;
        Value_p->direct = false;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_filterFromStyleAttributes(
    nh_css_Filter *Filter_p, nh_dom_Element *Element_p)
{
    int oldLength = Filter_p->Candidates.length;

    nh_core_List *Attributes_p = nh_dom_getAttrList(nh_dom_getNamedNodeMap(Element_p));

    for (int i = 0; i < Attributes_p->size; ++i) 
    {
        nh_webidl_DOMString *LocalName_p = nh_dom_getAttrLocalName(Attributes_p->pp[i]);
        nh_webidl_DOMString *Value_p = nh_dom_getAttrValue(Attributes_p->pp[i]);

        if (!strcmp(LocalName_p->p, "style")) {
// TODO free
            nh_css_DeclarationData Data = nh_css_parseDeclarationsFromBytes(NULL, Value_p->p, Value_p->length);
            for (int d = 0; d < Data.Declarations.length; ++d) {
                nh_css_Declaration *Declaration_p = &((nh_css_Declaration*)Data.Declarations.p)[d];
                nh_css_insertOrDropCandidate(Filter_p, Declaration_p, NULL);
            }
        }
    }

    for (int i = oldLength; i < Filter_p->Candidates.length; ++i) {
        nh_css_Candidate *Value_p = &((nh_css_Candidate*)Filter_p->Candidates.p)[i];
        Value_p->origin = NH_CSS_DECLARATION_ORIGIN_AUTHOR;
        Value_p->direct = true;
    }

    return NH_API_SUCCESS;
}

/**
 * A filter consists of all declared values that match to the Element, for example via Selectors.
 * Values are gathered from default style-sheet, author style-sheets and inline style attributes.
 * Cascading will afterwards decide which filtered value to apply.
 */
nh_css_Filter nh_css_filter(
    nh_dom_Element *Element_p, nh_css_StyleSheetListObject *AuthorStyleSheets_p, nh_core_List UserStyleSheets)
{
    nh_css_Filter Filter;
    Filter.Candidates = nh_core_initArray(sizeof(nh_css_Candidate), 255);
    Filter.CandidateLists = nh_core_initList(NH_CSS_PROPERTY_COUNT);
    Filter.CandidateListsArray = nh_core_initArray(sizeof(nh_core_List), 16);

    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) {
        nh_core_appendToList(&Filter.CandidateLists, NULL);
    }

    nh_css_filterFromDefaultStyleSheet(&Filter, Element_p);
    nh_css_filterFromStyleAttributes(&Filter, Element_p);

    nh_core_List *List_p = nh_css_getStyleSheetListData(AuthorStyleSheets_p);
    for (int i = 0; i < List_p->size; ++i) {
        nh_css_filterFromAuthorStyleSheet(&Filter, Element_p, List_p->pp[i]);
    }

    return Filter;
}
