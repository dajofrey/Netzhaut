// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Viewport.h"
#include "CommandBuffer.h"

#include "../Base/Viewport.h"
#include "../../nh-core/System/Thread.h"

#include <string.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_createOpenGLViewport(
    nh_gfx_Viewport *Viewport_p)
{
    Viewport_p->OpenGL.CommandBuffers_p[0] = nh_gfx_initOpenGLCommandBuffer();
    Viewport_p->OpenGL.CommandBuffers_p[1] = nh_gfx_initOpenGLCommandBuffer();
    Viewport_p->OpenGL.CommandBuffer_p = NULL;

    return NH_API_SUCCESS;
}

void nh_gfx_destroyOpenGLViewport(
    nh_gfx_Viewport *Viewport_p)
{
    return;
}

NH_API_RESULT nh_gfx_beginOpenGLRecording(
    nh_gfx_Viewport *Viewport_p)
{
    *(Viewport_p->OpenGL.CommandBuffer_p) = nh_gfx_initOpenGLCommandBuffer();

    // Set viewport.
    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glViewport",
        nh_gfx_glint(NULL, Viewport_p->Settings.Position.x+Viewport_p->Settings.borderWidth),
        nh_gfx_glint(NULL, Viewport_p->Settings.Position.y+Viewport_p->Settings.borderWidth),
        nh_gfx_glsizei(NULL, Viewport_p->Settings.Size.width-(Viewport_p->Settings.borderWidth*2)),
        nh_gfx_glsizei(NULL, Viewport_p->Settings.Size.height-(Viewport_p->Settings.borderWidth*2)));
 
    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glColorMask",
        nh_gfx_glboolean(NULL, GL_TRUE),
        nh_gfx_glboolean(NULL, GL_TRUE),
        nh_gfx_glboolean(NULL, GL_TRUE),
        nh_gfx_glboolean(NULL, GL_TRUE));
 
    // Clear entire viewport.
    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glEnable", 
        nh_gfx_glenum(NULL, GL_SCISSOR_TEST));
 
    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glScissor",
        nh_gfx_glint(NULL, Viewport_p->Settings.Position.x),
        nh_gfx_glint(NULL, Viewport_p->Settings.Position.y),
        nh_gfx_glsizei(NULL, Viewport_p->Settings.Size.width),
        nh_gfx_glsizei(NULL, Viewport_p->Settings.Size.height));
 
    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glClearColor",
        nh_gfx_glfloat(NULL, Viewport_p->Settings.BorderColor.r),
        nh_gfx_glfloat(NULL, Viewport_p->Settings.BorderColor.g),
        nh_gfx_glfloat(NULL, Viewport_p->Settings.BorderColor.b),
        nh_gfx_glfloat(NULL, Viewport_p->Settings.BorderColor.a));

    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glClear",
        nh_gfx_glint(NULL, GL_COLOR_BUFFER_BIT));

    // Clear viewport, except borders.
    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glScissor",
        nh_gfx_glint(NULL, Viewport_p->Settings.Position.x+Viewport_p->Settings.borderWidth),
        nh_gfx_glint(NULL, Viewport_p->Settings.Position.y+Viewport_p->Settings.borderWidth),
        nh_gfx_glsizei(NULL, Viewport_p->Settings.Size.width-Viewport_p->Settings.borderWidth*2),
        nh_gfx_glsizei(NULL, Viewport_p->Settings.Size.height-Viewport_p->Settings.borderWidth*2));
    
    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glClearColor",
        nh_gfx_glfloat(NULL, Viewport_p->Settings.ClearColor.r),
        nh_gfx_glfloat(NULL, Viewport_p->Settings.ClearColor.g),
        nh_gfx_glfloat(NULL, Viewport_p->Settings.ClearColor.b),
        nh_gfx_glfloat(NULL, Viewport_p->Settings.ClearColor.a));

    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glClear", 
        nh_gfx_glenum(NULL, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_endOpenGLRecording(
    nh_gfx_Viewport *Viewport_p)
{
    nh_gfx_addOpenGLCommand(Viewport_p->OpenGL.CommandBuffer_p, "glDisable", nh_gfx_glenum(NULL, GL_SCISSOR_TEST));

    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glColorMask", 
        nh_gfx_glboolean(NULL, GL_FALSE),
        nh_gfx_glboolean(NULL, GL_FALSE),
        nh_gfx_glboolean(NULL, GL_FALSE),
        nh_gfx_glboolean(NULL, GL_TRUE));

    nh_gfx_addOpenGLCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glClearColor",
        nh_gfx_glfloat(NULL, 0.0f),
        nh_gfx_glfloat(NULL, 0.0f),
        nh_gfx_glfloat(NULL, 0.0f),
        nh_gfx_glfloat(NULL, 1.0f)); // Transparency can be set here.

    nh_gfx_addOpenGLCommand(Viewport_p->OpenGL.CommandBuffer_p, "glClear", nh_gfx_glenum(NULL, GL_COLOR_BUFFER_BIT));

    return NH_API_SUCCESS;
}
