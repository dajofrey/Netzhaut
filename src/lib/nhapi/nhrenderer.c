// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nhrenderer.h"

#include "../nhcore/Loader/Loader.h"
#include "../nhrenderer/Main/Renderer.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE ==========================================================================================

nh_renderer_Renderer *nh_renderer_createRenderer(
    nh_css_LayoutEngine *LayoutEngine_p)
{
    nh_renderer_createRenderer_f createRenderer_f = !NH_LOADER_P || !LayoutEngine_p ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_RENDERER, 0, "nh_renderer_createRenderer");
    return createRenderer_f ? createRenderer_f(LayoutEngine_p) : NULL;
}

NH_RENDERER_RESULT nh_renderer_addViewport(
    nh_renderer_Renderer *Renderer_p, nh_gfx_Viewport *Viewport_p)
{
    nh_renderer_addViewport_f addViewport_f = !NH_LOADER_P || !Renderer_p || !Viewport_p ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_RENDERER, 0, "nh_renderer_addViewport");
    return addViewport_f ? addViewport_f(Renderer_p, Viewport_p) : NH_RENDERER_ERROR_BAD_STATE;
}

