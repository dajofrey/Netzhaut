// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-wsi.h"

#include "../nh-core/Loader/Loader.h"
#include "../nh-wsi/Window/Window.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_api_Window *nh_api_createWindow(
    char *namespace_p, nh_api_SurfaceRequirements *Requirements_p)
{
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_createWindow_f createWindow_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_createWindow");
    return createWindow_f ? createWindow_f(namespace_p, Requirements_p) : NULL;
}

NH_API_RESULT nh_api_setWindowEventListener(
    nh_api_Window *Window_p, nh_api_windowCallback_f callback_f)
{
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setEventListener_f setEventListener_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEventListener");
    return setEventListener_f ? setEventListener_f(Window_p, callback_f) : NH_API_ERROR_BAD_STATE;
}
