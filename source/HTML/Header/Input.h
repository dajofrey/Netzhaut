#ifndef NH_HTML_INPUT_H
#define NH_HTML_INPUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../IO/Header/Input.h"
#include "../../Core/Header/RingIterator.h"
#include "../../API/Header/Netzhaut.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;

#endif

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    typedef struct Nh_HTML_MouseEvent {
        NH_IO_MouseEvent Event; 
        Nh_HTML_Node *Node_p;
    } Nh_HTML_MouseEvent;

    typedef struct Nh_HTML_KeyboardEvent {
        NH_IO_KeyboardEvent Event; 
        Nh_HTML_Node *Node_p;
    } Nh_HTML_KeyboardEvent;

    typedef struct Nh_HTML_Input {

        struct Events {
            Nh_RingIterator Keyboard;
            Nh_RingIterator Mouse;
        } Events;

        struct Marks {
            NH_IO_InputMark Window; /**<Used to figure out not yet handled window-input events.*/ 
            NH_IO_InputMark CSS;
            NH_IO_InputMark JS; 
        } Marks;

    } Nh_HTML_Input;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_RESULT Nh_HTML_initInput(
        Nh_HTML_Input *Input_p, int bufferSize
    );

    void Nh_HTML_freeInput(
        Nh_HTML_Input *Input_p
    );

    NH_RESULT Nh_HTML_processInput(
        Nh_Tab *Tab_p
    );

/** @} */

#endif 
