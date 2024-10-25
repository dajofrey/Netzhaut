// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "StyleRule.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// API =============================================================================================

nh_css_StyleRuleObject *nh_css_getStyleRule(
    nh_webidl_Object *Object_p)
{
    return (nh_css_StyleRuleObject*)nh_webidl_getObject(Object_p, "CSS", "CSSStyleRule");
}

nh_css_SelectorParseNode *nh_css_getStyleRuleSelectors(
    nh_css_StyleRuleObject *StyleRule_p)
{
    return ((nh_webidl_Object*)StyleRule_p)->internal_p;
}

nh_css_StyleDeclarationObject *nh_css_getStyleRuleDeclaration(
    nh_css_StyleRuleObject *StyleRule_p)
{
    return ((nh_webidl_Object*)StyleRule_p)->Attributes.pp[0];
}

nh_css_StyleRuleObject *nh_css_createStyleRule(
    nh_css_SelectorParseNode *Selectors_p, nh_css_StyleDeclarationObject *StyleDeclaration_p)
{
    nh_webidl_Object *StyleRule_p = nh_webidl_createObject("CSS", "CSSStyleRule");
    NH_CORE_CHECK_MEM_2(NULL, StyleRule_p)

    StyleRule_p->internal_p = Selectors_p;
    StyleRule_p->Attributes.pp[0] = StyleDeclaration_p;

    return (nh_css_StyleRuleObject*)StyleRule_p;
}
