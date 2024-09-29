#ifndef NH_NETWORK_LOG_H
#define NH_NETWORK_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "Result.h"

#endif

/** @addtogroup lib_nh-network_functions
 *  @{
 */

    void nh_network_logReceive(
        char *data_p
    );

    void nh_network_logSend(
        char *data_p
    );

    NH_API_RESULT _nh_network_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_network_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_network_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_API_RESULT result, int line
    );

/** @} */

#endif 
