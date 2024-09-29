// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Algorithms.h"

#include "../Common/Macros.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-dom/Interfaces/NodeList.h"
#include "../../nh-dom/Interfaces/Document.h"
#include "../../nh-dom/Interfaces/Text.h"
#include "../../nh-dom/Common/Macros.h"

#include "../../nh-core/Util/Array.h"

#include "../../nh-url/Main/URLParser.h"

#include "../../nh-encoding/Encodings/UTF8.h"
#include "../../nh-encoding/Encodings/UTF32.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// HELPER ==========================================================================================

// https://html.spec.whatwg.org/multipage/semantics.html#update-a-style-block
NH_API_RESULT nh_html_updateAStyleBlock(
    nh_webidl_Object *Object_p)
{
NH_HTML_BEGIN()

    nh_webidl_Object *Document_p = nh_dom_getNodeDocument(nh_dom_getNode(Object_p));
    NH_HTML_CHECK_NULL(Document_p)

    nh_List *Children_p = nh_dom_getNodeChildren(nh_dom_getNode(Object_p));
    if (!Children_p || Children_p->size < 1) {NH_HTML_DIAGNOSTIC_END(NH_API_SUCCESS)}

    nh_dom_Text *Text_p = nh_dom_getText(Children_p->pp[0]);
    if (!Text_p) {NH_HTML_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}

    // TODO

    nh_webidl_DOMString *String_p = nh_dom_getTextString(Text_p);
    NH_HTML_CHECK_NULL(String_p)

puts("TODO updateAStyleBlock");
exit(0);
//    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(String_p->bytes_p, String_p->length, NULL);
//    NH_HTML_CHECK_NULL(nh_css_createCSSStyleSheet(nh_css_getDocument(Document_p), &String))
//    nh_encoding_freeUTF32(&String);

    // TODO

NH_HTML_DIAGNOSTIC_END(NH_API_SUCCESS)
}

nh_url_URL *nh_html_parseURL(
    nh_encoding_UTF32String Input, nh_url_URL *Base_p)

{
NH_HTML_BEGIN()

    nh_url_URL *URL_p = nh_url_parseBasicURL(Input, Base_p, NULL, NH_URL_PARSER_STATE_UNDEFINED);
    if (!URL_p) {NH_HTML_END(NULL)}

    if (URL_p->Scheme.p && nh_encoding_compareUTF32ASCII(URL_p->Scheme.p, "blob")) {
        // TODO
    }

NH_HTML_END(URL_p)
}

