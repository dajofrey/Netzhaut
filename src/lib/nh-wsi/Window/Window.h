#ifndef NH_WSI_WINDOW_WINDOW_H
#define NH_WSI_WINDOW_WINDOW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Platforms/X11/Window.h"

#endif

/** @addtogroup lib_nh-wsi_structs
 *  @{
 */

    typedef struct nh_wsi_Window {
        NH_BYTE namespace_p[255];
        NH_WSI_TYPE_E type;
        void *surface_p;
        nh_wsi_callback_f callback_f;
        nh_x11_Window X11;
        nh_RingBuffer Events;
        NH_WSI_CURSOR_E cursor;
    } nh_wsi_Window;

/** @} */

/** @addtogroup lib_nh-wsi_typedefs
 *  @{
 */

    typedef nh_wsi_Window *(*nh_wsi_createWindow_f)(
        NH_BYTE *namespace_p, nh_gfx_SurfaceRequirements *Requirements_p
    );
   
    typedef NH_WSI_RESULT_E (*nh_wsi_moveWindow_f)(
        nh_wsi_Window *Window_p
    );
 
    typedef NH_WSI_RESULT_E (*nh_wsi_setEventListener_f)(
        nh_wsi_Window *Window_p, nh_wsi_callback_f callback_f
    );

/** @} */

/** @addtogroup lib_nh-wsi_functions
 *  @{
 */
    
    NH_WSI_RESULT_E nh_wsi_destroyWindow(
        nh_wsi_Window *Window_p
    );
    
/** @} */

#endif // NH_WSI_WINDOW_WINDOW_H 
