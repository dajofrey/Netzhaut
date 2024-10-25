#ifndef NH_WSI_PLATFORMS_X11_WINDOW_SETTINGS_H
#define NH_WSI_PLATFORMS_X11_WINDOW_SETTINGS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Window.h"
#include "../../Window/Window.h"
#include "../../Common/Includes.h"
#include "../../../nh-css/Properties/Color.h"

#endif

/** @addtogroup IOFunctions Function
 *  \ingroup IO
 *  @{
 */

    NH_API_RESULT nh_x11_setWindowBackgroundColor(
        nh_x11_Window *Window_p, nh_css_Color Color 
    );
    
    NH_API_RESULT nh_x11_setWindowTitle(
        nh_x11_Window *Window_p, char *title_p
    );
    
    NH_API_RESULT nh_x11_setWindowDecorated(
        nh_x11_Window *Window_p, bool decorated
    );

    NH_API_RESULT nh_x11_setWindowState(
        nh_x11_Window *Window_p, bool *state_p
    );
 
    NH_API_RESULT nh_x11_setWindowType(
        nh_x11_Window *Window_p, NH_WSI_WINDOW_TYPE_E type
    );

    NH_API_RESULT nh_x11_setMouseCursor(
        nh_x11_Window *Window_p, NH_WSI_CURSOR_E type
    );

/** @} */

#endif // NH_WSI_PLATFORMS_X11_WINDOW_SETTINGS_H
