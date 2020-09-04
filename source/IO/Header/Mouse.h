#ifndef NH_IO_MOUSE_H
#define NH_IO_MOUSE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Core/Header/RingIterator.h"
#include "../../Core/Header/System.h"

typedef struct Nh_Tab Nh_Tab;
typedef struct NH_IO_Input NH_IO_Input;

#endif

/** @addtogroup IOEnums Enums
 *  \ingroup IO
 *  @{
 */

    typedef enum NH_IO_MOUSE {
        NH_IO_MOUSE_UNDEFINED,
        NH_IO_MOUSE_LEFT,      
        NH_IO_MOUSE_RIGHT,    
        NH_IO_MOUSE_MIDDLE,    
        NH_IO_MOUSE_SCROLL,    
        NH_IO_MOUSE_MOVE,      
    } NH_IO_MOUSE;

    typedef enum NH_IO_MOUSE_FLAG {
        NH_IO_MOUSE_FLAG_UNDEFINED = 1 << 0,
        NH_IO_MOUSE_FLAG_LEFT      = 1 << 1,      
        NH_IO_MOUSE_FLAG_RIGHT     = 1 << 2,    
        NH_IO_MOUSE_FLAG_MIDDLE    = 1 << 3,    
        NH_IO_MOUSE_FLAG_SCROLL    = 1 << 4,    
        NH_IO_MOUSE_FLAG_MOVE      = 1 << 5,      
    } NH_IO_MOUSE_FLAG;

/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */  

    typedef struct NH_IO_MouseEvent {
        NH_IO_MOUSE type; 
        int trigger;     
        int position_p[2];
    } NH_IO_MouseEvent;

    typedef struct NH_IO_Mouse {
        Nh_RingIterator Events;
        Nh_RingIteratorMark dumpMark;
        NH_IO_MOUSE_FLAG state;
    } NH_IO_Mouse;

/** @} */

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    NH_RESULT Nh_IO_initMouse(
        NH_IO_Mouse *Mouse_p, int bufferSize
    );

    NH_RESULT Nh_IO_processRawMouseInput(
        Nh_Tab *Tab_p
    );
    
    char *Nh_IO_dumpMouse(
        NH_IO_Mouse *Mouse_p
    );

    NH_RESULT Nh_IO_stringifyMouseState(
        NH_IO_MOUSE_FLAG state, char *string_p
    );

/** @} */

#endif 
