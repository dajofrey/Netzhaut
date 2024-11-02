// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Compute.h"
#include "Properties.h"
#include "Color.h"
#include "Cascade.h"

#include "../Interfaces/Document.h"
#include "../Common/Log.h"

#include "../../nh-core/Util/String.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-encoding/Encodings/UTF8.h"

#include "../../nh-dom/Interfaces/Node.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DECLARE =========================================================================================

typedef struct nh_css_ComputeVariables {
    NH_CSS_PROPERTY property;
    nh_dom_Node *Node_p;
    nh_core_Array *SpecifiedValues_p;
    nh_core_List *ComputedValues_p;
    nh_css_Value **ComputedValue_pp;
    nh_css_Value *SpecifiedValue_p;
} nh_css_ComputeVariables;

// HELPER ==========================================================================================

static nh_css_Value *nh_css_compareSpecifiedValues(
    nh_css_Value *Value1_p, nh_css_Value *Value2_p)
{
    if (!Value1_p->Common.Origin_p && Value2_p->Common.Origin_p) {
        return Value2_p;
    }
    else if (Value1_p->Common.Origin_p && !Value2_p->Common.Origin_p) {
        return Value1_p;
    }
    else if (Value1_p->Common.Origin_p && Value2_p->Common.Origin_p) {
        nh_css_Candidate *Declared_p = nh_css_compare(Value1_p->Common.Origin_p, Value2_p->Common.Origin_p);
        return Declared_p == Value1_p->Common.Origin_p ? Value1_p : Value2_p;
    }

    return NULL;
}

// COLOR ===========================================================================================

static NH_API_RESULT nh_css_computeColorUnit(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *ColorUnit_p = Variables_p->SpecifiedValue_p;

    if (ColorUnit_p->Common.type == NH_CSS_VALUE_KEYWORD) 
    {
        char color_p[6] = {0};

        if (nh_css_getColorFromName(ColorUnit_p->String.p, color_p) == NH_API_SUCCESS) 
        {
            nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(NewValue_p)
            *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_HEX);
            NewValue_p->String = nh_core_initString(7);
            nh_core_appendToString(&NewValue_p->String, color_p, 6);
            *Variables_p->ComputedValue_pp = NewValue_p;
        }
        else if (!strcmp(ColorUnit_p->String.p, "transparent")) 
        {
            nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(NewValue_p)
            *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_FUNCTION);
            NewValue_p->String = nh_core_initString(14);
            nh_core_appendToString(&NewValue_p->String, "rgba(0,0,0,0)", 13);
            *Variables_p->ComputedValue_pp = NewValue_p;
        }
        else if (!strcmp(ColorUnit_p->String.p, "currentcolor")) 
        {
            *Variables_p->ComputedValue_pp = ColorUnit_p;
        }
        else {
            return NH_API_ERROR_BAD_STATE;
        }
    }
    else 
    if (ColorUnit_p->Common.type == NH_CSS_VALUE_HEX
    ||  ColorUnit_p->Common.type == NH_CSS_VALUE_FUNCTION) {
        *Variables_p->ComputedValue_pp = ColorUnit_p;
    }
    else {
        return NH_API_ERROR_BAD_STATE;
    }

    return NH_API_SUCCESS;
}

// LENGTH ==========================================================================================

static NH_API_RESULT nh_css_computeLength(
    nh_css_ComputeVariables *Variables_p)
{
    if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_NUMBER) {
        *Variables_p->ComputedValue_pp = Variables_p->SpecifiedValue_p;
        return NH_API_SUCCESS;
    }

    switch (Variables_p->SpecifiedValue_p->Common.type)
    {
        case NH_CSS_VALUE_EM   :
        {
            nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(NewValue_p)
            *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_PX);

            if (Variables_p->property != NH_CSS_PROPERTY_FONT_SIZE) {
                nh_css_Value *FontSize_p = Variables_p->ComputedValues_p->pp[NH_CSS_PROPERTY_FONT_SIZE]; // DEPENDENCY: FONT-SIZE
                NewValue_p->number = Variables_p->SpecifiedValue_p->number * FontSize_p->number;
                *Variables_p->ComputedValue_pp = NewValue_p;
            }
            else {
                nh_webidl_Object *Parent_p = nh_dom_getParentNode(Variables_p->Node_p);
                NH_CORE_CHECK_NULL(Parent_p)
                nh_core_List *ParentValues_p = nh_dom_getComputedPropertyValues(nh_dom_getNode(Parent_p));
                NH_CORE_CHECK_NULL(ParentValues_p)
                NewValue_p->number = Variables_p->SpecifiedValue_p->number * ((nh_css_Value*)ParentValues_p->pp[NH_CSS_PROPERTY_FONT_SIZE])->number;
                *Variables_p->ComputedValue_pp = NewValue_p;
            }
            break;
        }
        case NH_CSS_VALUE_EX   : break;
        case NH_CSS_VALUE_CAP  : break;
        case NH_CSS_VALUE_CH   : break;
        case NH_CSS_VALUE_IC   : break;
        case NH_CSS_VALUE_REM  : break;
        case NH_CSS_VALUE_LH   : break;
        case NH_CSS_VALUE_RLH  : break;
        case NH_CSS_VALUE_VW   : break;
        case NH_CSS_VALUE_VH   : break;
        case NH_CSS_VALUE_VI   : break;
        case NH_CSS_VALUE_VB   : break;
        case NH_CSS_VALUE_VMIN : break;
        case NH_CSS_VALUE_VMAX : break;
        case NH_CSS_VALUE_IN   : break;
        case NH_CSS_VALUE_CM   : break;
        case NH_CSS_VALUE_MM   : break;
        case NH_CSS_VALUE_PT   : break;
        case NH_CSS_VALUE_PC   : break; 
        case NH_CSS_VALUE_PX   :
            *Variables_p->ComputedValue_pp = Variables_p->SpecifiedValue_p;
            break;
        case NH_CSS_VALUE_Q    : break; 
        case NH_CSS_VALUE_S    : break;
        case NH_CSS_VALUE_MS   : break;
        case NH_CSS_VALUE_HZ   : break;
        case NH_CSS_VALUE_KHZ  : break;
        case NH_CSS_VALUE_DPI  : break;
        case NH_CSS_VALUE_DPCM : break;
        case NH_CSS_VALUE_DPPX : break;
        case NH_CSS_VALUE_DEG  : break;
        case NH_CSS_VALUE_GRAD : break;
        case NH_CSS_VALUE_RAD  : break;
        case NH_CSS_VALUE_TURN : break;

        default : return NH_API_ERROR_BAD_STATE;
    }

    return NH_API_SUCCESS;
}

// MARGIN ==========================================================================================

