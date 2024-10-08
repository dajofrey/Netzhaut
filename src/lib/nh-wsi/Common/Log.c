// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"
#include "Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/System/Logger.h"

#include <string.h>
#include <stdio.h>

// EVENT ===========================================================================================

void nh_wsi_logEvent( // TODO multithreading
    nh_wsi_Window *Window_p, nh_api_WSIEvent *Event_p)
{
NH_WSI_BEGIN()

NH_WSI_SILENT_END()
}

// FLOW ============================================================================================

NH_API_RESULT _nh_wsi_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.io) {return NH_API_SUCCESS;}
//    return _nh_begin(file_p, function_p);
}

NH_API_RESULT _nh_wsi_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.io) {return NH_API_SUCCESS;}
//    return _nh_end(file_p, function_p);
}

NH_API_RESULT _nh_wsi_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_API_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.io) {return result;}
//    _nh_diagnosticEnd(file_p, function_p, NH_API_RESULTS_PP[result], line, result == NH_API_SUCCESS);
    return result;
}

