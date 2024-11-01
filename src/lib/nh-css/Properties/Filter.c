// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Filter.h"
#include "Properties.h"
#include "Selector.h"

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
	
// ORIGIN NAMES ====================================================================================

const char *NH_CSS_DECLARATION_ORIGIN_NAMES_PP[] =
{
    "author",
    "user",
    "useragent",
    "animation",
    "transition",
};

// FILTER ==========================================================================================

static NH_API_RESULT nh_css_insertDeclaredValue(
    nh_css_Filter *Filter_p, nh_css_DeclaredValue *Value_p, NH_CSS_PROPERTY property)
{
    nh_core_List *List_p = Filter_p->DeclaredValueLists.pp[property];
    if (!List_p) {
        List_p = nh_core_incrementArray(&Filter_p->DeclaredValueListsArray);
        *List_p = nh_core_initList(8);
        Filter_p->DeclaredValueLists.pp[property] = List_p;
    }
    nh_core_appendToList(List_p, Value_p);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_insertOrDropDeclaredValue(
    nh_css_Filter *Filter_p, nh_css_Declaration *Declaration_p)
{
    unsigned int *property_p = nh_core_getFromHashMap(&NH_CSS_INDEXMAP.Properties, Declaration_p->Name.p);

    if (property_p) 
    {
        nh_css_DeclaredValue *Value_p = nh_core_incrementArray(&Filter_p->DeclaredValues);
        NH_CORE_CHECK_MEM(Value_p)

        Value_p->Declaration_p = Declaration_p;

        nh_css_insertDeclaredValue(Filter_p, Value_p, *property_p);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_getStyleSheetDeclarations(
    nh_css_Filter *Filter_p, nh_dom_Element *Element_p, nh_css_StyleSheetObject *StyleSheet_p)
{
    nh_core_List *Rules_p = nh_css_getRuleListData(nh_css_getRuleList(StyleSheet_p));

    for (int i = 0; i < Rules_p->size; ++i) {
        nh_css_StyleRuleObject *Rule_p = nh_css_getStyleRule(Rules_p->pp[i]);
        if (Rule_p && nh_css_matchSelectors(nh_css_getStyleRuleSelectors(Rule_p), Element_p)) {
            nh_core_Array *Array_p = 
                nh_css_getStyleDeclarationData(nh_css_getStyleRuleDeclaration(Rule_p));
            for (int j = 0; j < Array_p->length; ++j) {
                nh_css_Declaration *Declaration_p = &((nh_css_Declaration*)Array_p->p)[j];
                nh_css_insertOrDropDeclaredValue(Filter_p, Declaration_p);
            }
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_getDefaultStyleSheetDeclarations(
    nh_css_Filter *Filter_p, nh_dom_Element *Element_p)
{
    int oldLength = Filter_p->DeclaredValues.length;
    nh_css_getStyleSheetDeclarations(Filter_p, Element_p, NH_CSS_DEFAULT_STYLE_SHEET_P);

    for (int i = oldLength; i < Filter_p->DeclaredValues.length; ++i) {
        nh_css_DeclaredValue *Value_p = &((nh_css_DeclaredValue*)Filter_p->DeclaredValues.p)[i];
        Value_p->origin = NH_CSS_DECLARATION_ORIGIN_USER_AGENT;
        Value_p->direct = false;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_getAuthorStyleSheetDeclarations(
    nh_css_Filter *Filter_p, nh_dom_Element *Element_p, nh_css_StyleSheetObject *StyleSheet_p)
{
    int oldLength = Filter_p->DeclaredValues.length;
    nh_css_getStyleSheetDeclarations(Filter_p, Element_p, StyleSheet_p);

    for (int i = oldLength; i < Filter_p->DeclaredValues.length; ++i) {
        nh_css_DeclaredValue *Value_p = &((nh_css_DeclaredValue*)Filter_p->DeclaredValues.p)[i];
        Value_p->origin = NH_CSS_DECLARATION_ORIGIN_AUTHOR;
        Value_p->direct = false;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_getStyleAttributeDeclarations(
    nh_css_Filter *Filter_p, nh_dom_Element *Element_p)
{
    int oldLength = Filter_p->DeclaredValues.length;

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
                nh_css_insertOrDropDeclaredValue(Filter_p, Declaration_p);
            }
        }
    }

    for (int i = oldLength; i < Filter_p->DeclaredValues.length; ++i) {
        nh_css_DeclaredValue *Value_p = &((nh_css_DeclaredValue*)Filter_p->DeclaredValues.p)[i];
        Value_p->origin = NH_CSS_DECLARATION_ORIGIN_AUTHOR;
        Value_p->direct = true;
    }

    return NH_API_SUCCESS;
}

/**
 * Filtered rules are all rules that match to the Element via Selectors.
 * Cascading will afterwards decide which filtered rules to keep.
 */
nh_css_Filter nh_css_filterDeclarations(
    nh_dom_Element *Element_p, nh_css_StyleSheetListObject *AuthorStyleSheets_p, nh_core_List UserStyleSheets)
{
    nh_css_Filter Filter;
    Filter.DeclaredValues = nh_core_initArray(sizeof(nh_css_DeclaredValue), 255);
    Filter.DeclaredValueLists = nh_core_initList(NH_CSS_PROPERTY_COUNT);
    Filter.DeclaredValueListsArray = nh_core_initArray(sizeof(nh_core_List), 16);

    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) {
        nh_core_appendToList(&Filter.DeclaredValueLists, NULL);
    }

    nh_css_getDefaultStyleSheetDeclarations(&Filter, Element_p);
    nh_css_getStyleAttributeDeclarations(&Filter, Element_p);

    nh_core_List *List_p = nh_css_getStyleSheetListData(AuthorStyleSheets_p);
    for (int i = 0; i < List_p->size; ++i) {
        nh_css_getAuthorStyleSheetDeclarations(&Filter, Element_p, List_p->pp[i]);
    }

    return Filter;
}
