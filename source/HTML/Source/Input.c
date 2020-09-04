// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Input.h"
#include "../Header/Macros.h"
#include "../Header/Document.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/RingIterator.h"

#include "../../CSS/Header/Box.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <string.h>

// DECLARE ========================================================================================

static NH_RESULT Nh_HTML_processMouseEvent(
    Nh_Tab *Tab_p, NH_IO_MouseEvent *IOEvent_p
);
static Nh_HTML_Node *Nh_HTML_getHit(
    Nh_Tab *Tab_p, int position_p[2]
);

// IMPLEMENT ======================================================================================

NH_RESULT Nh_HTML_initInput(
    Nh_HTML_Input *Input_p, int bufferSize)
{
NH_BEGIN()

    NH_CHECK(Nh_initRingIterator(&Input_p->Events.Mouse, bufferSize, sizeof(Nh_HTML_MouseEvent)))
    NH_CHECK(Nh_initRingIterator(&Input_p->Events.Keyboard, bufferSize, sizeof(Nh_HTML_KeyboardEvent)))

    Nh_IO_initInputMark(&Input_p->Marks.Window);
    Nh_IO_initInputMark(&Input_p->Marks.CSS);
    Nh_IO_initInputMark(&Input_p->Marks.JS);

NH_END(NH_SUCCESS)
}

void Nh_HTML_freeInput(
    Nh_HTML_Input *Input_p)
{
NH_BEGIN()

    Nh_freeRingIterator(&Input_p->Events.Mouse);
    Nh_freeRingIterator(&Input_p->Events.Keyboard);

NH_SILENT_END()
}

// PROCESS =========================================================================================

NH_RESULT Nh_HTML_processInput(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    while (1) {
        NH_IO_MouseEvent *MouseEvent_p = Nh_incrementRingIteratorMark(
            &Tab_p->Window_p->Input.Mouse.Events, &Tab_p->Document.Input.Marks.Window.Mouse
        );
        if (MouseEvent_p == NULL) {break;}
        NH_CHECK(Nh_HTML_processMouseEvent(Tab_p, MouseEvent_p))
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_processMouseEvent(
    Nh_Tab *Tab_p, NH_IO_MouseEvent *IOEvent_p)
{
NH_BEGIN()

    Nh_HTML_Node *Node_p = Nh_HTML_getHit(Tab_p, IOEvent_p->position_p);

    while (Node_p != NULL) {
        Nh_HTML_MouseEvent *Event_p = Nh_advanceRingIterator(&Tab_p->Document.Input.Events.Mouse);
        Event_p->Node_p = Node_p;
        Event_p->Event  = *IOEvent_p;
        Node_p = Node_p->Parent_p;
    }

NH_END(NH_SUCCESS)
}

// HIT =============================================================================================

static Nh_HTML_Node *Nh_HTML_getHit(
    Nh_Tab *Tab_p, int position_p[2])
{
NH_BEGIN()

    // mouse position in 0/1 space
    float relativeWidth  = ((float)position_p[0]) / ((float)Tab_p->Window_p->Info.pxSize_p[0]);
    float relativeHeight = ((float)position_p[1]) / ((float)Tab_p->Window_p->Info.pxSize_p[1]);

    // mouse position in 1/-1 space
    float mouseWidth  = (2.0f * relativeWidth) - 1.0f;
    float mouseHeight = (2.0f * relativeHeight) - 1.0f;

    // tab size modifier for the lower right coord in 1/-1 space
//    float size_p[2] = {
//        (Tab_p->Info.relativeSize_p[0] *  2.0f) - 2.0f,
//        (Tab_p->Info.relativeSize_p[1] * 2.0f) - 2.0f
//    };

    Nh_HTML_Node *Return_p = NULL;

    for (int i = 0; i < Tab_p->Document.Tree.Flat.Unformatted.count; ++i) 
    {
        Nh_HTML_Node *Node_p = Nh_getListItem(&Tab_p->Document.Tree.Flat.Unformatted, i);

        if (Nh_HTML_isMetaNode(Node_p) || Node_p->tag == NH_HTML_TAG_HTML 
        || (Tab_p->Flags.crop && Node_p->tag == NH_HTML_TAG_BODY)
        || (Node_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_NONE)) 
        {continue;}

        Nh_CSS_Box Box = Nh_CSS_getBorderBox(Node_p);
        Nh_CSS_resize(Tab_p, Node_p, &Box);

        // take into account tab offset and size in 1/-1 space
        float absolutePosition_p[4] = {
            Box.TopLeft.x + Tab_p->Info.relativeOffset_p[0] * 2.0f,
            Box.TopLeft.y + Tab_p->Info.relativeOffset_p[1] * 2.0f,
            (Box.BottomRight.x + Tab_p->Info.relativeOffset_p[0] * 2.0f),
            (Box.BottomRight.y + Tab_p->Info.relativeOffset_p[1] * 2.0f)
//            (Box.BottomRight.x + Tab_p->Info.relativeOffset_p[0] * 2.0f) + size_p[0],
//            (Box.BottomRight.y + Tab_p->Info.relativeOffset_p[1] * 2.0f) + size_p[1]
        };

        if (i > 0) {
            absolutePosition_p[2] = (Box.BottomRight.x + Tab_p->Info.relativeOffset_p[0] * 2.0f);
            absolutePosition_p[3] = (Box.BottomRight.y + Tab_p->Info.relativeOffset_p[1] * 2.0f);
//            absolutePosition_p[2] = (Box.BottomRight.x + Tab_p->Info.relativeOffset_p[0] * 2.0f) * Tab_p->Info.relativeSize_p[0];
//            absolutePosition_p[3] = (Box.BottomRight.y + Tab_p->Info.relativeOffset_p[1] * 2.0f) * Tab_p->Info.relativeSize_p[1];
        }

        // check if inside node
        if ((absolutePosition_p[0] < mouseWidth && absolutePosition_p[2] > mouseWidth)
        &&  (absolutePosition_p[1] < mouseHeight && absolutePosition_p[3] > mouseHeight))
        {
            Return_p = Node_p;
        }
    }

NH_END(Return_p)
}

