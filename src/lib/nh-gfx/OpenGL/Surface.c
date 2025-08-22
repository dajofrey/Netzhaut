// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Surface.h"
#include "ContextX11.h"
#include "ContextCocoa.h"

#include "../Base/Surface.h"

#include "../../nh-core/System/Thread.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-wsi/Window/Window.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// FUNCTIONS ============================================================================================

nh_gfx_OpenGLSurface nh_gfx_initOpenGLSurface()
{
    nh_gfx_OpenGLSurface Surface;
    memset(&Surface, 0, sizeof(nh_gfx_OpenGLSurface));
    return Surface;
}

NH_API_RESULT nh_gfx_createOpenGLSurface(
    nh_gfx_OpenGLSurface *Surface_p, nh_api_Window *Window_p)
{
    switch (((nh_wsi_Window*)Window_p)->type) {
        case NH_WSI_TYPE_X11 :
#if defined(__unix__)
            NH_CORE_CHECK(nh_gfx_createOpenGLX11Context(Surface_p, Window_p))
            break;
#else
            return NH_API_ERROR_BAD_STATE;
#endif
        case NH_WSI_TYPE_COCOA :
#if defined(__APPLE__)
            NH_CORE_CHECK(nh_gfx_createOpenGLCocoaContext(Surface_p, (nh_wsi_Window*)Window_p))
            break;
#else
            return NH_API_ERROR_BAD_STATE;
#endif
    }

    int bufferCount = 3;

    Surface_p->CommandBuffers_p = (nh_gfx_OpenGLCommandBuffer*)nh_core_allocate(sizeof(nh_gfx_OpenGLCommandBuffer)*bufferCount);
    NH_CORE_CHECK_MEM(Surface_p->CommandBuffers_p)

    Surface_p->bufferCount = bufferCount;
    Surface_p->currentBuffer = 0;

    for (int i = 0; i < Surface_p->bufferCount; ++i) {
         Surface_p->CommandBuffers_p[i] = nh_gfx_initOpenGLCommandBuffer();
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_destroyOpenGLSurface(
    nh_gfx_OpenGLSurface *Surface_p, nh_api_Window *Window_p)
{
    switch (((nh_wsi_Window*)Window_p)->type) {
        case NH_WSI_TYPE_X11 :
#if defined(__unix__)
            nh_gfx_destroyOpenGLX11Context(Surface_p);
            break;
#else
            return NH_API_ERROR_BAD_STATE;
#endif
        case NH_WSI_TYPE_COCOA :
#if defined(__APPLE__)
            nh_gfx_destroyOpenGLCocoaContext(Surface_p);
            break;
#else
            return NH_API_ERROR_BAD_STATE;
#endif
    }

    nh_core_free(Surface_p->CommandBuffers_p);
    
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_updateOpenGLSurface(
    nh_gfx_OpenGLSurface *Surface_p, nh_api_Window *Window_p)
{
    switch (((nh_wsi_Window*)Window_p)->type) {
        case NH_WSI_TYPE_X11 :
            break;
        case NH_WSI_TYPE_COCOA :
#if defined(__APPLE__)
            nh_gfx_updateOpenGLCocoaContext(Surface_p);
            break;
#else
            return NH_API_ERROR_BAD_STATE;
#endif
    }

    return NH_API_SUCCESS;
}

