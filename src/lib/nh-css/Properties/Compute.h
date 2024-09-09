#ifndef NH_CSS_COMPUTE_H
#define NH_CSS_COMPUTE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Main/SourceTree.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    NH_CSS_RESULT nh_css_computeSpecifiedValues(
        nh_css_LogContext *LogContext_p, nh_dom_Node *Node_p
    );

    NH_CSS_RESULT nh_css_freeComputedValues(
        nh_dom_Node *Node_p
    );

/** @} */

#endif
