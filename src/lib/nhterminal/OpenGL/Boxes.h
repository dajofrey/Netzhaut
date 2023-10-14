#ifndef NH_TERMINAL_OPENGL_BOXES_H
#define NH_TERMINAL_OPENGL_BOXES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../../nhgfx/OpenGL/CommandBuffer.h"
#include "../../nhgfx/OpenGL/Commands.h"

#endif

/** @addtogroup lib_nhterminal_structs
 *  @{
 */

    typedef struct nh_terminal_OpenGLBoxes {
        nh_opengl_Data *VertexArray_p;
        nh_opengl_Data *VerticesBuffer_p;
        nh_opengl_Command *BufferData_p;
    } nh_terminal_OpenGLBoxes;

/** @} */

/** @addtogroup lib_nhterminal_functions
 *  @{
 */

    NH_TERMINAL_RESULT nh_terminal_updateOpenGLBoxes(
        void *state_p, void *data_p
    );

    NH_TERMINAL_RESULT nh_terminal_initOpenGLBoxes(
        nh_terminal_OpenGLBoxes *Boxes_p
    );

    NH_TERMINAL_RESULT nh_terminal_freeOpenGLBoxes(
        nh_terminal_OpenGLBoxes *Boxes_p
    );

/** @} */

#endif 
