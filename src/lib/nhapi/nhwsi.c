// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nhwsi.h"

#include "../nhcore/Loader/Loader.h"
#include "../nhwsi/Window/Window.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE ==========================================================================================

nh_wsi_Window *nh_api_createWindow(
    NH_BYTE *namespace_p, nh_gfx_SurfaceRequirements *Requirements_p)
{
    nh_wsi_createWindow_f createWindow_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_createWindow");
    return createWindow_f ? createWindow_f(namespace_p, Requirements_p) : NULL;
}

NH_WSI_RESULT_E nh_api_setWindowEventListener(
    nh_wsi_Window *Window_p, nh_wsi_callback_f callback_f)
{
    nh_wsi_setEventListener_f setEventListener_f = !NH_LOADER_P || !Window_p ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEventListener");
    return setEventListener_f ? setEventListener_f(Window_p, callback_f) : NH_WSI_ERROR_BAD_STATE;
}
