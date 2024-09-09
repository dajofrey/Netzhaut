#ifndef NH_RENDERER_H
#define NH_RENDERER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-css.h"

#endif

/** @defgroup api_nh-renderer nh-renderer
 *  @brief Create and configure renderers based on CSS layout engines.
 */

// TYPEDEFS ========================================================================================

    typedef struct nh_renderer_Renderer nh_renderer_Renderer; 

// ENUMS ===========================================================================================

    typedef enum NH_RENDERER_RESULT { 
        NH_RENDERER_SUCCESS,
        NH_RENDERER_ERROR_NULL_POINTER, 
        NH_RENDERER_ERROR_BAD_STATE, 
        NH_RENDERER_ERROR_MEMORY_ALLOCATION, 
    } NH_RENDERER_RESULT;  

// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    nh_renderer_Renderer *nh_api_createRenderer(
        nh_css_LayoutEngine *LayoutEngine_p
    );

    /**
     * Todo.
     */
    NH_RENDERER_RESULT nh_api_addViewport(
        nh_renderer_Renderer *Renderer_p, nh_gfx_Viewport *Viewport_p
    );

/** @} */

#endif // NH_RENDERER_H
