#ifndef NH_GFX_OPENGL_SURFACE_H
#define NH_GFX_OPENGL_SURFACE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "CommandBuffer.h"
#include "../Common/Includes.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-wsi/Window/Window.h"

typedef struct nh_gfx_OpenGLSurface {
    GLXContext Context_p;
    nh_gfx_OpenGLCommandBuffer *CommandBuffers_p;
    unsigned int bufferCount;         
    uint32_t currentBuffer;               
} nh_gfx_OpenGLSurface;

nh_gfx_OpenGLSurface nh_gfx_initOpenGLSurface(
);

NH_API_RESULT nh_gfx_createOpenGLSurface(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p
);

NH_API_RESULT nh_gfx_destroyOpenGLSurface(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p
);

#endif
