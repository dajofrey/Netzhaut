#include "ContextAndroid.h"

#include "../../nh-wsi/Platforms/Android/Window.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>

static void nh_gfx_destroyAndroidEGLSurface(
    nh_gfx_OpenGLSurface *Surface_p)
{
    if (Surface_p->Display == EGL_NO_DISPLAY) {
        Surface_p->EGLSurface = EGL_NO_SURFACE;
        return;
    }

    if (eglGetCurrentContext() == Surface_p->Context) {
        eglMakeCurrent(Surface_p->Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
    if (Surface_p->EGLSurface != EGL_NO_SURFACE) {
        eglDestroySurface(Surface_p->Display, Surface_p->EGLSurface);
        Surface_p->EGLSurface = EGL_NO_SURFACE;
    }
}

static NH_API_RESULT nh_gfx_createAndroidEGLSurface(
    nh_gfx_OpenGLSurface *Surface_p, ANativeWindow *NativeWindow_p, uint32_t generation)
{
    EGLint format;
    if (!eglGetConfigAttrib(Surface_p->Display, Surface_p->Config, EGL_NATIVE_VISUAL_ID, &format) ||
        ANativeWindow_setBuffersGeometry(NativeWindow_p, 0, 0, format) != 0) {
        return NH_API_ERROR_BAD_STATE;
    }

    EGLSurface surface = eglCreateWindowSurface(Surface_p->Display, Surface_p->Config,
        NativeWindow_p, NULL);
    if (surface == EGL_NO_SURFACE) {
        return NH_API_ERROR_BAD_STATE;
    }

    Surface_p->EGLSurface = surface;
    Surface_p->WindowGeneration = generation;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_createOpenGLAndroidContext(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p)
{
    ANativeWindow *NativeWindow_p = nh_wsi_acquireAndroidNativeWindow(
        Window_p, &Surface_p->WindowGeneration);
    if (!NativeWindow_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    Surface_p->Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    Surface_p->Config = NULL;
    Surface_p->Context = EGL_NO_CONTEXT;
    Surface_p->EGLSurface = EGL_NO_SURFACE;
    if (Surface_p->Display == EGL_NO_DISPLAY) {
        nh_wsi_releaseAndroidNativeWindow(NativeWindow_p);
        return NH_API_ERROR_BAD_STATE;
    }

    EGLint major, minor;
    if (!eglInitialize(Surface_p->Display, &major, &minor)) {
        nh_wsi_releaseAndroidNativeWindow(NativeWindow_p);
        Surface_p->Display = EGL_NO_DISPLAY;
        return NH_API_ERROR_BAD_STATE;
    }
    if (!eglBindAPI(EGL_OPENGL_ES_API)) {
        nh_wsi_releaseAndroidNativeWindow(NativeWindow_p);
        nh_gfx_destroyOpenGLAndroidContext(Surface_p);
        return NH_API_ERROR_BAD_STATE;
    }

    const EGLint configAttributes[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };
    EGLint configCount = 0;
    if (!eglChooseConfig(Surface_p->Display, configAttributes, &Surface_p->Config, 1, &configCount) ||
        configCount == 0) {
        nh_wsi_releaseAndroidNativeWindow(NativeWindow_p);
        nh_gfx_destroyOpenGLAndroidContext(Surface_p);
        return NH_API_ERROR_BAD_STATE;
    }

    const EGLint contextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    Surface_p->Context = eglCreateContext(Surface_p->Display, Surface_p->Config,
        EGL_NO_CONTEXT, contextAttributes);
    if (Surface_p->Context == EGL_NO_CONTEXT) {
        nh_wsi_releaseAndroidNativeWindow(NativeWindow_p);
        nh_gfx_destroyOpenGLAndroidContext(Surface_p);
        return NH_API_ERROR_BAD_STATE;
    }

    NH_API_RESULT result = nh_gfx_createAndroidEGLSurface(
        Surface_p, NativeWindow_p, Surface_p->WindowGeneration);
    nh_wsi_releaseAndroidNativeWindow(NativeWindow_p);
    if (result != NH_API_SUCCESS) {
        nh_gfx_destroyOpenGLAndroidContext(Surface_p);
    }
    return result;
}

NH_API_RESULT nh_gfx_prepareOpenGLAndroidContext(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p, bool *ready_p)
{
    *ready_p = false;
    uint32_t generation = 0;
    ANativeWindow *NativeWindow_p = nh_wsi_acquireAndroidNativeWindow(Window_p, &generation);

    if (!NativeWindow_p) {
        nh_gfx_destroyAndroidEGLSurface(Surface_p);
        return NH_API_SUCCESS;
    }

    if (Surface_p->EGLSurface == EGL_NO_SURFACE || generation != Surface_p->WindowGeneration) {
        nh_gfx_destroyAndroidEGLSurface(Surface_p);
        NH_API_RESULT result = nh_gfx_createAndroidEGLSurface(Surface_p, NativeWindow_p, generation);
        nh_wsi_releaseAndroidNativeWindow(NativeWindow_p);
        if (result != NH_API_SUCCESS) {
            return result;
        }
    } else {
        nh_wsi_releaseAndroidNativeWindow(NativeWindow_p);
    }

    if (!eglMakeCurrent(Surface_p->Display, Surface_p->EGLSurface,
        Surface_p->EGLSurface, Surface_p->Context)) {
        return NH_API_ERROR_BAD_STATE;
    }
    *ready_p = true;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_swapOpenGLAndroidBuffers(
    nh_gfx_OpenGLSurface *Surface_p)
{
    return eglSwapBuffers(Surface_p->Display, Surface_p->EGLSurface) ?
        NH_API_SUCCESS : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_gfx_destroyOpenGLAndroidContext(
    nh_gfx_OpenGLSurface *Surface_p)
{
    if (Surface_p->Display != EGL_NO_DISPLAY) {
        nh_gfx_destroyAndroidEGLSurface(Surface_p);
        if (Surface_p->Context != EGL_NO_CONTEXT) {
            eglDestroyContext(Surface_p->Display, Surface_p->Context);
            Surface_p->Context = EGL_NO_CONTEXT;
        }
        eglTerminate(Surface_p->Display);
        Surface_p->Display = EGL_NO_DISPLAY;
    }
    Surface_p->Config = NULL;
    Surface_p->WindowGeneration = 0;
    return NH_API_SUCCESS;
}
