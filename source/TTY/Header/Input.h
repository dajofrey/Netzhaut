#ifndef NH_TTY_INPUT_H
#define NH_TTY_INPUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Terminal.h"

#endif
 
/** @addtogroup TTYEnums Enums
 *  \ingroup TTY
 *  @{
 */

    typedef enum NH_TTY_KEY {
        NH_TTY_KEY_ARROW_LEFT = 1000,
        NH_TTY_KEY_ARROW_RIGHT,
        NH_TTY_KEY_ARROW_UP,
        NH_TTY_KEY_ARROW_DOWN,
        NH_TTY_KEY_DEL,
        NH_TTY_KEY_HOME,
        NH_TTY_KEY_END,
        NH_TTY_KEY_PAGE_UP,
        NH_TTY_KEY_PAGE_DOWN
    } NH_TTY_KEY;

/** @} */

/** @addtogroup TTYFunctions Functions
 *  \ingroup TTY
 *  @{
 */

    void Nh_TTY_moveCursor(
        Nh_TTY_Tab *Tab_p, int key
    ); 
    
    void Nh_TTY_processKeypress(
        Nh_TTY_Terminal *Terminal_p
    ); 

/** @} */

#endif 
