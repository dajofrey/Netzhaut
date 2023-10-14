#ifndef NH_TERMINAL_OPENGL_FOREGROUND_H
#define NH_TERMINAL_OPENGL_FOREGROUND_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../../nhgfx/OpenGL/CommandBuffer.h"
#include "../../nhgfx/OpenGL/Commands.h"

#include "../../nhgfx/Fonts/FontManager.h"

#include <stdint.h>

typedef struct nh_terminal_Graphics nh_terminal_Graphics;

#endif

/** @addtogroup lib_nhterminal_structs
 *  @{
 */

    typedef struct nh_terminal_OpenGLForeground {
        nh_opengl_Command *VertexShader_p;
        nh_opengl_Command *FragmentShader_p;
        nh_opengl_Command *Program_p;
        nh_opengl_Command *VertexShader2_p;
        nh_opengl_Command *FragmentShader2_p;
        nh_opengl_Command *Program2_p;
        nh_opengl_Data *Texture_p;
        nh_opengl_Data *VertexArray_p;
        nh_opengl_Data *VertexArray2_p;
        nh_opengl_Data *IndicesBuffer_p;
        nh_opengl_Data *IndicesBuffer2_p;
        nh_opengl_Data *VerticesBuffer_p;
        nh_opengl_Data *VerticesBuffer2_p;
        nh_opengl_Command *BufferData_p;
        nh_opengl_Command *BufferData2_p;
        nh_opengl_Command *GetUniformLocationTexture_p;
        nh_opengl_Command *GetUniformLocationColor_p;
        nh_opengl_Command *GetUniformLocationColor2_p;
    } nh_terminal_OpenGLForeground;

/** @} */

/** @addtogroup lib_nhterminal_functions
 *  @{
 */

    NH_TERMINAL_RESULT nh_terminal_updateOpenGLForeground(
        void *state_p, void *data_p
    );

    NH_TERMINAL_RESULT nh_terminal_initOpenGLForeground(
        nh_terminal_OpenGLForeground *Text_p
    );

    NH_TERMINAL_RESULT nh_terminal_freeOpenGLForeground(
        nh_terminal_OpenGLForeground *Foreground_p
    );

/** @} */

#endif 
