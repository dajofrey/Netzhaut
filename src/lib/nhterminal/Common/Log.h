#ifndef NH_TERMINAL_LOG_H
#define NH_TERMINAL_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#endif

/** @addtogroup lib_nhterminal_functions
 *  @{
 */

    NH_TERMINAL_RESULT _nh_terminal_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_TERMINAL_RESULT _nh_terminal_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_TERMINAL_RESULT _nh_terminal_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_TERMINAL_RESULT result, int line
    );

/** @} */

#endif 
