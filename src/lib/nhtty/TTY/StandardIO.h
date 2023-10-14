#ifndef NH_TTY_STANDARD_IO_H
#define NH_TTY_STANDARD_IO_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "View.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    NH_TTY_RESULT nh_tty_getStandardOutputWindowSize(
        int *cols_p, int *rows_p
    );
    
    NH_TTY_RESULT nh_tty_readStandardInput(
        nh_tty_TTY *TTY_p
    );

    NH_TTY_RESULT nh_tty_writeCursorToStandardOutput(
        int x, int y
    );

    NH_TTY_RESULT nh_tty_writeToStandardOutput(
        nh_tty_Row *Rows_p, int cols, int rows
    );

    NH_TTY_RESULT nh_tty_claimStandardIO(
        nh_tty_TTY *TTY_p
    );
    
    NH_TTY_RESULT nh_tty_unclaimStandardIO(
        nh_tty_TTY *TTY_p
    );

    NH_BOOL nh_tty_claimsStandardIO(
        nh_tty_TTY *TTY_p
    );

/** @} */

#endif 
