#ifndef NH_HTML_HTML_IMAGE_ELEMENT_H
#define NH_HTML_HTML_IMAGE_ELEMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    NH_HTML_RESULT nh_html_initializeHTMLImageElement(
        nh_webidl_Object *Image_p
    );

    nh_html_HTMLImageElement *nh_html_getHTMLImageElement(
        nh_webidl_Object *Object_p
    );
    
    void nh_html_updateImageData(
        nh_html_HTMLImageElement *Image_p, NH_BOOL restartAnimations
    );

/** @} */

#endif
