#ifndef NH_CSS_COUNTER_STYLE_RULE_H
#define NH_CSS_COUNTER_STYLE_RULE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_css_CounterStyleRuleObject *nh_css_createCounterStyleRule(
        nh_core_Array Declaration
    );

    nh_css_CounterStyleRuleObject *nh_css_getCounterStyleRule(
        nh_webidl_Object *Object_p
    );

    nh_encoding_UTF32String nh_css_calculateMarkerString(
        nh_css_CounterStyleRuleObject *CounterStyleRule_p
    );

/** @} */

#endif
