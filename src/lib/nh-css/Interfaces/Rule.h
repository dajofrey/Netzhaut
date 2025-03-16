#ifndef NH_CSS_INTERFACES_RULE_H
#define NH_CSS_INTERFACES_RULE_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"
#include "../Parser/Parser.h"

// FUNCTIONS =======================================================================================

void nh_css_setRuleData(
    nh_webidl_Object *Rule_p, nh_css_Rule Rule
);

nh_css_Rule *nh_css_getRuleData(
    nh_webidl_Object *Rule_p
);

#endif // NH_CSS_INTERFACES_RULE_H
