// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "IndexMap.h"
#include "Macros.h"

#include "../Vulkan/Vulkan.h"
#include "../OpenGL/OpenGL.h"

#include "../Fonts/FontManager.h"
#include "../Base/Surface.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_gfx_initialize()
{
    NH_CORE_CHECK(nh_gfx_createIndexMap())
    NH_CORE_CHECK(nh_gfx_initializeFontManager())
    NH_CORE_CHECK(nh_gfx_createSurfaceRequirements())

    nh_gfx_initVulkan();
    nh_gfx_initOpenGL();
    nh_gfx_initMetal();
}

