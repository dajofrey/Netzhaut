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

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_logThread(
        nh_ThreadPool *ThreadPool_p, nh_Thread *Thread_p
    );

    NH_CORE_RESULT nh_core_logModules(
    );

    void nh_core_logSystemInfo(
        NH_BYTE *info_p, int line
    );

    NH_CORE_RESULT _nh_core_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_CORE_RESULT _nh_core_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_CORE_RESULT _nh_core_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_CORE_RESULT result, int line
    );

/** @} */

#endif 
