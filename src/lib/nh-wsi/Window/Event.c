// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Event.h"
#include "Window.h"

#include "../Common/Macros.h"
#include "../../nh-core/Util/RingBuffer.h"

#include <string.h>

// HELPER ==========================================================================================

static nh_wsi_Event nh_wsi_initEvent(
    NH_WSI_EVENT_E type)
{
NH_WSI_BEGIN()

    nh_wsi_Event Event;
    memset(&Event, 0, sizeof(nh_wsi_Event));
    Event.type = type;
 
NH_WSI_END(Event)
}

// SEND ============================================================================================

void nh_wsi_sendWindowEvent(
    nh_wsi_Window *Window_p, NH_WSI_WINDOW_E type, NH_PIXEL x, NH_PIXEL y, NH_PIXEL width, NH_PIXEL height)
{
NH_WSI_BEGIN()

    nh_wsi_Event Event = nh_wsi_initEvent(NH_WSI_EVENT_WINDOW);

    Event.Window.type        = type;
    Event.Window.Position.x  = x;
    Event.Window.Position.y  = y;
    Event.Window.Size.width  = width;
    Event.Window.Size.height = height;

    if (Window_p && Window_p->callback_f) {
        Window_p->callback_f(Window_p, Event);
    }

    nh_wsi_Event *Event_p = nh_core_advanceRingBuffer(&Window_p->Events);
    *Event_p = Event;

NH_WSI_SILENT_END()
}

void nh_wsi_sendMouseEvent(
    nh_wsi_Window *Window_p, NH_PIXEL x, NH_PIXEL y, NH_WSI_TRIGGER_E trigger, NH_WSI_MOUSE_E type)
{
NH_WSI_BEGIN()

    nh_wsi_Event Event = nh_wsi_initEvent(NH_WSI_EVENT_MOUSE);

    Event.Mouse.Position.x = x;
    Event.Mouse.Position.y = y;
    Event.Mouse.trigger = trigger;
    Event.Mouse.type = type;

    if (Window_p && Window_p->callback_f) {
        Window_p->callback_f(Window_p, Event);
    }

    nh_wsi_Event *Event_p = nh_core_advanceRingBuffer(&Window_p->Events);
    *Event_p = Event;

NH_WSI_SILENT_END()
}

void nh_wsi_sendKeyboardEvent(
    nh_wsi_Window *Window_p, NH_ENCODING_UTF32 codepoint, NH_WSI_KEY_E special, NH_WSI_TRIGGER_E trigger,
    NH_WSI_MODIFIER_FLAG state)
{
NH_WSI_BEGIN()

    nh_wsi_Event Event = nh_wsi_initEvent(NH_WSI_EVENT_KEYBOARD);

    Event.Keyboard.codepoint = codepoint;
    Event.Keyboard.trigger = trigger;
    Event.Keyboard.special = special;
    Event.Keyboard.state = state;

    if (Window_p && Window_p->callback_f) {
        Window_p->callback_f(Window_p, Event);
    }

    nh_wsi_Event *Event_p = nh_core_advanceRingBuffer(&Window_p->Events);
    *Event_p = Event;

NH_WSI_SILENT_END()
}

