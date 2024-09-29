#ifndef NH_CSS_MATCH_H
#define NH_CSS_MATCH_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "SelectorParser.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    bool nh_css_matchSelectors(
        nh_css_SelectorParseNode *Selectors_p, nh_dom_Element *Element_p
    );

/** @} */

#endif
