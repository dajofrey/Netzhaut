#ifndef NH_HTML_INTERFACES_HTML_IMAGE_ELEMENT_H
#define NH_HTML_INTERFACES_HTML_IMAGE_ELEMENT_H

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

    NH_API_RESULT nh_html_initializeHTMLImageElement(
        nh_webidl_Object *Image_p
    );

    void nh_html_updateImageData(
        void *Image_p, bool restartAnimations
    );

/** @} */

#endif
