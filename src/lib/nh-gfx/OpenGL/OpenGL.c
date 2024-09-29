// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "OpenGL.h"

#include "../Common/Macros.h"

// HOST ============================================================================================

NH_API_RESULT nh_opengl_initOpenGL()
{
NH_GFX_BEGIN()

    if (!sogl_loadOpenGL()) {
        const char **failures_pp = sogl_getFailures();
        while (*failures_pp) {
            failures_pp++;
        }
    }

NH_GFX_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_opengl_terminateOpenGL()
{
NH_GFX_BEGIN()

    sogl_cleanup();

NH_GFX_DIAGNOSTIC_END(NH_API_SUCCESS)
}

