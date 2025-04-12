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
#include "../Loader/Loader.h"
#include "../Config/Config.h"
#include "../Config/Updater.h"
#include "../Util/Debug.h"

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
    nh_core_createIndexMap();
    nh_core_initSystem();
    nh_core_startSystemWorkload();
    nh_core_startConfigWorkload();
    nh_core_Loader *Loader_p = nh_core_initLoader(false, false);
    if (Loader_p == NULL) {return NULL;}

    // Do not delete, this might block execution for monitor to catch up. 
    nh_core_debug("Init complete.");

    return Loader_p;
}
