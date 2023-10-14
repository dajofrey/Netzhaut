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

#include "../../nhcore/System/Thread.h"

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

    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glViewport",
        nh_opengl_glint(NULL, Viewport_p->Settings.Position.x),
        nh_opengl_glint(NULL, Viewport_p->Settings.Position.y),
        nh_opengl_glsizei(NULL, Viewport_p->Settings.Size.width),
        nh_opengl_glsizei(NULL, Viewport_p->Settings.Size.height)
    );
 
    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glClearColor",
        nh_opengl_glfloat(NULL, Viewport_p->Settings.ClearColor.r),
        nh_opengl_glfloat(NULL, Viewport_p->Settings.ClearColor.g),
        nh_opengl_glfloat(NULL, Viewport_p->Settings.ClearColor.b),
        nh_opengl_glfloat(NULL, Viewport_p->Settings.ClearColor.a)
    );

    nh_opengl_addCommand(
        Viewport_p->OpenGL.CommandBuffer_p,
        "glClear",
        nh_opengl_glint(NULL, GL_COLOR_BUFFER_BIT)
    );

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

NH_GFX_RESULT nh_opengl_endRecording(
    nh_gfx_Viewport *Viewport_p)
{
NH_GFX_BEGIN()

    // TODO Needs something to happen here?

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

