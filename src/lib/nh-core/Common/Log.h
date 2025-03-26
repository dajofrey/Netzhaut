#ifndef NH_CORE_COMMON_LOG_H
#define NH_CORE_COMMON_LOG_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Result.h"
#include "../System/Thread.h"
#include "../Util/String.h"

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_core_logThread(
    nh_core_ThreadPool *ThreadPool_p, nh_core_Thread *Thread_p
);

NH_API_RESULT nh_core_logModules(
);

void nh_core_logSystemInfo(
    char *info_p, int line
);

#endif // NH_CORE_COMMON_LOG_H
