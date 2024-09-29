#ifndef NH_OPENGL_OPENGL_H
#define NH_OPENGL_OPENGL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#define NETZHAUT_OPENGL
#define NETZHAUT_OPENGL_IMPLEMENTATION

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_API_RESULT nh_opengl_initOpenGL(
    );

    NH_API_RESULT nh_opengl_terminateOpenGL(
    );

/** @} */

#endif 
