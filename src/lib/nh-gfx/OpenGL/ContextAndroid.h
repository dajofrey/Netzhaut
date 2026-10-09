#ifndef NH_GFX_OPENGL_CONTEXT_ANDROID_H
#define NH_GFX_OPENGL_CONTEXT_ANDROID_H

#include "Surface.h"
#include "../../nh-wsi/Window/Window.h"

NH_API_RESULT nh_gfx_createOpenGLAndroidContext(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p
);

NH_API_RESULT nh_gfx_prepareOpenGLAndroidContext(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p, bool *ready_p
);

NH_API_RESULT nh_gfx_swapOpenGLAndroidBuffers(
    nh_gfx_OpenGLSurface *Surface_p
);

NH_API_RESULT nh_gfx_destroyOpenGLAndroidContext(
    nh_gfx_OpenGLSurface *Surface_p
);

#endif
