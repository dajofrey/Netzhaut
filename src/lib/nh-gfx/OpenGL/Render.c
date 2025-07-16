// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Render.h"

#include "../Base/Viewport.h"
#include "../Base/Surface.h"

#include "../../nh-wsi/Window/Window.h"

#if defined(__unix__)
    #include "../../nh-wsi/Platforms/X11/Init.h"
#endif

// RENDER ==========================================================================================

NH_API_RESULT nh_gfx_renderOpenGL(
    nh_gfx_Surface *Surface_p, nh_core_List *SortedViewports_p)
{
#if defined(__unix__)
    glXMakeCurrent(NH_WSI_X11.Display_p, Surface_p->Window_p->X11.Handle, 
        Surface_p->OpenGL.Context_p);
#elif defined(__APPLE__)
    CGLError err = CGLSetCurrentContext(Surface_p->OpenGL.Context_p);
    if (err != kCGLNoError) {
        fprintf(stderr, "[gfx] Failed to make OpenGL context current: %s\n", CGLErrorString(err));
        return NH_API_ERROR_BAD_STATE;
    }
    if (CGLGetCurrentContext() == NULL) {
        return NH_API_ERROR_BAD_STATE;
    }
#endif

    for (int i = 0; i < SortedViewports_p->size; ++i) {
        nh_gfx_Viewport *Viewport_p = SortedViewports_p->pp[i];
        NH_CORE_CHECK(nh_gfx_executeOpenGLCommandBuffer(Viewport_p->OpenGL.CommandBuffer_p))
        NH_CORE_CHECK(nh_gfx_freeOpenGLCommandBuffer(Viewport_p->OpenGL.CommandBuffer_p))
    }

#if defined(__unix__)
    glXSwapBuffers(NH_WSI_X11.Display_p, Surface_p->Window_p->X11.Handle);
#elif defined(__APPLE__)
    CGLFlushDrawable(Surface_p->OpenGL.Context_p);
#endif

    return NH_API_SUCCESS;
}
