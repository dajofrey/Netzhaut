#ifndef NH_TTY_PROGRAM_H
#define NH_TTY_PROGRAM_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nhcore/Util/List.h"

typedef struct nh_tty_TTY nh_tty_TTY;

#endif

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_ProgramPrototype *nh_tty_createProgramPrototype(
        NH_TTY_PROGRAM_E type
    );

    NH_TTY_RESULT nh_tty_addProgram(
        nh_tty_TTY *TTY_p, nh_tty_ProgramPrototype *Prototype_p, NH_BOOL once
    );

    nh_tty_Program *nh_tty_createProgramInstance(
        nh_tty_ProgramPrototype *Prototype_p, NH_BOOL once
    );

    NH_TTY_RESULT nh_tty_destroyProgramInstance(
        nh_tty_Program *Program_p
    );

/** @} */

#endif 

