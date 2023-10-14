#ifndef NH_WSI_WINDOW_EVENT_H
#define NH_WSI_WINDOW_EVENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup wsiFunctions
 *  @{
 */

    void nh_wsi_sendWindowEvent(
        nh_wsi_Window *Window_p, NH_WSI_WINDOW_E type, NH_PIXEL x, NH_PIXEL y, NH_PIXEL width, NH_PIXEL height
    );

    void nh_wsi_sendMouseEvent(
        nh_wsi_Window *Window_p, NH_PIXEL x, NH_PIXEL y, NH_WSI_TRIGGER_E trigger, NH_WSI_MOUSE_E type
    );

    void nh_wsi_sendKeyboardEvent(
        nh_wsi_Window *Window_p, NH_ENCODING_UTF32 codepoint, NH_WSI_KEY_E special, NH_WSI_TRIGGER_E trigger, NH_WSI_MODIFIER_FLAG state
    );

/** @} */

#endif // NH_WSI_WINDOW_EVENT_H 
