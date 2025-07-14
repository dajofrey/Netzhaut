#ifndef NH_GFX_SURFACE_REQUIREMENTS_H
#define NH_GFX_SURFACE_REQUIREMENTS_H

// LICENSE NOTICE ==================================================================================
 
/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"
#include "../../nh-core/Util/Array.h"

// STRUCTS =========================================================================================

typedef struct nh_gfx_OpenGLSurfaceRequirements {
    nh_core_Array Ids; // Contains visualid's that are glx framebuffer compatible. This is used for WSI window creation.
} nh_gfx_OpenGLSurfaceRequirements;

typedef struct nh_gfx_SurfaceRequirements {
    nh_gfx_OpenGLSurfaceRequirements OpenGL;
} nh_gfx_SurfaceRequirements;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_createSurfaceRequirements(
);

NH_API_RESULT nh_gfx_freeSurfaceRequirements(
);

#endif // NH_GFX_SURFACE_REQUIREMENTS_H 