static NH_API_RESULT nh_css_computeMarginValue(
    nh_css_ComputeVariables *Variables_p, nh_css_Value *Value_p, NH_CSS_PROPERTY property)
{
    Variables_p->SpecifiedValue_p = Value_p;
    Variables_p->property = property;
    Variables_p->ComputedValue_pp = (nh_css_Value**)&Variables_p->ComputedValues_p->pp[property];

    if (Value_p->Common.type == NH_CSS_VALUE_KEYWORD && !strcmp(Value_p->String.p, "auto")) {
        *(Variables_p->ComputedValue_pp) = Value_p;
    }
    else if (nh_css_isLengthValue(Value_p)) {
        NH_CORE_CHECK(nh_css_computeLength(Variables_p))
    } 
    else if (Value_p->Common.type == NH_CSS_VALUE_PERCENTAGE) {
        *(Variables_p->ComputedValue_pp) = Value_p;
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_computeMargin(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *ShorthandValue_p = 
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN];

    nh_css_Value *Compare_p = 
        nh_css_compareSpecifiedValues(ShorthandValue_p, Variables_p->SpecifiedValue_p);

    if (Compare_p == ShorthandValue_p) {return NH_API_SUCCESS;}

    NH_API_RESULT result = 
        nh_css_computeMarginValue(Variables_p, Variables_p->SpecifiedValue_p, Variables_p->property);

    return result;
}

static NH_API_RESULT nh_css_computeMarginShorthand(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *Value_p = &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN];
    nh_css_Value *SecondValue_p = NULL;
    nh_css_Value *ThirdValue_p = NULL;
    nh_css_Value *FourthValue_p = NULL;

    if (Value_p->Common.Next_p) {
        SecondValue_p = Value_p->Common.Next_p;
    }
    if (SecondValue_p && SecondValue_p->Common.Next_p) {
        ThirdValue_p = SecondValue_p->Common.Next_p;
    }
    if (ThirdValue_p && ThirdValue_p->Common.Next_p) {
        FourthValue_p = ThirdValue_p->Common.Next_p;
    }

    bool setTop = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN_TOP], Value_p
    );
    bool setRight = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN_RIGHT], Value_p
    );
    bool setBottom = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN_BOTTOM], Value_p
    );
    bool setLeft = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN_LEFT], Value_p
    );

    if (FourthValue_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Value_p, NH_CSS_PROPERTY_MARGIN_TOP))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_MARGIN_RIGHT))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, ThirdValue_p, NH_CSS_PROPERTY_MARGIN_BOTTOM))}
        if (setLeft)   {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, FourthValue_p, NH_CSS_PROPERTY_MARGIN_LEFT))}
    }
    else if (ThirdValue_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Value_p, NH_CSS_PROPERTY_MARGIN_TOP))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_MARGIN_RIGHT))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, ThirdValue_p, NH_CSS_PROPERTY_MARGIN_BOTTOM))}
        if (setLeft)   {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_MARGIN_LEFT))}
    }
    else if (SecondValue_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Value_p, NH_CSS_PROPERTY_MARGIN_TOP))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_MARGIN_RIGHT))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Value_p, NH_CSS_PROPERTY_MARGIN_BOTTOM))} 
        if (setLeft)   {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_MARGIN_LEFT))}
    }
    else if (Value_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Value_p, NH_CSS_PROPERTY_MARGIN_TOP))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Value_p, NH_CSS_PROPERTY_MARGIN_RIGHT))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Value_p, NH_CSS_PROPERTY_MARGIN_BOTTOM))}
        if (setLeft)   {NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Value_p, NH_CSS_PROPERTY_MARGIN_LEFT))}
    }

    return NH_API_SUCCESS;
}

// MARGIN BLOCK/INLINE =============================================================================

static NH_API_RESULT nh_css_computeMarginBlockOrMarginInline(
    nh_css_ComputeVariables *Variables_p)
{
    switch(Variables_p->property)
    {
        case NH_CSS_PROPERTY_MARGIN_BLOCK_START :
        {
            nh_css_Value *Compare_p = &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN_TOP];
            if (nh_css_compareSpecifiedValues(Compare_p, Variables_p->SpecifiedValue_p) == Variables_p->SpecifiedValue_p) {
                NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Variables_p->SpecifiedValue_p, NH_CSS_PROPERTY_MARGIN_TOP))
            }
            break;
        }
        case NH_CSS_PROPERTY_MARGIN_BLOCK_END :
        {
            nh_css_Value *Compare_p = &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN_BOTTOM];
            if (nh_css_compareSpecifiedValues(Compare_p, Variables_p->SpecifiedValue_p) == Variables_p->SpecifiedValue_p) {
                NH_CORE_CHECK(nh_css_computeMarginValue(Variables_p, Variables_p->SpecifiedValue_p, NH_CSS_PROPERTY_MARGIN_BOTTOM))
            }
            break;
        }
//        case NH_CSS_PROPERTY_MARGIN_INLINE_START :
//            if (nh_css_compareSpecifiedValues(&((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN_LEFT], SpecifiedValue_p) == SpecifiedValue_p) {
//                NH_CORE_CHECK(nh_css_computeMarginValue(SpecifiedValues_p, ComputedValues_p, SpecifiedValue_p, NH_CSS_PROPERTY_MARGIN_LEFT))
//            }
//            break;
//        case NH_CSS_PROPERTY_MARGIN_INLINE_END :
//            if (nh_css_compareSpecifiedValues(&((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_MARGIN_RIGHT], SpecifiedValue_p) == SpecifiedValue_p) {
//                NH_CORE_CHECK(nh_css_computeMarginValue(SpecifiedValues_p, ComputedValues_p, SpecifiedValue_p, NH_CSS_PROPERTY_MARGIN_RIGHT))
//            }
//            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_computeMarginBlockShorthandOrMarginInlineShorthand(
    nh_css_ComputeVariables *Variables_p)
{
//    if (property == NH_CSS_PROPERTY_MARGIN_BLOCK) {
//        if (Value_p->Common.Next_p) {
//            NH_CORE_CHECK(nh_css_computeMarginBlockOrMarginInline(ComputedValues_p, Value_p, ComputedValue_pp, NH_CSS_PROPERTY_MARGIN_BLOCK_START))
//            NH_CORE_CHECK(nh_css_computeMarginBlockOrMarginInline(ComputedValues_p, Value_p->Common.Next_p, ComputedValue_pp, NH_CSS_PROPERTY_MARGIN_BLOCK_END))
//        } else {
//            NH_CORE_CHECK(nh_css_computeMarginBlockOrMarginInline(ComputedValues_p, Value_p, ComputedValue_pp, NH_CSS_PROPERTY_MARGIN_BLOCK_START))
//            NH_CORE_CHECK(nh_css_computeMarginBlockOrMarginInline(ComputedValues_p, Value_p, ComputedValue_pp, NH_CSS_PROPERTY_MARGIN_BLOCK_END))
//        }
//    }
//    else if (property == NH_CSS_PROPERTY_MARGIN_INLINE) {
//        if (Value_p->Common.Next_p) {
//            NH_CORE_CHECK(nh_css_computeMarginBlockOrMarginInline(ComputedValues_p, Value_p, ComputedValue_pp, NH_CSS_PROPERTY_MARGIN_INLINE_START))
//            NH_CORE_CHECK(nh_css_computeMarginBlockOrMarginInline(ComputedValues_p, Value_p->Common.Next_p, ComputedValue_pp, NH_CSS_PROPERTY_MARGIN_INLINE_END))
//        } else {
//            NH_CORE_CHECK(nh_css_computeMarginBlockOrMarginInline(ComputedValues_p, Value_p, ComputedValue_pp, NH_CSS_PROPERTY_MARGIN_INLINE_START))
//            NH_CORE_CHECK(nh_css_computeMarginBlockOrMarginInline(ComputedValues_p, Value_p, ComputedValue_pp, NH_CSS_PROPERTY_MARGIN_INLINE_END))
//        }
//    }

    return NH_API_SUCCESS;
}

// FONT SIZE =======================================================================================

