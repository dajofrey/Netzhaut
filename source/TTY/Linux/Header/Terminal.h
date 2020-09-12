#ifndef NH_TTY_LINUX_TERMINAL_H
#define NH_TTY_LINUX_TERMINAL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Header/Terminal.h"

#include <sys/types.h>
#include <termios.h>
#include <stdbool.h>

#endif

/** @addtogroup TTYFunctions Functions
 *  \ingroup TTY 
 *  @{
 */

    void Nh_TTY_Linux_init(
    ); 

    void Nh_TTY_Linux_die(
        const char *s
    ); 

    void Nh_TTY_Linux_update(
    ); 
    
    void Nh_TTY_Linux_initTerminal(
        Nh_TTY_Terminal *Terminal_p
    ); 

/** @} */

#endif 
