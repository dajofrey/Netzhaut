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

/** @addtogroup lib_nh-dom_functions
 *  @{
 */

    extern nh_encoding_UTF8String NH_DOM_DOCUMENT_MODE_NO_QUIRKS;
    extern nh_encoding_UTF8String NH_DOM_DOCUMENT_MODE_QUIRKS;
    extern nh_encoding_UTF8String NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS;

/** @} */

/** @addtogroup lib_nh-dom_functions
 *  @{
 */

    NH_API_RESULT nh_dom_initDocumentModes(
    );
    
    NH_API_RESULT nh_dom_freeDocumentModes(
    );

    NH_API_RESULT nh_dom_setDocumentMode(
        nh_ecmascript_Object *Document_p, nh_encoding_UTF8String *Mode_p
    );

    NH_API_RESULT nh_dom_setDocumentType(
        nh_ecmascript_Object *Document_p, nh_ecmascript_Object *DocumentType_p
    );

/** @} */

#endif
