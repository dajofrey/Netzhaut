// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "Macros.h"

#include "../Window/Listener.h"
#include "../Platforms/X11/Init.h"

#include "../../nhcore/Util/LinkedList.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_WSI_RESULT_E nh_wsi_initialize()
{
    NH_WSI_CHECK(nh_wsi_normalizeListener())
    NH_WSI_CHECK(nh_x11_initialize())
}
