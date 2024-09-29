#ifndef NH_DOM_TEXT_H
#define NH_DOM_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-dom_functions
 *  @{
 */
    nh_dom_Text *nh_dom_getText(
        nh_webidl_Object *Object_p
    );

    nh_dom_Text *nh_dom_createText(
        nh_webidl_DOMString Data, nh_webidl_Object *NodeDocument_p
    );

    NH_API_RESULT nh_dom_appendToText(
        nh_dom_Text *Text_p, nh_webidl_DOMString DOMString
    );

    nh_webidl_DOMString *nh_dom_getTextString(
        nh_dom_Text *Text_p
    );

/** @} */

#endif
