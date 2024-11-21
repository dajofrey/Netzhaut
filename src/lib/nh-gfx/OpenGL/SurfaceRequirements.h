#ifndef NH_OPENGL_SURFACE_REQUIREMENTS_H
#define NH_OPENGL_SURFACE_REQUIREMENTS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "CommandBuffer.h"
#include "../Common/Includes.h"

#include "../../nh-core/Util/Array.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_gfx_OpenGLSurfaceRequirements {
        nh_core_Array Ids; // Contains visualid's that are glx framebuffer compatible. This is used for WSI window creation.
    } nh_gfx_OpenGLSurfaceRequirements;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_API_RESULT nh_gfx_createOpenGLSurfaceRequirements(
        nh_gfx_OpenGLSurfaceRequirements *Requirements_p
    );

    NH_API_RESULT nh_gfx_freeOpenGLSurfaceRequirements(
        nh_gfx_OpenGLSurfaceRequirements *Requirements_p
    );

/** @} */

#endif
