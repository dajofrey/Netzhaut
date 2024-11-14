// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "IndexMap.h"
#include "Config.h"

#include "../System/System.h"
#include "../System/Memory.h"
#include "../System/Process.h"
#include "../System/Thread.h"
#include "../Logger/Logger.h"
#include "../Loader/Loader.h"
#include "../Config/Config.h"
#include "../Config/Updater.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_core_Loader *nh_core_initialize(
    char *path_p, char *config_p, int length)
{
    nh_core_initGlobalConfig();

    if (config_p != NULL && length > 0) {
        nh_core_appendConfig(config_p, length, true);
    }

    nh_core_initMemory();
    nh_core_initProcessPool();
    nh_core_initLogger(NULL);

    // Next call may block if nh-core.logger.block:1
    nh_core_startLoggerWorkload();

    nh_core_createIndexMap();
    nh_core_initSystem();
    nh_core_startSystemWorkload();
    nh_core_startConfigWorkload();

    return nh_core_initLoader(false, false);
}
