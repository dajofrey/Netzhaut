// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "ContextX11.h"
#include "Surface.h"

#include "../../nh-core/System/Thread.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-wsi/Window/Window.h"
#include "../../nh-wsi/Platforms/X11/Init.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <X11/extensions/Xrender.h>

// FUNCTIONS =======================================================================================

/**
 * GLX_DRAWABLE_TYPE is probably a hack, i had serious perfomance issues with just the GLX_WINDOW_BIT
 * after resizing the window.
 */
static int VISUAL_DATA_P[] = {
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT | GLX_PIXMAP_BIT | GLX_PBUFFER_BIT,
    GLX_DOUBLEBUFFER, True,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 16,
    None
};

typedef GLXContext (*glXCreateContextAttribsARBProc)
    (Display*, GLXFBConfig, GLXContext, Bool, const int*);

NH_API_RESULT nh_gfx_createOpenGLX11Context(
    nh_gfx_OpenGLSurface *Surface_p, nh_api_Window *Window_p)
{
    int count = 0;
    GLXFBConfig *FrameBufferConfigurations_p = 
        glXChooseFBConfig(NH_WSI_X11.Display_p, NH_WSI_X11.screen, VISUAL_DATA_P, &count);

    GLXFBConfig FrameBufferConfiguration_p = NULL; 

    for (int i = 0; i < count; i++) {
        XVisualInfo *Info_p = (XVisualInfo*) 
            glXGetVisualFromFBConfig(NH_WSI_X11.Display_p, FrameBufferConfigurations_p[i]);
        // We need to use the FBC that relates to the Visual that was used for window creation.
        if (Info_p->visualid == ((nh_wsi_Window*)Window_p)->X11.Info.visualid) {
            XFree(Info_p);
            FrameBufferConfiguration_p = FrameBufferConfigurations_p[i];
            break;
        }
        XFree(Info_p);
    }

    if (!FrameBufferConfiguration_p) {
        return NH_API_ERROR_BAD_STATE;
    }
 
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB_f = 0;
    glXCreateContextAttribsARB_f = (glXCreateContextAttribsARBProc)
        glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

    if (!glXCreateContextAttribsARB_f) {
        return NH_API_ERROR_BAD_STATE;
    }

    /* Set desired minimum OpenGL version */
    static int contextAttributes_p[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 6,
        None
    };

    /* Create modern OpenGL context */
    Surface_p->Context_p = glXCreateContextAttribsARB_f(
        NH_WSI_X11.Display_p, FrameBufferConfiguration_p, NULL, true, contextAttributes_p);

    if (!Surface_p->Context_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    XFree(FrameBufferConfigurations_p);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_destroyOpenGLX11Context(
    nh_gfx_OpenGLSurface *Surface_p)
{
    glXDestroyContext(NH_WSI_X11.Display_p, Surface_p->Context_p);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_createOpenGLX11ContextRequirements(
    nh_gfx_OpenGLSurfaceRequirements *Requirements_p)
{
    Requirements_p->Ids = nh_core_initArray(sizeof(VisualID), 8);

    Display *Display_p = XOpenDisplay(NULL);

    int count = 0;
    GLXFBConfig *FrameBufferConfigurations_p = 
        glXChooseFBConfig(Display_p, DefaultScreen(Display_p), VISUAL_DATA_P, &count);

    for (int i = 0; i < count; i++) {
        XVisualInfo *Visual_p = 
            (XVisualInfo*) glXGetVisualFromFBConfig(Display_p, FrameBufferConfigurations_p[i]);
        if (!Visual_p) {continue;}

        XRenderPictFormat *Format_p = XRenderFindVisualFormat(Display_p, Visual_p->visual);
        if (!Format_p) {
            XFree(Visual_p);
            continue;
        }

        // We need a framebuffer with alpha capabilities.
        if (Format_p->direct.alphaMask > 0) {
            nh_core_appendToArray(&Requirements_p->Ids, &Visual_p->visualid, 1);
        }

        XFree(Visual_p);
    }

    XFree(FrameBufferConfigurations_p);
    XCloseDisplay(Display_p);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_freeOpenGLX11ContextRequirements(
    nh_gfx_OpenGLSurfaceRequirements *Requirements_p)
{
    nh_core_freeArray(&Requirements_p->Ids);

    return NH_API_SUCCESS;
}
