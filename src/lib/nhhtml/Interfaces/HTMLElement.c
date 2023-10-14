// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HTMLElement.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhdom/Interfaces/Node.h"
#include "../../nhwebidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

//NH_HTML_RESULT nh_html_initializeDocument(
//    nh_webidl_Object *Document_p)
//{
//NH_HTML_BEGIN()
//NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
//}

// API =============================================================================================

nh_html_HTMLElement *nh_html_getHTMLElement(
    nh_webidl_Object *Object_p)
{
NH_HTML_BEGIN()
NH_HTML_END((nh_html_HTMLElement*)nh_webidl_getObject(Object_p, "HTML", "HTMLElement"))
}

void nh_html_setToken(
    nh_html_HTMLElement *HTMLElement_p, nh_html_Token *Token_p)
{
NH_HTML_BEGIN()

    ((nh_webidl_Object*)HTMLElement_p)->internal_p = Token_p;

NH_HTML_SILENT_END()
}

nh_html_Token *nh_html_getToken(
    nh_html_HTMLElement *HTMLElement_p)
{
NH_HTML_BEGIN()
NH_HTML_END(((nh_webidl_Object*)HTMLElement_p)->internal_p)
}

