// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Input.h"
#include "../Header/Macros.h"

#include "../DOM/Header/Event.h"
#include "../DOM/Header/MouseEvent.h"
#include "../DOM/Header/WheelEvent.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/RingIterator.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// DECLARE ========================================================================================

static NH_RESULT Nh_JS_processMouseClick(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p
);
static NH_RESULT Nh_JS_processMouseRelease(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p
);
static NH_RESULT Nh_JS_processMouseMove(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p
);
static NH_RESULT Nh_JS_processMouseScroll(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, bool up
);

// API =============================================================================================

NH_RESULT Nh_JS_processInput(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    while (1) 
    {
        Nh_HTML_MouseEvent *MouseEvent_p = Nh_incrementRingIteratorMark(
            &Tab_p->Document.Input.Events.Mouse, &Tab_p->Document.Input.Marks.JS.Mouse
        );
        if (MouseEvent_p == NULL) {break;}

        switch (MouseEvent_p->Event.trigger)
        {
            case NH_IO_TRIGGER_RELEASE : NH_CHECK(Nh_JS_processMouseRelease(Tab_p, MouseEvent_p)) break;
            case NH_IO_TRIGGER_PRESS   : NH_CHECK(Nh_JS_processMouseClick(Tab_p, MouseEvent_p)) break;
            case NH_IO_TRIGGER_MOVE    : NH_CHECK(Nh_JS_processMouseMove(Tab_p, MouseEvent_p)) break;
            case NH_IO_TRIGGER_UP      : NH_CHECK(Nh_JS_processMouseScroll(Tab_p, MouseEvent_p, true)) break;
            case NH_IO_TRIGGER_DOWN    : NH_CHECK(Nh_JS_processMouseScroll(Tab_p, MouseEvent_p, false)) break;
        } 
    }

NH_END(NH_SUCCESS)
}

// HELPER ==========================================================================================

static NH_RESULT Nh_JS_processMouseClick(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p)
{
NH_BEGIN()

    Nh_HTML_Node *Node_p = MouseEvent_p->Node_p;

    NH_JS_MouseEvent MouseEvent;
    MouseEvent.position_p[0] = MouseEvent_p->Event.position_p[0];
    MouseEvent.position_p[1] = MouseEvent_p->Event.position_p[1];

    NH_CHECK(Nh_JS_broadcastInternalEvent(Tab_p, Node_p, NH_JS_EVENT_CLICK, NULL, &MouseEvent))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_processMouseRelease(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p)
{
NH_BEGIN()

    // TODO

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_processMouseMove(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p)
{
NH_BEGIN()

    NH_CHECK(Nh_JS_broadcastInternalEvent(Tab_p, MouseEvent_p->Node_p, NH_JS_EVENT_MOUSE_MOVE, NULL, NULL))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_processMouseScroll(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, bool up)
{
NH_BEGIN()

    NH_JS_WheelEvent WheelEvent = {0.0};
    if (Tab_p->Window_p->Input.Keyboard.state_p[NH_IO_KEY_SHIFT_L]) {
        WheelEvent.deltaX = up ? 1.0 : -1.0;
    } else {
        WheelEvent.deltaY = up ? 1.0 : -1.0;
    }

    NH_CHECK(Nh_JS_broadcastInternalEvent(Tab_p, MouseEvent_p->Node_p, NH_JS_EVENT_WHEEL, NULL, &WheelEvent))

NH_END(NH_SUCCESS)
}

