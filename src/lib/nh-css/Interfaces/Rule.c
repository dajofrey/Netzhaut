// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Rule.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_API_RESULT nh_css_initializeCSSRule(
    nh_webidl_Object *Rule_p)
{
NH_CSS_BEGIN()

    Rule_p->internal_p = nh_core_allocate(sizeof(nh_css_Rule));
    NH_CSS_CHECK_MEM(Rule_p->internal_p)

NH_CSS_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// API =============================================================================================

nh_css_RuleObject *nh_css_getRule(
    nh_webidl_Object *Object_p)
{
NH_CSS_BEGIN()
NH_CSS_END((nh_css_RuleObject*)nh_webidl_getObject(Object_p, "CSS", "CSSRule"))
}

void nh_css_setRuleData(
    nh_css_RuleObject *Rule_p, nh_css_Rule Rule)
{
NH_CSS_BEGIN()

    *((nh_css_Rule*)((nh_webidl_Object*)Rule_p)->internal_p) = Rule;

NH_CSS_SILENT_END()
}

nh_css_Rule *nh_css_getRuleData(
    nh_css_RuleObject *Rule_p)
{
NH_CSS_BEGIN()
NH_CSS_END((nh_css_Rule*)((nh_webidl_Object*)Rule_p)->internal_p)
}

