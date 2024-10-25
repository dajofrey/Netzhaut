// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Rule.h"

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
    Rule_p->internal_p = nh_core_allocate(sizeof(nh_css_Rule));
    NH_CORE_CHECK_MEM(Rule_p->internal_p)

    return NH_API_SUCCESS;
}

// API =============================================================================================

nh_css_RuleObject *nh_css_getRule(
    nh_webidl_Object *Object_p)
{
    return (nh_css_RuleObject*)nh_webidl_getObject(Object_p, "CSS", "CSSRule");
}

void nh_css_setRuleData(
    nh_css_RuleObject *Rule_p, nh_css_Rule Rule)
{
    *((nh_css_Rule*)((nh_webidl_Object*)Rule_p)->internal_p) = Rule;
    return;
}

nh_css_Rule *nh_css_getRuleData(
    nh_css_RuleObject *Rule_p)
{
    return (nh_css_Rule*)((nh_webidl_Object*)Rule_p)->internal_p;
}

