// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Mouse.h"
#include "../Header/Macros.h"

#include "../../Core/Header/String.h"
#include "../../Core/Header/Memory.h"

#include "../../JavaScript/DOM/Header/WheelEvent.h"
#include "../../HTML/Header/Document.h"
#include "../../CSS/Header/Associate.h"
#include "../../CSS/Header/Box.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

// INIT ============================================================================================

NH_RESULT Nh_IO_initMouse(
    NH_IO_Mouse *Mouse_p, int bufferSize)
{
NH_BEGIN()

    NH_CHECK(Nh_initRingIterator(&Mouse_p->Events, bufferSize, sizeof(NH_IO_MouseEvent)))
    Nh_initRingIteratorMark(&Mouse_p->dumpMark);

    for (int i = 0; i < bufferSize; ++i) 
    {
        NH_IO_MouseEvent *Event_p = Nh_advanceRingIterator(&Mouse_p->Events);

        Event_p->position_p[0] = -1;
        Event_p->position_p[1] = -1;
        Event_p->type    = NH_IO_MOUSE_UNDEFINED;
        Event_p->trigger = NH_IO_TRIGGER_UNDEFINED;
    }

NH_END(NH_SUCCESS)
}

// HANDLE ==========================================================================================

NH_RESULT Nh_IO_processRawMouseInput(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

//    NH_IO_Input *Input_p = &Tab_p->Window_p->Input;
//    NH_IO_Record *Record_p = &Tab_p->Records.Mouse;
//    NH_IO_Mouse *Mouse_p = &Input_p->Mouse;
//    
//    while (1) {
//        int index = Nh_IO_getNextIndex(&Mouse_p->Tracker, Record_p); 
//        if (index > -1) { printf("%d\n", index);
//NH_CHECK(Nh_IO_addMouseEvent(Tab_p, Input_p, index))}
//        else {break;}
//    }
    
NH_END(NH_SUCCESS)
}

// DUMP ============================================================================================

char *Nh_IO_dumpMouse(
    NH_IO_Mouse *Mouse_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_IO_MouseEvent *MouseEvent_p = Nh_incrementRingIteratorMark(
        &Mouse_p->Events, &Mouse_p->dumpMark
    );
    if (MouseEvent_p == NULL) {NH_END(NULL)}

    Nh_String *String_p = Nh_allocateString(NULL);

    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "MOUSE %d/%d ", MouseEvent_p->position_p[0], MouseEvent_p->position_p[1]))
    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "%s\n", Nh_IO_stringifyTrigger(MouseEvent_p->trigger)))
  
//    Nh_IO_stringifyMouseState(MouseEvent_p->type, str_p);
//    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "Type   : %s\n", str_p))

    char *string_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);

#include NH_DEFAULT_CHECK

NH_END(string_p)
}

// STRINGIFY =======================================================================================

NH_RESULT Nh_IO_stringifyMouseState(
    NH_IO_MOUSE_FLAG state, char *string_p)
{
NH_BEGIN()

    int index = 0;
    char *values_pp[] = {
        "LEFT ", 
        "RIGHT ",  
        "MIDDLE ",
        "SCROLL ", 
        "MOVE ",   
    };

    if (state & NH_IO_MOUSE_FLAG_LEFT)   {memcpy(string_p + index, values_pp[0], sizeof(char) * 5); index += 5;}
    if (state & NH_IO_MOUSE_FLAG_RIGHT)  {memcpy(string_p + index, values_pp[1], sizeof(char) * 6); index += 6;}
    if (state & NH_IO_MOUSE_FLAG_MIDDLE) {memcpy(string_p + index, values_pp[2], sizeof(char) * 7); index += 7;}
    if (state & NH_IO_MOUSE_FLAG_SCROLL) {memcpy(string_p + index, values_pp[3], sizeof(char) * 7); index += 7;}
    if (state & NH_IO_MOUSE_FLAG_MOVE)   {memcpy(string_p + index, values_pp[4], sizeof(char) * 5); index += 5;}

    string_p[index] = '\0';

NH_END(NH_SUCCESS)
}

