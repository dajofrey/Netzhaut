// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"
#include "Macros.h"

#include "../../nh-core/System/Logger.h"

#include <stdio.h>
#include <string.h>

// LOG IDNA MAPPING TABLE ==========================================================================

NH_API_RESULT nh_url_logIDNAMappingTable(
    nh_url_IDNAMappingTable *MappingTable_p)
{
NH_URL_BEGIN()

    char message_p[1024] = {0};
    memset(message_p, 0, 1024);

    for (int i = 0; i < MappingTable_p->Entries.length; ++i)
    {
        nh_url_IDNAMappingTableEntry *Entry_p = &((nh_url_IDNAMappingTableEntry*)MappingTable_p->Entries.p)[i];

        sprintf(message_p, "0x%x", Entry_p->begin);
        if (Entry_p->end != 0) {
            sprintf(message_p+strlen(message_p), " - 0x%x", Entry_p->end);
        }

        switch (Entry_p->status)
        {
            case NH_URL_IDNA_STATUS_UNDEFINED              : 
                sprintf(message_p+strlen(message_p), " undefined"); 
                break; 
            case NH_URL_IDNA_STATUS_VALID                  :
                sprintf(message_p+strlen(message_p), " valid"); 
                break; 
            case NH_URL_IDNA_STATUS_IGNORED                :
                sprintf(message_p+strlen(message_p), " ignored"); 
                break; 
            case NH_URL_IDNA_STATUS_MAPPED                 :
                sprintf(message_p+strlen(message_p), " mapped"); 
                break; 
            case NH_URL_IDNA_STATUS_DEVIATION              :
                sprintf(message_p+strlen(message_p), " deviation"); 
                break; 
            case NH_URL_IDNA_STATUS_DISALLOWED             :
                sprintf(message_p+strlen(message_p), " disallowed"); 
                break; 
            case NH_URL_IDNA_STATUS_DISALLOWED_STD3_VALID  :
                sprintf(message_p+strlen(message_p), " disallowed std3 valid"); 
                break; 
            case NH_URL_IDNA_STATUS_DISALLOWED_STD3_MAPPED :
                sprintf(message_p+strlen(message_p), " disallowed std3 mapped"); 
                break; 
        }

        nh_core_sendLogMessage("nh-url:IDNAMappingTable", NULL, message_p);

        memset(message_p, 0, 1024);
    }

NH_URL_END(NH_API_SUCCESS)
}

// LOG FLOW ========================================================================================

NH_API_RESULT _nh_url_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_begin(file_p, function_p);
}

NH_API_RESULT _nh_url_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_end(file_p, function_p);
}

NH_API_RESULT _nh_url_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_API_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

