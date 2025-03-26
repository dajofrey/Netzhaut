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
#include "../Loader/Loader.h"
#include "../Config/Config.h"

#include "IndexMap.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_core_terminate(
    nh_core_Loader *Loader_p)
{
    // This also deactivates all workloads.
    nh_core_freeProcessPool();

    // Next, we "decouple" all loaded libs aka modules.
    nh_core_freeLoader();    

    // Now, everything should have been freed which was used by other modules.
    // Next, we free all core data.
    nh_core_freeIndexMap();
    nh_core_freeGlobalConfig();

    // Great. Now what remains to be done is for nh-api/nh-core.c to close this module aka the nh-core library
    // and Netzhaut should be terminated.

    return NH_API_SUCCESS;
}
