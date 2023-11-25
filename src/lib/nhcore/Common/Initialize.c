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

static NH_CORE_RESULT nh_core_startWorkloads()
{
NH_CORE_BEGIN()

    nh_core_activateWorkload(nh_core_initSystemUpdater, nh_core_runSystemUpdater, NULL, NULL, NULL, NH_FALSE);
    NH_CORE_CHECK(nh_core_startConfigUpdater())

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_core_initialize(
    NH_LOADER_SCOPE_E scope, NH_BYTE *path_p, NH_BYTE *config_p, int length)
{
    NH_CORE_CHECK(nh_core_initGlobalConfig())

    if (config_p != NULL && length > 0) {
        NH_CORE_CHECK(nh_core_appendConfig(config_p, length, NH_TRUE))
    }

    NH_CORE_CHECK(nh_core_initLogger())
    NH_CORE_CHECK(nh_core_initMemory())
    NH_CORE_CHECK(nh_core_initProcessPool())
    NH_CORE_CHECK(nh_core_createIndexMap())
    NH_CORE_CHECK(nh_core_initSystem())

    NH_CORE_CHECK(nh_core_startWorkloads())

    return NH_CORE_SUCCESS;
}

