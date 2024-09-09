#ifndef NH_OPENGL_COMMANDS_H
#define NH_OPENGL_COMMANDS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "CommandBuffer.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_GFX_RESULT nh_opengl_executeCommand(
        nh_opengl_Command *Command_p
    );

/** @} */

#endif
