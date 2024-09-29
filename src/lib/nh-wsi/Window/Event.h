#ifndef NH_WSI_WINDOW_EVENT_H
#define NH_WSI_WINDOW_EVENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Window.h"

#endif

/** @addtogroup wsiFunctions
 *  @{
 */

    void nh_wsi_sendWindowEvent(
        nh_wsi_Window *Window_p, NH_API_WINDOW_E type, int x, int y, int width, int height
    );

    void nh_wsi_sendMouseEvent(
        nh_wsi_Window *Window_p, int x, int y, NH_API_TRIGGER_E trigger, NH_API_MOUSE_E type
    );

    void nh_wsi_sendKeyboardEvent(
        nh_wsi_Window *Window_p, NH_ENCODING_UTF32 codepoint, NH_API_KEY_E special, NH_API_TRIGGER_E trigger, NH_API_MODIFIER_FLAG state
    );

/** @} */

#endif // NH_WSI_WINDOW_EVENT_H 
