#ifndef NH_OPENGL_OPENGL_H
#define NH_OPENGL_OPENGL_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#define NETZHAUT_OPENGL
#define NETZHAUT_OPENGL_IMPLEMENTATION

#include "../Common/Includes.h"

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_API_RESULT nh_gfx_initOpenGL(
    );

    NH_API_RESULT nh_gfx_terminateOpenGL(
    );

/** @} */

#endif 
