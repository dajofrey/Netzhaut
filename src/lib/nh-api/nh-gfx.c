// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-gfx.h"
#include "nh-core.h"

#include "../nh-core/Loader/Loader.h"
#include "../nh-gfx/Base/Viewport.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_api_SurfaceRequirements *nh_api_getSurfaceRequirements()
{
    typedef nh_api_SurfaceRequirements *(*nh_gfx_getSurfaceRequirements_f)(); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_gfx_getSurfaceRequirements_f getSurfaceRequirements_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_GFX, 0, "nh_gfx_getSurfaceRequirements");
    return getSurfaceRequirements_f ? getSurfaceRequirements_f() : NULL;
}

nh_api_Surface *nh_api_createSurface(
    nh_api_Window *Window_p)
{
    typedef nh_api_Surface *(*nh_gfx_createSurface_f)(nh_api_Window *Window_p);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_gfx_createSurface_f createSurface_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_GFX, 0, "nh_gfx_createSurface");
    return createSurface_f ? createSurface_f(Window_p) : NULL;
}

nh_api_Viewport *nh_api_createViewport(
    nh_api_Surface *Surface_p, nh_api_PixelPosition *Position_p, nh_api_PixelSize *Size_p)
{
    typedef nh_api_Viewport *(*nh_gfx_createViewport_f)(nh_api_Surface *Surface_p, nh_api_PixelPosition *Position_p, nh_api_PixelSize *Size_p);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_gfx_createViewport_f createViewport_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_GFX, 0, "nh_gfx_createViewport");
    return createViewport_f ? createViewport_f(Surface_p, Position_p, Size_p) : NULL;
}

NH_API_RESULT nh_api_configureViewport(
    nh_api_Viewport *Viewport_p, nh_api_PixelPosition Position, nh_api_PixelSize Size)
{
    typedef NH_API_RESULT (*nh_gfx_configureViewport_f)(nh_api_Viewport *Viewport_p, nh_api_PixelPosition Position, nh_api_PixelSize Size);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_gfx_configureViewport_f configureViewport_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_GFX, 0, "nh_gfx_configureViewport");
    return configureViewport_f ? configureViewport_f(Viewport_p, Position, Size) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_scroll(
    nh_api_Viewport *Viewport_p, float amount)
{
    typedef NH_API_RESULT (*nh_gfx_scroll_f)(nh_api_Viewport *Viewport_p, float amount);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_gfx_scroll_f scroll_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_GFX, 0, "nh_gfx_scroll");
    return scroll_f ? scroll_f(Viewport_p, amount) : NH_API_ERROR_BAD_STATE;
}
