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

    NH_API_RESULT nh_wsi_setX11WindowBackgroundColor(
        nh_wsi_X11Window *Window_p, nh_css_Color Color 
    );
    
    NH_API_RESULT nh_wsi_setX11WindowTitle(
        nh_wsi_X11Window *Window_p, char *title_p
    );
    
    NH_API_RESULT nh_wsi_setX11WindowDecorated(
        nh_wsi_X11Window *Window_p, bool decorated
    );

    NH_API_RESULT nh_wsi_setX11WindowState(
        nh_wsi_X11Window *Window_p, bool *state_p
    );
 
    NH_API_RESULT nh_wsi_setX11WindowType(
        nh_wsi_X11Window *Window_p, NH_WSI_WINDOW_TYPE_E type
    );

    NH_API_RESULT nh_wsi_setX11MouseCursor(
        nh_wsi_X11Window *Window_p, NH_WSI_CURSOR_E type
    );

/** @} */

#endif // NH_WSI_PLATFORMS_X11_WINDOW_SETTINGS_H
