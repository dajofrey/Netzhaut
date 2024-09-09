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
#include "../Common/Macros.h"

#include "../../nh-core/System/Thread.h"

#include <string.h>

// RESOURCES =======================================================================================

NH_GFX_RESULT nh_opengl_createViewport(
    nh_gfx_Viewport *Viewport_p)
{
NH_GFX_BEGIN()

    Viewport_p->OpenGL.CommandBuffers_p[0] = nh_opengl_initCommandBuffer();
    Viewport_p->OpenGL.CommandBuffers_p[1] = nh_opengl_initCommandBuffer();
    Viewport_p->OpenGL.CommandBuffer_p = NULL;

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

void nh_opengl_destroyViewport(
    nh_gfx_Viewport *Viewport_p)
{
NH_GFX_BEGIN()

NH_GFX_SILENT_END()
}

// RECORD ==========================================================================================

NH_GFX_RESULT nh_opengl_beginRecording(
    nh_gfx_Viewport *Viewport_p)
{
NH_GFX_BEGIN()

    *(Viewport_p->OpenGL.CommandBuffer_p) = nh_opengl_initCommandBuffer();

    // Set viewport.
    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glViewport",
        nh_opengl_glint(NULL, Viewport_p->Settings.Position.x+Viewport_p->Settings.borderWidth),
        nh_opengl_glint(NULL, Viewport_p->Settings.Position.y+Viewport_p->Settings.borderWidth),
        nh_opengl_glsizei(NULL, Viewport_p->Settings.Size.width-(Viewport_p->Settings.borderWidth*2)),
        nh_opengl_glsizei(NULL, Viewport_p->Settings.Size.height-(Viewport_p->Settings.borderWidth*2)));
 
    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glColorMask",
        nh_opengl_glboolean(NULL, GL_TRUE),
        nh_opengl_glboolean(NULL, GL_TRUE),
        nh_opengl_glboolean(NULL, GL_TRUE),
        nh_opengl_glboolean(NULL, GL_TRUE));
 
    // Clear entire viewport.
    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glEnable", 
        nh_opengl_glenum(NULL, GL_SCISSOR_TEST));
 
    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glScissor",
        nh_opengl_glint(NULL, Viewport_p->Settings.Position.x),
        nh_opengl_glint(NULL, Viewport_p->Settings.Position.y),
        nh_opengl_glsizei(NULL, Viewport_p->Settings.Size.width),
        nh_opengl_glsizei(NULL, Viewport_p->Settings.Size.height));
 
    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glClearColor",
        nh_opengl_glfloat(NULL, Viewport_p->Settings.BorderColor.r),
        nh_opengl_glfloat(NULL, Viewport_p->Settings.BorderColor.g),
        nh_opengl_glfloat(NULL, Viewport_p->Settings.BorderColor.b),
        nh_opengl_glfloat(NULL, Viewport_p->Settings.BorderColor.a));

    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glClear",
        nh_opengl_glint(NULL, GL_COLOR_BUFFER_BIT));

    // Clear viewport, except borders.
    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glScissor",
        nh_opengl_glint(NULL, Viewport_p->Settings.Position.x+Viewport_p->Settings.borderWidth),
        nh_opengl_glint(NULL, Viewport_p->Settings.Position.y+Viewport_p->Settings.borderWidth),
        nh_opengl_glsizei(NULL, Viewport_p->Settings.Size.width-Viewport_p->Settings.borderWidth*2),
        nh_opengl_glsizei(NULL, Viewport_p->Settings.Size.height-Viewport_p->Settings.borderWidth*2));
    
    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glClearColor",
        nh_opengl_glfloat(NULL, Viewport_p->Settings.ClearColor.r),
        nh_opengl_glfloat(NULL, Viewport_p->Settings.ClearColor.g),
        nh_opengl_glfloat(NULL, Viewport_p->Settings.ClearColor.b),
        nh_opengl_glfloat(NULL, Viewport_p->Settings.ClearColor.a));

    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glClear", 
        nh_opengl_glenum(NULL, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

NH_GFX_RESULT nh_opengl_endRecording(
    nh_gfx_Viewport *Viewport_p)
{
NH_GFX_BEGIN()

    nh_opengl_addCommand(Viewport_p->OpenGL.CommandBuffer_p, "glDisable", nh_opengl_glenum(NULL, GL_SCISSOR_TEST));

    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glColorMask", 
        nh_opengl_glboolean(NULL, GL_FALSE),
        nh_opengl_glboolean(NULL, GL_FALSE),
        nh_opengl_glboolean(NULL, GL_FALSE),
        nh_opengl_glboolean(NULL, GL_TRUE));

    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p, 
        "glClearColor",
        nh_opengl_glfloat(NULL, 0.0f),
        nh_opengl_glfloat(NULL, 0.0f),
        nh_opengl_glfloat(NULL, 0.0f),
        nh_opengl_glfloat(NULL, 1.0f)); // Transparency can be set here.

    nh_opengl_addCommand(Viewport_p->OpenGL.CommandBuffer_p, "glClear", nh_opengl_glenum(NULL, GL_COLOR_BUFFER_BIT));

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