static nh_css_Value *nh_css_computeAbsoluteSize(
    char *value_p)
{
    int px = 0;

    if (!strcmp(value_p, "xx-small")) {
        px = 12;
    }
    else if (!strcmp(value_p, "x-small")) {
        px = 14;
    }
    else if (!strcmp(value_p, "small")) {
        px = 15;
    }
    else if (!strcmp(value_p, "medium")) {
        px = 16;
    }
    else if (!strcmp(value_p, "large")) {
        px = 18;
    }
    else if (!strcmp(value_p, "x-large")) {
        px = 20;
    }
    else if (!strcmp(value_p, "xx-large")) {
        px = 21;
    }

    nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
    NH_CORE_CHECK_MEM_2(NULL, NewValue_p)
    *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_PX);
    NewValue_p->number = px;

    return NewValue_p;
}

static NH_API_RESULT nh_css_computeFontSize(
    nh_css_ComputeVariables *Variables_p)
{
    if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_KEYWORD) {
        *(Variables_p->ComputedValue_pp) = nh_css_computeAbsoluteSize(Variables_p->SpecifiedValue_p->String.p);
        NH_CORE_CHECK_NULL(*(Variables_p->ComputedValue_pp))
    }
    else if (nh_css_isLengthValue(Variables_p->SpecifiedValue_p)) {
        NH_CORE_CHECK(nh_css_computeLength(Variables_p))
    } 
    else if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_PERCENTAGE) {
        *(Variables_p->ComputedValue_pp) = Variables_p->SpecifiedValue_p;
    }

    return NH_API_SUCCESS;
}

// FONT WEIGHT =====================================================================================

static NH_API_RESULT nh_css_computeFontWeightAbsolute(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value **ComputedValue_pp = Variables_p->ComputedValue_pp;

    if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_KEYWORD) 
    {
        if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "normal")) 
        {
            *ComputedValue_pp = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(*ComputedValue_pp)
            *(*ComputedValue_pp) = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_NUMBER);
            (*ComputedValue_pp)->number = 400; 
        }
        else if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "bold")) 
        {
            *ComputedValue_pp = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(*ComputedValue_pp)
            *(*ComputedValue_pp) = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_NUMBER);
            (*ComputedValue_pp)->number = 700; 
        }
        else {return NH_API_ERROR_BAD_STATE;}
    }
    else if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_NUMBER) {
        *ComputedValue_pp = Variables_p->SpecifiedValue_p;
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_computeFontWeight(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value **ComputedValue_pp = Variables_p->ComputedValue_pp;

    if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_KEYWORD) 
    {
        if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "bolder")) 
        {
            nh_webidl_Object *Parent_p = nh_dom_getParentNode(Variables_p->Node_p);
            NH_CORE_CHECK_NULL(Parent_p)
            nh_core_List *ParentValues_p = nh_dom_getComputedPropertyValues(nh_dom_getNode(Parent_p));
            NH_CORE_CHECK_NULL(ParentValues_p)
            *ComputedValue_pp = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(*ComputedValue_pp)
            *(*ComputedValue_pp) = *((nh_css_Value*)ParentValues_p->pp[NH_CSS_PROPERTY_FONT_WEIGHT]);

            if ((*ComputedValue_pp)->number < 100) {
                (*ComputedValue_pp)->number = 400;
            }
            else if ((*ComputedValue_pp)->number >= 100 && (*ComputedValue_pp)->number < 350) {
                (*ComputedValue_pp)->number = 400;
            }
            else if ((*ComputedValue_pp)->number >= 350 && (*ComputedValue_pp)->number < 550) {
                (*ComputedValue_pp)->number = 700;
            }
            else if ((*ComputedValue_pp)->number >= 550 && (*ComputedValue_pp)->number < 750) {
                (*ComputedValue_pp)->number = 900;
            }
            else if ((*ComputedValue_pp)->number >= 750 && (*ComputedValue_pp)->number < 900) {
                (*ComputedValue_pp)->number = 900;
            }
        }
        else if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "lighter")) 
        {
            nh_webidl_Object *Parent_p = nh_dom_getParentNode(Variables_p->Node_p);
            NH_CORE_CHECK_NULL(Parent_p)
            nh_core_List *ParentValues_p = nh_dom_getComputedPropertyValues(Variables_p->Node_p);
            NH_CORE_CHECK_NULL(ParentValues_p)
            *ComputedValue_pp = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(*ComputedValue_pp)
            *(*ComputedValue_pp) = *((nh_css_Value*)ParentValues_p->pp[NH_CSS_PROPERTY_FONT_WEIGHT]);

            if ((*ComputedValue_pp)->number >= 100 && (*ComputedValue_pp)->number < 350) {
                (*ComputedValue_pp)->number = 100;
            }
            else if ((*ComputedValue_pp)->number >= 350 && (*ComputedValue_pp)->number < 550) {
                (*ComputedValue_pp)->number = 100;
            }
            else if ((*ComputedValue_pp)->number >= 550 && (*ComputedValue_pp)->number < 750) {
                (*ComputedValue_pp)->number = 400;
            }
            else if ((*ComputedValue_pp)->number >= 750 && (*ComputedValue_pp)->number < 900) {
                (*ComputedValue_pp)->number = 700;
            }
            else if ((*ComputedValue_pp)->number >= 900) {
                (*ComputedValue_pp)->number = 700;
            }
        }
        else {
            NH_CORE_CHECK(nh_css_computeFontWeightAbsolute(Variables_p))
        }
    }
    else if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_NUMBER) {
        NH_CORE_CHECK(nh_css_computeFontWeightAbsolute(Variables_p))
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

// FONT STYLE ======================================================================================

static NH_API_RESULT nh_css_computeFontStyle(
    nh_css_ComputeVariables *Variables_p)
{
    if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_KEYWORD) {
        if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "normal")) {
            *(Variables_p->ComputedValue_pp) = Variables_p->SpecifiedValue_p;
        }
        else if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "italic")) {
            *(Variables_p->ComputedValue_pp) = Variables_p->SpecifiedValue_p;
        }
        else if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "oblique")) {
            *(Variables_p->ComputedValue_pp) = Variables_p->SpecifiedValue_p;
        }
        else {return NH_API_ERROR_BAD_STATE;}
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

// FONT FAMILY =====================================================================================

static NH_API_RESULT nh_css_computeFontFamily(
    nh_css_ComputeVariables *Variables_p)
{
    *(Variables_p->ComputedValue_pp) = Variables_p->SpecifiedValue_p;

    while (Variables_p->SpecifiedValue_p) {
        if (Variables_p->SpecifiedValue_p->Common.type != NH_CSS_VALUE_KEYWORD 
        &&  Variables_p->SpecifiedValue_p->Common.type != NH_CSS_VALUE_STRING) {
            return NH_API_ERROR_BAD_STATE;
        }
        Variables_p->SpecifiedValue_p = Variables_p->SpecifiedValue_p->Common.Next_p;
    }

    return NH_API_SUCCESS;
}

// BORDER STYLE ====================================================================================

