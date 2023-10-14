#ifndef NH_CSS_RULE_LIST_H
#define NH_CSS_RULE_LIST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    NH_CSS_RESULT nh_css_appendToRuleList(
        nh_css_RuleListObject *RuleList_p, nh_webidl_Object *Object_p
    );

    nh_List *nh_css_getRuleListData(
        nh_css_RuleListObject *RuleList_p
    );

/** @} */

#endif
