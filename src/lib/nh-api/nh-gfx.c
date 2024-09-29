// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-gfx.h"

#include "../nh-core/Loader/Loader.h"

#include "../nh-gfx/Base/Surface.h"
#include "../nh-gfx/Base/Viewport.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TYPEDEFS ========================================================================================


// CREATE ==========================================================================================

nh_api_SurfaceRequirements *nh_api_getSurfaceRequirements()
{
    nh_gfx_getSurfaceRequirements_f getSurfaceRequirements_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_GFX, 0, "nh_gfx_getSurfaceRequirements");
    return getSurfaceRequirements_f ? getSurfaceRequirements_f() : NULL;
}

nh_api_Surface *nh_api_createSurface(
    nh_api_Window *Window_p, NH_API_GRAPHICS_BACKEND_E api)
{
    nh_gfx_createSurface_f createSurface_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_GFX, 0, "nh_gfx_createSurface");
    return createSurface_f ? createSurface_f(Window_p, api) : NULL;
}

nh_api_Viewport *nh_api_createViewport(
    nh_api_Surface *Surface_p, nh_api_PixelPosition Position, nh_api_PixelSize Size)
{
    nh_gfx_createViewport_f createViewport_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_GFX, 0, "nh_gfx_createViewport");
    return createViewport_f ? createViewport_f(Surface_p, Position, Size) : NULL;
}

NH_API_RESULT nh_api_configureViewport(
    nh_api_Viewport *Viewport_p, nh_api_PixelPosition Position, nh_api_PixelSize Size)
{
    nh_gfx_configureViewport_f configureViewport_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_GFX, 0, "nh_gfx_configureViewport");
    return configureViewport_f ? configureViewport_f(Viewport_p, Position, Size) : NH_API_ERROR_BAD_STATE;
}
