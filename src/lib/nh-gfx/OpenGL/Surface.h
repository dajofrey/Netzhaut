#ifndef NH_GFX_OPENGL_SURFACE_H
#define NH_GFX_OPENGL_SURFACE_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "CommandBuffer.h"
#include "../Common/Includes.h"

#include "../../nh-core/Util/Array.h"

// STRUCTS =========================================================================================

typedef struct nh_gfx_OpenGLSurface {
#ifdef __unix__
    GLXContext Context_p;
#elif __APPLE__
    void* Context_p; // CGLContext
    void* OpenGLContext_p;
    void* PixelFormat_p;
#endif
    nh_gfx_OpenGLCommandBuffer *CommandBuffers_p;
    unsigned int bufferCount;         
    uint32_t currentBuffer;               
} nh_gfx_OpenGLSurface;

// FUNCTIONS =======================================================================================

nh_gfx_OpenGLSurface nh_gfx_initOpenGLSurface(
);

NH_API_RESULT nh_gfx_createOpenGLSurface(
    nh_gfx_OpenGLSurface *Surface_p, nh_api_Window *Window_p
);

NH_API_RESULT nh_gfx_destroyOpenGLSurface(
    nh_gfx_OpenGLSurface *Surface_p, nh_api_Window *Window_p
);

NH_API_RESULT nh_gfx_updateOpenGLSurface(
    nh_gfx_OpenGLSurface *Surface_p, nh_api_Window *Window_p
);

#endif
