// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"

#include "../Window/Listener.h"
#include "../../nh-core/Common/Platform.h"

#if defined(NH_PLATFORM_UNIX)
    #include "../Platforms/X11/Init.h"
#elif defined(NH_PLATFORM_MACOS)
    #include "../Platforms/Cocoa/Init.h"
#elif defined(NH_PLATFORM_IOS)
    #include "../Platforms/IOS/Init.h"
#endif
#include "../../nh-core/Util/LinkedList.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_wsi_initialize()
{
    NH_CORE_CHECK(nh_wsi_normalizeListener())
#if defined(NH_PLATFORM_UNIX)
        NH_CORE_CHECK(nh_wsi_initializeX11())
#elif defined(NH_PLATFORM_MACOS)
        NH_CORE_CHECK(nh_wsi_initializeCocoa())
#elif defined(NH_PLATFORM_IOS)
        NH_CORE_CHECK(nh_wsi_initializeIOS())
#endif
}
