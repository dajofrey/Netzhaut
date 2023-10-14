// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

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

// FUNCTIONS =======================================================================================

static NH_TERMINAL_RESULT nh_terminal_initOpenGLBoxesVertices(
    nh_terminal_OpenGLBoxes *Boxes_p, nh_opengl_CommandBuffer *CommandBuffer_p)
{
NH_TERMINAL_BEGIN()

    Boxes_p->VertexArray_p = nh_opengl_disableDataAutoFree(nh_opengl_gluint(NULL, 0));
    nh_opengl_addCommand(CommandBuffer_p, "glGenVertexArrays", nh_opengl_gluint(NULL, 1),
        Boxes_p->VertexArray_p);
    nh_opengl_addCommand(CommandBuffer_p, "glBindVertexArray", Boxes_p->VertexArray_p);

    Boxes_p->VerticesBuffer_p = nh_opengl_disableDataAutoFree(nh_opengl_gluint(NULL, 0));
    nh_opengl_addCommand(CommandBuffer_p, "glGenBuffers", nh_opengl_gluint(NULL, 1), 
        Boxes_p->VerticesBuffer_p);
    nh_opengl_addCommand(CommandBuffer_p, "glBindBuffer",
        nh_opengl_glenum(NULL, GL_ELEMENT_ARRAY_BUFFER), Boxes_p->VerticesBuffer_p);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_updateOpenGLBoxesVertices(
    nh_terminal_GraphicsBoxes *Boxes_p, nh_opengl_CommandBuffer *CommandBuffer_p)
{
NH_TERMINAL_BEGIN()

    nh_opengl_addCommand(CommandBuffer_p, "glBindVertexArray", Boxes_p->OpenGL.VertexArray_p);

    // Vertices.
    nh_opengl_addCommand(CommandBuffer_p, "glBindBuffer",
        nh_opengl_glenum(NULL, GL_ARRAY_BUFFER), Boxes_p->OpenGL.VerticesBuffer_p);
    nh_opengl_addCommand(CommandBuffer_p, "glBufferData", 
        nh_opengl_glenum(NULL, GL_ARRAY_BUFFER), 
        nh_opengl_glsizeiptr(NULL, Boxes_p->Vertices.length*sizeof(GLfloat)),
        nh_opengl_glubyte(NULL, Boxes_p->Vertices.p, Boxes_p->Vertices.length*sizeof(GLfloat)),
        nh_opengl_glenum(NULL, GL_STATIC_DRAW));

    nh_opengl_addCommand(CommandBuffer_p, "glEnableVertexAttribArray", nh_opengl_gluint(NULL, 0));
    nh_opengl_addCommand(CommandBuffer_p, "glVertexAttribPointer",
        nh_opengl_gluint(NULL, 0), nh_opengl_gluint(NULL, 3), nh_opengl_glenum(NULL, GL_FLOAT),
        nh_opengl_glboolean(NULL, GL_FALSE), nh_opengl_glsizei(NULL, sizeof(float)*3), 
        nh_opengl_pointer(NULL, NULL));

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_updateOpenGLBoxes(
    void *state_p, void *data_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_GraphicsData *Data_p = data_p;
    nh_terminal_GraphicsState *State_p = state_p;

    if (Data_p->Boxes.Action.init) {
        nh_terminal_initOpenGLBoxesVertices(
            &Data_p->Boxes.OpenGL, State_p->Viewport_p->OpenGL.CommandBuffer_p);
        Data_p->Boxes.Action.init = NH_FALSE;
    }

    nh_terminal_updateOpenGLBoxesVertices(
        &Data_p->Boxes, State_p->Viewport_p->OpenGL.CommandBuffer_p);

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_initOpenGLBoxes(
    nh_terminal_OpenGLBoxes *Boxes_p)
{
NH_TERMINAL_BEGIN()

    memset(Boxes_p, 0, sizeof(nh_terminal_OpenGLBoxes));
 
NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_freeOpenGLBoxes(
    nh_terminal_OpenGLBoxes *Boxes_p)
{
NH_TERMINAL_BEGIN()

    nh_opengl_freeData(Boxes_p->VertexArray_p);
    nh_opengl_freeData(Boxes_p->VerticesBuffer_p);
    nh_opengl_freeCommand(Boxes_p->BufferData_p);
 
NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

