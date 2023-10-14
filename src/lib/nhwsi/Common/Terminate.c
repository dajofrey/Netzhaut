// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminate.h"
#include "Config.h"
#include "Macros.h"
#include "IndexMap.h"

#include "../Window/Listener.h"
#include "../Platforms/X11/Init.h"

#include "../../nhcore/Util/LinkedList.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_WSI_RESULT_E nh_wsi_terminate()
{
NH_WSI_BEGIN()

    NH_WSI_CHECK(nh_x11_close())
    nh_wsi_freeClipboard();
    nh_wsi_freeIndexMap();

NH_WSI_END(NH_WSI_SUCCESS)
}
