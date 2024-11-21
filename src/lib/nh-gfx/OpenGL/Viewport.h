#ifndef NH_OPENGL_VIEWPORT_H
#define NH_OPENGL_VIEWPORT_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "CommandBuffer.h"
#include "../Common/Includes.h"

typedef struct nh_gfx_OpenGLViewport {
    nh_gfx_OpenGLCommandBuffer CommandBuffers_p[2];
    nh_gfx_OpenGLCommandBuffer *CommandBuffer_p;
} nh_gfx_OpenGLViewport;

typedef struct nh_gfx_Viewport nh_gfx_Viewport;

NH_API_RESULT nh_gfx_createOpenGLViewport(
    nh_gfx_Viewport *Viewport_p
);

void nh_gfx_destroyOpenGLViewport(
    nh_gfx_Viewport *Viewport_p
);

NH_API_RESULT nh_gfx_beginOpenGLRecording(
    nh_gfx_Viewport *Viewport_p
);

NH_API_RESULT nh_gfx_endOpenGLRecording(
    nh_gfx_Viewport *Viewport_p
);

#endif
