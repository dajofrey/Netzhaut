#ifndef NH_WSI_COCOA_WINDOW_H
#define NH_WSI_COCOA_WINDOW_H

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

typedef struct NH_ALIGN_16 nh_wsi_CocoaWindow {
    void *Handle;      // NSWindow* (opaque pointer)
    void *Delegate;    // CustomWindowDelegate* (opaque pointer)
    void *Layer;       // CAMetalLayer* (opaque pointer)
    struct NH_ALIGN_16 {
        double x, y, width, height;
    } oldFrame;        // Store frame for unmaximize
} nh_wsi_CocoaWindow;

typedef struct nh_wsi_Window nh_wsi_Window;
typedef struct nh_wsi_WindowConfiguration nh_wsi_WindowConfiguration;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_wsi_createCocoaWindow(
    nh_wsi_Window *Window_p, nh_wsi_WindowConfig Config, nh_gfx_SurfaceRequirements *Requirements_p);

NH_API_RESULT nh_wsi_destroyCocoaWindow(
    nh_wsi_CocoaWindow *Window_p
);

NH_API_RESULT nh_wsi_setCocoaClipboardOwner(
    nh_wsi_CocoaWindow *Window_p
);

bool nh_wsi_isCocoaClipboardOwner(
    nh_wsi_CocoaWindow *Window_p
);

NH_API_RESULT nh_wsi_requestCocoaClipboardConversion(
    nh_wsi_CocoaWindow *Window_p
);

NH_API_RESULT nh_wsi_getCocoaInput(
    nh_wsi_Window *Window_p, bool *idle_p
);

NH_API_RESULT nh_wsi_moveCocoaWindow(
    nh_wsi_CocoaWindow *Window_p
);

NH_API_RESULT nh_wsi_setCocoaWindowType(
    nh_wsi_CocoaWindow *Window_p, NH_WSI_WINDOW_TYPE_E type
);

NH_API_RESULT nh_wsi_getCocoaWindowSize(
    nh_wsi_CocoaWindow *Window_p, int *x_p, int *y_p
);

#endif // NH_WSI_COCOA_WINDOW_H
