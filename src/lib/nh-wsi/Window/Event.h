#ifndef NH_WSI_WINDOW_EVENT_H
#define NH_WSI_WINDOW_EVENT_H

// INCLUDES

#include "Window.h"

// FUNCTIONS

void nh_wsi_sendWindowEvent(
    nh_wsi_Window *Window_p, NH_API_WINDOW_E type, int x, int y, int width, int height,
    int pixelWidth, int pixelHeight
);

void nh_wsi_sendMouseEvent(
    nh_wsi_Window *Window_p, int x, int y, NH_API_TRIGGER_E trigger, NH_API_MOUSE_E type
);

void nh_wsi_sendKeyboardEvent(
    nh_wsi_Window *Window_p, NH_ENCODING_UTF32 codepoint, NH_API_KEY_E special, NH_API_TRIGGER_E trigger,
    NH_API_MODIFIER_FLAG state
);

#endif // NH_WSI_WINDOW_EVENT_H 
