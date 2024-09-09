#ifndef NH_URL_LOG_H
#define NH_URL_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "../IDNA/IDNAMappingTable.h"

#endif

/** @addtogroup lib_nh-url_functions
 *  @{
 */

    NH_URL_RESULT nh_url_logIDNAMappingTable(
        nh_url_IDNAMappingTable *MappingTable_p
    );

    NH_URL_RESULT _nh_url_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_URL_RESULT _nh_url_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_URL_RESULT _nh_url_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_URL_RESULT result, int line
    );

/** @} */

#endif 
