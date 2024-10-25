// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"
#include "About.h"

#include "../Loader/Loader.h"
#include "../System/Logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// LOG THREAD ======================================================================================

NH_API_RESULT nh_core_logThread(
    nh_core_ThreadPool *ThreadPool_p, nh_core_Thread *Thread_p)
{
    char message_p[255] = {'\0'};
    char option_p[255] = {'\0'};

    char id_p[127] = {'\0'};
    sprintf(id_p, "nh-core:Threads:%p", Thread_p);

    for (int i = 0, count = 0; i < NH_MAX_WORKLOADS; ++i) 
    {
        nh_core_Workload *Workload_p = &ThreadPool_p->Workloads_p[i];

        if (Workload_p->Thread_p == Thread_p) 
        {
            sprintf(message_p, "%s", Workload_p->name_p);
            sprintf(option_p, "replace=%d", count++);
            nh_core_sendLogMessage(id_p, option_p, message_p);
    
            memset(message_p, 0, 255);

            sprintf(message_p, "  busy : %d", Workload_p->SignalCounter.ok);
            sprintf(option_p, "replace=%d", count++);
            nh_core_sendLogMessage(id_p, option_p, message_p);

            memset(message_p, 0, 255);

            sprintf(message_p, "  idle : %d", Workload_p->SignalCounter.idle);
            sprintf(option_p, "replace=%d", count++);
            nh_core_sendLogMessage(id_p, option_p, message_p);

            memset(message_p, 0, 255);
            memset(option_p, 0, 255);
        }
    }

    return NH_API_SUCCESS;
}

// LOG MODULES =====================================================================================

NH_API_RESULT nh_core_logModules()
{
    char message_p[255];
    char option_p[255] = {'\0'};
    char indent_p[64] = {'\0'};

    int maxLength = 0;
    for (int i = 0; i < NH_MODULE_E_COUNT; ++i) {
        if (strlen(NH_MODULE_NAMES_PP[i]) > maxLength) {maxLength = strlen(NH_MODULE_NAMES_PP[i]);}
    }

    for (int i = 0; i < NH_MODULE_E_COUNT; ++i) 
    {
        int indent = maxLength - strlen(NH_MODULE_NAMES_PP[i]);
        memset(indent_p, ' ', indent);

        if (!NH_LOADER.Modules_p[i].lib_p) {
            sprintf(message_p, "%s%s : not loaded", NH_MODULE_NAMES_PP[i], indent_p);
        }
        else {
            int *ver_p = NULL;
            switch (i) {
                case NH_MODULE_CORE :
                    ver_p = NH_CORE_VERSION_P;
                    break;
                case NH_MODULE_WSI :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_WSI_VERSION_P");
                    break;
                case NH_MODULE_NETWORK :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_NETWORK_VERSION_P");
                    break;
                case NH_MODULE_ECMASCRIPT :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_ECMASCRIPT_VERSION_P");
                    break;
                case NH_MODULE_HTML :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_HTML_VERSION_P");
                    break;
                case NH_MODULE_WEBIDL :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_WEBIDL_VERSION_P");
                    break;
                case NH_MODULE_ENCODING :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_ENCODING_VERSION_P");
                    break;
                case NH_MODULE_DOM :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_DOM_VERSION_P");
                    break;
                case NH_MODULE_GFX :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_GFX_VERSION_P");
                    break;
                case NH_MODULE_CSS :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_CSS_VERSION_P");
                    break;
                case NH_MODULE_RENDERER :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_RENDERER_VERSION_P");
                    break;
                case NH_MODULE_URL :
                    ver_p = NH_LOADER.loadSymbol_f(i, NH_LOADER.Modules_p[i].major, "NH_URL_VERSION_P");
                    break;
            }
            if (!ver_p) {return NH_API_ERROR_BAD_STATE;}
            sprintf(message_p, "%s%s : ver.%d.%d.%d.%d", NH_MODULE_NAMES_PP[i], indent_p, ver_p[0], ver_p[1], ver_p[2], ver_p[3]);
        }

        sprintf(option_p, "replace=%d", i);
        nh_core_sendLogMessage("nh-core:Modules", option_p, message_p);

        memset(message_p, 0, 255);
        memset(option_p, 0, 255);
        memset(indent_p, 0, 64);
    }

    return NH_API_SUCCESS;
}

void nh_core_logSystemInfo(
    char *info_p, int line)
{
    char option_p[64] = {'\0'};
    sprintf(option_p, "replace=%d", line);
    nh_core_sendLogMessage("nh-core:System", option_p, info_p);

    return;
}

// FLOW LOGGING ====================================================================================

NH_API_RESULT _nh_core_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.core) {return NH_API_SUCCESS;}
//    return _nh_begin(file_p, function_p);
}

NH_API_RESULT _nh_core_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.core) {return NH_API_SUCCESS;}
//    return _nh_end(file_p, function_p);
}

NH_API_RESULT _nh_core_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_API_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.core) {return result;}
//    _nh_diagnosticEnd(file_p, function_p, NH_API_RESULTS_PP[result], line, result == NH_API_SUCCESS);
    return result;
}
