#ifndef NH_WSI_PLATFORMS_X11_WINDOW_SETTINGS_H
#define NH_WSI_PLATFORMS_X11_WINDOW_SETTINGS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Window.h"
#include "../../Common/Includes.h"

#endif

/** @addtogroup IOFunctions Function
 *  \ingroup IO
 *  @{
 */

    NH_WSI_RESULT_E nh_x11_setWindowBackgroundColor(
        nh_x11_Window *Window_p, nh_Color Color 
    );
    
    NH_WSI_RESULT_E nh_x11_setWindowTitle(
        nh_x11_Window *Window_p, NH_BYTE *title_p
    );
    
    NH_WSI_RESULT_E nh_x11_setWindowDecorated(
        nh_x11_Window *Window_p, NH_BOOL decorated
    );

    NH_WSI_RESULT_E nh_x11_setWindowState(
        nh_x11_Window *Window_p, NH_BOOL *state_p
    );
 
    NH_WSI_RESULT_E nh_x11_setWindowType(
        nh_x11_Window *Window_p, NH_WSI_WINDOW_TYPE_E type
    );

    NH_WSI_RESULT_E nh_x11_setMouseCursor(
        nh_x11_Window *Window_p, NH_WSI_CURSOR_E type
    );

/** @} */

#endif // NH_WSI_PLATFORMS_X11_WINDOW_SETTINGS_H
