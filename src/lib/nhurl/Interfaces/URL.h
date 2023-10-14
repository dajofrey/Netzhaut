#ifndef NH_URL_URL_H
#define NH_URL_URL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../Parser/Parser.h"

#endif

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    nh_css_RuleObject *nh_css_getCSSRule(
        nh_webidl_Object *Object_p
    );

    void nh_css_setCSSRuleType(
        nh_css_RuleObject *CSSRule_p, NH_CSS_RULE type
    );
    
    void nh_css_setCSSRuleName(
        nh_css_RuleObject *CSSRule_p, nh_encoding_UTF32String *Name_p
    );
    
    void nh_css_setCSSRulePrelude(
        nh_css_RuleObject *CSSRule_p, nh_Array Prelude
    );
    
    void nh_css_setCSSRuleBlock(
        nh_css_RuleObject *CSSRule_p, nh_css_SimpleBlock Block
    );

    NH_CSS_RULE nh_css_getCSSRuleType(
        nh_css_RuleObject *CSSRule_p
    );
    
    nh_encoding_UTF32String *nh_css_getCSSRuleName(
        nh_css_RuleObject *CSSRule_p
    );
    
    nh_Array *nh_css_getCSSRulePrelude(
        nh_css_RuleObject *CSSRule_p
    );
    
    nh_css_SimpleBlock *nh_css_getCSSRuleBlock(
        nh_css_RuleObject *CSSRule_p
    );

/** @} */

#endif