static NH_API_RESULT nh_css_computeBorderStyle(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *Value_p = 
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[Variables_p->property];

    nh_css_Value *ShorthandValue_p = 
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_STYLE];

    nh_css_Value *Compare_p = nh_css_compareSpecifiedValues(ShorthandValue_p, Value_p);

    if (Compare_p == NULL || Compare_p == Value_p) {
        Variables_p->ComputedValues_p->pp[Variables_p->property] = Value_p;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_computeBorderStyleShorthand(
    nh_css_ComputeVariables *Variables_p)
{
    nh_core_Array *SpecifiedValues_p = Variables_p->SpecifiedValues_p;
    nh_core_List *ComputedValues_p = Variables_p->ComputedValues_p;

    nh_css_Value *Value_p = &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_STYLE];
    nh_css_Value *SecondValue_p = NULL;
    nh_css_Value *ThirdValue_p = NULL;
    nh_css_Value *FourthValue_p = NULL;

    if (Value_p->Common.Next_p) {
        SecondValue_p = Value_p->Common.Next_p;
    }
    if (SecondValue_p && SecondValue_p->Common.Next_p) {
        ThirdValue_p = SecondValue_p->Common.Next_p;
    }
    if (ThirdValue_p && ThirdValue_p->Common.Next_p) {
        FourthValue_p = ThirdValue_p->Common.Next_p;
    }

    bool setTop = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_TOP_STYLE], Value_p
    );
    bool setRight = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_RIGHT_STYLE], Value_p
    );
    bool setBottom = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE], Value_p
    );
    bool setLeft = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_LEFT_STYLE], Value_p
    );

    if (FourthValue_p) {
        if (setTop)    {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_TOP_STYLE] = Value_p;}
        if (setRight)  {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_RIGHT_STYLE] = SecondValue_p;}
        if (setBottom) {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE] = ThirdValue_p;}
        if (setLeft)   {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_LEFT_STYLE] = FourthValue_p;}
    }
    else if (ThirdValue_p) {
        if (setTop)    {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_TOP_STYLE] = Value_p;}
        if (setRight)  {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_RIGHT_STYLE] = SecondValue_p;}
        if (setBottom) {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE] = ThirdValue_p;}
        if (setLeft)   {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_LEFT_STYLE] = SecondValue_p;}
    }
    else if (SecondValue_p) {
        if (setTop)    {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_TOP_STYLE] = Value_p;}
        if (setRight)  {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_RIGHT_STYLE] = SecondValue_p;}
        if (setBottom) {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE] = Value_p;}
        if (setLeft)   {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_LEFT_STYLE] = SecondValue_p;}
    }
    else if (Value_p) {
        if (setTop)    {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_TOP_STYLE] = Value_p;}
        if (setRight)  {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_RIGHT_STYLE] = Value_p;}
        if (setBottom) {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE] = Value_p;}
        if (setLeft)   {ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_LEFT_STYLE] = Value_p;}
    }

    return NH_API_SUCCESS;
}

// LINE WIDTH ======================================================================================

static NH_API_RESULT nh_css_computeLineWidth(
    nh_css_ComputeVariables *Variables_p)
{
    if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_KEYWORD)
    {
        if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "thin")) {
            nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(NewValue_p)
            *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_PX);
            NewValue_p->number = 1;
            *Variables_p->ComputedValue_pp = NewValue_p;
        }
        else if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "medium")) {
            nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(NewValue_p)
            *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_PX);
            NewValue_p->number = 3;
            *Variables_p->ComputedValue_pp = NewValue_p;
        }
        else if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "thick")) {
            nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
            NH_CORE_CHECK_MEM(NewValue_p)
            *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_PX);
            NewValue_p->number = 5;
            *Variables_p->ComputedValue_pp = NewValue_p;
        }
        else {return NH_API_ERROR_BAD_STATE;}
    }
    else if (nh_css_isLengthValue(Variables_p->SpecifiedValue_p)) {
        NH_CORE_CHECK(nh_css_computeLength(Variables_p))
    } 
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

// BORDER WIDTH ====================================================================================

static NH_API_RESULT nh_css_computeBorderWidthValue(
    nh_css_ComputeVariables *Variables_p, nh_css_Value *SpecifiedValue_p, NH_CSS_PROPERTY property)
{
    nh_core_List *ComputedValues_p = Variables_p->ComputedValues_p;

    Variables_p->ComputedValue_pp = (nh_css_Value**)&ComputedValues_p->pp[property];
    Variables_p->SpecifiedValue_p = SpecifiedValue_p;

    switch (property)
    { 
        case NH_CSS_PROPERTY_BORDER_TOP_WIDTH :
        {
            nh_css_Value *BorderStyle_p = ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_TOP_STYLE];
            if (!strcmp(BorderStyle_p->String.p, "none") || !strcmp(BorderStyle_p->String.p, "hidden")) {
                nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
                NH_CORE_CHECK_MEM(NewValue_p)
                *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_PX);
                NewValue_p->number = 0;
                ComputedValues_p->pp[property] = NewValue_p;
            }
            else {nh_css_computeLineWidth(Variables_p);}
            break;
        }
        case NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH :
        {
            nh_css_Value *BorderStyle_p = ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_RIGHT_STYLE];
            if (!strcmp(BorderStyle_p->String.p, "none") || !strcmp(BorderStyle_p->String.p, "hidden")) {
                nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
                NH_CORE_CHECK_MEM(NewValue_p)
                *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_PX);
                NewValue_p->number = 0;
                ComputedValues_p->pp[property] = NewValue_p;
            }
            else {nh_css_computeLineWidth(Variables_p);}
            break;
        }
        case NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH :
        {
            nh_css_Value *BorderStyle_p = ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE];
            if (!strcmp(BorderStyle_p->String.p, "none") || !strcmp(BorderStyle_p->String.p, "hidden")) {
                nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
                NH_CORE_CHECK_MEM(NewValue_p)
                *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_PX);
                NewValue_p->number = 0;
                ComputedValues_p->pp[property] = NewValue_p;
            }
            else {nh_css_computeLineWidth(Variables_p);}
            break;
        }
        case NH_CSS_PROPERTY_BORDER_LEFT_WIDTH :
        {
            nh_css_Value *BorderStyle_p = ComputedValues_p->pp[NH_CSS_PROPERTY_BORDER_LEFT_STYLE];
            if (!strcmp(BorderStyle_p->String.p, "none") || !strcmp(BorderStyle_p->String.p, "hidden")) {
                nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
                NH_CORE_CHECK_MEM(NewValue_p)
                *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_PX);
                NewValue_p->number = 0;
                ComputedValues_p->pp[property] = NewValue_p;
            }
            else {nh_css_computeLineWidth(Variables_p);}
            break;
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_computeBorderWidth(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *Value_p = 
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[Variables_p->property];

    nh_css_Value *ShorthandValue_p = 
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_WIDTH];

    nh_css_Value *Compare_p = nh_css_compareSpecifiedValues(ShorthandValue_p, Value_p);
    if (Compare_p == ShorthandValue_p) {return NH_API_SUCCESS;}

    NH_API_RESULT result = nh_css_computeBorderWidthValue(Variables_p, Value_p, Variables_p->property);

    return result;
}

