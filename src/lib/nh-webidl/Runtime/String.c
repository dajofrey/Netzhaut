// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "String.h"

#include "../Common/Macros.h"

#include "../../nh-core/Util/String.h"
#include "../../nh-core/Common/Macros.h"

#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"
#include "../../nh-encoding/Common/Macros.h"

// NAMESPACES ======================================================================================

nh_webidl_DOMString NH_WEBIDL_HTML_NAMESPACE;
nh_webidl_DOMString NH_WEBIDL_MATHML_NAMESPACE;
nh_webidl_DOMString NH_WEBIDL_SVG_NAMESPACE;
nh_webidl_DOMString NH_WEBIDL_XLINK_NAMESPACE;
nh_webidl_DOMString NH_WEBIDL_XML_NAMESPACE;
nh_webidl_DOMString NH_WEBIDL_XMLNS_NAMESPACE;

// INIT NAMESPACES =================================================================================

NH_WEBIDL_RESULT nh_webidl_initNamespaces()
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_HTML_NAMESPACE   = nh_webidl_initDOMString(29);
    NH_WEBIDL_MATHML_NAMESPACE = nh_webidl_initDOMString(35);
    NH_WEBIDL_SVG_NAMESPACE    = nh_webidl_initDOMString(27);
    NH_WEBIDL_XLINK_NAMESPACE  = nh_webidl_initDOMString(29);
    NH_WEBIDL_XML_NAMESPACE    = nh_webidl_initDOMString(36);
    NH_WEBIDL_XMLNS_NAMESPACE  = nh_webidl_initDOMString(30);

    NH_WEBIDL_CHECK(nh_webidl_appendToDOMString(&NH_WEBIDL_HTML_NAMESPACE,   "http://www.w3.org/1999/xhtml", 28))
    NH_WEBIDL_CHECK(nh_webidl_appendToDOMString(&NH_WEBIDL_MATHML_NAMESPACE, "http://www.w3.org/1998/Math/MathML", 34))
    NH_WEBIDL_CHECK(nh_webidl_appendToDOMString(&NH_WEBIDL_SVG_NAMESPACE,    "http://www.w3.org/2000/svg", 26))
    NH_WEBIDL_CHECK(nh_webidl_appendToDOMString(&NH_WEBIDL_XLINK_NAMESPACE,  "http://www.w3.org/1999/xlink", 28))
    NH_WEBIDL_CHECK(nh_webidl_appendToDOMString(&NH_WEBIDL_XML_NAMESPACE,    "http://www.w3.org/XML/1998/namespace", 36))
    NH_WEBIDL_CHECK(nh_webidl_appendToDOMString(&NH_WEBIDL_XMLNS_NAMESPACE,  "http://www.w3.org/2000/xmlns/", 29))

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

// DOM STRING ======================================================================================

nh_webidl_DOMString nh_webidl_initDOMString(
    int chunkSize)
{
NH_WEBIDL_BEGIN()
NH_WEBIDL_END(nh_core_initString(chunkSize))
}

NH_WEBIDL_RESULT nh_webidl_appendToDOMString(
    nh_webidl_DOMString *String_p, NH_BYTE *p, int length)
{
NH_WEBIDL_BEGIN()

    NH_CORE_CHECK_2(NH_WEBIDL_ERROR_BAD_STATE, nh_core_appendToString(String_p, p, length))

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

NH_WEBIDL_RESULT nh_webidl_appendUnicodeToDOMString(
    nh_webidl_DOMString *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length)
{
NH_WEBIDL_BEGIN()

    nh_encoding_UTF8String String = nh_encoding_encodeUTF8(codepoints_p, length);
    NH_CORE_CHECK_2(NH_WEBIDL_ERROR_BAD_STATE, nh_core_appendToString(String_p, String.p, String.length))
    nh_core_freeString(&String);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

void nh_webidl_freeDOMString(
    nh_webidl_DOMString *String_p)
{
NH_WEBIDL_BEGIN()

    nh_core_freeString(String_p);

NH_WEBIDL_SILENT_END()
}

nh_webidl_DOMString nh_webidl_encodeTextToDOMString(
    NH_ENCODING_UTF32 *unicodeCodepoints_p, size_t length)
{
NH_CORE_BEGIN()
NH_CORE_END(nh_encoding_encodeUTF8(unicodeCodepoints_p, length))
}

// USV STRING ======================================================================================

nh_webidl_USVString nh_webidl_initUSVString(
    int chunkSize)
{
NH_WEBIDL_BEGIN()
NH_WEBIDL_END(nh_encoding_initUTF32(chunkSize))
}

NH_WEBIDL_RESULT nh_webidl_appendToUSVString(
    nh_webidl_USVString *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length)
{
NH_WEBIDL_BEGIN()

    NH_ENCODING_CHECK_2(NH_WEBIDL_ERROR_BAD_STATE, nh_encoding_appendUTF32(String_p, codepoints_p, length))

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

void nh_webidl_freeUSVString(
    nh_webidl_USVString *String_p)
{
NH_WEBIDL_BEGIN()

    nh_encoding_freeUTF32(String_p);

NH_WEBIDL_SILENT_END()
}

