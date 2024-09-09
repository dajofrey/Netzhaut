#ifndef NH_HTML_HTML_ELEMENT_H
#define NH_HTML_HTML_ELEMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Parser/Tokenizer.h"

#endif

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    nh_html_HTMLElement *nh_html_getHTMLElement(
        nh_webidl_Object *Object_p
    );
    
    void nh_html_setToken(
        nh_html_HTMLElement *HTMLElement_p, nh_html_Token *Token_p
    );
    
    nh_html_Token *nh_html_getToken(
        nh_html_HTMLElement *HTMLElement_p
    );

/** @} */

#endif
