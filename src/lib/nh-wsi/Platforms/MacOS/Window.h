#ifndef NH_WSI_PLATFORMS_X11_WINDOW_H
#define NH_WSI_PLATFORMS_X11_WINDOW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Init.h"

#include "../../Common/Config.h"
#include "../../Common/Includes.h"

#include "../../../nh-gfx/Base/SurfaceRequirements.h"

#ifdef __unix__
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <X11/extensions/Xrender.h>
    #include <linux/input.h>
    #include <X11/Xlib-xcb.h>
    #include <xkbcommon/xkbcommon-x11.h>
    #include <sys/mman.h>
#endif

typedef struct nh_wsi_Window nh_wsi_Window;
typedef struct nh_wsi_WindowConfiguration nh_wsi_WindowConfiguration;

#endif

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */

    typedef struct nh_x11_Window {
        nh_wsi_X11 *Common_p;
        Window Handle;
        XVisualInfo Info;
        int oldX, oldY;
    } nh_x11_Window;

/** @} */

/** @addtogroup IOFunctions Function
 *  \ingroup IO
 *  @{
 */

    NH_API_RESULT nh_x11_getInput(
        nh_wsi_Window *Window_p, bool *idle_p
    ); 

    NH_API_RESULT nh_x11_createWindow(
        nh_x11_Window *Window_p, nh_wsi_WindowConfig Config, nh_gfx_SurfaceRequirements *Requirements_p
    );

    NH_API_RESULT nh_x11_destroyWindow(
        nh_x11_Window *Window_p
    );

    NH_API_RESULT nh_x11_setClipboardOwner(
        nh_x11_Window *Window_p
    );

    bool nh_x11_isClipboardOwner(
        nh_x11_Window *Window_p
    );

    NH_API_RESULT nh_x11_requestClipboardConversion(
        nh_x11_Window *Window_p
    );

    void nh_x11_getWindowSize(
        nh_x11_Window *Window_p, int* width_p, int* height_p
    );

    NH_API_RESULT nh_x11_moveWindow(
        nh_x11_Window *Window_p
    );

/** @} */

#endif 
