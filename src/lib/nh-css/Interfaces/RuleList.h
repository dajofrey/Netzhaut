#ifndef NH_CSS_INTERFACES_RULE_LIST_H
#define NH_CSS_INTERFACES_RULE_LIST_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_css_appendToRuleList(
    nh_css_RuleListObject *RuleList_p, nh_webidl_Object *Object_p
);

nh_core_List *nh_css_getRuleListData(
    nh_webidl_Object *RuleList_p
);

#endif // NH_CSS_INTERFACES_RULE_LIST_H
