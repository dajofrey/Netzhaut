// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Keyboard.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Memory.h"

#include "../../JavaScript/DOM/Header/Location.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

// DECLARE ========================================================================================

static void Nh_IO_dispatchKeyboardEvent(
    Nh_Tab *Tab_p, NH_IO_Input *Input_p, int index
);

// INIT ============================================================================================

NH_RESULT Nh_IO_initKeyboard(
    NH_IO_Keyboard *Keyboard_p, int bufferSize) 
{
NH_BEGIN()

    NH_CHECK(Nh_initRingIterator(&Keyboard_p->Events, bufferSize, sizeof(NH_IO_KeyboardEvent)))

    for (int i = 0; i < bufferSize; ++i) 
    {
        NH_IO_KeyboardEvent *Event_p = Nh_advanceRingIterator(&Keyboard_p->Events);

        Event_p->key     = NH_IO_KEY_UNDEFINED;
        Event_p->trigger = NH_IO_TRIGGER_UNDEFINED;
    }

    for (int i = 0; i < NH_IO_KEY_COUNT; ++i) {Keyboard_p->state_p[i] = false;}

NH_END(NH_SUCCESS);
}

// HANDLE ==========================================================================================

void Nh_IO_handleKeyboard(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

//    NH_IO_Input *Input_p = &Tab_p->Window_p->Input;
//    NH_IO_Keyboard *Keyboard_p = &Input_p->Keyboard;
//    
//    int eventCount = Keyboard_p->Tracker.overflow > Tab_p->InputRecord.keyboardOverflow ? 
//        ((Keyboard_p->Tracker.bufferSize - 1) - Tab_p->InputRecord.handledKeyboard) + (Keyboard_p->Tracker.count % Keyboard_p->Tracker.bufferSize) : 
//        (Keyboard_p->Tracker.count % Keyboard_p->Tracker.bufferSize) - Tab_p->InputRecord.handledKeyboard;
//
//    int first = Keyboard_p->Tracker.newest;
//    int index = eventCount;
//    
//    while (--index > 0) {if (--first == -1) {first = Keyboard_p->Tracker.bufferSize - 1;}}
//    index = first;
//
//    for (int i = 0; i < eventCount && i < Keyboard_p->Tracker.bufferSize; ++i) 
//    {
//        if (index >= Keyboard_p->Tracker.bufferSize) {index = 0;}
//        Nh_IO_dispatchKeyboardEvent(Tab_p, Input_p, index);
//        index++;
//    }
//    
//    if (Keyboard_p->Tracker.overflow > Tab_p->InputRecord.keyboardOverflow) {
//        Tab_p->InputRecord.handledKeyboard = eventCount; Tab_p->InputRecord.keyboardOverflow += 1;
//    } 
//    else {Tab_p->InputRecord.handledKeyboard += eventCount;}

NH_SILENT_END()
}

// DISPATCH ========================================================================================

static void Nh_IO_dispatchKeyboardEvent(
    Nh_Tab *Tab_p, NH_IO_Input *Input_p, int index)
{
NH_BEGIN()

//    NH_IO_Keyboard *Keyboard_p = &Input_p->Keyboard;
//
//    bool pressed = Keyboard_p->triggers_p[index] == NH_IO_TRIGGER_PRESS;
//    bool holdsLeftControl = Keyboard_p->press_p[NH_IO_KEY_CONTROL_L];

NH_SILENT_END()
}

