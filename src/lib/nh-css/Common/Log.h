#ifndef NH_CSS_COMMON_LOG_H
#define NH_CSS_COMMON_LOG_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#include "../Main/Canvas.h"
#include "../Properties/Filter.h"
#include "../Properties/Values.h"

NH_API_RESULT nh_css_logTokens(
    nh_webidl_Object *CSSStyleSheet_p, nh_core_Array *Tokens_p
);

NH_API_RESULT nh_css_logRules(
    nh_webidl_Object *CSSStyleSheet_p, nh_core_Array *Rules_p
);

NH_API_RESULT nh_css_logObjects(
    nh_webidl_Object *CSSStyleSheet_p, nh_core_List *StyleRules_p
);

NH_API_RESULT nh_css_logFilter(
    nh_css_LogContext *LogContext_p, nh_css_Filter *Filter_p
);

NH_API_RESULT nh_css_logSpecifiedValues(
    nh_css_LogContext *LogContext_p, nh_core_Array *SpecifiedValues_p
);

NH_API_RESULT nh_css_logComputedValues(
    nh_css_LogContext *LogContext_p, nh_core_List *ComputedValues_p
);

//    NH_API_RESULT nh_css_logFormattingTree(
//        nh_css_LogContext *LogContext_p, nh_css_FormattingNode *Root_p
//    );

NH_API_RESULT nh_css_logBoxTree(
    nh_css_Canvas *Canvas_p, nh_css_BoxTree Tree 
);

NH_API_RESULT nh_css_logSourceTree(
    nh_css_Canvas *Canvas_p, nh_css_Source *Root_p
);

NH_API_RESULT nh_css_logFragmentTree(
    nh_css_Canvas *Canvas_p, nh_css_FragmentTree Tree
);

#endif // NH_CSS_COMMON_LOG_H
