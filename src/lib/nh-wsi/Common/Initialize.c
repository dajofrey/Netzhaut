// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"

#include "../Window/Listener.h"
#if defined(__unix__)
    #include "../Platforms/X11/Init.h"
#elif defined(__APPLE__)
#endif
#include "../../nh-core/Util/LinkedList.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_wsi_initialize()
{
    NH_CORE_CHECK(nh_wsi_normalizeListener())
    #if defined(__unix__)
        NH_CORE_CHECK(nh_x11_initialize())
    #elif defined(__APPLE__)
        // do nothing for now
    #endif
}
