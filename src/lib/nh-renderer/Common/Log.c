// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"

#include "../../nh-core/System/Logger.h"

#include <stdio.h>
#include <string.h>

// LOG EXECUTION FLOW ==============================================================================

NH_API_RESULT _nh_renderer_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_begin(file_p, function_p);
}

NH_API_RESULT _nh_renderer_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_end(file_p, function_p);
}

NH_API_RESULT _nh_renderer_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_API_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

