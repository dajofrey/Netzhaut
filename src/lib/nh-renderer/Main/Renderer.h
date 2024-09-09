#ifndef NH_RENDERER_RENDERER_H
#define NH_RENDERER_RENDERER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-renderer_structs
 *  @{
 */

    typedef struct nh_renderer_RenderTarget {
        nh_gfx_Viewport *Viewport_p;
        NH_BOOL render;
    } nh_renderer_RenderTarget;

    typedef struct nh_renderer_Renderer {
        nh_css_LayoutEngine *LayoutEngine_p;
        nh_Array RenderTargets;
    } nh_renderer_Renderer;

/** @} */

/** @addtogroup lib_nh-renderer_typedefs
 *  @{
 */

    typedef nh_renderer_Renderer *(*nh_renderer_createRenderer_f)(
        nh_css_LayoutEngine *LayoutEngine_p 
    );

    typedef NH_RENDERER_RESULT (*nh_renderer_addViewport_f)(
        nh_renderer_Renderer *Renderer_p, nh_gfx_Viewport *Viewport_p
    );

/** @} */

/** @addtogroup lib_nh-renderer_functions
 *  @{
 */

    nh_renderer_Renderer *nh_renderer_createRenderer(
        nh_css_LayoutEngine *LayoutEngine_p
    );

    NH_RENDERER_RESULT nh_renderer_addViewport(
        nh_renderer_Renderer *Renderer_p, nh_gfx_Viewport *Viewport_p
    );

/** @} */

#endif
