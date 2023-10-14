#ifndef NH_TTY_LOG_H
#define NH_TTY_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"

#include "../Shell/Shell.h"

#endif

/** @addtogroup lib_nhtty_functions
 *  @{
 */

//    NH_TTY_RESULT nh_tty_logTerminalMode(
//        nh_tty_Shell *Shell_p, int mode
//    );
//
//    NH_TTY_RESULT nh_tty_logCSI(
//        nh_tty_Shell *Shell_p, NH_BYTE type
//    );

    NH_TTY_RESULT _nh_tty_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_TTY_RESULT _nh_tty_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_TTY_RESULT _nh_tty_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_TTY_RESULT result, int line
    );

/** @} */

#endif 
