#ifndef NH_CSS_STYLE_SHEET_H
#define NH_CSS_STYLE_SHEET_H

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

    nh_css_StyleSheetObject *nh_css_getStyleSheet(
        nh_webidl_Object *Object_p
    );

    nh_css_RuleListObject *nh_css_getRuleList(
        nh_css_StyleSheetObject *StyleSheet_p
    );

    void nh_css_setStyleSheetTokens(
        nh_css_StyleSheetObject *StyleSheet_p, nh_core_Array Tokens
    );

    nh_webidl_Object *nh_css_findCounterStyleRule(
        nh_css_StyleSheetObject *StyleSheet_p, char *name_p
    );

/** @} */

#endif
