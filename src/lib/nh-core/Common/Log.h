#ifndef NH_CORE_LOG_H
#define NH_CORE_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"
#include "../System/Thread.h"
#include "../Util/String.h"

#endif

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_API_RESULT nh_core_logThread(
        nh_ThreadPool *ThreadPool_p, nh_Thread *Thread_p
    );

    NH_API_RESULT nh_core_logModules(
    );

    void nh_core_logSystemInfo(
        char *info_p, int line
    );

    NH_API_RESULT _nh_core_logBegin(
        const char *file_p, const char *function_p
    );

    NH_API_RESULT _nh_core_logEnd(
        const char *file_p, const char *function_p
    );

    NH_API_RESULT _nh_core_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_API_RESULT result, int line
    );

/** @} */

#endif 
