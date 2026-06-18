// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminate.h"
#include "Config.h"

#include "../Window/Listener.h"
#include "../../nh-core/Common/Platform.h"

#if defined(NH_PLATFORM_UNIX)
    #include "../Platforms/X11/Init.h"
#endif

#include "../../nh-core/Util/LinkedList.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_wsi_terminate()
{
    #if defined(NH_PLATFORM_UNIX)
        NH_CORE_CHECK(nh_wsi_closeX11())
    #endif

    nh_wsi_freeClipboard();

    return NH_API_SUCCESS;
}
