#ifndef NH_HTML_ALGORITHMS_H
#define NH_HTML_ALGORITHMS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Elements.h"
#include "Parser.h"

#endif

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    NH_HTML_RESULT nh_html_updateAStyleBlock(
        nh_webidl_Object *Object_p
    );

    nh_url_URL *nh_html_parseURL(
        nh_encoding_UTF32String Input, nh_url_URL *Base_p
    );

/** @} */

#endif
