#ifndef NH_API_NH_GFX_H
#define NH_API_NH_GFX_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-wsi.h"

// ENUMS ===========================================================================================

    typedef enum NH_API_GRAPHICS_BACKEND_E {
        NH_API_GRAPHICS_BACKEND_VULKAN,
        NH_API_GRAPHICS_BACKEND_OPENGL,
    } NH_API_GRAPHICS_BACKEND_E;

// TYPEDEFS ========================================================================================

    typedef struct nh_api_Surface nh_api_Surface;
    typedef struct nh_api_Viewport nh_api_Viewport;

// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    nh_api_SurfaceRequirements *nh_api_getSurfaceRequirements(
    );

    /**
     * Todo.
     */
    nh_api_Surface *nh_api_createSurface(
        nh_api_Window *Window_p, NH_API_GRAPHICS_BACKEND_E api
    );

    /**
     * Todo.
     */
    nh_api_Viewport *nh_api_createViewport(
        nh_api_Surface *Surface_p, nh_api_PixelPosition Position, nh_api_PixelSize Size
    );

    /**
     * Todo.
     */
    NH_API_RESULT nh_api_configureViewport(
        nh_api_Viewport *Viewport_p, nh_api_PixelPosition Position, nh_api_PixelSize Size
    );

#endif // NH_API_NH_GFX_H
