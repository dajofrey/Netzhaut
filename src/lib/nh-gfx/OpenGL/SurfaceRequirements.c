// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "SurfaceRequirements.h"

#include "../Base/Surface.h"
#include "../Common/Macros.h"

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

// REQUIREMENTS ====================================================================================

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

NH_API_RESULT nh_opengl_createSurfaceRequirements(
    nh_opengl_SurfaceRequirements *Requirements_p)
{
NH_GFX_BEGIN()

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

NH_GFX_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_opengl_freeSurfaceRequirements(
    nh_opengl_SurfaceRequirements *Requirements_p)
{
NH_GFX_BEGIN()

    nh_core_freeArray(&Requirements_p->Ids);

NH_GFX_END(NH_API_SUCCESS)
}

