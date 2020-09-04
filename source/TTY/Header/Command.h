#ifndef NH_TTY_COMMAND_H
#define NH_TTY_COMMAND_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Terminal.h"

#endif

/** @addtogroup TTYFunctions Functions
 *  \ingroup TTY
 *  @{
 */

    NH_RESULT Nh_TTY_executeCommand(
        Nh_TTY_Terminal *Terminal_p, Nh_String *String_p        
    );

    NH_RESULT Nh_TTY_log(
        Nh_TTY_Terminal *Terminal_p, int argc, char **argv_pp, bool IPC
    ); 

    NH_RESULT Nh_TTY_tty(
        Nh_TTY_Terminal *Terminal_p, int argc, char **argv_pp, bool IPC
    ); 

/** @} */

#endif 
