#ifndef NH_OPENGL_COMMAND_BUFFER_H
#define NH_OPENGL_COMMAND_BUFFER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Data.h"
#include "API.h"
#include "../Common/Includes.h"
#include "../../nh-core/Util/List.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_opengl_Command {
        NH_GFX_OPENGL_COMMAND_E type;
        nh_opengl_Data Result;
	nh_core_List Arguments;
        bool autoFree;
    } nh_opengl_Command;
    
    typedef struct nh_opengl_CommandBuffer {
        nh_core_List Commands;
    } nh_opengl_CommandBuffer;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    nh_opengl_CommandBuffer nh_opengl_initCommandBuffer(
    );

    nh_opengl_Command *nh_opengl_addCommand(
        nh_opengl_CommandBuffer *CommandBuffer_p, char *name_p, ... 
    );

    void nh_opengl_freeCommand(
        nh_opengl_Command *Command_p
    );

    nh_opengl_Command *nh_opengl_disableCommandAutoFree(
        nh_opengl_Command *Command_p
    );

    NH_API_RESULT nh_opengl_executeCommandBuffer(
        nh_opengl_CommandBuffer *CommandBuffer_p
    );

    NH_API_RESULT nh_opengl_freeCommandBuffer(
        nh_opengl_CommandBuffer *CommandBuffer_p
    );

    NH_API_RESULT nh_opengl_appendCommandBuffer(
        nh_opengl_CommandBuffer *CommandBuffer_p, nh_opengl_CommandBuffer *ToBeAppended_p
    );

/** @} */

#endif
