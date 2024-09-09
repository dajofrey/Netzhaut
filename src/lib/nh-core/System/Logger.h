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
        NH_BYTE *name_p;
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

    NH_CORE_RESULT nh_core_initLogger(
    );

    NH_CORE_RESULT nh_core_freeLogger(
    );

    NH_CORE_RESULT nh_core_addLogCallback(
        nh_core_logCallback_f logCallback_f
    );
    
    NH_CORE_RESULT _nh_core_sendLogMessage(
        NH_BYTE *node_p, NH_BYTE *options_p, NH_BYTE *message_p
    );
    
    NH_CORE_RESULT nh_core_sendLogMessage(
        NH_BYTE *node_p, NH_BYTE *options_p, NH_BYTE *message_p
    );

    NH_CORE_RESULT _nh_begin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_CORE_RESULT _nh_end(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_CORE_RESULT _nh_diagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, const NH_BYTE *result_p, int line, NH_BOOL success
    );

    void nh_core_getUniqueLogId(
        NH_BYTE *logId_p
    );

/** @} */

#endif 
