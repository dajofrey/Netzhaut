#ifndef NH_TERMINAL_OPENGL_RENDER_H
#define NH_TERMINAL_OPENGL_RENDER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Terminal/Graphics.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhcss_functions 
 *  @{
 */

    NH_TERMINAL_RESULT nh_terminal_renderUsingOpenGL(
        nh_terminal_Graphics *Graphics_p, nh_terminal_Grid *Grid_p, nh_terminal_Grid *Grid2_p
    );

/** @} */

#endif
