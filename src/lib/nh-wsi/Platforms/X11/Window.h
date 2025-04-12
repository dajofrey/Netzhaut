#ifndef NH_WSI_PLATFORMS_X11_WINDOW_H
#define NH_WSI_PLATFORMS_X11_WINDOW_H

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

// STRUCTS ==================================


    typedef struct nh_wsi_X11Window {
        nh_wsi_Window *Common_p;
        Window Handle;
        XVisualInfo Info;
        int oldX, oldY;
    } nh_wsi_X11Window;

// FUNCTIONS ================================

    NH_API_RESULT nh_wsi_getX11Input(
        nh_wsi_Window *Window_p, bool *idle_p
    ); 

    NH_API_RESULT nh_wsi_createX11Window(
        nh_wsi_X11Window *Window_p, nh_wsi_WindowConfig Config, nh_gfx_SurfaceRequirements *Requirements_p
    );

    NH_API_RESULT nh_wsi_destroyX11Window(
        nh_wsi_X11Window *Window_p
    );

    NH_API_RESULT nh_wsi_setX11ClipboardOwner(
        nh_wsi_X11Window *Window_p
    );

    bool nh_wsi_isX11ClipboardOwner(
        nh_wsi_X11Window *Window_p
    );

    NH_API_RESULT nh_wsi_requestX11ClipboardConversion(
        nh_wsi_X11Window *Window_p
    );

    void nh_wsi_getX11WindowSize(
        nh_wsi_X11Window *Window_p, int* width_p, int* height_p
    );

    NH_API_RESULT nh_wsi_moveX11Window(
        nh_wsi_X11Window *Window_p
    );

#endif 
