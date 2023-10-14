#ifndef NH_CSS_FORMATTING_CONTEXT_H
#define NH_CSS_FORMATTING_CONTEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "BoxTree.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    const NH_BYTE *nh_css_inPrimaryFormattingContextName(
        NH_CSS_FORMATTING_CONTEXT type
    );
    
    NH_CSS_RESULT nh_css_createFormattingContext(
        nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type
    );
    
    NH_BOOL nh_css_startsFormattingContext(
        nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type
    );

    nh_css_BoxTreeNode *nh_css_inFormattingContext(
        nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type, int level
    );

    nh_css_BoxTreeNode *nh_css_inPrimaryFormattingContext(
        nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type
    );

/** @} */

#endif
