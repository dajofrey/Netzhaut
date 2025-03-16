#ifndef NH_CSS_INTERFACES_STYLE_RULE_H
#define NH_CSS_INTERFACES_STYLE_RULE_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"
#include "../Parser/SelectorParser.h"

// FUNCTIONS =======================================================================================

nh_css_SelectorParseNode *nh_css_getCSSStyleRuleSelectors(
    nh_webidl_Object *CSSStyleRule_p
);

nh_webidl_Object *nh_css_getStyleRuleDeclaration(
    nh_webidl_Object *CSSStyleRule_p
);

nh_webidl_Object *nh_css_createStyleRule(
    nh_css_SelectorParseNode *Selectors_p, nh_webidl_Object *CSSStyleDeclaration_p
);

#endif // NH_CSS_INTERFACES_STYLE_RULE_H
