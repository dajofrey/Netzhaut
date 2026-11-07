// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Render.h"
#include "RenderIOS.h"
#if defined(NH_PLATFORM_ANDROID)
    #include "ContextAndroid.h"
    #include <android/log.h>
#endif

#include "../Base/Viewport.h"
#include "../Base/Surface.h"

#include "../../nh-wsi/Window/Window.h"

#if defined(NH_PLATFORM_UNIX)
    #include "../../nh-wsi/Platforms/X11/Init.h"
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_OSX
        #include <OpenGL/OpenGL.h>
        #include "ContextCocoa.h"
    #endif
#endif

// RENDER ==========================================================================================

NH_API_RESULT nh_gfx_renderOpenGL(
    nh_gfx_Surface *Surface_p, nh_core_List *SortedViewports_p)
{
#if defined(NH_PLATFORM_UNIX)
    glXMakeCurrent(NH_WSI_X11.Display_p, Surface_p->Window_p->X11.Handle, 
        Surface_p->OpenGL.Context_p);

#elif defined(__APPLE__)
    #if TARGET_OS_OSX
        CGLError err = CGLSetCurrentContext(Surface_p->OpenGL.Context_p);
        if (err != kCGLNoError) {
            fprintf(stderr, "[gfx] Failed to make OpenGL context current: %s\n", CGLErrorString(err));
            return NH_API_ERROR_BAD_STATE;
        }
        if (CGLGetCurrentContext() == NULL) {
            return NH_API_ERROR_BAD_STATE;
        }
        nh_gfx_updateOpenGLCocoaContext(&Surface_p->OpenGL);
    #elif TARGET_OS_IPHONE
        if (nh_gfx_makeOpenGLContextCurrentIOS(Surface_p->OpenGL.Context_p) != 0) {
            return NH_API_ERROR_BAD_STATE;
        }
    #endif
#elif defined(NH_PLATFORM_ANDROID)
    bool ready = false;
    NH_CORE_CHECK(nh_gfx_prepareOpenGLAndroidContext(&Surface_p->OpenGL, Surface_p->Window_p, &ready))
    if (!ready) {
        return NH_API_SUCCESS;
    }
#endif

// TARGET RESCUE: Force iOS to bind the layer-backed framebuffer before commands run
#if defined(__APPLE__) && TARGET_OS_IPHONE
    glBindFramebuffer(GL_FRAMEBUFFER, Surface_p->OpenGL.framebuffer);
#endif
#if defined(NH_PLATFORM_ANDROID)
    bool deferRawDrawing = false;
    nh_gfx_OpenGLCommandBuffer probe = {0};
#endif
    for (int i = 0; i < SortedViewports_p->size; ++i) {
        nh_gfx_Viewport *Viewport_p = SortedViewports_p->pp[i];
#if defined(NH_PLATFORM_ANDROID)
        deferRawDrawing |= Viewport_p->OpenGL.CommandBuffer_p->deferRawDrawing;
        if (Viewport_p->OpenGL.CommandBuffer_p->RawDrawingProbe.enabled) {
            probe.RawDrawingProbe = Viewport_p->OpenGL.CommandBuffer_p->RawDrawingProbe;
            probe.rawDrawingGeneration = Viewport_p->OpenGL.CommandBuffer_p->rawDrawingGeneration;
        }
        if (Viewport_p->OpenGL.CommandBuffer_p->commitRawDrawing)
            Surface_p->OpenGL.rawDrawingGeneration = Viewport_p->OpenGL.CommandBuffer_p->rawDrawingGeneration;
#endif
        NH_API_RESULT dbg_r = nh_gfx_executeOpenGLCommandBuffer(Viewport_p->OpenGL.CommandBuffer_p);
        NH_CORE_CHECK(dbg_r)
        NH_CORE_CHECK(nh_gfx_freeOpenGLCommandBuffer(Viewport_p->OpenGL.CommandBuffer_p))
    }

#if defined(NH_PLATFORM_UNIX)
    glXSwapBuffers(NH_WSI_X11.Display_p, Surface_p->Window_p->X11.Handle);

#elif defined(__APPLE__)
    #if TARGET_OS_OSX
        nh_gfx_flushOpenGLCocoaContext(&Surface_p->OpenGL);
    #elif TARGET_OS_IPHONE
        nh_gfx_flushOpenGLDrawableIOS(Surface_p->OpenGL.Context_p, Surface_p->OpenGL.framebuffer, Surface_p->OpenGL.colorRenderbuffer);
    #endif
#elif defined(NH_PLATFORM_ANDROID)
    uint64_t generation = deferRawDrawing ? 0 : Surface_p->OpenGL.rawDrawingGeneration;
    if (nh_wsi_canPostOnyxFrame(&Surface_p->Window_p->Android.Onyx.Frame, generation))
        glFinish();
    if (probe.RawDrawingProbe.enabled) {
        unsigned char pixels[17 * 17 * 4];
        GLint pack, packBuffer, rowLength, skipRows, skipPixels;
        glGetIntegerv(GL_PACK_ALIGNMENT, &pack);
        glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &packBuffer);
        glGetIntegerv(GL_PACK_ROW_LENGTH, &rowLength);
        glGetIntegerv(GL_PACK_SKIP_ROWS, &skipRows);
        glGetIntegerv(GL_PACK_SKIP_PIXELS, &skipPixels);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ROW_LENGTH, 0);
        glPixelStorei(GL_PACK_SKIP_ROWS, 0);
        glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
        GLenum before = glGetError();
        glReadPixels(probe.RawDrawingProbe.x, probe.RawDrawingProbe.y, 17, 17,
            GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        GLenum error = glGetError();
        glBindBuffer(GL_PIXEL_PACK_BUFFER, packBuffer);
        glPixelStorei(GL_PACK_ALIGNMENT, pack);
        glPixelStorei(GL_PACK_ROW_LENGTH, rowLength);
        glPixelStorei(GL_PACK_SKIP_ROWS, skipRows);
        glPixelStorei(GL_PACK_SKIP_PIXELS, skipPixels);
        int ink = 0, paper = 0, missing = 0, extra = 0;
        if (before == GL_NO_ERROR && error == GL_NO_ERROR) {
            for (int p = 0; p < 17 * 17; ++p) {
                if (probe.RawDrawingProbe.expected[p] == 1) {
                    ++ink;
                    if (pixels[p * 4] > 64) ++missing;
                } else if (probe.RawDrawingProbe.expected[p] == 2) {
                    ++paper;
                    if (pixels[p * 4] < 120) ++extra;
                }
            }
        }
        __android_log_print(ANDROID_LOG_INFO, "Netzhaut-gfx",
            "handoff GPU buffer=%llu current=%llu revision=%u xy=%d,%d "
            "ink=%d paper=%d missing-ink=%d extra-ink=%d GL-before=0x%x GL-read=0x%x",
            (unsigned long long)probe.rawDrawingGeneration,
            (unsigned long long)__atomic_load_n(
                &Surface_p->Window_p->Android.Onyx.Frame.generation, __ATOMIC_SEQ_CST),
            probe.RawDrawingProbe.revision, probe.RawDrawingProbe.x, probe.RawDrawingProbe.y,
            ink, paper, missing, extra, before, error);
    }
    NH_CORE_CHECK(nh_gfx_swapOpenGLAndroidBuffers(
        &Surface_p->OpenGL, &Surface_p->Window_p->Android.Onyx, generation))
    if (Surface_p->OpenGL.rawDrawingGeneration ==
            __atomic_load_n(&Surface_p->Window_p->Android.Onyx.Frame.posted, __ATOMIC_SEQ_CST) ||
        Surface_p->OpenGL.rawDrawingGeneration !=
            __atomic_load_n(&Surface_p->Window_p->Android.Onyx.Frame.generation, __ATOMIC_SEQ_CST))
        Surface_p->OpenGL.rawDrawingGeneration = 0;
#endif

    return NH_API_SUCCESS;
}
