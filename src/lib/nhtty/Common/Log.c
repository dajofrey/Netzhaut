// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"
#include "Macros.h"

#include "../../nhcore/System/Logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// LOG =============================================================================================

//NH_TTY_RESULT nh_tty_logTerminalMode(
//    nh_tty_Shell *Shell_p, int mode)
//{
//NH_TTY_BEGIN()
//
//    NH_BYTE *description_p = nh_tty_getTTYModeDescription(mode);
//
//    NH_BYTE message_p[255];
//    sprintf(message_p, "TERM %d %s", mode, description_p ? description_p : "(no description)");
//
//    NH_BYTE node_p[255];
//    sprintf(node_p, "nhtty:Shell:%p", Shell_p);
//
//    nh_core_sendLogMessage(node_p, NULL, message_p);
//
//NH_TTY_END(NH_TTY_SUCCESS)
//}
//
//NH_TTY_RESULT nh_tty_logCSI(
//    nh_tty_Shell *Shell_p, NH_BYTE type)
//{
//NH_TTY_BEGIN()
//
//    const NH_BYTE *description_p = nh_tty_getCSIDescription(type);
//
//    NH_BYTE message_p[255];
//    sprintf(message_p, "CSI \\e[%s %d %s", Shell_p->CSIEscape.buf, type, description_p ? description_p : "no description");
//
//    NH_BYTE node_p[255];
//    sprintf(node_p, "nhtty:Shell:%p", Shell_p);
//
//    nh_core_sendLogMessage(node_p, NULL, message_p);
//
//NH_TTY_END(NH_TTY_SUCCESS)
//}

// FLOW LOGGING ====================================================================================

NH_TTY_RESULT _nh_tty_logBegin(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.core) {return NH_SUCCESS;}
//    return _nh_begin(file_p, function_p);
}

NH_TTY_RESULT _nh_tty_logEnd(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.core) {return NH_SUCCESS;}
//    return _nh_end(file_p, function_p);
}

NH_TTY_RESULT _nh_tty_logDiagnosticEnd(
    const NH_BYTE *file_p, const NH_BYTE *function_p, NH_TTY_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.core) {return result;}
//    _nh_diagnosticEnd(file_p, function_p, NH_CORE_RESULTS_PP[result], line, result == NH_SUCCESS);
    return result;
}

