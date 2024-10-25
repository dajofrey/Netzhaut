// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "WindowProxy.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

// INITIALIZE ======================================================================================

NH_API_RESULT nh_html_initializeWindowProxy(
    nh_webidl_Object *WindowProxy_p)
{
    return NH_API_SUCCESS;
}

// API =============================================================================================

nh_webidl_Object *nh_html_createWindowProxy(
    nh_webidl_Object *Window_p)
{
    nh_webidl_Object *WindowProxy_p = nh_webidl_createObject("HTML", "WindowProxy");
    NH_CORE_CHECK_NULL_2(NULL, WindowProxy_p)

    NH_CORE_CHECK_2(NULL, nh_html_setWindowProxyWindow(WindowProxy_p, Window_p))

    return WindowProxy_p;
}

NH_API_RESULT nh_html_setWindowProxyWindow(
    nh_webidl_Object *WindowProxy_p, nh_webidl_Object *Window_p)
{
    NH_CORE_CHECK_NULL(WindowProxy_p)
    NH_CORE_CHECK_NULL(Window_p)

    WindowProxy_p->internal_p = Window_p;

    return NH_API_SUCCESS;
}

