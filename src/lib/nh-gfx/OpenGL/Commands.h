#ifndef NH_GFX_OPENGL_COMMANDS_H
#define NH_GFX_OPENGL_COMMANDS_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "CommandBuffer.h"
#include "../Common/Includes.h"

NH_API_RESULT nh_gfx_executeOpenGLCommand(
    nh_gfx_OpenGLCommand *Command_p
);

#endif