static NH_API_RESULT nh_css_computeBorderWidthShorthand(
    nh_css_ComputeVariables *Variables_p)
{
    nh_core_Array *SpecifiedValues_p = Variables_p->SpecifiedValues_p;
    nh_core_List *ComputedValues_p = Variables_p->ComputedValues_p;

    nh_css_Value *Value_p = &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_WIDTH];
    nh_css_Value *SecondValue_p = NULL;
    nh_css_Value *ThirdValue_p = NULL;
    nh_css_Value *FourthValue_p = NULL;

    if (Value_p->Common.Next_p) {
        SecondValue_p = Value_p->Common.Next_p;
    }
    if (SecondValue_p && SecondValue_p->Common.Next_p) {
        ThirdValue_p = SecondValue_p->Common.Next_p;
    }
    if (ThirdValue_p && ThirdValue_p->Common.Next_p) {
        FourthValue_p = ThirdValue_p->Common.Next_p;
    }

    bool setTop = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_TOP_WIDTH], Value_p
    );
    bool setRight = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH], Value_p
    );
    bool setBottom = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH], Value_p
    );
    bool setLeft = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_LEFT_WIDTH], Value_p
    );

    if (FourthValue_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, Value_p, NH_CSS_PROPERTY_BORDER_TOP_WIDTH))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, ThirdValue_p, NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH))}
        if (setLeft)   {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, FourthValue_p, NH_CSS_PROPERTY_BORDER_LEFT_WIDTH))}
    }
    else if (ThirdValue_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, Value_p, NH_CSS_PROPERTY_BORDER_TOP_WIDTH))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, ThirdValue_p, NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH))}
        if (setLeft)   {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_BORDER_LEFT_WIDTH))}
    }
    else if (SecondValue_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, Value_p, NH_CSS_PROPERTY_BORDER_TOP_WIDTH))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, Value_p, NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH))} 
        if (setLeft)   {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_BORDER_LEFT_WIDTH))}
    }
    else if (Value_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, Value_p, NH_CSS_PROPERTY_BORDER_TOP_WIDTH))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, Value_p, NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, Value_p, NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH))}
        if (setLeft)   {NH_CORE_CHECK(nh_css_computeBorderWidthValue(Variables_p, Value_p, NH_CSS_PROPERTY_BORDER_LEFT_WIDTH))}
    }

    return NH_API_SUCCESS;
}

// BORDER RADIUS ===================================================================================

static NH_API_RESULT nh_css_computeBorderRadiusValue(
    nh_css_ComputeVariables *Variables_p)
{
    if (nh_css_isLengthValue(Variables_p->SpecifiedValue_p)) {
        NH_CORE_CHECK(nh_css_computeLength(Variables_p))
    } 
    else if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_PERCENTAGE) {
        *(Variables_p->ComputedValue_pp) = Variables_p->SpecifiedValue_p;
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_computeBorderRadius(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *ShorthandValue_p = 
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_BORDER_RADIUS];

    nh_css_Value *Compare_p = 
        nh_css_compareSpecifiedValues(ShorthandValue_p, Variables_p->SpecifiedValue_p);

    if (Compare_p == ShorthandValue_p) {return NH_API_SUCCESS;}

    return nh_css_computeBorderRadiusValue(Variables_p);
}

// BORDER ==========================================================================================

static NH_API_RESULT nh_css_computeBorderShorthand(
    nh_css_ComputeVariables *Variables_p)
{
    return NH_API_SUCCESS;
}

// PADDING =========================================================================================

static NH_API_RESULT nh_css_computePaddingValue(
    nh_css_ComputeVariables *Variables_p, nh_css_Value *Value_p, NH_CSS_PROPERTY property)
{
    Variables_p->ComputedValue_pp = (nh_css_Value**)&Variables_p->ComputedValues_p->pp[property];
    Variables_p->SpecifiedValue_p = Value_p;
    Variables_p->property = property;

    if (nh_css_isLengthValue(Value_p)) {
        NH_CORE_CHECK(nh_css_computeLength(Variables_p))
    } 
    else if (Value_p->Common.type == NH_CSS_VALUE_PERCENTAGE) {
        *(Variables_p->ComputedValue_pp) = Value_p;
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_computePadding(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *ShorthandValue_p = 
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_PADDING];

    nh_css_Value *Compare_p = 
        nh_css_compareSpecifiedValues(ShorthandValue_p, Variables_p->SpecifiedValue_p);

    if (Compare_p == ShorthandValue_p) {return NH_API_SUCCESS;}

    NH_API_RESULT result = 
        nh_css_computePaddingValue(Variables_p, Variables_p->SpecifiedValue_p, Variables_p->property);

    return result;
}

static NH_API_RESULT nh_css_computePaddingShorthand(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *Value_p = &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_PADDING];
    nh_css_Value *SecondValue_p = NULL;
    nh_css_Value *ThirdValue_p = NULL;
    nh_css_Value *FourthValue_p = NULL;

    if (Value_p->Common.Next_p) {
        SecondValue_p = Value_p->Common.Next_p;
    }
    if (SecondValue_p && SecondValue_p->Common.Next_p) {
        ThirdValue_p = SecondValue_p->Common.Next_p;
    }
    if (ThirdValue_p && ThirdValue_p->Common.Next_p) {
        FourthValue_p = ThirdValue_p->Common.Next_p;
    }

    bool setTop = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_PADDING_TOP], Value_p
    );
    bool setRight = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_PADDING_RIGHT], Value_p
    );
    bool setBottom = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_PADDING_BOTTOM], Value_p
    );
    bool setLeft = Value_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_PADDING_LEFT], Value_p
    );

    if (FourthValue_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, Value_p, NH_CSS_PROPERTY_PADDING_TOP))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_PADDING_RIGHT))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, ThirdValue_p, NH_CSS_PROPERTY_PADDING_BOTTOM))}
        if (setLeft)   {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, FourthValue_p, NH_CSS_PROPERTY_PADDING_LEFT))}
    }
    else if (ThirdValue_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, Value_p, NH_CSS_PROPERTY_PADDING_TOP))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_PADDING_RIGHT))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, ThirdValue_p, NH_CSS_PROPERTY_PADDING_BOTTOM))}
        if (setLeft)   {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_PADDING_LEFT))}
    }
    else if (SecondValue_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, Value_p, NH_CSS_PROPERTY_PADDING_TOP))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_PADDING_RIGHT))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, Value_p, NH_CSS_PROPERTY_PADDING_BOTTOM))} 
        if (setLeft)   {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, SecondValue_p, NH_CSS_PROPERTY_PADDING_LEFT))}
    }
    else if (Value_p) {
        if (setTop)    {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, Value_p, NH_CSS_PROPERTY_PADDING_TOP))}
        if (setRight)  {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, Value_p, NH_CSS_PROPERTY_PADDING_RIGHT))}
        if (setBottom) {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, Value_p, NH_CSS_PROPERTY_PADDING_BOTTOM))}
        if (setLeft)   {NH_CORE_CHECK(nh_css_computePaddingValue(Variables_p, Value_p, NH_CSS_PROPERTY_PADDING_LEFT))}
    }

    return NH_API_SUCCESS;
}

// TEXT ALIGN ======================================================================================

static NH_API_RESULT nh_css_computeTextAlign(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *ShorthandValue_p = 
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_TEXT_ALIGN];

    nh_css_Value *Compare_p = 
        nh_css_compareSpecifiedValues(ShorthandValue_p, Variables_p->SpecifiedValue_p);

    if (Compare_p == ShorthandValue_p) {return NH_API_SUCCESS;}

    Variables_p->ComputedValues_p->pp[Variables_p->property] = Variables_p->SpecifiedValue_p;

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_computeTextAlignShorthand(
    nh_css_ComputeVariables *Variables_p)
{
    nh_css_Value *ShorthandValue_p = 
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_TEXT_ALIGN];

    Variables_p->ComputedValues_p->pp[NH_CSS_PROPERTY_TEXT_ALIGN] = ShorthandValue_p;

    bool setAll = ShorthandValue_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_TEXT_ALIGN_ALL], ShorthandValue_p
    );
    bool setLast = ShorthandValue_p == nh_css_compareSpecifiedValues(
        &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_TEXT_ALIGN_LAST], ShorthandValue_p
    );

    if (ShorthandValue_p) {
        if (setAll)  {Variables_p->ComputedValues_p->pp[NH_CSS_PROPERTY_TEXT_ALIGN_ALL] = ShorthandValue_p;}
        if (setLast) {
            Variables_p->ComputedValues_p->pp[NH_CSS_PROPERTY_TEXT_ALIGN_LAST] = 
                &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[NH_CSS_PROPERTY_TEXT_ALIGN_LAST];
        }
    }

    return NH_API_SUCCESS;
}

