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

    NH_API_RESULT nh_css_computeSpecifiedValues(
        nh_css_LogContext *LogContext_p, nh_webidl_Object *Node_p
    );

    NH_API_RESULT nh_css_freeComputedValues(
        nh_webidl_Object *Node_p
    );

/** @} */

#endif
