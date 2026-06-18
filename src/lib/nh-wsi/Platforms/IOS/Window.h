#ifndef NH_WSI_IOS_WINDOW_H
#define NH_WSI_IOS_WINDOW_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../../Common/Config.h"
#include "../../Common/Includes.h"
#include "../../../nh-gfx/Base/SurfaceRequirements.h"

// TYPES ===========================================================================================

typedef struct NH_ALIGN_16 nh_wsi_IOSWindow {
    void *Handle;         // UIWindow* (opaque pointer)
    void *ViewController; // UIViewController* (opaque pointer)
    void *Layer;          // CAMetalLayer* (opaque pointer)
    struct NH_ALIGN_16 {
        double x, y, width, height;
    } oldFrame;
} nh_wsi_IOSWindow;

typedef struct nh_wsi_Window nh_wsi_Window;
typedef struct nh_wsi_WindowConfiguration nh_wsi_WindowConfiguration;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_wsi_createIOSWindow(
    nh_wsi_Window *Window_p, nh_wsi_WindowConfig Config, nh_gfx_SurfaceRequirements *Requirements_p);

NH_API_RESULT nh_wsi_destroyIOSWindow(
    nh_wsi_IOSWindow *Window_p
);

NH_API_RESULT nh_wsi_setIOSClipboardOwner(
    nh_wsi_IOSWindow *Window_p
);

bool nh_wsi_isIOSClipboardOwner(
    nh_wsi_IOSWindow *Window_p
);

NH_API_RESULT nh_wsi_requestIOSClipboardConversion(
    nh_wsi_IOSWindow *Window_p
);

NH_API_RESULT nh_wsi_getIOSInput(
    nh_wsi_Window *Window_p, bool *idle_p
);

NH_API_RESULT nh_wsi_moveIOSWindow(
    nh_wsi_IOSWindow *Window_p
);

NH_API_RESULT nh_wsi_setIOSWindowType(
    nh_wsi_IOSWindow *Window_p, NH_WSI_WINDOW_TYPE_E type
);

NH_API_RESULT nh_wsi_getIOSWindowSize(
    nh_wsi_IOSWindow *Window_p, int *x_p, int *y_p
);

#endif // NH_WSI_IOS_WINDOW_H