// SIZE ============================================================================================

static NH_API_RESULT nh_css_computeSize(
    nh_css_ComputeVariables *Variables_p)
{
    if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_PERCENTAGE) {
      *(Variables_p->ComputedValue_pp) = Variables_p->SpecifiedValue_p;
    }
    else if (nh_css_isLengthValue(Variables_p->SpecifiedValue_p)) {
        NH_CORE_CHECK(nh_css_computeLength(Variables_p))
    }
    else if (Variables_p->SpecifiedValue_p->Common.type == NH_CSS_VALUE_KEYWORD)
    {
        if (Variables_p->property == NH_CSS_PROPERTY_MAX_WIDTH || Variables_p->property == NH_CSS_PROPERTY_MAX_HEIGHT) {
            if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "none")) {
                *Variables_p->ComputedValue_pp = Variables_p->SpecifiedValue_p;
                return NH_API_SUCCESS;
            }
        }
        else {
            if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "auto")) {
                *Variables_p->ComputedValue_pp = Variables_p->SpecifiedValue_p;
                return NH_API_SUCCESS;
            }
        }
        if (!strcmp(Variables_p->SpecifiedValue_p->String.p, "min-content")
        ||  !strcmp(Variables_p->SpecifiedValue_p->String.p, "max-content")) {
            *Variables_p->ComputedValue_pp = Variables_p->SpecifiedValue_p;
        }
        else {return NH_API_ERROR_BAD_STATE;}
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

// BLOCKIFY ========================================================================================

