// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-renderer.h"

#include "../nh-core/Loader/Loader.h"
#include "../nh-renderer/Main/Renderer.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_api_Renderer *nh_api_createRenderer(
    nh_api_LayoutEngine *LayoutEngine_p)
{
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_renderer_createRenderer_f createRenderer_f = !Loader_p || !LayoutEngine_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_RENDERER, 0, "nh_renderer_createRenderer");
    return createRenderer_f ? createRenderer_f(LayoutEngine_p) : NULL;
}

NH_API_RESULT nh_api_addViewport(
    nh_api_Renderer *Renderer_p, nh_api_Viewport *Viewport_p)
{
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_renderer_addViewport_f addViewport_f = !Loader_p || !Renderer_p || !Viewport_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_RENDERER, 0, "nh_renderer_addViewport");
    return addViewport_f ? addViewport_f(Renderer_p, Viewport_p) : NH_API_ERROR_BAD_STATE;
}

