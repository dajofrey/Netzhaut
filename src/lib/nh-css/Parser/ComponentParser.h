#ifndef NH_CSS_COMPONENT_PARSER_H
#define NH_CSS_COMPONENT_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "SelectorParser.h"
#include "TokenParser.h"
#include "Tokenizer.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_css_Token *nh_css_getMirrorToken(
        nh_css_Token *Token_p
    );

    nh_List nh_css_getTokensFromComponentValues(
        nh_Array *ComponentValues_p
    );

    NH_CSS_RESULT nh_css_parseRules(
        nh_Array *Rules_p, nh_css_RuleListObject *StyleRules_p
    );

/** @} */

#endif
