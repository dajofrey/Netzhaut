// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Algorithms.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-dom/Interfaces/NodeList.h"
#include "../../nh-dom/Interfaces/Document.h"
#include "../../nh-dom/Interfaces/Text.h"

#include "../../nh-core/Util/Array.h"

#include "../../nh-url/Main/URLParser.h"

#include "../../nh-encoding/Encodings/UTF8.h"
#include "../../nh-encoding/Encodings/UTF32.h"

#include "../../nh-css/Parser/Parser.h"
#include "../../nh-css/Interfaces/Document.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// HELPER ==========================================================================================

/**
 * https://html.spec.whatwg.org/multipage/semantics.html#update-a-style-block
 */
NH_API_RESULT nh_html_updateAStyleBlock(
    nh_ecmascript_Object *Object_p)
{
    nh_core_List *Children_p = nh_dom_getNodeChildren(NH_WEBIDL_GET_DOM_NODE(Object_p));
    if (!Children_p || Children_p->size < 1) {return NH_API_SUCCESS;}

    nh_ecmascript_Object *Text_p = NH_WEBIDL_GET_DOM_TEXT(Children_p->pp[0]);
    if (!Text_p) {return NH_API_ERROR_BAD_STATE;}

    nh_encoding_UTF8String *String_p = nh_dom_getTextString(Text_p);
    NH_CORE_CHECK_NULL(String_p)

    // puts(String_p->p) to print the style content

    nh_ecmascript_Object *Document_p = nh_dom_getNodeDocument(NH_WEBIDL_GET_DOM_NODE(Object_p));
    NH_CORE_CHECK_NULL(Document_p)

    puts("TODO updateAStyleBlock");

    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(String_p->p, String_p->length, NULL);
    nh_ecmascript_Object *StyleSheet_p = nh_css_parseStyleSheetFromUTF32(&String, nh_css_getDocument(Document_p));
    nh_encoding_freeUTF32(&String);

    return NH_API_SUCCESS;
}

nh_url_URL *nh_html_parseURL(
    nh_encoding_UTF32String Input, nh_url_URL *Base_p)

{
    nh_url_URL *URL_p = nh_url_parseBasicURL(Input, Base_p, NULL, NH_URL_PARSER_STATE_UNDEFINED);
    if (!URL_p) {return NULL;}

    if (URL_p->Scheme.p && nh_encoding_compareUTF32ASCII(URL_p->Scheme.p, "blob")) {
        // TODO
    }

    return URL_p;
}

