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

// FUNCTIONS =======================================================================================

nh_css_SelectorParseNode *nh_css_getCSSStyleRuleSelectors(
    nh_ecmascript_Object *CSSStyleRule_p)
{
    return CSSStyleRule_p->internal_p;
}

nh_ecmascript_Object *nh_css_getStyleRuleDeclaration(
    nh_ecmascript_Object *CSSStyleRule_p)
{
    return CSSStyleRule_p->Attributes.pp[0];
}

nh_ecmascript_Object *nh_css_createStyleRule(
    nh_css_SelectorParseNode *Selectors_p, nh_ecmascript_Object *StyleDeclaration_p)
{
    nh_ecmascript_Object *CSSStyleRule_p = nh_webidl_createObject("CSS", "CSSStyleRule");
    NH_CORE_CHECK_MEM_2(NULL, CSSStyleRule_p)

    CSSStyleRule_p->internal_p = Selectors_p;
    CSSStyleRule_p->Attributes.pp[0] = StyleDeclaration_p;

    return CSSStyleRule_p;
}
