// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Associate.h"
#include "../Header/Properties.h"
#include "../Header/Macros.h"
#include "../Header/Selector.h"

#include "../../Core/Header/List.h"
#include "../../Core/Header/HashMap.h"

#include "../../HTML/Header/Attribute.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_CSS_dispatchRuleSet(
    NH_CSS_Sheet *Sheet_p, NH_CSS_RuleSet *RuleSet_p, Nh_HTML_Node *Node_p, Nh_HashMaps *Maps_p, 
    bool recursive, bool hitRequired, bool handleDependentSelectors
);
static NH_RESULT Nh_CSS_handleStyleRuleSet(
    NH_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node *Node_p, Nh_HashMaps *Maps_p, 
    bool recursive, bool hitRequired, bool handleDependentSelectors
);
static NH_RESULT Nh_CSS_addStyleProperties(
    NH_CSS_Sheet *Sheet_p,  NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node* Node_p, map_t Properties, 
    NH_CSS_Pseudo *Pseudo_p, NH_CSS_SELECTOR selector
);

// APPLY ===========================================================================================

static NH_RESULT Nh_CSS_associateSheet(
    Nh_Tab *Tab_p, NH_CSS_Sheet *Sheet_p, Nh_HashMaps *Maps_p, Nh_HTML_Node *Node_p, 
    bool handleDependentSelectors)
{   
NH_BEGIN()

    for (size_t i = 0; i < Sheet_p->ruleSetCount; ++i) 
    {
        if (Node_p != NULL && (Sheet_p->Apply_p == NULL || Sheet_p->Apply_p == Node_p)) {
            NH_CHECK(Nh_CSS_dispatchRuleSet(
                Sheet_p, &Sheet_p->RuleSets_p[i], Node_p, Maps_p, false, true, handleDependentSelectors
            ))
        }
        else if (Node_p == NULL && Sheet_p->Apply_p == NULL) {
            NH_CHECK(Nh_CSS_dispatchRuleSet(
                Sheet_p, &Sheet_p->RuleSets_p[i], Nh_HTML_getNode(Tab_p, NH_HTML_TAG_BODY, NH_TRUE), Maps_p, 
                true, true, handleDependentSelectors
            ))
        }
        else if (Node_p == NULL && Sheet_p->Apply_p != NULL) {
            NH_CHECK(Nh_CSS_dispatchRuleSet(
                Sheet_p, &Sheet_p->RuleSets_p[i], Sheet_p->Apply_p, Maps_p, false, false, 
                handleDependentSelectors
            ))
        }
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_associateSheets(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{   
NH_BEGIN()

    for (int i = 0; i < Tab_p->Document.Sheets.count; ++i) {
        NH_CHECK(Nh_CSS_associateSheet(
            Tab_p, Nh_CSS_getSheet(&Tab_p->Document.Sheets, i), Nh_getHashMaps(), Node_p, false
        ))
    }
    for (int i = 0; i < Tab_p->Document.Sheets.count; ++i) {
        NH_CHECK(Nh_CSS_associateSheet(
            Tab_p, Nh_CSS_getSheet(&Tab_p->Document.Sheets, i), Nh_getHashMaps(), Node_p, true
        ))
    }

NH_END(NH_SUCCESS)
}

// DISPATCH ========================================================================================

static NH_RESULT Nh_CSS_dispatchRuleSet(
    NH_CSS_Sheet *Sheet_p, NH_CSS_RuleSet* RuleSet_p, Nh_HTML_Node* Node_p, Nh_HashMaps *Maps_p, 
    bool recursive, bool hitRequired, bool handleDependentSelectors)
{
NH_BEGIN()

    switch (RuleSet_p->type) 
    {
        case NH_CSS_RULE_SET_STYLE      : 
        {
            Nh_CSS_handleStyleRuleSet(
                Sheet_p, RuleSet_p->Data_p, Node_p, Maps_p, recursive, hitRequired, 
                handleDependentSelectors
            ); 
            break;
        }
        case NH_CSS_RULE_SET_IMPORT     : break;
        case NH_CSS_RULE_SET_FONT_FACE  : break;
        case NH_CSS_RULE_SET_KEY_FRAMES : break;
        case NH_CSS_RULE_SET_MEDIA      : break;
        case NH_CSS_RULE_SET_SUPPORTS   : break;
        default                         : break;
    }

NH_END(NH_SUCCESS)
}

// STYLE RULE-SET ==================================================================================

static NH_RESULT Nh_CSS_handleStyleRuleSet(
    NH_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node* Node_p, Nh_HashMaps *Maps_p, 
    bool recursive, bool hitRequired, bool handleDependentSelectors)
{
NH_BEGIN()

    NH_CSS_SELECTOR selector = NH_CSS_SELECTOR_UNDEFINED;

    if (!hitRequired) {
        // TODO
    }
    else {
        for (size_t i = 0; i < RuleSet_p->selectorCount; ++i) 
        {
            NH_CSS_Pseudo Pseudo = {-1};
            if (Nh_CSS_selectorHit(Node_p, Maps_p, RuleSet_p->selectors_pp[i], &Pseudo, &selector, handleDependentSelectors)) {
                Nh_CSS_addStyleProperties(Sheet_p, RuleSet_p, Node_p, Maps_p->CSS.Properties, &Pseudo, selector);
            }
        }
    }

    if (recursive) {
        for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) {
            Nh_CSS_handleStyleRuleSet(
                Sheet_p, RuleSet_p, Nh_getListItem(&Node_p->Children.Unformatted, i), Maps_p, true, hitRequired, 
                handleDependentSelectors
            );
        }
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_CSS_addStyleProperties(
    NH_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node* Node_p, map_t Properties, 
    NH_CSS_Pseudo *Pseudo_p, NH_CSS_SELECTOR selector)
{
NH_BEGIN()

    for (int i = 0; i < RuleSet_p->declarationCount; ++i)
    {
        NH_CSS_Declaration *Declaration_p = &RuleSet_p->Declarations_p[i];

        Nh_HashValue* HashValue_p;
        int error = hashmap_get(Properties, (char*)Declaration_p->property_p, (void**)(&HashValue_p));
        if (error != MAP_OK) {NH_END(NH_ERROR_BAD_STATE)}
    
        NH_CSS_GenericProperty Property = {0};

        Property.Sheet_p            = Sheet_p;
        Property.type               = HashValue_p->number;
        Property.Pseudo._class      = Pseudo_p->_class;
        Property.Pseudo.parentClass = Pseudo_p->parentClass;
        Property.Pseudo.element     = Pseudo_p->element;
        Property.selector           = selector;
        Property.active             = Pseudo_p->_class == -1 && Pseudo_p->parentClass == -1;
        Property.update             = false;
        Property.valueCount         = Declaration_p->valueCount;
        Property.values_pp          = Declaration_p->values_pp;

        Nh_CSS_addGenericProperty(&Node_p->Properties, &Property);
    }

NH_END(NH_SUCCESS)
}

