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

// COMMAND BUFFER ==================================================================================

nh_opengl_CommandBuffer nh_opengl_initCommandBuffer()
{
    nh_opengl_CommandBuffer CommandBuffer;
    CommandBuffer.Commands = nh_core_initList(64);

    return CommandBuffer;
}

void nh_opengl_freeCommand(
    nh_opengl_Command *Command_p)
{
    if (!Command_p) {return ;}

    for (int i = 0; i < Command_p->Arguments.size; ++i) {
        nh_opengl_Data *Data_p = Command_p->Arguments.pp[i];
        if (Data_p->autoFree) {
            nh_opengl_freeData(Data_p);
        }
    }

    nh_core_freeList(&Command_p->Arguments, false);
    nh_core_free(Command_p);

    return;
}

nh_opengl_Command *nh_opengl_addCommand(
    nh_opengl_CommandBuffer *CommandBuffer_p, char *name_p, ...)
{
    nh_opengl_Command *Command_p = nh_core_allocate(sizeof(nh_opengl_Command));
    NH_CORE_CHECK_MEM_2(NULL, Command_p)

    NH_GFX_OPENGL_COMMAND_E type = nh_opengl_getCommandType(name_p);
    if (type == NH_GFX_OPENGL_COMMAND_UNDEFINED) {
        nh_core_free(Command_p);
        return NULL;
    }

    int arguments = nh_opengl_getCommandParameterCount(type);

    Command_p->type = type;
    Command_p->autoFree = true;
    Command_p->Arguments = nh_core_initList(arguments);
    Command_p->Result = nh_opengl_initData();

    va_list args_p;
    va_start(args_p, name_p);
    for(int i = 0; i < arguments; i++) {
        nh_opengl_Data *Data_p = va_arg(args_p, nh_opengl_Data*);
        nh_core_appendToList(&Command_p->Arguments, Data_p);
    }
    va_end(args_p);

    nh_core_appendToList(&CommandBuffer_p->Commands, Command_p);

    return Command_p;
}

nh_opengl_Command *nh_opengl_disableCommandAutoFree(
    nh_opengl_Command *Command_p)
{
    Command_p->autoFree = false;
    Command_p->Result.autoFree = false;

    return Command_p;
}

NH_API_RESULT nh_opengl_executeCommandBuffer(
    nh_opengl_CommandBuffer *CommandBuffer_p)
{
    for (int i = 0; i < CommandBuffer_p->Commands.size; ++i) {
        nh_opengl_executeCommand(CommandBuffer_p->Commands.pp[i]);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_opengl_freeCommandBuffer(
    nh_opengl_CommandBuffer *CommandBuffer_p)
{
    for (int i = 0; i < CommandBuffer_p->Commands.size; ++i) {
        if (((nh_opengl_Command*)CommandBuffer_p->Commands.pp[i])->autoFree) {
            nh_opengl_freeCommand(CommandBuffer_p->Commands.pp[i]);
        }
    }

    nh_core_freeList(&CommandBuffer_p->Commands, false);
    *CommandBuffer_p = nh_opengl_initCommandBuffer();

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_opengl_appendCommandBuffer(
    nh_opengl_CommandBuffer *CommandBuffer_p, nh_opengl_CommandBuffer *ToBeAppended_p)
{
    for (int i = 0; i < ToBeAppended_p->Commands.size; ++i) {
        nh_core_appendToList(&CommandBuffer_p->Commands, ToBeAppended_p->Commands.pp[i]);
    }

    return NH_API_SUCCESS;
}

