#ifndef NH_WSI_COCOA_WINDOW_SETTINGS_H
#define NH_WSI_COCOA_WINDOW_SETTINGS_H

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

/** @addtogroup lib_nh-wsi_functions
 *  @{
 */

    NH_API_RESULT nh_wsi_setCocoaWindowBackgroundColor(
        nh_wsi_CocoaWindow *Window_p, nh_css_Color Color
    );

    NH_API_RESULT nh_wsi_setCocoaWindowTitle(
        nh_wsi_CocoaWindow *Window_p, char *title_p
    );

    NH_API_RESULT nh_wsi_setCocoaWindowDecorated(
        nh_wsi_CocoaWindow *Window_p, bool decorated
    );

    NH_API_RESULT nh_wsi_setCocoaWindowState(
        nh_wsi_CocoaWindow *Window_p, bool *state_p
    );

    NH_API_RESULT nh_wsi_setCocoaWindowMouseCursor(
        nh_wsi_CocoaWindow *Window_p, NH_WSI_CURSOR_E cursor
    );

/** @} */

#endif // NH_WSI_COCOA_WINDOW_SETTINGS_H 