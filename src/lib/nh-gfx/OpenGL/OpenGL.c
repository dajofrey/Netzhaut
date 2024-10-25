// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "OpenGL.h"

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_opengl_initOpenGL()
{
    if (!sogl_loadOpenGL()) {
        const char **failures_pp = sogl_getFailures();
        while (*failures_pp) {
            failures_pp++;
        }
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_opengl_terminateOpenGL()
{
    sogl_cleanup();

    return NH_API_SUCCESS;
}

