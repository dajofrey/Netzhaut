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
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Thread.h"

#include "../../nh-wsi/Window/Window.h"

#include <string.h>
#include <limits.h>

// REQUIREMENTS ====================================================================================

static nh_gfx_SurfaceRequirements Requirements;

nh_gfx_SurfaceRequirements *nh_gfx_getSurfaceRequirements()
{
NH_GFX_BEGIN()
NH_GFX_END(&Requirements)
}

NH_API_RESULT nh_gfx_createSurfaceRequirements()
{
NH_GFX_BEGIN()

    NH_GFX_CHECK(nh_opengl_createSurfaceRequirements(&Requirements.OpenGL))

NH_GFX_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_gfx_freeSurfaceRequirements()
{
NH_GFX_BEGIN()

    NH_GFX_CHECK(nh_opengl_freeSurfaceRequirements(&Requirements.OpenGL))

NH_GFX_DIAGNOSTIC_END(NH_API_SUCCESS)
}

