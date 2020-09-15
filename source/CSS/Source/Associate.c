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
    Nh_CSS_Sheet *Sheet_p, NH_CSS_RuleSet *RuleSet_p, Nh_HTML_Node *Node_p, bool recursive, 
    bool hitRequired
);
static NH_RESULT Nh_CSS_handleStyleRuleSet(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node *Node_p, bool recursive, 
    bool hitRequired
);
static NH_RESULT Nh_CSS_addStyleProperties(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node* Node_p,
    Nh_CSS_Selector *Selector_p
);

// APPLY ===========================================================================================

static NH_RESULT Nh_CSS_associateSheet(
    Nh_Tab *Tab_p, Nh_CSS_Sheet *Sheet_p, Nh_HTML_Node *Node_p)
{   
NH_BEGIN()

    for (size_t i = 0; i < Sheet_p->ruleSetCount; ++i) 
    {
        if (Node_p != NULL && (Sheet_p->Apply_p == NULL || Sheet_p->Apply_p == Node_p)) {
            NH_CHECK(Nh_CSS_dispatchRuleSet(
                Sheet_p, &Sheet_p->RuleSets_p[i], Node_p, false, true
            ))
        }
        else if (Node_p == NULL && Sheet_p->Apply_p == NULL) {
            NH_CHECK(Nh_CSS_dispatchRuleSet(
                Sheet_p, &Sheet_p->RuleSets_p[i], Nh_HTML_getNode(Tab_p, NH_HTML_TAG_BODY, NH_TRUE), 
                true, true
            ))
        }
        else if (Node_p == NULL && Sheet_p->Apply_p != NULL) {
            NH_CHECK(Nh_CSS_dispatchRuleSet(
                Sheet_p, &Sheet_p->RuleSets_p[i], Sheet_p->Apply_p, false, false
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
            Tab_p, Nh_CSS_getSheet(&Tab_p->Document.Sheets, i), Node_p
        ))
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_reassociateSheets(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{   
NH_BEGIN()

    Nh_CSS_destroyGenericProperties(&Node_p->Properties);
    NH_CHECK(Nh_CSS_associateSheets(Tab_p, Node_p))

NH_END(NH_SUCCESS)
}

// DISPATCH ========================================================================================

static NH_RESULT Nh_CSS_dispatchRuleSet(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_RuleSet* RuleSet_p, Nh_HTML_Node* Node_p, bool recursive, 
    bool hitRequired)
{
NH_BEGIN()

    switch (RuleSet_p->type) 
    {
        case NH_CSS_RULE_SET_STYLE      : 
        {
            Nh_CSS_handleStyleRuleSet(
                Sheet_p, RuleSet_p->Data_p, Node_p, recursive, hitRequired
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
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node* Node_p, bool recursive, 
    bool hitRequired)
{
NH_BEGIN()

    if (!hitRequired) {
        // TODO
    }
    else {
        for (size_t i = 0; i < RuleSet_p->selectorCount; ++i) 
        {
            Nh_CSS_Selector Selector;
            if (Nh_CSS_naiveSelectorHit(Node_p, RuleSet_p->selectors_pp[i], &Selector)) {
                Nh_CSS_addStyleProperties(Sheet_p, RuleSet_p, Node_p, &Selector);
            }
        }
    }

    if (recursive) {
        for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) {
            Nh_CSS_handleStyleRuleSet(
                Sheet_p, RuleSet_p, Nh_getListItem(&Node_p->Children.Unformatted, i), true, hitRequired
            );
        }
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_CSS_addStyleProperties(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, Nh_HTML_Node* Node_p,
    Nh_CSS_Selector *Selector_p)
{
NH_BEGIN()

    for (int i = 0; i < RuleSet_p->declarationCount; ++i)
    {
        NH_CSS_Declaration *Declaration_p = &RuleSet_p->Declarations_p[i];

        Nh_HashValue* HashValue_p;
        int error = hashmap_get(NH_HASHMAPS.CSS.Properties, (char*)Declaration_p->property_p, (void**)(&HashValue_p));
        if (error != MAP_OK) {NH_END(NH_ERROR_BAD_STATE)}
    
        Nh_CSS_GenericProperty Property = {0};

        Property.Sheet_p    = Sheet_p;
        Property.type       = HashValue_p->number;
        Property.Selector   = *Selector_p;
        Property.active     = NH_FALSE;
        Property.update     = NH_FALSE;
        Property.valueCount = Declaration_p->valueCount;
        Property.values_pp  = Declaration_p->values_pp;

        NH_CHECK(Nh_CSS_addGenericProperty(&Node_p->Properties, &Property))
    }

NH_END(NH_SUCCESS)
}

