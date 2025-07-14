#ifndef NH_GFX_OPENGL_CONTEXT_X11_H
#define NH_GFX_OPENGL_CONTEXT_X11_H

#include "Surface.h"

#include "../Base/SurfaceRequirements.h"
#include "../Common/Includes.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-wsi/Window/Window.h"

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_createOpenGLX11Context(
    nh_gfx_OpenGLSurface *Surface_p, nh_api_Window *Window_p
);

NH_API_RESULT nh_gfx_destroyOpenGLX11Context(
    nh_gfx_OpenGLSurface *Surface_p
);

NH_API_RESULT nh_gfx_createOpenGLX11ContextRequirements(
    nh_gfx_OpenGLSurfaceRequirements *Requirements_p
);

NH_API_RESULT nh_gfx_freeOpenGLX11ContextRequirements(
    nh_gfx_OpenGLSurfaceRequirements *Requirements_p
);

#endif
