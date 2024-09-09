#ifndef NH_CSS_LOG_H
#define NH_CSS_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#include "../Main/Canvas.h"
#include "../Properties/Filter.h"
#include "../Properties/Values.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    NH_CSS_RESULT _nh_css_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_CSS_RESULT _nh_css_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_CSS_RESULT _nh_css_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_CSS_RESULT result, int line
    );

    NH_CSS_RESULT nh_css_logTokens(
        NH_BYTE *logId_p, nh_Array *Tokens_p
    );

    NH_CSS_RESULT nh_css_logRules(
        nh_css_StyleSheetObject *Sheet_p, nh_Array *Rules_p
    );

    NH_CSS_RESULT nh_css_logRuleObjects(
        nh_css_StyleSheetObject *Sheet_p, nh_List *StyleRules_p
    );

    NH_CSS_RESULT nh_css_logFilter(
        nh_css_LogContext *LogContext_p, nh_css_Filter *Filter_p
    );

    NH_CSS_RESULT nh_css_logSpecifiedValues(
        nh_css_LogContext *LogContext_p, nh_Array *SpecifiedValues_p
    );

    NH_CSS_RESULT nh_css_logComputedValues(
        nh_css_LogContext *LogContext_p, nh_List *ComputedValues_p
    );

//    NH_CSS_RESULT nh_css_logFormattingTree(
//        nh_css_LogContext *LogContext_p, nh_css_FormattingNode *Root_p
//    );

    NH_CSS_RESULT nh_css_logBoxTree(
        nh_css_Canvas *Canvas_p, nh_css_BoxTree Tree 
    );

    NH_CSS_RESULT nh_css_logSourceTree(
        nh_css_Canvas *Canvas_p, nh_css_Source *Root_p
    );

    NH_CSS_RESULT nh_css_logFragmentTree(
        nh_css_Canvas *Canvas_p, nh_css_FragmentTree Tree
    );

/** @} */

#endif 
