#ifndef NH_WSI_WINDOW_WINDOW_H
#define NH_WSI_WINDOW_WINDOW_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include <stdalign.h>
#include "../Common/Includes.h"

#if defined(__unix__)
    #include "../Platforms/X11/Window.h"
#elif defined(__APPLE__)
    #include "../Platforms/Cocoa/Window.h"
#endif

#include "../../nh-gfx/Base/SurfaceRequirements.h"
#include "../../nh-core/Util/RingBuffer.h"

// ENUMS ===========================================================================================

typedef enum NH_WSI_CURSOR_E { 
    NH_WSI_CURSOR_ARROW, 
    NH_WSI_CURSOR_ARROW_DOWN, 
    NH_WSI_CURSOR_ARROW_UP, 
    NH_WSI_CURSOR_BOAT, 
    NH_WSI_CURSOR_BOGOSITY, 
    NH_WSI_CURSOR_BOTTOM_LEFT_CORNER, 
    NH_WSI_CURSOR_BOTTOM_RIGHT_CORNER, 
    NH_WSI_CURSOR_BOTTOM_SIDE, 
    NH_WSI_CURSOR_BOTTOM_TEE, 
    NH_WSI_CURSOR_BOX_SPIRAL, 
    NH_WSI_CURSOR_CENTER_PTR, 
    NH_WSI_CURSOR_CIRCLE, 
    NH_WSI_CURSOR_CLOCK, 
    NH_WSI_CURSOR_COFFEE_MUG, 
    NH_WSI_CURSOR_CROSS, 
    NH_WSI_CURSOR_CROSS_REVERSE, 
    NH_WSI_CURSOR_CROSSHAIR, 
    NH_WSI_CURSOR_DIAMOND_CROSS, 
    NH_WSI_CURSOR_DOT, 
    NH_WSI_CURSOR_DOTBOX, 
    NH_WSI_CURSOR_DOUBLE_ARROW, 
    NH_WSI_CURSOR_DRAFT_LARGE, 
    NH_WSI_CURSOR_DRAFT_SMALL, 
    NH_WSI_CURSOR_DRAPED_BOX, 
    NH_WSI_CURSOR_EXCHANGE, 
    NH_WSI_CURSOR_FLEUR, 
    NH_WSI_CURSOR_GOBBLER, 
    NH_WSI_CURSOR_GUMBY, 
    NH_WSI_CURSOR_HAND1, 
    NH_WSI_CURSOR_HAND2, 
    NH_WSI_CURSOR_HEART, 
    NH_WSI_CURSOR_ICON, 
    NH_WSI_CURSOR_IRON_CROSS, 
    NH_WSI_CURSOR_LEFT_PTR, 
    NH_WSI_CURSOR_LEFT_SIDE, 
    NH_WSI_CURSOR_LEFT_TEE, 
    NH_WSI_CURSOR_LEFT_BUTTON, 
    NH_WSI_CURSOR_LL_ANGLE, 
    NH_WSI_CURSOR_LR_ANGLE, 
    NH_WSI_CURSOR_MAN, 
    NH_WSI_CURSOR_MIDDLEBUTTON, 
    NH_WSI_CURSOR_MOUSE, 
    NH_WSI_CURSOR_PENCIL, 
    NH_WSI_CURSOR_PIRATE, 
    NH_WSI_CURSOR_PLUS, 
    NH_WSI_CURSOR_QUESTION_ARROW, 
    NH_WSI_CURSOR_RIGHT_PTR, 
    NH_WSI_CURSOR_RIGHT_SIDE, 
    NH_WSI_CURSOR_RIGHT_TEE, 
    NH_WSI_CURSOR_RIGHT_BUTTON, 
    NH_WSI_CURSOR_RTL_LOGO, 
    NH_WSI_CURSOR_SAILBOAT, 
    NH_WSI_CURSOR_SB_DOWN_ARROW, 
    NH_WSI_CURSOR_SB_H_DOUBLE_ARROW, 
    NH_WSI_CURSOR_SB_LEFT_ARROW, 
    NH_WSI_CURSOR_SB_RIGHT_ARROW, 
    NH_WSI_CURSOR_SB_UP_ARROW, 
    NH_WSI_CURSOR_SB_V_DOUBLE_ARROW, 
    NH_WSI_CURSOR_SHUTTLE, 
    NH_WSI_CURSOR_SIZING, 
    NH_WSI_CURSOR_SPIDER, 
    NH_WSI_CURSOR_SPRAYCAN, 
    NH_WSI_CURSOR_STAR, 
    NH_WSI_CURSOR_TARGET, 
    NH_WSI_CURSOR_TCROSS, 
    NH_WSI_CURSOR_TOP_LEFT_ARROW, 
    NH_WSI_CURSOR_TOP_LEFT_CORNER, 
    NH_WSI_CURSOR_TOP_RIGHT_CORNER, 
    NH_WSI_CURSOR_TOP_SIDE, 
    NH_WSI_CURSOR_TOP_TEE, 
    NH_WSI_CURSOR_TREK, 
    NH_WSI_CURSOR_UL_ANGLE, 
    NH_WSI_CURSOR_UMBRELLA, 
    NH_WSI_CURSOR_UR_ANGLE, 
    NH_WSI_CURSOR_WATCH, 
    NH_WSI_CURSOR_XTERM, 
} NH_WSI_CURSOR_E; 

typedef enum NH_WSI_TYPE_E { 
    NH_WSI_TYPE_X11, 
    NH_WSI_TYPE_WAYLAND, 
    NH_WSI_TYPE_WIN32, 
    NH_WSI_TYPE_COCOA,
} NH_WSI_TYPE_E; 

// STRUCTS =====================================================================================

typedef struct NH_ALIGN_16 nh_wsi_Window {
    char namespace_p[255];
    NH_WSI_TYPE_E type;
    void *surface_p;
    nh_api_windowCallback_f callback_f;
#if defined(__unix__)
    nh_wsi_X11Window X11;
#elif defined(__APPLE__)
    nh_wsi_CocoaWindow Cocoa;
#endif
    nh_core_RingBuffer Events;
    NH_WSI_CURSOR_E cursor;
} nh_wsi_Window;

// FUNCTIONS ===================================================================================

NH_API_RESULT nh_wsi_destroyWindow(
    nh_wsi_Window *Window_p
);
    
#endif // NH_WSI_WINDOW_WINDOW_H 