static NH_API_RESULT nh_css_blockify(
    nh_dom_Node *Node_p, nh_core_List *ComputedValues_p)
{
    nh_css_Value *Display_p = ComputedValues_p->pp[NH_CSS_PROPERTY_DISPLAY];

    if (strcmp(Display_p->String.p, "block")) 
    {
        if (Display_p->Common.scope == NH_CSS_VALUE_SCOPE_COMPUTED) {puts("nh_css_blockify");exit(0);} // TODO

        nh_css_Value *NewValue_p = nh_core_allocate(sizeof(nh_css_Value));
        NH_CORE_CHECK_MEM(NewValue_p)
        *NewValue_p = nh_css_initValue(NH_CSS_VALUE_SCOPE_COMPUTED, NH_CSS_VALUE_KEYWORD);
        NewValue_p->String = nh_core_initString(6);
        nh_core_appendToString(&NewValue_p->String, "block", 5);
        ComputedValues_p->pp[NH_CSS_PROPERTY_DISPLAY] = NewValue_p;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_blockifyNode(
    nh_dom_Node *Node_p, nh_core_List *ComputedValues_p)
{
    nh_css_Value *Float_p = ComputedValues_p->pp[NH_CSS_PROPERTY_FLOAT];

    // The root element’s display type is always blockified.
    if (nh_dom_getParentElement(Node_p) == NULL) {
        nh_css_blockify(Node_p, ComputedValues_p);
    }

    // Absolute positioning or floating an element blockifies the box’s display type.
    else if (strcmp(Float_p->String.p, "none")) {
        nh_css_blockify(Node_p, ComputedValues_p);
    }

    return NH_API_SUCCESS;
}

// COMPUTE =========================================================================================

static NH_API_RESULT nh_css_computeSpecifiedValue(
    nh_css_ComputeVariables *Variables_p, NH_CSS_PROPERTY property)
{
    Variables_p->property = property;
    Variables_p->ComputedValue_pp = (nh_css_Value**)&Variables_p->ComputedValues_p->pp[property];
    Variables_p->SpecifiedValue_p = &((nh_css_Value*)Variables_p->SpecifiedValues_p->p)[property];

    switch (Variables_p->SpecifiedValue_p->Common.type)
    {
        case NH_CSS_VALUE_UNDEFINED :
            break;
        case NH_CSS_VALUE_KEYWORD_INITIAL :
            break;
        case NH_CSS_VALUE_KEYWORD_INHERIT :
            break;
        case NH_CSS_VALUE_KEYWORD_UNSET :
            break;
        case NH_CSS_VALUE_KEYWORD_REVERT :
            break;
    }

    switch (property) 
    {
        case NH_CSS_PROPERTY_AZIMUTH                    : break;
        case NH_CSS_PROPERTY_BACKGROUND_ATTACHMENT      : break;
        case NH_CSS_PROPERTY_BACKGROUND_COLOR           : return nh_css_computeColorUnit(Variables_p);
        case NH_CSS_PROPERTY_BACKGROUND_IMAGE           : break;
        case NH_CSS_PROPERTY_BACKGROUND_POSITION        : break;
        case NH_CSS_PROPERTY_BACKGROUND_REPEAT          : break;
        case NH_CSS_PROPERTY_BACKGROUND                 : break;
        case NH_CSS_PROPERTY_BORDER_COLLAPSE            : break;
        case NH_CSS_PROPERTY_BORDER_COLOR               : break;
        case NH_CSS_PROPERTY_BORDER_SPACING             : break;
        case NH_CSS_PROPERTY_BORDER_STYLE               : return nh_css_computeBorderStyleShorthand(Variables_p);
        case NH_CSS_PROPERTY_BORDER_RADIUS              : break;
        case NH_CSS_PROPERTY_BORDER_TOP                 : break;
        case NH_CSS_PROPERTY_BORDER_RIGHT               : break;
        case NH_CSS_PROPERTY_BORDER_BOTTOM              : break;
        case NH_CSS_PROPERTY_BORDER_LEFT                : break;
        case NH_CSS_PROPERTY_BORDER_TOP_COLOR           : return nh_css_computeColorUnit(Variables_p);
        case NH_CSS_PROPERTY_BORDER_RIGHT_COLOR         : return nh_css_computeColorUnit(Variables_p);
        case NH_CSS_PROPERTY_BORDER_BOTTOM_COLOR        : return nh_css_computeColorUnit(Variables_p);
        case NH_CSS_PROPERTY_BORDER_LEFT_COLOR          : return nh_css_computeColorUnit(Variables_p);
        case NH_CSS_PROPERTY_BORDER_TOP_STYLE           : return nh_css_computeBorderStyle(Variables_p); 
        case NH_CSS_PROPERTY_BORDER_RIGHT_STYLE         : return nh_css_computeBorderStyle(Variables_p); 
        case NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE        : return nh_css_computeBorderStyle(Variables_p); 
        case NH_CSS_PROPERTY_BORDER_LEFT_STYLE          : return nh_css_computeBorderStyle(Variables_p); 
        case NH_CSS_PROPERTY_BORDER_TOP_WIDTH           : return nh_css_computeBorderWidth(Variables_p); 
        case NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH         : return nh_css_computeBorderWidth(Variables_p); 
        case NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH        : return nh_css_computeBorderWidth(Variables_p); 
        case NH_CSS_PROPERTY_BORDER_LEFT_WIDTH          : return nh_css_computeBorderWidth(Variables_p); 
        case NH_CSS_PROPERTY_BORDER_WIDTH               : return nh_css_computeBorderWidthShorthand(Variables_p); 
        case NH_CSS_PROPERTY_BORDER_TOP_LEFT_RADIUS     : return nh_css_computeBorderRadius(Variables_p);
        case NH_CSS_PROPERTY_BORDER_TOP_RIGHT_RADIUS    : return nh_css_computeBorderRadius(Variables_p);
        case NH_CSS_PROPERTY_BORDER_BOTTOM_LEFT_RADIUS  : return nh_css_computeBorderRadius(Variables_p);
        case NH_CSS_PROPERTY_BORDER_BOTTOM_RIGHT_RADIUS : return nh_css_computeBorderRadius(Variables_p);
        case NH_CSS_PROPERTY_BORDER                     : return nh_css_computeBorderShorthand(Variables_p);
        case NH_CSS_PROPERTY_BOTTOM                     : break;
        case NH_CSS_PROPERTY_CAPTION_SIDE               : break;
        case NH_CSS_PROPERTY_CLEAR                      : break;
        case NH_CSS_PROPERTY_CLIP                       : break;
        case NH_CSS_PROPERTY_COLOR                      : return nh_css_computeColorUnit(Variables_p);
        case NH_CSS_PROPERTY_CONTENT                    : break;
        case NH_CSS_PROPERTY_COUNTER_INCREMENT          : break;
        case NH_CSS_PROPERTY_COUNTER_RESET              : break;
        case NH_CSS_PROPERTY_CUE_AFTER                  : break;
        case NH_CSS_PROPERTY_CUE_BEFORE                 : break;
        case NH_CSS_PROPERTY_CUE                        : break;
        case NH_CSS_PROPERTY_CURSOR                     : break;
        case NH_CSS_PROPERTY_DIRECTION                  : break;
        case NH_CSS_PROPERTY_DISPLAY                    : break;
        case NH_CSS_PROPERTY_ELEVATION                  : break;
        case NH_CSS_PROPERTY_EMPTY_CELLS                : break;
        case NH_CSS_PROPERTY_FLOAT                      : break;
        case NH_CSS_PROPERTY_FONT_FAMILY                : return nh_css_computeFontFamily(Variables_p);
        case NH_CSS_PROPERTY_FONT_SIZE                  : return nh_css_computeFontSize(Variables_p);
        case NH_CSS_PROPERTY_FONT_STYLE                 : return nh_css_computeFontStyle(Variables_p);
        case NH_CSS_PROPERTY_FONT_VARIANT               : break;
        case NH_CSS_PROPERTY_FONT_WEIGHT                : return nh_css_computeFontWeight(Variables_p);
        case NH_CSS_PROPERTY_FONT                       : break;
        case NH_CSS_PROPERTY_HEIGHT                     : return nh_css_computeSize(Variables_p);
        case NH_CSS_PROPERTY_LEFT                       : break;
        case NH_CSS_PROPERTY_LETTER_SPACING             : break;
        case NH_CSS_PROPERTY_LINE_HEIGHT                : break;
        case NH_CSS_PROPERTY_LIST_STYLE_IMAGE           : break;
        case NH_CSS_PROPERTY_LIST_STYLE_POSITION        : break;
        case NH_CSS_PROPERTY_LIST_STYLE_TYPE            : break;
        case NH_CSS_PROPERTY_LIST_STYLE                 : break;
        case NH_CSS_PROPERTY_MARGIN_RIGHT               : return nh_css_computeMargin(Variables_p);
        case NH_CSS_PROPERTY_MARGIN_LEFT                : return nh_css_computeMargin(Variables_p);
        case NH_CSS_PROPERTY_MARGIN_TOP                 : return nh_css_computeMargin(Variables_p);
        case NH_CSS_PROPERTY_MARGIN_BOTTOM              : return nh_css_computeMargin(Variables_p);
        case NH_CSS_PROPERTY_MARGIN                     : return nh_css_computeMarginShorthand(Variables_p); 
        case NH_CSS_PROPERTY_MARGIN_BLOCK_START         : return nh_css_computeMarginBlockOrMarginInline(Variables_p);
        case NH_CSS_PROPERTY_MARGIN_BLOCK_END           : return nh_css_computeMarginBlockOrMarginInline(Variables_p);
        case NH_CSS_PROPERTY_MARGIN_INLINE_START        : return nh_css_computeMarginBlockOrMarginInline(Variables_p);
        case NH_CSS_PROPERTY_MARGIN_INLINE_END          : return nh_css_computeMarginBlockOrMarginInline(Variables_p);
        case NH_CSS_PROPERTY_MARGIN_BLOCK               : return nh_css_computeMarginBlockShorthandOrMarginInlineShorthand(Variables_p);
        case NH_CSS_PROPERTY_MARGIN_INLINE              : return nh_css_computeMarginBlockShorthandOrMarginInlineShorthand(Variables_p);
        case NH_CSS_PROPERTY_MAX_HEIGHT                 : return nh_css_computeSize(Variables_p);
        case NH_CSS_PROPERTY_MAX_WIDTH                  : return nh_css_computeSize(Variables_p);
        case NH_CSS_PROPERTY_MIN_HEIGHT                 : return nh_css_computeSize(Variables_p);
        case NH_CSS_PROPERTY_MIN_WIDTH                  : return nh_css_computeSize(Variables_p);
        case NH_CSS_PROPERTY_ORPHANS                    : break;
        case NH_CSS_PROPERTY_OUTLINE_COLOR              : break;
        case NH_CSS_PROPERTY_OUTLINE_STYLE              : break;
        case NH_CSS_PROPERTY_OUTLINE_WIDTH              : break;
        case NH_CSS_PROPERTY_OUTLINE                    : break;
        case NH_CSS_PROPERTY_OVERFLOW                   : break;
        case NH_CSS_PROPERTY_PADDING_TOP                : return nh_css_computePadding(Variables_p);
        case NH_CSS_PROPERTY_PADDING_RIGHT              : return nh_css_computePadding(Variables_p);
        case NH_CSS_PROPERTY_PADDING_BOTTOM             : return nh_css_computePadding(Variables_p);
        case NH_CSS_PROPERTY_PADDING_LEFT               : return nh_css_computePadding(Variables_p);
        case NH_CSS_PROPERTY_PADDING                    : return nh_css_computePaddingShorthand(Variables_p); 
        case NH_CSS_PROPERTY_PAGE_BREAK_AFTER           : break;
        case NH_CSS_PROPERTY_PAGE_BREAK_BEFORE          : break;
        case NH_CSS_PROPERTY_PAGE_BREAK_INSIDE          : break;
        case NH_CSS_PROPERTY_PAUSE_AFTER                : break;
        case NH_CSS_PROPERTY_PAUSE_BEFORE               : break;
        case NH_CSS_PROPERTY_PAUSE                      : break;
        case NH_CSS_PROPERTY_PITCH_RANGE                : break;
        case NH_CSS_PROPERTY_PITCH                      : break;
        case NH_CSS_PROPERTY_PLAY_DURING                : break;
        case NH_CSS_PROPERTY_POSITION                   : break;
        case NH_CSS_PROPERTY_QUOTES                     : break;
        case NH_CSS_PROPERTY_RICHNESS                   : break;
        case NH_CSS_PROPERTY_RIGHT                      : break;
        case NH_CSS_PROPERTY_SPEAK_HEADER               : break;
        case NH_CSS_PROPERTY_SPEAK_NUMERAL              : break;
        case NH_CSS_PROPERTY_SPEAK_PUNCTUATION          : break; 
        case NH_CSS_PROPERTY_SPEAK                      : break;
        case NH_CSS_PROPERTY_SPEECH_RATE                : break;
        case NH_CSS_PROPERTY_STRESS                     : break;
        case NH_CSS_PROPERTY_TABLE_LAYOUT               : break;
        case NH_CSS_PROPERTY_TEXT_ALIGN                 : return nh_css_computeTextAlignShorthand(Variables_p); 
        case NH_CSS_PROPERTY_TEXT_ALIGN_ALL             : return nh_css_computeTextAlign(Variables_p); 
        case NH_CSS_PROPERTY_TEXT_ALIGN_LAST            : return nh_css_computeTextAlign(Variables_p); 
        case NH_CSS_PROPERTY_TEXT_DECORATION            : break;
        case NH_CSS_PROPERTY_TEXT_INDENT                : break;
        case NH_CSS_PROPERTY_TEXT_ORIENTATION           : break;
        case NH_CSS_PROPERTY_TEXT_TRANSFORM             : break;
        case NH_CSS_PROPERTY_TOP                        : break;
        case NH_CSS_PROPERTY_UNICODE_BIDI               : break;
        case NH_CSS_PROPERTY_VERTICAL_ALIGN             : break;
        case NH_CSS_PROPERTY_VISIBILITY                 : break;
        case NH_CSS_PROPERTY_VOICE_FAMILY               : break;
        case NH_CSS_PROPERTY_VOLUME                     : break;
        case NH_CSS_PROPERTY_WHITE_SPACE                : break;
        case NH_CSS_PROPERTY_WIDOWS                     : break;
        case NH_CSS_PROPERTY_WIDTH                      : return nh_css_computeSize(Variables_p); 
        case NH_CSS_PROPERTY_WORD_BREAK                 : break; 
        case NH_CSS_PROPERTY_WORD_SPACING               : break; 
        case NH_CSS_PROPERTY_WRITING_MODE               : break;
        case NH_CSS_PROPERTY_Z_INDEX                    : break;
    }

    *(Variables_p->ComputedValue_pp) = Variables_p->SpecifiedValue_p;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_computeSpecifiedValues(
    nh_css_LogContext *LogContext_p, nh_dom_Node *Node_p)
{
    nh_core_List ComputedValues = nh_core_initList(NH_CSS_PROPERTY_COUNT);
    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) {
        nh_core_appendToList(&ComputedValues, NULL);
    }

    nh_core_Array *SpecifiedValues_p = nh_dom_getSpecifiedPropertyValues(Node_p);
    NH_CORE_CHECK_NULL(SpecifiedValues_p)

    nh_css_ComputeVariables Variables;
    Variables.SpecifiedValues_p = SpecifiedValues_p;
    Variables.ComputedValues_p = &ComputedValues;
    Variables.Node_p = Node_p;

    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_WRITING_MODE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_DIRECTION))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_TEXT_ORIENTATION))

    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_AZIMUTH))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BACKGROUND_ATTACHMENT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BACKGROUND_COLOR))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BACKGROUND_IMAGE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BACKGROUND_POSITION))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BACKGROUND_REPEAT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BACKGROUND))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_COLLAPSE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_COLOR))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_SPACING))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_STYLE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_TOP))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_RIGHT))          
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_BOTTOM))        
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_LEFT))       
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_TOP_COLOR)) 
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_RIGHT_COLOR))  
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_BOTTOM_COLOR))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_LEFT_COLOR))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_TOP_STYLE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_RIGHT_STYLE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_LEFT_STYLE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_TOP_WIDTH))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_LEFT_WIDTH))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_WIDTH))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_TOP_LEFT_RADIUS))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_TOP_RIGHT_RADIUS))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_BOTTOM_LEFT_RADIUS))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER_BOTTOM_RIGHT_RADIUS))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BORDER))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_BOTTOM))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_CAPTION_SIDE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_CLEAR))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_CLIP))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_COLOR))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_CONTENT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_COUNTER_INCREMENT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_COUNTER_RESET))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_CUE_AFTER))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_CUE_BEFORE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_CUE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_CURSOR))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_DISPLAY))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_ELEVATION))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_EMPTY_CELLS))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_FLOAT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_FONT_FAMILY))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_FONT_SIZE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_FONT_STYLE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_FONT_VARIANT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_FONT_WEIGHT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_FONT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_HEIGHT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_LEFT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_LETTER_SPACING))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_LINE_HEIGHT))      
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_LIST_STYLE_IMAGE)) 
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_LIST_STYLE_POSITION))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_LIST_STYLE_TYPE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_LIST_STYLE))        
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_RIGHT))         
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_LEFT))          
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_TOP))       
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_BOTTOM))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN))    
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_BLOCK_START))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_BLOCK_END))  
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_INLINE_START))   
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_INLINE_END)) 
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_BLOCK))   
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MARGIN_INLINE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MAX_HEIGHT))       
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MAX_WIDTH))          
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MIN_HEIGHT))           
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_MIN_WIDTH))          
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_ORPHANS))           
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_OUTLINE_COLOR)) 
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_OUTLINE_STYLE))       
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_OUTLINE_WIDTH))       
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_OUTLINE))       
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_OVERFLOW))             
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PADDING_TOP))           
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PADDING_RIGHT))         
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PADDING_BOTTOM))       
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PADDING_LEFT))      
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PADDING))        
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PAGE_BREAK_AFTER))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PAGE_BREAK_BEFORE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PAGE_BREAK_INSIDE))   
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PAUSE_AFTER))   
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PAUSE_BEFORE))         
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PAUSE))        
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PITCH_RANGE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PITCH))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_PLAY_DURING))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_POSITION))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_QUOTES))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_RICHNESS))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_RIGHT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_SPEAK_HEADER))  
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_SPEAK_NUMERAL))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_SPEAK_PUNCTUATION))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_SPEAK))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_SPEECH_RATE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_STRESS))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_TABLE_LAYOUT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_TEXT_ALIGN))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_TEXT_ALIGN_ALL))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_TEXT_ALIGN_LAST))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_TEXT_DECORATION))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_TEXT_INDENT))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_TEXT_TRANSFORM))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_TOP))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_UNICODE_BIDI))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_VERTICAL_ALIGN))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_VISIBILITY))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_VOICE_FAMILY)) 
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_VOLUME))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_WHITE_SPACE))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_WIDOWS))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_WIDTH))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_WORD_BREAK))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_WORD_SPACING))
    NH_CORE_CHECK(nh_css_computeSpecifiedValue(&Variables, NH_CSS_PROPERTY_Z_INDEX))

    nh_css_blockifyNode(Node_p, &ComputedValues);
    nh_dom_setComputedPropertyValues(Node_p, ComputedValues);

    if (LogContext_p) {nh_css_logComputedValues(LogContext_p, &ComputedValues);}

    return NH_API_SUCCESS;
}

// FREE ============================================================================================

NH_API_RESULT nh_css_freeComputedValues(
    nh_dom_Node *Node_p)
{
    nh_core_List *ComputedValues_p = nh_dom_getComputedPropertyValues(Node_p);
    NH_CORE_CHECK_NULL(ComputedValues_p)

    for (int i = 0; i < ComputedValues_p->size; ++i) {
        nh_css_Value *Value_p = ComputedValues_p->pp[i];
        if (Value_p->Common.scope == NH_CSS_VALUE_SCOPE_COMPUTED) {
            if (nh_css_isStringValue(Value_p)) {
                nh_encoding_freeUTF8(&Value_p->String);
            }
        }
    }

    nh_core_freeList(ComputedValues_p, false);

    return NH_API_SUCCESS;
}
