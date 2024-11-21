// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "CommandBuffer.h"
#include "Commands.h"
#include "API.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// FUNCTIONS =======================================================================================

nh_gfx_OpenGLCommandBuffer nh_gfx_initOpenGLCommandBuffer()
{
    nh_gfx_OpenGLCommandBuffer CommandBuffer;
    CommandBuffer.Commands = nh_core_initList(64);

    return CommandBuffer;
}

void nh_gfx_freeOpenGLCommand(
    nh_gfx_OpenGLCommand *Command_p)
{
    if (!Command_p) {return ;}

    for (int i = 0; i < Command_p->Arguments.size; ++i) {
        nh_gfx_OpenGLData *Data_p = Command_p->Arguments.pp[i];
        if (Data_p->autoFree) {
            nh_gfx_freeOpenGLData(Data_p);
        }
    }

    nh_core_freeList(&Command_p->Arguments, false);
    nh_core_free(Command_p);

    return;
}

nh_gfx_OpenGLCommand *nh_gfx_addOpenGLCommand(
    nh_gfx_OpenGLCommandBuffer *CommandBuffer_p, char *name_p, ...)
{
    nh_gfx_OpenGLCommand *Command_p = nh_core_allocate(sizeof(nh_gfx_OpenGLCommand));
    NH_CORE_CHECK_MEM_2(NULL, Command_p)

    NH_GFX_OPENGL_COMMAND_E type = nh_gfx_getOpenGLCommandType(name_p);
    if (type == NH_GFX_OPENGL_COMMAND_UNDEFINED) {
        nh_core_free(Command_p);
        return NULL;
    }

    int arguments = nh_gfx_getOpenGLCommandParameterCount(type);

    Command_p->type = type;
    Command_p->autoFree = true;
    Command_p->Arguments = nh_core_initList(arguments);
    Command_p->Result = nh_gfx_initOpenGLData();

    va_list args_p;
    va_start(args_p, name_p);
    for(int i = 0; i < arguments; i++) {
        nh_gfx_OpenGLData *Data_p = va_arg(args_p, nh_gfx_OpenGLData*);
        nh_core_appendToList(&Command_p->Arguments, Data_p);
    }
    va_end(args_p);

    nh_core_appendToList(&CommandBuffer_p->Commands, Command_p);

    return Command_p;
}

nh_gfx_OpenGLCommand *nh_gfx_disableOpenGLCommandAutoFree(
    nh_gfx_OpenGLCommand *Command_p)
{
    Command_p->autoFree = false;
    Command_p->Result.autoFree = false;

    return Command_p;
}

NH_API_RESULT nh_gfx_executeOpenGLCommandBuffer(
    nh_gfx_OpenGLCommandBuffer *CommandBuffer_p)
{
    for (int i = 0; i < CommandBuffer_p->Commands.size; ++i) {
        nh_gfx_executeOpenGLCommand(CommandBuffer_p->Commands.pp[i]);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_freeOpenGLCommandBuffer(
    nh_gfx_OpenGLCommandBuffer *CommandBuffer_p)
{
    for (int i = 0; i < CommandBuffer_p->Commands.size; ++i) {
        if (((nh_gfx_OpenGLCommand*)CommandBuffer_p->Commands.pp[i])->autoFree) {
            nh_gfx_freeOpenGLCommand(CommandBuffer_p->Commands.pp[i]);
        }
    }

    nh_core_freeList(&CommandBuffer_p->Commands, false);
    *CommandBuffer_p = nh_gfx_initOpenGLCommandBuffer();

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_appendOpenGLCommandBuffer(
    nh_gfx_OpenGLCommandBuffer *CommandBuffer_p, nh_gfx_OpenGLCommandBuffer *ToBeAppended_p)
{
    for (int i = 0; i < ToBeAppended_p->Commands.size; ++i) {
        nh_core_appendToList(&CommandBuffer_p->Commands, ToBeAppended_p->Commands.pp[i]);
    }

    return NH_API_SUCCESS;
}
