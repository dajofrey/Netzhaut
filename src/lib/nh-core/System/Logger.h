#ifndef NH_CORE_LOGGER_H
#define NH_CORE_LOGGER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Util/String.h"
#include "../Util/List.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-core_structs
 *  @{
 */

    typedef struct nh_core_LoggerNode {
        char *name_p;
        nh_List Children;
        nh_List Messages;
        struct nh_core_LoggerNode *Parent_p;
    } nh_core_LoggerNode;

    typedef struct nh_core_Logger {
        nh_core_LoggerNode Root;
    } nh_core_Logger;

/** @} */

/** @addtogroup lib_nh-core_vars
 *  @{
 */

    extern nh_core_Logger NH_LOGGER;

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_API_RESULT nh_core_initLogger(
    );

    NH_API_RESULT nh_core_freeLogger(
    );

    NH_API_RESULT nh_core_addLogCallback(
        nh_api_logCallback_f logCallback_f
    );
    
    NH_API_RESULT _nh_core_sendLogMessage(
        char *node_p, char *options_p, char *message_p
    );
    
    NH_API_RESULT nh_core_sendLogMessage(
        char *node_p, char *options_p, char *message_p
    );

    NH_API_RESULT _nh_begin(
        const char *file_p, const char *function_p
    );

    NH_API_RESULT _nh_end(
        const char *file_p, const char *function_p
    );
    
    NH_API_RESULT _nh_diagnosticEnd(
        const char *file_p, const char *function_p, const char *result_p, int line, bool success
    );

    void nh_core_getUniqueLogId(
        char *logId_p
    );

/** @} */

#endif 
