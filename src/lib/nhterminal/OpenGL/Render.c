// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Render.h"

#include "../Common/Macros.h"
#include "../Common/Config.h"

#include "../../nhgfx/Base/Viewport.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// BACKGROUND ======================================================================================

static NH_TERMINAL_RESULT nh_terminal_drawOpenGLBackground(
    nh_terminal_GraphicsState *State_p, nh_terminal_GraphicsData *Data_p, nh_terminal_Grid *Grid_p)
{
NH_TERMINAL_BEGIN()

    nh_opengl_CommandBuffer *CommandBuffer_p = State_p->Viewport_p->OpenGL.CommandBuffer_p;

    int offset = 0;
    for (int i = 0; i < Data_p->Background.Ranges.length; ++i) {
        nh_terminal_AttributeRange *Range_p = ((nh_terminal_AttributeRange*)Data_p->Background.Ranges.p)+i;
        nh_Color Color = nh_terminal_getGlyphColor(State_p, &Range_p->Glyph, NH_FALSE);
        nh_opengl_addCommand(CommandBuffer_p, "glUniform3f", &Data_p->Background.OpenGL.GetUniformLocation_p->Result, 
            nh_opengl_glfloat(NULL, Color.r), nh_opengl_glfloat(NULL, Color.g), nh_opengl_glfloat(NULL, Color.b));
        nh_opengl_addCommand(CommandBuffer_p, "glDrawElements", 
            nh_opengl_glenum(NULL, GL_TRIANGLES), nh_opengl_glsizei(NULL, Range_p->indices), 
            nh_opengl_glenum(NULL, GL_UNSIGNED_INT),
            nh_opengl_pointer(NULL, (void*)(sizeof(uint32_t)*offset)));
        offset += Range_p->indices;
    }

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_prepareOpenGLBackgroundDraw(
    nh_terminal_GraphicsState *State_p, nh_terminal_GraphicsData *Data_p)
{
NH_TERMINAL_BEGIN()

    nh_opengl_CommandBuffer *CommandBuffer_p = State_p->Viewport_p->OpenGL.CommandBuffer_p;
    nh_opengl_addCommand(CommandBuffer_p, "glUseProgram", &Data_p->Background.OpenGL.Program_p->Result);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

// FOREGROUND ======================================================================================

static NH_TERMINAL_RESULT nh_terminal_drawOpenGLForeground(
    nh_terminal_GraphicsState *State_p, nh_terminal_GraphicsData *Data_p, nh_terminal_Grid *Grid_p)
{
NH_TERMINAL_BEGIN()

    nh_opengl_CommandBuffer *CommandBuffer_p = State_p->Viewport_p->OpenGL.CommandBuffer_p;

    nh_opengl_addCommand(CommandBuffer_p, "glBindVertexArray", Data_p->Foreground.OpenGL.VertexArray_p);
    nh_opengl_addCommand(CommandBuffer_p, "glUseProgram", &Data_p->Foreground.OpenGL.Program_p->Result);

    int offset = 0;
    for (int i = 0; i < Data_p->Foreground.Ranges.length; ++i) {
        nh_terminal_AttributeRange *Range_p = ((nh_terminal_AttributeRange*)Data_p->Foreground.Ranges.p)+i;
        nh_Color Color = nh_terminal_getGlyphColor(State_p, &Range_p->Glyph, NH_TRUE);
        nh_opengl_addCommand(CommandBuffer_p, "glUniform3f", &Data_p->Foreground.OpenGL.GetUniformLocationColor_p->Result, 
            nh_opengl_glfloat(NULL, Color.r), nh_opengl_glfloat(NULL, Color.g), nh_opengl_glfloat(NULL, Color.b));
        nh_opengl_addCommand(CommandBuffer_p, "glDrawElements", 
            nh_opengl_glenum(NULL, GL_TRIANGLES), nh_opengl_glsizei(NULL, Range_p->indices), 
            nh_opengl_glenum(NULL, GL_UNSIGNED_INT),
            nh_opengl_pointer(NULL, (void*)(sizeof(uint32_t)*offset)));
        offset += Range_p->indices;
    }

    nh_opengl_addCommand(CommandBuffer_p, "glUseProgram", &Data_p->Foreground.OpenGL.Program2_p->Result);
    nh_opengl_addCommand(CommandBuffer_p, "glBindVertexArray", Data_p->Foreground.OpenGL.VertexArray2_p);

    offset = 0;
    for (int i = 0; i < Data_p->Foreground.Ranges2.length; ++i) {
        nh_terminal_AttributeRange *Range_p = ((nh_terminal_AttributeRange*)Data_p->Foreground.Ranges2.p)+i;
        nh_Color Color = nh_terminal_getGlyphColor(State_p, &Range_p->Glyph, NH_TRUE);
        nh_opengl_addCommand(CommandBuffer_p, "glUniform3f", &Data_p->Foreground.OpenGL.GetUniformLocationColor2_p->Result, 
            nh_opengl_glfloat(NULL, Color.r), nh_opengl_glfloat(NULL, Color.g), nh_opengl_glfloat(NULL, Color.b));
        nh_opengl_addCommand(CommandBuffer_p, "glDrawElements", 
            nh_opengl_glenum(NULL, GL_TRIANGLES), nh_opengl_glsizei(NULL, Range_p->indices), 
            nh_opengl_glenum(NULL, GL_UNSIGNED_INT),
            nh_opengl_pointer(NULL, (void*)(sizeof(uint32_t)*offset)));
        offset += Range_p->indices;
    }

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_prepareOpenGLForegroundDraw(
    nh_terminal_GraphicsState *State_p, nh_terminal_GraphicsData *Data_p)
{
NH_TERMINAL_BEGIN()

    nh_opengl_CommandBuffer *CommandBuffer_p = State_p->Viewport_p->OpenGL.CommandBuffer_p;

    nh_opengl_addCommand(CommandBuffer_p, "glUseProgram", &Data_p->Foreground.OpenGL.Program_p->Result);
    nh_opengl_addCommand(CommandBuffer_p, "glUniform1i", &Data_p->Foreground.OpenGL.GetUniformLocationTexture_p->Result, nh_opengl_glint(NULL, 0));

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

// BOXES ===========================================================================================

static NH_TERMINAL_RESULT nh_terminal_drawOpenGLBoxes(
    nh_terminal_GraphicsState *State_p, nh_terminal_GraphicsData *Data_p, nh_terminal_Grid *Grid_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_Config Config = nh_terminal_getConfig();

    nh_opengl_CommandBuffer *CommandBuffer_p = State_p->Viewport_p->OpenGL.CommandBuffer_p;

    int offset = 0;
    for (int i = 0; i < Grid_p->Boxes.length; ++i) {
        nh_terminal_Box *Box_p = ((nh_terminal_Box*)Grid_p->Boxes.p)+i;
        // Render inner box.
        nh_opengl_addCommand(CommandBuffer_p, "glUniform3f", 
           &Data_p->Background.OpenGL.GetUniformLocation_p->Result, 
           nh_opengl_glfloat(NULL, Config.Background.r), 
           nh_opengl_glfloat(NULL, Config.Background.g), 
           nh_opengl_glfloat(NULL, Config.Background.b));
        nh_opengl_addCommand(CommandBuffer_p, "glDrawArrays", 
            nh_opengl_glenum(NULL, GL_TRIANGLES), nh_opengl_glint(NULL, offset), nh_opengl_glsizei(NULL, 6));
        offset += 6;
        // Render outer box.
        nh_opengl_addCommand(CommandBuffer_p, "glUniform3f", 
           &Data_p->Background.OpenGL.GetUniformLocation_p->Result, 
           nh_opengl_glfloat(NULL, Config.Foreground.r), 
           nh_opengl_glfloat(NULL, Config.Foreground.g), 
           nh_opengl_glfloat(NULL, Config.Foreground.b));
        nh_opengl_addCommand(CommandBuffer_p, "glDrawArrays", 
            nh_opengl_glenum(NULL, GL_TRIANGLES), nh_opengl_glint(NULL, offset), nh_opengl_glsizei(NULL, 6));
        offset += 6;
    }

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_prepareOpenGLBoxesDraw(
    nh_terminal_GraphicsState *State_p, nh_terminal_GraphicsData *Data_p)
{
NH_TERMINAL_BEGIN()

    nh_opengl_CommandBuffer *CommandBuffer_p = State_p->Viewport_p->OpenGL.CommandBuffer_p;
    nh_opengl_addCommand(CommandBuffer_p, "glUseProgram", &Data_p->Background.OpenGL.Program_p->Result);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

// RENDER ==========================================================================================

NH_TERMINAL_RESULT nh_terminal_renderUsingOpenGL(
    nh_terminal_Graphics *Graphics_p, nh_terminal_Grid *Grid_p, nh_terminal_Grid *Grid2_p)
{
NH_TERMINAL_BEGIN()

    NH_BOOL blockUntilRender = Graphics_p->Data1.Background.Action.init || Graphics_p->Data1.Foreground.Action.init;

    nh_gfx_beginRecording(Graphics_p->State.Viewport_p);

    nh_opengl_CommandBuffer *CommandBuffer_p = Graphics_p->State.Viewport_p->OpenGL.CommandBuffer_p;

    nh_opengl_addCommand(
        CommandBuffer_p, "glColorMask", 
        nh_opengl_glboolean(NULL, GL_TRUE),
        nh_opengl_glboolean(NULL, GL_TRUE),
        nh_opengl_glboolean(NULL, GL_TRUE),
        nh_opengl_glboolean(NULL, GL_TRUE));

    nh_opengl_addCommand(CommandBuffer_p, "glClear", nh_opengl_glenum(NULL, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    nh_opengl_addCommand(CommandBuffer_p, "glEnable", nh_opengl_glenum(NULL, GL_BLEND));
    nh_opengl_addCommand(CommandBuffer_p, "glEnable", nh_opengl_glenum(NULL, GL_DEPTH_TEST));
 
    nh_opengl_addCommand(CommandBuffer_p, "glBlendFunc", nh_opengl_glenum(NULL, GL_SRC_ALPHA),
        nh_opengl_glenum(NULL, GL_ONE_MINUS_SRC_ALPHA));

    NH_TERMINAL_CHECK(nh_terminal_updateOpenGLBackground(&Graphics_p->State, &Graphics_p->Data1))
    NH_TERMINAL_CHECK(nh_terminal_prepareOpenGLBackgroundDraw(&Graphics_p->State, &Graphics_p->Data1))
    NH_TERMINAL_CHECK(nh_terminal_drawOpenGLBackground(&Graphics_p->State, &Graphics_p->Data1, Grid_p))

    NH_TERMINAL_CHECK(nh_terminal_updateOpenGLBoxes(&Graphics_p->State, &Graphics_p->Data1))
    NH_TERMINAL_CHECK(nh_terminal_prepareOpenGLBoxesDraw(&Graphics_p->State, &Graphics_p->Data1))
    NH_TERMINAL_CHECK(nh_terminal_drawOpenGLBoxes(&Graphics_p->State, &Graphics_p->Data1, Grid_p))

    NH_TERMINAL_CHECK(nh_terminal_updateOpenGLForeground(&Graphics_p->State, &Graphics_p->Data1))
    NH_TERMINAL_CHECK(nh_terminal_prepareOpenGLForegroundDraw(&Graphics_p->State, &Graphics_p->Data1))
    NH_TERMINAL_CHECK(nh_terminal_drawOpenGLForeground(&Graphics_p->State, &Graphics_p->Data1, Grid_p))

    NH_TERMINAL_CHECK(nh_terminal_updateOpenGLBackground(&Graphics_p->State, &Graphics_p->Data2))
    NH_TERMINAL_CHECK(nh_terminal_prepareOpenGLBackgroundDraw(&Graphics_p->State, &Graphics_p->Data2))
    NH_TERMINAL_CHECK(nh_terminal_drawOpenGLBackground(&Graphics_p->State, &Graphics_p->Data2, Grid2_p))

    NH_TERMINAL_CHECK(nh_terminal_updateOpenGLForeground(&Graphics_p->State, &Graphics_p->Data2))
    NH_TERMINAL_CHECK(nh_terminal_prepareOpenGLForegroundDraw(&Graphics_p->State, &Graphics_p->Data2))
    NH_TERMINAL_CHECK(nh_terminal_drawOpenGLForeground(&Graphics_p->State, &Graphics_p->Data2, Grid2_p))

    nh_opengl_addCommand(
        CommandBuffer_p, "glColorMask", 
        nh_opengl_glboolean(NULL, GL_FALSE),
        nh_opengl_glboolean(NULL, GL_FALSE),
        nh_opengl_glboolean(NULL, GL_FALSE),
        nh_opengl_glboolean(NULL, GL_TRUE));

    nh_opengl_addCommand(
        CommandBuffer_p, "glClearColor",
        nh_opengl_glfloat(NULL, 0.0f),
        nh_opengl_glfloat(NULL, 0.0f),
        nh_opengl_glfloat(NULL, 0.0f),
        nh_opengl_glfloat(NULL, 1.0f)); // Transparency can be set here.

    nh_opengl_addCommand(CommandBuffer_p, "glClear", nh_opengl_glenum(NULL, GL_COLOR_BUFFER_BIT));
 
    nh_gfx_endRecording(Graphics_p->State.Viewport_p, blockUntilRender);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

