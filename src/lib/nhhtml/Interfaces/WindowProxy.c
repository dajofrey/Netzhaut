// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "WindowProxy.h"

#include "../Common/Macros.h"

#include "../../nhdom/Interfaces/Node.h"
#include "../../nhwebidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

// INITIALIZE ======================================================================================

NH_HTML_RESULT nh_html_initializeWindowProxy(
    nh_webidl_Object *WindowProxy_p)
{
NH_HTML_BEGIN()
NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// API =============================================================================================

nh_webidl_Object *nh_html_createWindowProxy(
    nh_webidl_Object *Window_p)
{
NH_HTML_BEGIN()

    nh_webidl_Object *WindowProxy_p = nh_webidl_createObject("HTML", "WindowProxy");
    NH_HTML_CHECK_NULL_2(NULL, WindowProxy_p)

    NH_HTML_CHECK_2(NULL, nh_html_setWindowProxyWindow(WindowProxy_p, Window_p))

NH_HTML_END(WindowProxy_p)
}

NH_HTML_RESULT nh_html_setWindowProxyWindow(
    nh_webidl_Object *WindowProxy_p, nh_webidl_Object *Window_p)
{
NH_HTML_BEGIN()

    NH_HTML_CHECK_NULL(WindowProxy_p)
    NH_HTML_CHECK_NULL(Window_p)

    WindowProxy_p->internal_p = Window_p;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

