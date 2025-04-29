#ifndef NH_RENDERER_RENDERER_H
#define NH_RENDERER_RENDERER_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

#include "../../nh-gfx/Base/Viewport.h"
#include "../../nh-css/Main/LayoutEngine.h"

// STRUCTS =========================================================================================

typedef struct nh_renderer_RenderTarget {
    nh_gfx_Viewport *Viewport_p;
    bool render;
} nh_renderer_RenderTarget;

typedef struct nh_renderer_Renderer {
    nh_css_LayoutEngine *LayoutEngine_p;
    nh_core_Array RenderTargets;
} nh_renderer_Renderer;

// FUNCTIONS =======================================================================================

nh_renderer_Renderer *nh_renderer_createRenderer(
    nh_css_LayoutEngine *LayoutEngine_p
);

NH_API_RESULT nh_renderer_addViewport(
    nh_renderer_Renderer *Renderer_p, nh_gfx_Viewport *Viewport_p
);

#endif // NH_RENDERER_RENDERER_H
