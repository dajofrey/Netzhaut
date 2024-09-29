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

// CREATE ==========================================================================================

nh_api_Renderer *nh_api_createRenderer(
    nh_api_LayoutEngine *LayoutEngine_p)
{
    nh_renderer_createRenderer_f createRenderer_f = !NH_LOADER_P || !LayoutEngine_p ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_RENDERER, 0, "nh_renderer_createRenderer");
    return createRenderer_f ? createRenderer_f(LayoutEngine_p) : NULL;
}

NH_API_RESULT nh_api_addViewport(
    nh_api_Renderer *Renderer_p, nh_api_Viewport *Viewport_p)
{
    nh_renderer_addViewport_f addViewport_f = !NH_LOADER_P || !Renderer_p || !Viewport_p ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_RENDERER, 0, "nh_renderer_addViewport");
    return addViewport_f ? addViewport_f(Renderer_p, Viewport_p) : NH_API_ERROR_BAD_STATE;
}

