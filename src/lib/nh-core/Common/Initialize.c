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
#include "Macros.h"

#include "../System/System.h"
#include "../System/Memory.h"
#include "../System/Process.h"
#include "../System/Thread.h"
#include "../System/Logger.h"
#include "../Loader/Loader.h"
#include "../Config/Config.h"
#include "../Config/Updater.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// INIT ============================================================================================

static NH_API_RESULT nh_core_startWorkloads()
{
NH_CORE_BEGIN()

    nh_core_activateWorkload(nh_core_initSystemUpdater, nh_core_runSystemUpdater, NULL, NULL, NULL, false);
    NH_CORE_CHECK(nh_core_startConfigUpdater())

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

nh_Loader *nh_core_initialize(
    char *path_p, char *config_p, int length)
{
    nh_core_initGlobalConfig();

    if (config_p != NULL && length > 0) {
        nh_core_appendConfig(config_p, length, true);
    }

    nh_core_initLogger();
    nh_core_initMemory();
    nh_core_initProcessPool();
    nh_core_createIndexMap();
    nh_core_initSystem();

    nh_core_startWorkloads();

    return nh_core_initLoader(false, false);
}

