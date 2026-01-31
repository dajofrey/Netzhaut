// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "CSSRule.h"

#include "../Common/Macros.h"
#include NH_CSS_FLOW
#include NH_CSS_DEFAULT_CHECK

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

typedef struct nh_css_Rule {
    NH_CSS_RULE type;
    nh_encoding_UTF32String *Name_p;
    nh_core_Array Prelude;
    nh_css_SimpleBlock Block;
} nh_css_Rule;

// INITIALIZE ======================================================================================

NH_API_RESULT nh_css_initializeCSSRule(
    nh_ecmascript_Object *CSSRule_p)
{
NH_CSS_BEGIN()

    CSSRule_p->internal_p = nh_core_allocate(sizeof(nh_css_Rule));
    NH_CORE_CHECK_MEM(CSSRule_p->internal_p)

NH_CSS_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// API =============================================================================================

nh_ecmascript_Object *nh_css_getCSSRule(
    nh_ecmascript_Object *Object_p)
{
NH_CSS_BEGIN()
NH_CSS_END((nh_ecmascript_Object*)nh_webidl_getObject(Object_p, "CSS", "CSSRule"))
}

void nh_css_setCSSRuleType(
    nh_ecmascript_Object *CSSRule_p, NH_CSS_RULE type)
{
NH_CSS_BEGIN()

    ((nh_css_Rule*)((nh_ecmascript_Object*)CSSRule_p)->internal_p)->type = type;

NH_CSS_SILENT_END()
}

void nh_css_setCSSRuleName(
    nh_ecmascript_Object *CSSRule_p, nh_encoding_UTF32String *Name_p)
{
NH_CSS_BEGIN()

    ((nh_css_Rule*)((nh_ecmascript_Object*)CSSRule_p)->internal_p)->Name_p = Name_p;

NH_CSS_SILENT_END()
}

void nh_css_setCSSRulePrelude(
    nh_ecmascript_Object *CSSRule_p, nh_core_Array Prelude)
{
NH_CSS_BEGIN()

    ((nh_css_Rule*)((nh_ecmascript_Object*)CSSRule_p)->internal_p)->Prelude = Prelude;

NH_CSS_SILENT_END()
}

void nh_css_setCSSRuleBlock(
    nh_ecmascript_Object *CSSRule_p, nh_css_SimpleBlock Block)
{
NH_CSS_BEGIN()

    ((nh_css_Rule*)((nh_ecmascript_Object*)CSSRule_p)->internal_p)->Block = Block;

NH_CSS_SILENT_END()
}

NH_CSS_RULE nh_css_getCSSRuleType(
    nh_ecmascript_Object *CSSRule_p)
{
NH_CSS_BEGIN()
NH_CSS_END(((nh_css_Rule*)((nh_ecmascript_Object*)CSSRule_p)->internal_p)->type)
}

nh_encoding_UTF32String *nh_css_getCSSRuleName(
    nh_ecmascript_Object *CSSRule_p)
{
NH_CSS_BEGIN()
NH_CSS_END(((nh_css_Rule*)((nh_ecmascript_Object*)CSSRule_p)->internal_p)->Name_p)
}

nh_core_Array *nh_css_getCSSRulePrelude(
    nh_ecmascript_Object *CSSRule_p)
{
NH_CSS_BEGIN()
NH_CSS_END(&((nh_css_Rule*)((nh_ecmascript_Object*)CSSRule_p)->internal_p)->Prelude)
}

nh_css_SimpleBlock *nh_css_getCSSRuleBlock(
    nh_ecmascript_Object *CSSRule_p)
{
NH_CSS_BEGIN()
NH_CSS_END(&((nh_css_Rule*)((nh_ecmascript_Object*)CSSRule_p)->internal_p)->Block)
}

