// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"
#include "Macros.h"

#include "../../nh-core/Util/String.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/Debug.h"

#include <string.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

void nh_network_logReceive(
    char *data_p)
{
NH_NETWORK_BEGIN()

//    if (NH_CONFIG.Flags.Log.receive == true)
//    {
//        nh_core_String String = nh_core_initString(128); 
//
//        if (nh_core_appendFormatToString(&String, "%s\n", data_p) != NH_SUCCESS) {NH_NETWORK_SILENT_END()}
//
//        nh_core_log(String.bytes_p);
//        nh_core_freeString(&String);
//    }

NH_NETWORK_SILENT_END()
}

void nh_network_logSend(
    char *data_p)
{
NH_NETWORK_BEGIN()

//    if (NH_CONFIG.Flags.Log.send == true)
//    {
//        nh_core_String String = nh_core_initString(128); 
//
//        if (nh_core_appendFormatToString(&String, "%s\n", data_p) != NH_SUCCESS) {NH_NETWORK_SILENT_END()}
//
//        nh_core_log(String.bytes_p);
//        nh_core_freeString(&String);
//    }

NH_NETWORK_SILENT_END()
}

NH_API_RESULT _nh_network_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.net) {return NH_API_SUCCESS;}
//    return _nh_begin(file_p, function_p);
}

NH_API_RESULT _nh_network_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.net) {return NH_API_SUCCESS;}
//    return _nh_end(file_p, function_p);
}

NH_API_RESULT _nh_network_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_API_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.net) {return result;}
//    _nh_diagnosticEnd(file_p, function_p, NH_API_RESULTS_PP[result], line, result == NH_API_SUCCESS);
    return result;
}
