#ifndef NH_OPENGL_RENDER_IOS_H
#define NH_OPENGL_RENDER_IOS_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Surface.h"
#include "../Base/Surface.h"
#include "../Common/Includes.h"

int nh_gfx_makeOpenGLContextCurrentIOS(
    void* Context_p
);

void nh_gfx_flushOpenGLDrawableIOS(
    void* Context_p
);

#endif
