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

#if defined(__unix__)
    #include "../Vulkan/Vulkan.h"
#endif

#include "../OpenGL/OpenGL.h"

#if defined(NH_PLATFORM_MACOS) || defined(NH_PLATFORM_IOS)
    #include "../Metal/Metal.h"
#endif

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

#if defined(__unix__)
    nh_gfx_initVulkan();
#endif
    nh_gfx_initOpenGL();
#if defined(NH_PLATFORM_MACOS) || defined(NH_PLATFORM_IOS)
    nh_gfx_initMetal();
#endif
}
