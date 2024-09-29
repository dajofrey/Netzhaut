// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HTMLElement.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

//NH_API_RESULT nh_html_initializeDocument(
//    nh_webidl_Object *Document_p)
//{
//NH_HTML_BEGIN()
//NH_HTML_DIAGNOSTIC_END(NH_API_SUCCESS)
//}

// API =============================================================================================

void *nh_html_getHTMLElement(
    nh_webidl_Object *Object_p)
{
NH_HTML_BEGIN()
NH_HTML_END(nh_webidl_getObject(Object_p, "HTML", "HTMLElement"))
}

void nh_html_setToken(
    void *HTMLElement_p, nh_html_Token *Token_p)
{
NH_HTML_BEGIN()

    ((nh_webidl_Object*)HTMLElement_p)->internal_p = Token_p;

NH_HTML_SILENT_END()
}

nh_html_Token *nh_html_getToken(
    void *HTMLElement_p)
{
NH_HTML_BEGIN()
NH_HTML_END(((nh_webidl_Object*)HTMLElement_p)->internal_p)
}

