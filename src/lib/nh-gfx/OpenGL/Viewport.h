#ifndef NH_OPENGL_VIEWPORT_H
#define NH_OPENGL_VIEWPORT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "CommandBuffer.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_opengl_Viewport {
        nh_opengl_CommandBuffer CommandBuffers_p[2];
        nh_opengl_CommandBuffer *CommandBuffer_p;
    } nh_opengl_Viewport;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_GFX_RESULT nh_opengl_createViewport(
        nh_gfx_Viewport *Viewport_p
    );
    
    void nh_opengl_destroyViewport(
        nh_gfx_Viewport *Viewport_p
    );
    
    NH_GFX_RESULT nh_opengl_beginRecording(
        nh_gfx_Viewport *Viewport_p
    );
    
    NH_GFX_RESULT nh_opengl_endRecording(
        nh_gfx_Viewport *Viewport_p
    );

/** @} */

#endif
