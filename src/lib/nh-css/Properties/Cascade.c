// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Cascade.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// COMPARE =========================================================================================

static int nh_css_getOriginAndImportancePrecedence(
    nh_css_DeclaredValue *Value_p)
{
NH_CSS_BEGIN()

    switch (Value_p->origin)
    {
        case NH_CSS_DECLARATION_ORIGIN_TRANSITION :
            NH_CSS_END(1)

        case NH_CSS_DECLARATION_ORIGIN_USER_AGENT :
            if (Value_p->Declaration_p->important) {
                NH_CSS_END(2)
            }
            NH_CSS_END(8)

        case NH_CSS_DECLARATION_ORIGIN_USER :
            if (Value_p->Declaration_p->important) {
                NH_CSS_END(3)
            }
            NH_CSS_END(7)

        case NH_CSS_DECLARATION_ORIGIN_AUTHOR :
            if (Value_p->Declaration_p->important) {
                NH_CSS_END(4)
            }
            NH_CSS_END(6)

        case NH_CSS_DECLARATION_ORIGIN_ANIMATION :
            NH_CSS_END(5)
    }

NH_CSS_END(9)
}

static int nh_css_getContextPrecedence(
    nh_css_DeclaredValue *Value_p)
{
NH_CSS_BEGIN()

    // TODO

NH_CSS_END(0)
}

static int nh_css_getLayersPrecedence(
    nh_css_DeclaredValue *Value_p)
{
NH_CSS_BEGIN()

    // TODO

NH_CSS_END(0)
}

nh_css_DeclaredValue *nh_css_compare(
    nh_css_DeclaredValue *Value1_p, nh_css_DeclaredValue *Value2_p)
{
NH_CSS_BEGIN()

    int v1, v2;

    v1 = nh_css_getOriginAndImportancePrecedence(Value1_p);
    v2 = nh_css_getOriginAndImportancePrecedence(Value2_p);

    if (v1 != v2) {
        if (v1 < v2) NH_CSS_END(Value1_p) else NH_CSS_END(Value2_p)
    }

// TODO context

    if (Value1_p->direct != Value2_p->direct) {
        if (Value1_p->direct) NH_CSS_END(Value1_p) else NH_CSS_END(Value2_p)
    }

// TODO layers
// TODO specificity

    if (Value1_p == Value2_p) {NH_CSS_END(NULL)}

NH_CSS_END(Value1_p < Value2_p ? Value2_p : Value1_p)
}
	
// CONVERT =========================================================================================

static void nh_css_cascadeUsingQuickSort(
    nh_List *DeclaredValues_p, int first, int last)
{
NH_CSS_BEGIN()

    int i, j, pivot;
    nh_css_DeclaredValue *Temp_p;

    if (first < last)
    {
        pivot = first;
        i = first;
        j = last;

        while (i < j)
        {
            while (nh_css_compare(DeclaredValues_p->pp[i], DeclaredValues_p->pp[pivot]) == DeclaredValues_p->pp[pivot] && i < last)
                i++;
            while (nh_css_compare(DeclaredValues_p->pp[j], DeclaredValues_p->pp[pivot]) != DeclaredValues_p->pp[pivot])
                j--;
            if (i < j){
                Temp_p = DeclaredValues_p->pp[i];
                DeclaredValues_p->pp[i] = DeclaredValues_p->pp[j];
                DeclaredValues_p->pp[j] = Temp_p;
            }
        }
    
        Temp_p = DeclaredValues_p->pp[pivot];
        DeclaredValues_p->pp[pivot] = DeclaredValues_p->pp[j];
        DeclaredValues_p->pp[j] = Temp_p;

        nh_css_cascadeUsingQuickSort(DeclaredValues_p, first, j - 1);
        nh_css_cascadeUsingQuickSort(DeclaredValues_p, j + 1, last);
    }

NH_CSS_SILENT_END()
}

static void nh_css_cascadeDeclaredValues(
    nh_List *DeclaredValues_p)
{
NH_CSS_BEGIN()

    if (!DeclaredValues_p) {NH_CSS_SILENT_END()}

    nh_css_cascadeUsingQuickSort(DeclaredValues_p, 0, DeclaredValues_p->size);

NH_CSS_SILENT_END()
}

void nh_css_cascade(
    nh_css_Filter *Filter_p)
{
NH_CSS_BEGIN()

    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) {
        nh_css_cascadeDeclaredValues(Filter_p->DeclaredValueLists.pp[i]);
    }

NH_CSS_END()
}

