#ifndef NH_WEBIDL_STRING_H
#define NH_WEBIDL_STRING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include <stdint.h>
#include <stddef.h>

#endif

/** @addtogroup lib_nh-webidl_vars
 *  @{
 */

    extern nh_webidl_DOMString NH_WEBIDL_HTML_NAMESPACE;
    extern nh_webidl_DOMString NH_WEBIDL_MATHML_NAMESPACE;
    extern nh_webidl_DOMString NH_WEBIDL_SVG_NAMESPACE;
    extern nh_webidl_DOMString NH_WEBIDL_XLINK_NAMESPACE;
    extern nh_webidl_DOMString NH_WEBIDL_XML_NAMESPACE;
    extern nh_webidl_DOMString NH_WEBIDL_XMLNS_NAMESPACE;

/** @} */

/** @addtogroup lib_nh-webidl_functions
 *  @{
 */

    NH_WEBIDL_RESULT nh_webidl_initNamespaces(
    );

    nh_webidl_DOMString nh_webidl_initDOMString(
        int chunkSize
    );

    NH_WEBIDL_RESULT nh_webidl_appendToDOMString(
        nh_webidl_DOMString *String_p, NH_BYTE *p, int length
    );

    NH_WEBIDL_RESULT nh_webidl_appendUnicodeToDOMString(
        nh_webidl_DOMString *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );

    void nh_webidl_freeDOMString(
        nh_webidl_DOMString *String_p
    );

    nh_webidl_DOMString nh_webidl_encodeTextToDOMString(
        NH_ENCODING_UTF32 *unicodeCodepoints_p, size_t length
    );

    nh_webidl_USVString nh_webidl_initUSVString(
        int chunkSize
    );
    
    NH_WEBIDL_RESULT nh_webidl_appendToUSVString(
        nh_webidl_USVString *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length
    );
    
    void nh_webidl_freeUSVString(
        nh_webidl_USVString *String_p
    );

/** @} */

#endif 
