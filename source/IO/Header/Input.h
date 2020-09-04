#ifndef NH_IO_INPUT_H
#define NH_IO_INPUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Mouse.h"
#include "Keyboard.h"

#include "../../API/Header/Netzhaut.h"

typedef struct Nh_Window Nh_Window;

#endif

/** @addtogroup IOEnums Enums
 *  \ingroup IO
 *  @{
 */

    typedef enum NH_IO_TRIGGER {
        NH_IO_TRIGGER_UNDEFINED,  
        NH_IO_TRIGGER_PRESS,      
        NH_IO_TRIGGER_RELEASE,    
        NH_IO_TRIGGER_UP,         
        NH_IO_TRIGGER_DOWN,       
        NH_IO_TRIGGER_MOVE,       
        NH_IO_TRIGGER_GET_FOCUS,  
        NH_IO_TRIGGER_LOOSE_FOCUS, 
    } NH_IO_TRIGGER;
   
/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */

    typedef struct NH_IO_InputMark {
        Nh_RingIteratorMark Mouse;
        Nh_RingIteratorMark Keyboard;
    } NH_IO_InputMark;

    typedef struct NH_IO_Input {
        NH_IO_Mouse Mouse;
        NH_IO_Keyboard Keyboard;
//        bool drop;
//        void *dropData_pp[NH_INPUT_BUFFER_SIZE];
    } NH_IO_Input;

/** @} */

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    NH_RESULT Nh_IO_initInput(
        NH_IO_Input *Input_p, int bufferSize
    );

    void Nh_IO_initInputMark(
        NH_IO_InputMark *Mark_p
    );

    void Nh_IO_freeInput(
        NH_IO_Input *Input_p
    );

    NH_RESULT Nh_IO_getInput(
        Nh_Window *Window_p
    );
    
    const char *Nh_IO_stringifyTrigger(
        NH_IO_TRIGGER trigger
    );

/** @} */

#endif 
