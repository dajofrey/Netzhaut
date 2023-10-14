#ifndef NH_DOM_DOCUMENT_H
#define NH_DOM_DOCUMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhdom_functions
 *  @{
 */

    extern nh_webidl_DOMString NH_DOM_DOCUMENT_MODE_NO_QUIRKS;
    extern nh_webidl_DOMString NH_DOM_DOCUMENT_MODE_QUIRKS;
    extern nh_webidl_DOMString NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS;

/** @} */

/** @addtogroup lib_nhdom_functions
 *  @{
 */

    NH_DOM_RESULT nh_dom_initDocumentModes(
    );
    
    NH_DOM_RESULT nh_dom_freeDocumentModes(
    );

    NH_DOM_RESULT nh_dom_setDocumentMode(
        nh_webidl_Object *Document_p, nh_webidl_DOMString *Mode_p
    );

    NH_DOM_RESULT nh_dom_setDocumentType(
        nh_webidl_Object *Document_p, nh_webidl_Object *DocumentType_p
    );

/** @} */

#endif
