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

#include "../../nh-core/Util/Array.h"
#include "../../nh-wsi/Window/Window.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_opengl_Surface {
        GLXContext Context_p;
        nh_opengl_CommandBuffer *CommandBuffers_p;
        unsigned int bufferCount;         
        uint32_t currentBuffer;               
    } nh_opengl_Surface;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    nh_opengl_Surface nh_opengl_initSurface(
    );

    NH_API_RESULT nh_opengl_createSurface(
        nh_opengl_Surface *Surface_p, nh_wsi_Window *Window_p
    );

    NH_API_RESULT nh_opengl_destroySurface(
        nh_opengl_Surface *Surface_p, nh_wsi_Window *Window_p
    );

/** @} */

#endif
