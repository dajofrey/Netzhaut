#ifndef NH_TERMINAL_OPENGL_BACKGROUND_H
#define NH_TERMINAL_OPENGL_BACKGROUND_H

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

    typedef struct nh_terminal_OpenGLBackground {
        nh_opengl_Command *VertexShader_p;
        nh_opengl_Command *FragmentShader_p;
        nh_opengl_Command *Program_p;
        nh_opengl_Data *VertexArray_p;
        nh_opengl_Data *IndicesBuffer_p;
        nh_opengl_Data *VerticesBuffer_p;
        nh_opengl_Command *BufferData_p;
        nh_opengl_Command *GetUniformLocation_p;
    } nh_terminal_OpenGLBackground;

/** @} */

/** @addtogroup lib_nhterminal_functions
 *  @{
 */

    NH_TERMINAL_RESULT nh_terminal_updateOpenGLBackground(
        void *state_p, void *data_p
    );

    NH_TERMINAL_RESULT nh_terminal_initOpenGLBackground(
        nh_terminal_OpenGLBackground *Background_p
    );

    NH_TERMINAL_RESULT nh_terminal_freeOpenGLBackground(
        nh_terminal_OpenGLBackground *Background_p
    );

/** @} */

#endif 
