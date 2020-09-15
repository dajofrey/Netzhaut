// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Input.h"
#include "../Header/Associate.h"
#include "../Header/Macros.h"
#include "../Header/Arrange.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/RingIterator.h"

#include "../../Graphics/Header/Node.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// DECLARE ========================================================================================

static NH_RESULT Nh_CSS_processMouseRelease(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, NH_BOOL clear
);
static NH_RESULT Nh_CSS_processMouseClick(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, NH_BOOL clear
);
static NH_RESULT Nh_CSS_processMouseMove(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, NH_BOOL clear
);

// API =============================================================================================

NH_RESULT Nh_CSS_processInput(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_BOOL clear = NH_FALSE;
    NH_IO_TRIGGER oldTrigger = -1;

    while (1) 
    {
        Nh_HTML_MouseEvent *MouseEvent_p = Nh_incrementRingIteratorMark(
            &Tab_p->Document.Input.Events.Mouse, &Tab_p->Document.Input.Marks.CSS.Mouse
        );

        if (MouseEvent_p == NULL) {break;}

        clear = MouseEvent_p->Event.trigger != oldTrigger;

        switch (MouseEvent_p->Event.trigger)
        {
            case NH_IO_TRIGGER_RELEASE : NH_CHECK(Nh_CSS_processMouseRelease(Tab_p, MouseEvent_p, clear)) break;
            case NH_IO_TRIGGER_PRESS   : NH_CHECK(Nh_CSS_processMouseClick(Tab_p, MouseEvent_p, clear)) break;
            case NH_IO_TRIGGER_MOVE    : NH_CHECK(Nh_CSS_processMouseMove(Tab_p, MouseEvent_p, clear)) break;
        } 

        oldTrigger = MouseEvent_p->Event.trigger;
    }

NH_END(NH_SUCCESS)
}

// HELPER ==========================================================================================

static NH_RESULT Nh_CSS_processMouseRelease(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, NH_BOOL clear)
{
NH_BEGIN()

    if (!clear) {NH_END(NH_SUCCESS)}

    Nh_CSS_deactivatePseudoClass(&Tab_p->Document.Tree, NH_CSS_PSEUDO_CLASS_ACTIVE);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_CSS_processMouseClick(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, NH_BOOL clear)
{
NH_BEGIN()

    if (clear) {Nh_CSS_deactivatePseudoClass(&Tab_p->Document.Tree, NH_CSS_PSEUDO_CLASS_FOCUS);}

    Nh_HTML_Node *Node_p = MouseEvent_p->Node_p;
    if (Node_p == NULL) {NH_END(NH_SUCCESS)}

    Nh_CSS_activatePseudoClass(Node_p, NH_CSS_PSEUDO_CLASS_ACTIVE);
    Nh_CSS_activatePseudoClass(Node_p, NH_CSS_PSEUDO_CLASS_FOCUS);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_CSS_processMouseMove(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, NH_BOOL clear)
{
NH_BEGIN()

    if (clear) {Nh_CSS_deactivatePseudoClass(&Tab_p->Document.Tree, NH_CSS_PSEUDO_CLASS_HOVER);}

    Nh_HTML_Node *Node_p = MouseEvent_p->Node_p;
    if (Node_p == NULL) {NH_END(NH_SUCCESS)}

    Nh_CSS_activatePseudoClass(Node_p, NH_CSS_PSEUDO_CLASS_HOVER);

NH_END(NH_SUCCESS)
}

