#ifndef NH_GFX_OPENGL_COMMAND_BUFFER_H
#define NH_GFX_OPENGL_COMMAND_BUFFER_H

#include "Data.h"
#include "API.h"
#include "../Common/Includes.h"
#include "../../nh-core/Util/List.h"

typedef struct nh_gfx_OpenGLCommand {
    NH_GFX_OPENGL_COMMAND_E type;
    nh_gfx_OpenGLData Result;
    nh_core_List Arguments;
    bool autoFree;
} nh_gfx_OpenGLCommand;

typedef struct nh_gfx_OpenGLCommandBuffer {
    nh_core_List Commands;
} nh_gfx_OpenGLCommandBuffer;

nh_gfx_OpenGLCommandBuffer nh_gfx_initOpenGLCommandBuffer(
);

nh_gfx_OpenGLCommand *nh_gfx_addOpenGLCommand(
    nh_gfx_OpenGLCommandBuffer *CommandBuffer_p, char *name_p, ... 
);

void nh_gfx_freeOpenGLCommand(
    nh_gfx_OpenGLCommand *Command_p
);

nh_gfx_OpenGLCommand *nh_gfx_disableOpenGLCommandAutoFree(
    nh_gfx_OpenGLCommand *Command_p
);

NH_API_RESULT nh_gfx_executeOpenGLCommandBuffer(
    nh_gfx_OpenGLCommandBuffer *CommandBuffer_p
);

NH_API_RESULT nh_gfx_freeOpenGLCommandBuffer(
    nh_gfx_OpenGLCommandBuffer *CommandBuffer_p
);

NH_API_RESULT nh_gfx_appendOpenGLCommandBuffer(
    nh_gfx_OpenGLCommandBuffer *CommandBuffer_p, nh_gfx_OpenGLCommandBuffer *ToBeAppended_p
);

#endif // NH_GFX_OPENGL_COMMAND_BUFFER_H
