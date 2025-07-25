#ifndef NH_API_NH_GFX_H
#define NH_API_NH_GFX_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-wsi.h"

// STRUCTS =========================================================================================

/**
 * Opaque handle for a surface.
 */
typedef struct nh_api_Surface nh_api_Surface;

/**
 * Opaque handle for a viewport.
 */
typedef struct nh_api_Viewport nh_api_Viewport;

// FUNCTIONS =======================================================================================

/**
 * Todo.
 */
nh_api_SurfaceRequirements *nh_api_getSurfaceRequirements(
);

/**
 * create a surface.
 */
nh_api_Surface *nh_api_createSurface(
    nh_api_Window *Window_p
);

/**
 * Todo.
 */
nh_api_Viewport *nh_api_createViewport(
    nh_api_Surface *Surface_p, nh_api_PixelPosition *Position_p, nh_api_PixelSize *Size_p
);

/**
 * Todo.
 */
NH_API_RESULT nh_api_configureViewport(
    nh_api_Viewport *Viewport_p, nh_api_PixelPosition Position, nh_api_PixelSize Size
);

/**
 * Todo.
 */
NH_API_RESULT nh_api_scroll(
    nh_api_Viewport *Viewport_p, float amount
);

#endif // NH_API_NH_GFX_H
