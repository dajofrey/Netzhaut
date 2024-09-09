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

#include "../Vulkan/Vulkan.h"
#include "../OpenGL/OpenGL.h"

#include "../Fonts/FontManager.h"
#include "../Base/Surface.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_GFX_RESULT nh_gfx_initialize()
{
    _nh_gfx_initConfig();

    NH_GFX_CHECK(nh_gfx_createIndexMap())
    NH_GFX_CHECK(nh_gfx_initializeFontManager())
    NH_GFX_CHECK(nh_gfx_createSurfaceRequirements())

    nh_vk_initVulkan();
    nh_opengl_initOpenGL();
}

