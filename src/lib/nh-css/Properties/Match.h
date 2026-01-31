#ifndef NH_CSS_PROPERTIES_SELECTOR_H
#define NH_CSS_PROPERTIES_SELECTOR_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Parser/SelectorParser.h"
#include "../Common/Includes.h"

bool nh_css_matchSelectors(
    nh_css_SelectorParseNode *Selectors_p, nh_ecmascript_Object *Object_p
);

#endif // NH_CSS_PROPERTIES_SELECTOR_H
