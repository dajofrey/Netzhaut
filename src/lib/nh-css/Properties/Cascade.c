// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Cascade.h"

#include "../Common/Log.h"
#include "../Interfaces/StyleRule.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// TYPEDEFS ========================================================================================

typedef struct nh_css_Specificity {
    unsigned int a;
    unsigned int b;
    unsigned int c;
} nh_css_Specificity;

// FUNCTIONS =======================================================================================

static int nh_css_getOriginAndImportancePrecedence(
    nh_css_Candidate *Candidate_p)
{
    switch (Candidate_p->origin)
    {
        case NH_CSS_DECLARATION_ORIGIN_TRANSITION :
            return 1;

        case NH_CSS_DECLARATION_ORIGIN_USER_AGENT :
            if (Candidate_p->Declaration_p->important) {
                return 2;
            }
            return 8;

        case NH_CSS_DECLARATION_ORIGIN_USER :
            if (Candidate_p->Declaration_p->important) {
                return 3;
            }
            return 7;

        case NH_CSS_DECLARATION_ORIGIN_AUTHOR :
            if (Candidate_p->Declaration_p->important) {
                return 4;
            }
            return 6;

        case NH_CSS_DECLARATION_ORIGIN_ANIMATION :
            return 5;
    }

    return 9;
}

static int nh_css_getContextPrecedence(
    nh_css_Candidate *Candidate_p)
{
    // TODO

    return 0;
}

static int nh_css_getLayersPrecedence(
    nh_css_Candidate *Candidate_p)
{
    // TODO

    return 0;
}

static void nh_css_countSelectorType(
    nh_css_SelectorParseNode *Node_p, NH_CSS_SELECTOR_PARSE_NODE type, int *count_p)
{
    if (Node_p->type == type) {++(*count_p);}

    // Ignore the universal selector.
    if (Node_p->type == NH_CSS_SELECTOR_PARSE_NODE_TYPE_SELECTOR) {
        if (((nh_css_SelectorParseNode*)Node_p->Children.pp[0])->Token_p != NULL) {
            nh_css_Token *Token_p = ((nh_css_SelectorParseNode*)Node_p->Children.pp[0])->Token_p;
            if (Token_p->type == NH_CSS_TOKEN_DELIM && Token_p->Delim.value == 0x2A) { 
                --(*count_p);
            } 
        }
    }

    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_css_countSelectorType(Node_p->Children.pp[i], type, count_p);
    }
}

/**
 * https://www.w3.org/TR/selectors/#specificity
 */
static nh_css_Specificity nh_css_getSpecificity(
    nh_css_Candidate *Candidate_p)
{
    nh_css_Specificity Specificity;
    memset(&Specificity, 0, sizeof(nh_css_Specificity));

    if (Candidate_p->CSSStyleRule_p == NULL) {
        return Specificity;
    }

    nh_css_SelectorParseNode *Node_p = 
        nh_css_getCSSStyleRuleSelectors(Candidate_p->CSSStyleRule_p);

    nh_css_countSelectorType(Node_p, NH_CSS_SELECTOR_PARSE_NODE_ID_SELECTOR, &Specificity.a);
    nh_css_countSelectorType(Node_p, NH_CSS_SELECTOR_PARSE_NODE_CLASS_SELECTOR, &Specificity.b);
    nh_css_countSelectorType(Node_p, NH_CSS_SELECTOR_PARSE_NODE_ATTRIBUTE_SELECTOR, &Specificity.b);
    nh_css_countSelectorType(Node_p, NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_CLASS_SELECTOR, &Specificity.b);
    nh_css_countSelectorType(Node_p, NH_CSS_SELECTOR_PARSE_NODE_TYPE_SELECTOR, &Specificity.c);
    nh_css_countSelectorType(Node_p, NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_ELEMENT_SELECTOR, &Specificity.c);

    return Specificity;
}

nh_css_Candidate *nh_css_compare(
    nh_css_Candidate *Candidate1_p, nh_css_Candidate *Candidate2_p)
{
    int v1, v2;

    v1 = nh_css_getOriginAndImportancePrecedence(Candidate1_p);
    v2 = nh_css_getOriginAndImportancePrecedence(Candidate2_p);

    if (v1 != v2) {
        if (v1 < v2) {return Candidate1_p;} else {return Candidate2_p;}
    }

// TODO context

    if (Candidate1_p->direct != Candidate2_p->direct) {
        if (Candidate1_p->direct) {return Candidate1_p;} else {return Candidate2_p;}
    }

// TODO layers

    nh_css_Specificity Specificity1 = nh_css_getSpecificity(Candidate1_p);
    nh_css_Specificity Specificity2 = nh_css_getSpecificity(Candidate2_p);

    if (Specificity1.a != Specificity2.a) {
        if (Specificity1.a > Specificity2.a) {return Candidate1_p;} else {return Candidate2_p;}
    }
    if (Specificity1.b != Specificity2.b) {
        if (Specificity1.b > Specificity2.b) {return Candidate1_p;} else {return Candidate2_p;}
    }
    if (Specificity1.c != Specificity2.c) {
        if (Specificity1.c > Specificity2.c) {return Candidate1_p;} else {return Candidate2_p;}
    }

    if (Candidate1_p == Candidate2_p) {return NULL;}

    return Candidate1_p < Candidate2_p ? Candidate2_p : Candidate1_p;
}

static void nh_css_cascadeUsingQuickSort(
    nh_core_List *Candidates_p, int first, int last)
{
    int i, j, pivot;
    nh_css_Candidate *Temp_p;

    if (first < last)
    {
        pivot = first;
        i = first;
        j = last;

        while (i < j)
        {
            while (i < last && nh_css_compare(Candidates_p->pp[i], Candidates_p->pp[pivot]) == Candidates_p->pp[pivot])
                i++;
            while (j > first && nh_css_compare(Candidates_p->pp[j], Candidates_p->pp[pivot]) != Candidates_p->pp[pivot])
                j--;
            if (i < j){
                Temp_p = Candidates_p->pp[i];
                Candidates_p->pp[i] = Candidates_p->pp[j];
                Candidates_p->pp[j] = Temp_p;
            }
        }
    
        Temp_p = Candidates_p->pp[pivot];
        Candidates_p->pp[pivot] = Candidates_p->pp[j];
        Candidates_p->pp[j] = Temp_p;

        nh_css_cascadeUsingQuickSort(Candidates_p, first, j - 1);
        nh_css_cascadeUsingQuickSort(Candidates_p, j + 1, last);
    }

    return;
}

static void nh_css_cascadeCandidates(
    nh_core_List *Candidates_p)
{
    if (!Candidates_p) {return ;}

    nh_css_cascadeUsingQuickSort(Candidates_p, 0, Candidates_p->size-1);

    return;
}

/**
 * https://www.w3.org/TR/css-cascade-4/#cascade
 */
void nh_css_cascade(
    nh_css_Filter *Filter_p)
{
    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) {
        nh_css_cascadeCandidates(Filter_p->CandidateLists.pp[i]);
    }

    return;
}
