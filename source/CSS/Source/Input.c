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

    Nh_CSS_deactivate(Tab_p, NH_CSS_PSEUDO_CLASS_ACTIVE);
    for (int i = 0; i < Tab_p->Document.Tree.Flat.Unformatted.count; ++i) {
        NH_CHECK(Nh_CSS_computeProperties(Tab_p, Nh_getListItem(&Tab_p->Document.Tree.Flat.Unformatted, i), NH_FALSE))
    }

    NH_CHECK(Nh_CSS_arrange(Tab_p, NH_TRUE))
    NH_CHECK(Nh_HTML_recomputeFormattedTree(Tab_p))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_CSS_processMouseClick(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, NH_BOOL clear)
{
NH_BEGIN()

    if (clear) {
        Nh_CSS_deactivate(Tab_p, NH_CSS_PSEUDO_CLASS_FOCUS);
        for (int i = 0; i < Tab_p->Document.Tree.Flat.Unformatted.count; ++i) {
            NH_CHECK(Nh_CSS_computeProperties(Tab_p, Nh_getListItem(&Tab_p->Document.Tree.Flat.Unformatted, i), NH_FALSE))
        }
    }

    Nh_HTML_Node *Node_p = MouseEvent_p->Node_p;
    if (Node_p == NULL) {NH_END(NH_SUCCESS)}

    for (int i = 0; i < Node_p->Properties.count; ++i) 
    {
        NH_CSS_GenericProperty *Property_p = Nh_CSS_getProperty(&Node_p->Properties, i);
        
        Nh_CSS_activate(Tab_p, Node_p, Property_p, NH_CSS_PSEUDO_CLASS_ACTIVE);
        Nh_CSS_activate(Tab_p, Node_p, Property_p, NH_CSS_PSEUDO_CLASS_FOCUS);
    }
    
    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) 
    {
        Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Children.Unformatted, i);

        for (int j = 0; j < Child_p->Properties.count; ++j)
        {
            NH_CSS_GenericProperty *ChildProperty_p = Nh_CSS_getProperty(&Child_p->Properties, j);
            
            Nh_CSS_activateChild(Tab_p, Child_p, ChildProperty_p, NH_CSS_PSEUDO_CLASS_ACTIVE);
            Nh_CSS_activateChild(Tab_p, Child_p, ChildProperty_p, NH_CSS_PSEUDO_CLASS_FOCUS);
        }
    }

    NH_CHECK(Nh_CSS_computeProperties(Tab_p, Node_p, NH_FALSE))
    NH_CHECK(Nh_CSS_arrange(Tab_p, NH_TRUE))
    NH_CHECK(Nh_HTML_recomputeFormattedTree(Tab_p))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_CSS_processMouseMove(
    Nh_Tab *Tab_p, Nh_HTML_MouseEvent *MouseEvent_p, NH_BOOL clear)
{
NH_BEGIN()

    if (clear) {
        Nh_CSS_deactivate(Tab_p, NH_CSS_PSEUDO_CLASS_HOVER);
        for (int i = 0; i < Tab_p->Document.Tree.Flat.Unformatted.count; ++i) {
            NH_CHECK(Nh_CSS_computeProperties(Tab_p, Nh_getListItem(&Tab_p->Document.Tree.Flat.Unformatted, i), NH_FALSE))
        }
    }

    Nh_HTML_Node *Node_p = MouseEvent_p->Node_p;
    if (Node_p == NULL) {NH_END(NH_SUCCESS)}

    for (int i = 0; i < Node_p->Properties.count; ++i) {
        NH_CHECK(Nh_CSS_activate(
            Tab_p, Node_p, Nh_CSS_getProperty(&Node_p->Properties, i), NH_CSS_PSEUDO_CLASS_HOVER
        ))
    }

    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) 
    {
        Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Children.Unformatted, i);

        for (int j = 0; j < Child_p->Properties.count; ++j)
        {
            NH_CSS_GenericProperty *ChildProperty_p = Nh_CSS_getProperty(&Child_p->Properties, j);
            Nh_CSS_activateChild(Tab_p, Child_p, ChildProperty_p, NH_CSS_PSEUDO_CLASS_HOVER);
        }
    }

    NH_CHECK(Nh_CSS_computeProperties(Tab_p, Node_p, NH_FALSE))
    NH_CHECK(Nh_CSS_arrange(Tab_p, NH_TRUE))
    NH_CHECK(Nh_HTML_recomputeFormattedTree(Tab_p))

NH_END(NH_SUCCESS)
}

