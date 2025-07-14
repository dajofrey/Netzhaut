#ifndef NH_GFX_OPENGL_CONTEXT_COCOA_H
#define NH_GFX_OPENGL_CONTEXT_COCOA_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Surface.h"

#include "../Common/Includes.h"
#include "../../nh-core/Util/Array.h"
#include "../../nh-wsi/Window/Window.h"

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_createOpenGLCocoaContext(
    nh_gfx_OpenGLSurface *Surface_p, nh_wsi_Window *Window_p
);

NH_API_RESULT nh_gfx_destroyOpenGLCocoaContext(
    nh_gfx_OpenGLSurface *Surface_p
);

#endif
