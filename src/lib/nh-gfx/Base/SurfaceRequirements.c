// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "SurfaceRequirements.h"
#include "Viewport.h"

#include "../Vulkan/Vulkan.h"
#include "../Vulkan/Render.h"
#include "../OpenGL/Render.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Thread.h"

#include "../../nh-wsi/Window/Window.h"

#include <string.h>
#include <limits.h>

// FUNCTIONS =======================================================================================

static nh_gfx_SurfaceRequirements Requirements;

/**
 * API function.
 */
nh_gfx_SurfaceRequirements *nh_gfx_getSurfaceRequirements()
{
    return &Requirements;
}

NH_API_RESULT nh_gfx_createSurfaceRequirements()
{
    NH_CORE_CHECK(nh_opengl_createSurfaceRequirements(&Requirements.OpenGL))
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_freeSurfaceRequirements()
{
    NH_CORE_CHECK(nh_opengl_freeSurfaceRequirements(&Requirements.OpenGL))
    return NH_API_SUCCESS;
}

