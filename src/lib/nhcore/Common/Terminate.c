// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminate.h"
#include "IndexMap.h"
#include "Config.h"

#include "../System/Memory.h"
#include "../System/Process.h"
#include "../System/Thread.h"
#include "../System/Logger.h"
#include "../Loader/Loader.h"
#include "../Config/Config.h"

#include "IndexMap.h"
#include "Macros.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TERMINATE =======================================================================================

NH_CORE_RESULT nh_core_terminate(
    nh_Loader *Loader_p)
{
NH_CORE_BEGIN()

    // This also deactivates all workloads.
    nh_core_freeProcessPool();

    // Next, we "decouple" all loaded libs aka modules.
    nh_core_freeLoader();    

    // Now, everything should have been freed which was used by other modules.
    // Next, we free all core data.
    nh_core_freeLogger();
    nh_core_freeIndexMap();

    nh_core_freeGlobalConfig();

    // Great. Now what remains to be done is for nhapi/nhcore.c to close this module aka the nhcore library
    // and Netzhaut should be terminated.

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}
