// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminate.h"
#include "IndexMap.h"
#include "Macros.h"

#include "../Fonts/FontManager.h"
#include "../Base/Surface.h"
#include "../OpenGL/OpenGL.h"
#include "../Vulkan/Vulkan.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_GFX_RESULT nh_gfx_terminate()
{
NH_GFX_BEGIN()

    nh_gfx_freeIndexMap();
    nh_gfx_terminateFontManager();
    nh_gfx_freeSurfaceRequirements();

//    nh_vk_terminateVulkan();
    nh_opengl_terminateOpenGL();

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

