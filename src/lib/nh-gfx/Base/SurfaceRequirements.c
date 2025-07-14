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
#include "../OpenGL/ContextX11.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Thread.h"

#include "../../nh-wsi/Window/Window.h"

#include <string.h>
#include <limits.h>

// FUNCTIONS =======================================================================================

static nh_gfx_SurfaceRequirements Requirements;

nh_gfx_SurfaceRequirements *nh_gfx_getSurfaceRequirements()
{
    return &Requirements;
}

NH_API_RESULT nh_gfx_createSurfaceRequirements()
{
#if defined(__unix__)
    NH_CORE_CHECK(nh_gfx_createOpenGLX11ContextRequirements(&Requirements.OpenGL))
#endif
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_freeSurfaceRequirements()
{
#if defined(__unix__)
    NH_CORE_CHECK(nh_gfx_freeOpenGLX11ContextRequirements(&Requirements.OpenGL))
#endif
    return NH_API_SUCCESS;
}
