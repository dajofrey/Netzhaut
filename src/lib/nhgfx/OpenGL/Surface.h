#ifndef NH_OPENGL_SURFACE_H
#define NH_OPENGL_SURFACE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "CommandBuffer.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

    typedef struct nh_opengl_SurfaceRequirements {
        nh_Array Ids; // Contains visualid's that are glx framebuffer compatible. This is used for WSI window creation.
    } nh_opengl_SurfaceRequirements;

    typedef struct nh_opengl_Surface {
        GLXContext Context_p;
        nh_opengl_CommandBuffer *CommandBuffers_p;
        unsigned int bufferCount;         
        uint32_t currentBuffer;               
    } nh_opengl_Surface;

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    nh_opengl_Surface nh_opengl_initSurface(
    );

    NH_GFX_RESULT nh_opengl_createSurface(
        nh_opengl_Surface *Surface_p, nh_wsi_Window *Window_p
    );

    NH_GFX_RESULT nh_opengl_destroySurface(
        nh_opengl_Surface *Surface_p, nh_wsi_Window *Window_p
    );

    NH_GFX_RESULT nh_opengl_createSurfaceRequirements(
        nh_opengl_SurfaceRequirements *Requirements_p
    );

    NH_GFX_RESULT nh_opengl_freeSurfaceRequirements(
        nh_opengl_SurfaceRequirements *Requirements_p
    );

/** @} */

#endif
