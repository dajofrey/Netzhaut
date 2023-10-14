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
#include <string.h>

// LOG EXECUTION FLOW ==============================================================================

NH_RENDERER_RESULT _nh_renderer_logBegin(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_begin(file_p, function_p);
}

NH_RENDERER_RESULT _nh_renderer_logEnd(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_end(file_p, function_p);
}

NH_RENDERER_RESULT _nh_renderer_logDiagnosticEnd(
    const NH_BYTE *file_p, const NH_BYTE *function_p, NH_RENDERER_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

