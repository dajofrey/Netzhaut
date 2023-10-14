// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Background.h"

#include "../Terminal/Graphics.h"
#include "../Common/Macros.h"

#include "../../nhgfx/Base/Viewport.h"
#include "../../nhgfx/Common/Macros.h"

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <float.h>

// INIT HELPER =====================================================================================

static NH_TERMINAL_RESULT nh_terminal_initOpenGLBackgroundProgram(
    nh_terminal_OpenGLBackground *Background_p, nh_opengl_CommandBuffer *CommandBuffer_p)
{
NH_TERMINAL_BEGIN()

    static const NH_BYTE* vsSource_p =
        "#version 450\n"
        "layout(location=0) in vec3 position;\n"
        "uniform vec3 in_color;\n"
        "out vec3 color;\n"
        "void main() {\n"
        "    color = in_color;\n"
        "    gl_Position = vec4(position, 1.0);\n"
        "}\n";

    static const NH_BYTE* fsSource_p =
        "#version 450\n"
        "in vec3 color;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "    fragColor = vec4(color, 1.0);\n"
        "}\n";

    Background_p->VertexShader_p =
        nh_opengl_disableCommandAutoFree(
            nh_opengl_addCommand(CommandBuffer_p, "glCreateShader", nh_opengl_glint(NULL, GL_VERTEX_SHADER)));
    nh_opengl_addCommand(CommandBuffer_p, "glShaderSource",
        &Background_p->VertexShader_p->Result, nh_opengl_gluint(NULL, 1),
        nh_opengl_glchar(NULL, NULL, 0, (GLchar**)&vsSource_p),
        nh_opengl_pointer(NULL, NULL));
    nh_opengl_addCommand(CommandBuffer_p, "glCompileShader", &Background_p->VertexShader_p->Result);

    Background_p->FragmentShader_p =
        nh_opengl_disableCommandAutoFree(
            nh_opengl_addCommand(CommandBuffer_p, "glCreateShader", nh_opengl_glint(NULL, GL_FRAGMENT_SHADER)));
    nh_opengl_addCommand(CommandBuffer_p, "glShaderSource",
        &Background_p->FragmentShader_p->Result, nh_opengl_gluint(NULL, 1),
        nh_opengl_glchar(NULL, NULL, 0, (GLchar**)&fsSource_p),
        nh_opengl_pointer(NULL, NULL));
    nh_opengl_addCommand(CommandBuffer_p, "glCompileShader", &Background_p->FragmentShader_p->Result);

    Background_p->Program_p = 
        nh_opengl_disableCommandAutoFree(nh_opengl_addCommand(CommandBuffer_p, "glCreateProgram"));

    nh_opengl_addCommand(CommandBuffer_p, "glAttachShader", &Background_p->Program_p->Result,
        &Background_p->VertexShader_p->Result);
    nh_opengl_addCommand(CommandBuffer_p, "glAttachShader", &Background_p->Program_p->Result,
        &Background_p->FragmentShader_p->Result);

    nh_opengl_addCommand(CommandBuffer_p, "glLinkProgram", &Background_p->Program_p->Result);

    static GLchar *colorName_p = "in_color";
    Background_p->GetUniformLocation_p =
        nh_opengl_disableCommandAutoFree(nh_opengl_addCommand(CommandBuffer_p, "glGetUniformLocation", 
            &Background_p->Program_p->Result, nh_opengl_glchar(NULL, NULL, 0, &colorName_p)));
  
NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_initOpenGLBackgroundVertices(
    nh_terminal_OpenGLBackground *Background_p, nh_opengl_CommandBuffer *CommandBuffer_p)
{
NH_TERMINAL_BEGIN()

    Background_p->VertexArray_p = nh_opengl_disableDataAutoFree(nh_opengl_gluint(NULL, 0));
    nh_opengl_addCommand(CommandBuffer_p, "glGenVertexArrays", nh_opengl_gluint(NULL, 1),
        Background_p->VertexArray_p);
    nh_opengl_addCommand(CommandBuffer_p, "glBindVertexArray", Background_p->VertexArray_p);

    Background_p->IndicesBuffer_p = nh_opengl_disableDataAutoFree(nh_opengl_gluint(NULL, 0));
    nh_opengl_addCommand(CommandBuffer_p, "glGenBuffers", nh_opengl_gluint(NULL, 1), 
        Background_p->IndicesBuffer_p);
    nh_opengl_addCommand(CommandBuffer_p, "glBindBuffer",
        nh_opengl_glenum(NULL, GL_ELEMENT_ARRAY_BUFFER), Background_p->IndicesBuffer_p);

    Background_p->VerticesBuffer_p = nh_opengl_disableDataAutoFree(nh_opengl_gluint(NULL, 0));
    nh_opengl_addCommand(CommandBuffer_p, "glGenBuffers", nh_opengl_gluint(NULL, 1), 
        Background_p->VerticesBuffer_p);
    nh_opengl_addCommand(CommandBuffer_p, "glBindBuffer",
        nh_opengl_glenum(NULL, GL_ELEMENT_ARRAY_BUFFER), Background_p->VerticesBuffer_p);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

// UPDATE ==========================================================================================

static NH_TERMINAL_RESULT nh_terminal_updateOpenGLBackgroundVertices(
    nh_terminal_OpenGLBackground *Background_p, nh_opengl_CommandBuffer *CommandBuffer_p, nh_Array *Vertices_p,
    nh_Array *Indices_p)
{
NH_TERMINAL_BEGIN()

    nh_opengl_addCommand(CommandBuffer_p, "glBindVertexArray", Background_p->VertexArray_p);

    // Indices.
    nh_opengl_addCommand(CommandBuffer_p, "glBindBuffer",
        nh_opengl_glenum(NULL, GL_ELEMENT_ARRAY_BUFFER), Background_p->IndicesBuffer_p);
    Background_p->BufferData_p = nh_opengl_addCommand(CommandBuffer_p, "glBufferData",
        nh_opengl_glenum(NULL, GL_ELEMENT_ARRAY_BUFFER),
        nh_opengl_glsizeiptr(NULL, Indices_p->length*sizeof(uint32_t)),
        nh_opengl_glubyte(NULL, Indices_p->p, Indices_p->length*sizeof(uint32_t)),
        nh_opengl_glenum(NULL, GL_STATIC_DRAW));

    // Vertices.
    nh_opengl_addCommand(CommandBuffer_p, "glBindBuffer",
        nh_opengl_glenum(NULL, GL_ARRAY_BUFFER), Background_p->VerticesBuffer_p);
    nh_opengl_addCommand(CommandBuffer_p, "glBufferData", 
        nh_opengl_glenum(NULL, GL_ARRAY_BUFFER), 
        nh_opengl_glsizeiptr(NULL, Vertices_p->length*sizeof(GLfloat)),
        nh_opengl_glubyte(NULL, Vertices_p->p, Vertices_p->length*sizeof(GLfloat)),
        nh_opengl_glenum(NULL, GL_STATIC_DRAW));

    nh_opengl_addCommand(CommandBuffer_p, "glEnableVertexAttribArray", nh_opengl_gluint(NULL, 0));
    nh_opengl_addCommand(CommandBuffer_p, "glVertexAttribPointer",
        nh_opengl_gluint(NULL, 0), nh_opengl_gluint(NULL, 3), nh_opengl_glenum(NULL, GL_FLOAT),
        nh_opengl_glboolean(NULL, GL_FALSE), nh_opengl_glsizei(NULL, sizeof(float)*3), 
        nh_opengl_pointer(NULL, NULL));

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_updateOpenGLBackground(
    void *state_p, void *data_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_GraphicsState *State_p = state_p;
    nh_terminal_GraphicsData *Data_p = data_p;

    if (Data_p->Background.Action.init) {
        nh_terminal_initOpenGLBackgroundProgram(&Data_p->Background.OpenGL, 
            State_p->Viewport_p->OpenGL.CommandBuffer_p);
        nh_terminal_initOpenGLBackgroundVertices(&Data_p->Background.OpenGL, 
            State_p->Viewport_p->OpenGL.CommandBuffer_p);
        Data_p->Background.Action.init = NH_FALSE;
    }

    nh_terminal_updateOpenGLBackgroundVertices(&Data_p->Background.OpenGL,
        State_p->Viewport_p->OpenGL.CommandBuffer_p, &Data_p->Background.Vertices,
        &Data_p->Background.Indices);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_initOpenGLBackground(
    nh_terminal_OpenGLBackground *Background_p)
{
NH_TERMINAL_BEGIN()

    memset(Background_p, 0, sizeof(nh_terminal_OpenGLBackground));
 
NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_freeOpenGLBackground(
    nh_terminal_OpenGLBackground *Background_p)
{
NH_TERMINAL_BEGIN()

    nh_opengl_freeCommand(Background_p->VertexShader_p);
    nh_opengl_freeCommand(Background_p->FragmentShader_p);
    nh_opengl_freeCommand(Background_p->Program_p);
    nh_opengl_freeCommand(Background_p->GetUniformLocation_p);
    
    nh_opengl_freeData(Background_p->VertexArray_p);
    nh_opengl_freeData(Background_p->IndicesBuffer_p);
    nh_opengl_freeData(Background_p->VerticesBuffer_p);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

