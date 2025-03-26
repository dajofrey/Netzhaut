// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"

#include "../Logger.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_monitor_initialize()
{
    nh_monitor_initLogger(NULL);

    // This may block if nh-monitor.block is set.
    nh_monitor_startLoggerWorkload();

    return NH_API_SUCCESS;
}
