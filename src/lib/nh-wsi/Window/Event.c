// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Event.h"
#include "Window.h"

#include "../../nh-core/Util/RingBuffer.h"

#include <string.h>

// HELPER ==========================================================================================

static nh_api_WSIEvent nh_wsi_initEvent(
    NH_API_WSI_EVENT_E type)
{
    nh_api_WSIEvent Event;
    memset(&Event, 0, sizeof(nh_api_WSIEvent));
    Event.type = type;
 
    return Event;
}

// SEND ============================================================================================

void nh_wsi_sendWindowEvent(
    nh_wsi_Window *Window_p, NH_API_WINDOW_E type, int x, int y, int width, int height,
    int pixelWidth, int pixelHeight)
{
    if (!Window_p) return;

    nh_api_WSIEvent Event = nh_wsi_initEvent(NH_API_WSI_EVENT_WINDOW);

    Event.Window.type        = type;
    Event.Window.Position.x  = x;
    Event.Window.Position.y  = y;
    Event.Window.PixelSize.width  = pixelWidth;
    Event.Window.PixelSize.height = pixelHeight;

    if (Window_p->callback_f != NULL) {
        Window_p->callback_f((nh_api_Window*)Window_p, Event);
    }

    nh_api_WSIEvent *Event_p = (nh_api_WSIEvent*)nh_core_advanceRingBuffer(&Window_p->Events);
    if (Event_p) {
        *Event_p = Event;
    }

    return;
}

void nh_wsi_sendMouseEvent(
    nh_wsi_Window *Window_p, int x, int y, NH_API_TRIGGER_E trigger, NH_API_MOUSE_E type)
{
    if (!Window_p) return;

    nh_api_WSIEvent Event = nh_wsi_initEvent(NH_API_WSI_EVENT_MOUSE);

    Event.Mouse.Position.x = x;
    Event.Mouse.Position.y = y;
    Event.Mouse.trigger = trigger;
    Event.Mouse.type = type;

    if (Window_p->callback_f) {
        Window_p->callback_f((nh_api_Window*)Window_p, Event);
    }

    nh_api_WSIEvent *Event_p = (nh_api_WSIEvent*)nh_core_advanceRingBuffer(&Window_p->Events);
    if (Event_p) {
        *Event_p = Event;
    }

    return;
}

void nh_wsi_sendKeyboardEvent(
    nh_wsi_Window *Window_p, NH_ENCODING_UTF32 codepoint, NH_API_KEY_E special, NH_API_TRIGGER_E trigger,
    NH_API_MODIFIER_FLAG state)
{
    if (!Window_p) return;

    nh_api_WSIEvent Event = nh_wsi_initEvent(NH_API_WSI_EVENT_KEYBOARD);

    Event.Keyboard.codepoint = codepoint;
    Event.Keyboard.trigger = trigger;
    Event.Keyboard.special = special;
    Event.Keyboard.state = state;

    if (Window_p->callback_f) {
        Window_p->callback_f((nh_api_Window*)Window_p, Event);
    }

    nh_api_WSIEvent *Event_p = (nh_api_WSIEvent*)nh_core_advanceRingBuffer(&Window_p->Events);
    if (Event_p) {
        *Event_p = Event;
    }

    return;
}
