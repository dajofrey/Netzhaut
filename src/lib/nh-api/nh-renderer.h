#ifndef NH_API_NH_RENDERER_H
#define NH_API_NH_RENDERER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-css.h"

// TYPEDEFS ========================================================================================

    typedef struct nh_api_Renderer nh_api_Renderer; 

// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    nh_api_Renderer *nh_api_createRenderer(
        nh_api_LayoutEngine *LayoutEngine_p
    );

    /**
     * Todo.
     */
    NH_API_RESULT nh_api_addViewport(
        nh_api_Renderer *Renderer_p, nh_api_Viewport *Viewport_p
    );

#endif // NH_API_NH_RENDERER_H
