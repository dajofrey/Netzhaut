// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Input.h"
#include "../Header/Macros.h"

#include "../X11/Header/Input.h"
#include "../Win/Header/Input.h"

#include "../../Core/Header/Window.h"
#include "../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// DATA ============================================================================================

const char *trigger_pp[] = {
    "UNDEFINED",
    "PRESS",
    "RELEASE",
    "UP",
    "DOWN",
    "MOVE",
    "GET_FOCUS",
    "LOOSE_FOCUS",
};

// INPUT ===========================================================================================

NH_RESULT Nh_IO_initInput(
    NH_IO_Input *Input_p, int bufferSize) 
{
NH_BEGIN()

    NH_CHECK(Nh_IO_initMouse(&Input_p->Mouse, bufferSize))
    NH_CHECK(Nh_IO_initKeyboard(&Input_p->Keyboard, bufferSize))

NH_END(NH_SUCCESS);
}

void Nh_IO_initInputMark(
    NH_IO_InputMark *Mark_p)
{
NH_BEGIN()

    Nh_initRingIteratorMark(&Mark_p->Mouse);
    Nh_initRingIteratorMark(&Mark_p->Keyboard);

NH_SILENT_END()
}

void Nh_IO_freeInput(
    NH_IO_Input *Input_p) 
{
NH_BEGIN()

    Nh_freeRingIterator(&Input_p->Mouse.Events);
    Nh_freeRingIterator(&Input_p->Keyboard.Events);

NH_SILENT_END();
}

NH_RESULT Nh_IO_getInput(
    Nh_Window *Window_p) 
{
NH_BEGIN()

    switch (Window_p->WSI.type)
    {
        case NH_IO_TYPE_X11 : NH_CHECK(NH_IO_X11_getInput(Window_p)) break;
        case NH_IO_TYPE_WIN : NH_CHECK(NH_IO_Win_getInput(Window_p)) break;
        default             : NH_END(NH_ERROR_BAD_STATE)
    }

NH_END(NH_SUCCESS);
}

const char *Nh_IO_stringifyTrigger(
    NH_IO_TRIGGER trigger)
{
NH_BEGIN()
NH_END(trigger_pp[trigger])
}

