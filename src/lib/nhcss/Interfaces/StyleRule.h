#ifndef NH_CSS_STYLE_RULE_H
#define NH_CSS_STYLE_RULE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../Parser/SelectorParser.h"

#endif

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    nh_css_StyleRuleObject *nh_css_getStyleRule(
        nh_webidl_Object *Object_p
    );

    nh_css_SelectorParseNode *nh_css_getStyleRuleSelectors(
        nh_css_StyleRuleObject *StyleRule_p
    );
    
    nh_css_StyleDeclarationObject *nh_css_getStyleRuleDeclaration(
        nh_css_StyleRuleObject *StyleRule_p
    );

    nh_css_StyleRuleObject *nh_css_createStyleRule(
        nh_css_SelectorParseNode *Selectors_p, nh_css_StyleDeclarationObject *CSSStyleDeclaration_p
    );

/** @} */

#endif
