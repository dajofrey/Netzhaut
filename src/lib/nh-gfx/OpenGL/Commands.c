// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Commands.h"
#include "API.h"

#include "../Common/Log.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// COMMANDS ========================================================================================

// nh_gfx_glAccum

static NH_API_RESULT nh_gfx_glActiveTexture(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glActiveTexture(texture: %d)\n",*((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    glActiveTexture(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glAddSwapHintRectWIN
// nh_gfx_glAlphaFunc
// nh_gfx_glAreTexturesResident
// nh_gfx_glArrayElement

static NH_API_RESULT nh_gfx_glAttachShader(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glAttachShader(program: %d, shader: %d)\n",
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));

    glAttachShader(
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glBegin
// nh_gfx_glEnd

static NH_API_RESULT nh_gfx_glBindBuffer(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glBindBuffer(target: %d, buffer: %d)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));

    glBindBuffer(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_glBindTexture(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glBindTexture(target: %d, texture: %d)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));

    glBindTexture(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_glBindVertexArray(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glBindVertexArray(array: %d)\n",
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    glBindVertexArray(
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glBitmap

static NH_API_RESULT nh_gfx_glBlendFunc(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glBlendFunc(sfactor: %d, dfactor: %d)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));

    glBlendFunc(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_glBufferData(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glBufferData(target: %d, size: %d, data: %p, usage: %d)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
       *(GLsizeiptr*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p,
      (const void*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p,
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));

    glBufferData(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
       *(GLsizeiptr*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p,
      (const void*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p,
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glCallList
// nh_gfx_glCallLists

static NH_API_RESULT nh_gfx_glClear(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glClear(mask: %d)\n",
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    glClear(
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glClearAccum

static NH_API_RESULT nh_gfx_glClearColor(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glClearColor(red: %f, green: %f, blue: %f, alpha: %f)\n",
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));

    glClearColor(
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glClearDepth
// nh_gfx_glClearIndex
// nh_gfx_glClearStencil
// nh_gfx_glClipPlane
// nh_gfx_glColor3b
// nh_gfx_glColor3d
// nh_gfx_glColor3f
// nh_gfx_glColor3i
// nh_gfx_glColor3s
// nh_gfx_glColor3ub
// nh_gfx_glColor3ui
// nh_gfx_glColor3us
// nh_gfx_glColor4b
// nh_gfx_glColor4d
// nh_gfx_glColor4f
// nh_gfx_glColor4i
// nh_gfx_glColor4s
// nh_gfx_glColor4ub
// nh_gfx_glColor4ui
// nh_gfx_glColor4us
// nh_gfx_glColor3bv
// nh_gfx_glColor3dv
// nh_gfx_glColor3fv
// nh_gfx_glColor3iv
// nh_gfx_glColor3sv
// nh_gfx_glColor3ubv
// nh_gfx_glColor3uiv
// nh_gfx_glColor3usv
// nh_gfx_glColor4bv
// nh_gfx_glColor4dv
// nh_gfx_glColor4fv
// nh_gfx_glColor4iv
// nh_gfx_glColor4sv
// nh_gfx_glColor4ubv
// nh_gfx_glColor4uiv
// nh_gfx_glColor4usv

static NH_API_RESULT nh_gfx_glColorMask(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glColorMask(red: %d, green: %d, blue: %d, alpha: %d)\n",
        *((GLboolean*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLboolean*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLboolean*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLboolean*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));
 
     glColorMask(
        *((GLboolean*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLboolean*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLboolean*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLboolean*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glColorMaterial
// nh_gfx_glColorPointer
// nh_gfx_glColorTableEXT
// nh_gfx_glColorSubTableEXT

static NH_API_RESULT nh_gfx_glCompileShader(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glCompileShader(shader: %d)\n",
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    glCompileShader(
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glCopyPixels
// nh_gfx_glCopyTexImage1D
// nh_gfx_glCopyTexImage2D
// nh_gfx_glCopyTexSubImage1D
// nh_gfx_glCopyTexSubImage2D

static NH_API_RESULT nh_gfx_glCreateProgram(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_gluint(&Command_p->Result, glCreateProgram());
    nh_gfx_logOpenGLCommand("glCreateProgram(void) -> %d\n", *((GLuint*)Command_p->Result.p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_glCreateShader(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_gluint(&Command_p->Result, 
        glCreateShader(
            *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p)));

    nh_gfx_logOpenGLCommand("glCreateShader(shaderType: %d) -> %d\n", 
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)Command_p->Result.p));

    return NH_API_SUCCESS;
}

// nh_gfx_glCullFace
// nh_gfx_glDeleteLists
// nh_gfx_glDeleteTextures

static NH_API_RESULT nh_gfx_glDepthFunc(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glDepthFunc(func: %d)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));
 
    glDepthFunc(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glDepthMask
// nh_gfx_glDepthRange

static NH_API_RESULT nh_gfx_glDrawArrays(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glDrawArrays(mode: %d, first: %d, count: %d)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p));

    glDrawArrays(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glDrawBuffer

static NH_API_RESULT nh_gfx_glDrawElements(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glDrawElements(mode: %d, count: %d, type: %d, indices: %p)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        (const void*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p);

    glDrawElements(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        (const void*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p);

    return NH_API_SUCCESS;
}

// nh_gfx_glDrawPixels
// nh_gfx_glEdgeFlag
// nh_gfx_glEdgeFlagv
// nh_gfx_glEdgeFlagPointer

static NH_API_RESULT nh_gfx_glEnable(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glEnable(cap: %d)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));
 
    glEnable(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_glEnableVertexAttribArray(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glEnableVertexAttribArray(index: %d)\n",
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));
 
    glEnableVertexAttribArray(
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_glDisable(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glDisable(cap: %d)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));
 
    glDisable(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glEnableClientState
// nh_gfx_glDisableClientState
// nh_gfx_glEvalCoord1d
// nh_gfx_glEvalCoord1f
// nh_gfx_glEvalCoord2d
// nh_gfx_glEvalCoord2f
// nh_gfx_glEvalCoord1dv
// nh_gfx_glEvalCoord2dv
// nh_gfx_glEvalCoord1fv
// nh_gfx_glEvalCoord2fv
// nh_gfx_glEvalMesh1
// nh_gfx_glEvalMesh2
// nh_gfx_glEvalPoint1
// nh_gfx_glEvalPoint2
// nh_gfx_glFeedbackBuffer
// nh_gfx_glFinish
// nh_gfx_glFlush
// nh_gfx_glFogf
// nh_gfx_glFogi
// nh_gfx_glFogfv
// nh_gfx_glFogiv
// nh_gfx_glFrontFace
// nh_gfx_glFrustum

static NH_API_RESULT nh_gfx_glGenBuffers(
    nh_gfx_OpenGLCommand *Command_p)
{
    glGenBuffers(
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p);

    char b[255] = {};
    memset(b, 0, 255);
     
    sprintf(b, "glGenBuffers(n: %d, buffers: %p) -> (",
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p);
     
    for (int i = 0; i < *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p); ++i) {
        if (i) {sprintf(b+strlen(b), ", ");}
        sprintf(b+strlen(b), "%d", ((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p)[i]);
    }
    sprintf(b+strlen(b), ")\n");
    nh_gfx_logOpenGLCommand(b);

    return NH_API_SUCCESS;
}

// nh_gfx_glGenLists

static NH_API_RESULT nh_gfx_glGenTextures(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glGenTextures(n: %d, textures: %p)\n",
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p);
 
    glGenTextures(
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_glGenVertexArrays(
    nh_gfx_OpenGLCommand *Command_p)
{
    glGenVertexArrays(
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p);

    char b[255] = {};
    memset(b, 0, 255);
     
    sprintf(b, "glGenVertexArrays(n: %d, arrays: %p) -> (",
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p);
     
    for (int i = 0; i < *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p); ++i) {
        if (i) {sprintf(b+strlen(b), ", ");}
        sprintf(b+strlen(b), "%d", ((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p)[i]);
    }

    sprintf(b+strlen(b), ")\n");
    nh_gfx_logOpenGLCommand(b);

    return NH_API_SUCCESS;
}

// nh_gfx_glGetBooleanv
// nh_gfx_glGetClipPlane
// nh_gfx_glGetColorTableEXT
// nh_gfx_glGetColorTableParameterfvEXT
// nh_gfx_glGetColorTableParameterivEXT
// nh_gfx_glGetDoublev
// nh_gfx_glGetError
// nh_gfx_glGetFloatv
// nh_gfx_glGetIntegerv
// nh_gfx_glGetLightfv
// nh_gfx_glGetLightiv
// nh_gfx_glGetMapdv
// nh_gfx_glGetMapfv
// nh_gfx_glGetMapiv
// nh_gfx_glGetMaterialfv
// nh_gfx_glGetMaterialiv
// nh_gfx_glGetPixelMapfv
// nh_gfx_glGetPixelMapuiv
// nh_gfx_glGetPixelMapusv
// nh_gfx_glGetPointerv
// nh_gfx_glGetPolygonStipple
// nh_gfx_glGetString
// nh_gfx_glGetTexEnvfv
// nh_gfx_glGetTexEnviv
// nh_gfx_glGetTexGendv
// nh_gfx_glGetTexGenfv
// nh_gfx_glGetTexGeniv
// nh_gfx_glGetTexImage
// nh_gfx_glGetTexLevelParameterfv
// nh_gfx_glGetTexLevelParameteriv
// nh_gfx_glGetTexParameterfv
// nh_gfx_glGetTexParameteriv

static NH_API_RESULT nh_gfx_glGetUniformLocation(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glGetUniformLocation(program: %d, name: %s)\n",
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((const GLchar**)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->pp));
 
    nh_gfx_glint(&Command_p->Result,
        glGetUniformLocation(
            *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
            *((const GLchar**)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->pp)));

    return NH_API_SUCCESS;
}

// nh_gfx_glHint
// nh_gfx_glIndexd
// nh_gfx_glIndexf
// nh_gfx_glIndexi
// nh_gfx_glIndexs
// nh_gfx_glIndexdv
// nh_gfx_glIndexfv
// nh_gfx_glIndexiv
// nh_gfx_glIndexsv
// nh_gfx_glIndexMask
// nh_gfx_glIndexPointer
// nh_gfx_glInitNames
// nh_gfx_glInterleavedArrays
// nh_gfx_glIsEnabled
// nh_gfx_glIsList
// nh_gfx_glIsTexture
// nh_gfx_glLightf
// nh_gfx_glLighti
// nh_gfx_glLightfv
// nh_gfx_glLightiv
// nh_gfx_glLightModelf
// nh_gfx_glLightModelfv
// nh_gfx_glLightModeli
// nh_gfx_glLightModeliv
// nh_gfx_glLineStipple
// nh_gfx_glLineWidth

static NH_API_RESULT nh_gfx_glLinkProgram(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glLinkProgram(program: %d)\n",
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));
 
    glLinkProgram(
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glListBase
// nh_gfx_glLoadIdentity
// nh_gfx_glLoadMatrixd
// nh_gfx_glLoadMatrixf
// nh_gfx_glLoadName
// nh_gfx_glLogicOp
// nh_gfx_glMap1d
// nh_gfx_glMap1f
// nh_gfx_glMap2d
// nh_gfx_glMap2f
// nh_gfx_glMapGrid1d
// nh_gfx_glMapGrid1f
// nh_gfx_glMapGrid2d
// nh_gfx_glMapGrid2f
// nh_gfx_glMaterialf
// nh_gfx_glMateriali
// nh_gfx_glMaterialfv
// nh_gfx_glMaterialiv
// nh_gfx_glMatrixMode
// nh_gfx_glMultMatrixd
// nh_gfx_glMultMatrixf
// nh_gfx_glNewList
// nh_gfx_glEndList
// nh_gfx_glNormal3b
// nh_gfx_glNormal3d
// nh_gfx_glNormal3f
// nh_gfx_glNormal3i
// nh_gfx_glNormal3s
// nh_gfx_glNormal3bv
// nh_gfx_glNormal3dv
// nh_gfx_glNormal3fv
// nh_gfx_glNormal3iv
// nh_gfx_glNormal3sv
// nh_gfx_glNormalPointer
// nh_gfx_glOrtho
// nh_gfx_glPassThrough
// nh_gfx_glPixelMapfv
// nh_gfx_glPixelMapuiv
// nh_gfx_glPixelMapusv
// nh_gfx_glPixelStoref
// nh_gfx_glPixelStorei
// nh_gfx_glPixelTransferf
// nh_gfx_glPixelTransferi
// nh_gfx_glPixelZoom
// nh_gfx_glPointSize
// nh_gfx_glPolygonMode
// nh_gfx_glPolygonOffset
// nh_gfx_glPolygonStipple
// nh_gfx_glPrioritizeTextures
// nh_gfx_glPushAttrib
// nh_gfx_glPopAttrib
// nh_gfx_glPushClientAttrib
// nh_gfx_glPopClientAttrib
// nh_gfx_glPushMatrix
// nh_gfx_glPopMatrix
// nh_gfx_glPushName
// nh_gfx_glPopName
// nh_gfx_glRasterPos2d
// nh_gfx_glRasterPos2f
// nh_gfx_glRasterPos2i
// nh_gfx_glRasterPos2s
// nh_gfx_glRasterPos3d
// nh_gfx_glRasterPos3f
// nh_gfx_glRasterPos3i
// nh_gfx_glRasterPos3s
// nh_gfx_glRasterPos4d
// nh_gfx_glRasterPos4f
// nh_gfx_glRasterPos4i
// nh_gfx_glRasterPos4s
// nh_gfx_glRasterPos2dv
// nh_gfx_glRasterPos2fv
// nh_gfx_glRasterPos2iv
// nh_gfx_glRasterPos2sv
// nh_gfx_glRasterPos3dv
// nh_gfx_glRasterPos3fv
// nh_gfx_glRasterPos3iv
// nh_gfx_glRasterPos3sv
// nh_gfx_glRasterPos4dv
// nh_gfx_glRasterPos4fv
// nh_gfx_glRasterPos4iv
// nh_gfx_glRasterPos4sv
// nh_gfx_glReadBuffer
// nh_gfx_glReadPixels
// nh_gfx_glRectd
// nh_gfx_glRectf
// nh_gfx_glRecti
// nh_gfx_glRects
// nh_gfx_glRectdv
// nh_gfx_glRectfv
// nh_gfx_glRectiv
// nh_gfx_glRectsv
// nh_gfx_glRenderMode
// nh_gfx_glRotated
// nh_gfx_glRotatef
// nh_gfx_glScaled
// nh_gfx_glScalef

static NH_API_RESULT nh_gfx_glScissor(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glScissor(x: %d, y: %d, width: %d, height: %d)\n",
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));
 
    glScissor(
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glSelectBuffer
// nh_gfx_glShadeModel

static NH_API_RESULT nh_gfx_glShaderSource(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glShaderSource(shader: %d, count: %d, string: %p, length: %d)\n",
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
          (const GLchar**)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->pp,
          (const GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p);
 
    glShaderSource(
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
          (const GLchar**)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->pp,
          (const GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p);

    return NH_API_SUCCESS;
}

// nh_gfx_glStencilFunc
// nh_gfx_glStencilMask
// nh_gfx_glStencilOp
// nh_gfx_glTexCoord1d
// nh_gfx_glTexCoord1f
// nh_gfx_glTexCoord1i
// nh_gfx_glTexCoord1s
// nh_gfx_glTexCoord2d
// nh_gfx_glTexCoord2i
// nh_gfx_glTexCoord2s
// nh_gfx_glTexCoord3d
// nh_gfx_glTexCoord3f
// nh_gfx_glTexCoord3i
// nh_gfx_glTexCoord3s
// nh_gfx_glTexCoord4d
// nh_gfx_glTexCoord4f
// nh_gfx_glTexCoord4i
// nh_gfx_glTexCoord4s
// nh_gfx_glTexCoord1dv
// nh_gfx_glTexCoord1fv
// nh_gfx_glTexCoord1iv
// nh_gfx_glTexCoord1sv
// nh_gfx_glTexCoord2dv
// nh_gfx_glTexCoord2fv
// nh_gfx_glTexCoord2iv
// nh_gfx_glTexCoord2sv
// nh_gfx_glTexCoord3dv
// nh_gfx_glTexCoord3fv
// nh_gfx_glTexCoord3iv
// nh_gfx_glTexCoord3sv
// nh_gfx_glTexCoord4dv
// nh_gfx_glTexCoord4fv
// nh_gfx_glTexCoord4iv
// nh_gfx_glTexCoord4sv
// nh_gfx_glTexCoordPointer
// nh_gfx_glTexEnvf
// nh_gfx_glTexEnvi
// nh_gfx_glTexEnvfv
// nh_gfx_glTexEnviv
// nh_gfx_glTexGend
// nh_gfx_glTexGenf
// nh_gfx_glTexGeni
// nh_gfx_glTexGendv
// nh_gfx_glTexGenfv
// nh_gfx_glTexGeniv
// nh_gfx_glTexImage1D

static NH_API_RESULT nh_gfx_glTexImage2D(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glTexImage2D(target: %d, level: %d, internalformat: %d, width: %d, height: %d, border: %d, format: %d, type: %d, data: %p)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[4])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[5])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[6])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[7])->p),
        *((const void**)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[8])->pp));
 
    glTexImage2D(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[4])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[5])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[6])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[7])->p),
        *((const void**)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[8])->pp));

    return NH_API_SUCCESS;
}

// nh_gfx_glTexParameterf

static NH_API_RESULT nh_gfx_glTexParameteri(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glTexParameteri(target: %d, pname: %d, param: %d)\n",
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p));
 
    glTexParameteri(
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glTexParameterfv
// nh_gfx_glTexParameteriv
// nh_gfx_glTexSubImage1D
// nh_gfx_glTexSubImage2D
// nh_gfx_glTranslated
// nh_gfx_glTranslatef

static NH_API_RESULT nh_gfx_glUniform1i(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glUniform1i(location: %d, v0: %d)\n",
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));
 
    glUniform1i(
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_glUniform3f(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glUniform3f(location: %d, v0: %f, v1: %f, v2: %f)\n",
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));
 
    glUniform3f(
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLfloat*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_glUseProgram(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glUseProgram(program: %d)\n",
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));
 
    glUseProgram(
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_gfx_glVertex2d
// nh_gfx_glVertex2f
// nh_gfx_glVertex2i
// nh_gfx_glVertex2s
// nh_gfx_glVertex3d
// nh_gfx_glVertex3f
// nh_gfx_glVertex3i
// nh_gfx_glVertex3s
// nh_gfx_glVertex4d
// nh_gfx_glVertex4f
// nh_gfx_glVertex4i
// nh_gfx_glVertex4s
// nh_gfx_glVertex2dv
// nh_gfx_glVertex2fv
// nh_gfx_glVertex2iv
// nh_gfx_glVertex2sv
// nh_gfx_glVertex3dv
// nh_gfx_glVertex3fv
// nh_gfx_glVertex3iv
// nh_gfx_glVertex3sv
// nh_gfx_glVertex4dv
// nh_gfx_glVertex4fv
// nh_gfx_glVertex4iv
// nh_gfx_glVertex4sv

static NH_API_RESULT nh_gfx_glVertexAttribPointer(
    nh_gfx_OpenGLCommand *Command_p)
{
    void *p = NULL;
    if ((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[5])->p) {
        p = (void*)*((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[5])->p);
    }

    glVertexAttribPointer(
        *((GLuint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
         *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLenum*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
     *((GLboolean*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p),
       *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[4])->p), p);

    return NH_API_SUCCESS;
}

// nh_gfx_glVertexPointer

static NH_API_RESULT nh_gfx_glViewport(
    nh_gfx_OpenGLCommand *Command_p)
{
    nh_gfx_logOpenGLCommand("glViewport(x: %d, y: %d, width: %d, height: %d)\n",
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));
 
    glViewport(
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_gfx_OpenGLData*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// COMMANDS LIST ===================================================================================

typedef NH_API_RESULT (*nh_gfx_command_f)(
    nh_gfx_OpenGLCommand *Command_p    
);

nh_gfx_command_f commands_p[] = {
    NULL, // nh_gfx_glAccum,
    nh_gfx_glActiveTexture,
    NULL, // nh_gfx_glAddSwapHintRectWIN,
    NULL, // nh_gfx_glAlphaFunc,
    NULL, // nh_gfx_glAreTexturesResident,
    NULL, // nh_gfx_glArrayElement,
    nh_gfx_glAttachShader,
    NULL, // nh_gfx_glBegin,
    NULL, // nh_gfx_glEnd,
    nh_gfx_glBindBuffer,
    nh_gfx_glBindTexture,
    nh_gfx_glBindVertexArray,
    NULL, // nh_gfx_glBitmap,
    nh_gfx_glBlendFunc,
    nh_gfx_glBufferData,
    NULL, // nh_gfx_glCallList,
    NULL, // nh_gfx_glCallLists,
    nh_gfx_glClear,
    NULL, // nh_gfx_glClearAccum,
    nh_gfx_glClearColor,
    NULL, // nh_gfx_glClearDepth,
    NULL, // nh_gfx_glClearIndex,
    NULL, // nh_gfx_glClearStencil,
    NULL, // nh_gfx_glClipPlane,
    NULL, // nh_gfx_glColor3b,
    NULL, // nh_gfx_glColor3d,
    NULL, // nh_gfx_glColor3f,
    NULL, // nh_gfx_glColor3i,
    NULL, // nh_gfx_glColor3s,
    NULL, // nh_gfx_glColor3ub,
    NULL, // nh_gfx_glColor3ui,
    NULL, // nh_gfx_glColor3us,
    NULL, // nh_gfx_glColor4b,
    NULL, // nh_gfx_glColor4d,
    NULL, // nh_gfx_glColor4f,
    NULL, // nh_gfx_glColor4i,
    NULL, // nh_gfx_glColor4s,
    NULL, // nh_gfx_glColor4ub,
    NULL, // nh_gfx_glColor4ui,
    NULL, // nh_gfx_glColor4us,
    NULL, // nh_gfx_glColor3bv,
    NULL, // nh_gfx_glColor3dv,
    NULL, // nh_gfx_glColor3fv,
    NULL, // nh_gfx_glColor3iv,
    NULL, // nh_gfx_glColor3sv,
    NULL, // nh_gfx_glColor3ubv,
    NULL, // nh_gfx_glColor3uiv,
    NULL, // nh_gfx_glColor3usv,
    NULL, // nh_gfx_glColor4bv,
    NULL, // nh_gfx_glColor4dv,
    NULL, // nh_gfx_glColor4fv,
    NULL, // nh_gfx_glColor4iv,
    NULL, // nh_gfx_glColor4sv,
    NULL, // nh_gfx_glColor4ubv,
    NULL, // nh_gfx_glColor4uiv,
    NULL, // nh_gfx_glColor4usv,
    nh_gfx_glColorMask,
    NULL, // nh_gfx_glColorMaterial,
    NULL, // nh_gfx_glColorPointer,
    NULL, // nh_gfx_glColorTableEXT,
    NULL, // nh_gfx_glColorSubTableEXT,
    nh_gfx_glCompileShader,
    NULL, // nh_gfx_glCopyPixels,
    NULL, // nh_gfx_glCopyTexImage1D,
    NULL, // nh_gfx_glCopyTexImage2D,
    NULL, // nh_gfx_glCopyTexSubImage1D,
    NULL, // nh_gfx_glCopyTexSubImage2D,
    nh_gfx_glCreateProgram,
    nh_gfx_glCreateShader,
    NULL, // nh_gfx_glCullFace,
    NULL, // nh_gfx_glDeleteLists,
    NULL, // nh_gfx_glDeleteTextures,
    nh_gfx_glDepthFunc,
    NULL, // nh_gfx_glDepthMask,
    NULL, // nh_gfx_glDepthRange,
    nh_gfx_glDrawArrays,
    NULL, // nh_gfx_glDrawBuffer,
    nh_gfx_glDrawElements,
    NULL, // nh_gfx_glDrawPixels,
    NULL, // nh_gfx_glEdgeFlag,
    NULL, // nh_gfx_glEdgeFlagv,
    NULL, // nh_gfx_glEdgeFlagPointer,
    nh_gfx_glEnable,
    nh_gfx_glEnableVertexAttribArray,
    nh_gfx_glDisable,
    NULL, // nh_gfx_glEnableClientState,
    NULL, // nh_gfx_glDisableClientState,
    NULL, // nh_gfx_glEvalCoord1d,
    NULL, // nh_gfx_glEvalCoord1f,
    NULL, // nh_gfx_glEvalCoord2d,
    NULL, // nh_gfx_glEvalCoord2f,
    NULL, // nh_gfx_glEvalCoord1dv,
    NULL, // nh_gfx_glEvalCoord2dv,
    NULL, // nh_gfx_glEvalCoord1fv,
    NULL, // nh_gfx_glEvalCoord2fv,
    NULL, // nh_gfx_glEvalMesh1,
    NULL, // nh_gfx_glEvalMesh2,
    NULL, // nh_gfx_glEvalPoint1,
    NULL, // nh_gfx_glEvalPoint2,
    NULL, // nh_gfx_glFeedbackBuffer,
    NULL, // nh_gfx_glFinish,
    NULL, // nh_gfx_glFlush,
    NULL, // nh_gfx_glFogf,
    NULL, // nh_gfx_glFogi,
    NULL, // nh_gfx_glFogfv,
    NULL, // nh_gfx_glFogiv,
    NULL, // nh_gfx_glFrontFace,
    NULL, // nh_gfx_glFrustum,
    nh_gfx_glGenBuffers,
    NULL, // nh_gfx_glGenLists,
    nh_gfx_glGenTextures,
    nh_gfx_glGenVertexArrays,
    NULL, // nh_gfx_glGetBooleanv,
    NULL, // nh_gfx_glGetClipPlane,
    NULL, // nh_gfx_glGetColorTableEXT,
    NULL, // nh_gfx_glGetColorTableParameterfvEXT,
    NULL, // nh_gfx_glGetColorTableParameterivEXT,
    NULL, // nh_gfx_glGetDoublev,
    NULL, // nh_gfx_glGetError,
    NULL, // nh_gfx_glGetFloatv,
    NULL, // nh_gfx_glGetIntegerv,
    NULL, // nh_gfx_glGetLightfv,
    NULL, // nh_gfx_glGetLightiv,
    NULL, // nh_gfx_glGetMapdv,
    NULL, // nh_gfx_glGetMapfv,
    NULL, // nh_gfx_glGetMapiv,
    NULL, // nh_gfx_glGetMaterialfv,
    NULL, // nh_gfx_glGetMaterialiv,
    NULL, // nh_gfx_glGetPixelMapfv,
    NULL, // nh_gfx_glGetPixelMapuiv,
    NULL, // nh_gfx_glGetPixelMapusv,
    NULL, // nh_gfx_glGetPointerv,
    NULL, // nh_gfx_glGetPolygonStipple,
    NULL, // nh_gfx_glGetString,
    NULL, // nh_gfx_glGetTexEnvfv,
    NULL, // nh_gfx_glGetTexEnviv,
    NULL, // nh_gfx_glGetTexGendv,
    NULL, // nh_gfx_glGetTexGenfv,
    NULL, // nh_gfx_glGetTexGeniv,
    NULL, // nh_gfx_glGetTexImage,
    NULL, // nh_gfx_glGetTexLevelParameterfv,
    NULL, // nh_gfx_glGetTexLevelParameteriv,
    NULL, // nh_gfx_glGetTexParameterfv,
    NULL, // nh_gfx_glGetTexParameteriv,
    nh_gfx_glGetUniformLocation,
    NULL, // nh_gfx_glHint,
    NULL, // nh_gfx_glIndexd,
    NULL, // nh_gfx_glIndexf,
    NULL, // nh_gfx_glIndexi,
    NULL, // nh_gfx_glIndexs,
    NULL, // nh_gfx_glIndexdv,
    NULL, // nh_gfx_glIndexfv,
    NULL, // nh_gfx_glIndexiv,
    NULL, // nh_gfx_glIndexsv,
    NULL, // nh_gfx_glIndexMask,
    NULL, // nh_gfx_glIndexPointer,
    NULL, // nh_gfx_glInitNames,
    NULL, // nh_gfx_glInterleavedArrays,
    NULL, // nh_gfx_glIsEnabled,
    NULL, // nh_gfx_glIsList,
    NULL, // nh_gfx_glIsTexture,
    NULL, // nh_gfx_glLightf,
    NULL, // nh_gfx_glLighti,
    NULL, // nh_gfx_glLightfv,
    NULL, // nh_gfx_glLightiv,
    NULL, // nh_gfx_glLightModelf,
    NULL, // nh_gfx_glLightModelfv,
    NULL, // nh_gfx_glLightModeli,
    NULL, // nh_gfx_glLightModeliv,
    NULL, // nh_gfx_glLineStipple,
    NULL, // nh_gfx_glLineWidth,
    nh_gfx_glLinkProgram,
    NULL, // nh_gfx_glListBase,
    NULL, // nh_gfx_glLoadIdentity,
    NULL, // nh_gfx_glLoadMatrixd,
    NULL, // nh_gfx_glLoadMatrixf,
    NULL, // nh_gfx_glLoadName,
    NULL, // nh_gfx_glLogicOp,
    NULL, // nh_gfx_glMap1d,
    NULL, // nh_gfx_glMap1f,
    NULL, // nh_gfx_glMap2d,
    NULL, // nh_gfx_glMap2f,
    NULL, // nh_gfx_glMapGrid1d,
    NULL, // nh_gfx_glMapGrid1f,
    NULL, // nh_gfx_glMapGrid2d,
    NULL, // nh_gfx_glMapGrid2f,
    NULL, // nh_gfx_glMaterialf,
    NULL, // nh_gfx_glMateriali,
    NULL, // nh_gfx_glMaterialfv,
    NULL, // nh_gfx_glMaterialiv,
    NULL, // nh_gfx_glMatrixMode,
    NULL, // nh_gfx_glMultMatrixd,
    NULL, // nh_gfx_glMultMatrixf,
    NULL, // nh_gfx_glNewList,
    NULL, // nh_gfx_glEndList,
    NULL, // nh_gfx_glNormal3b,
    NULL, // nh_gfx_glNormal3d,
    NULL, // nh_gfx_glNormal3f,
    NULL, // nh_gfx_glNormal3i,
    NULL, // nh_gfx_glNormal3s,
    NULL, // nh_gfx_glNormal3bv,
    NULL, // nh_gfx_glNormal3dv,
    NULL, // nh_gfx_glNormal3fv,
    NULL, // nh_gfx_glNormal3iv,
    NULL, // nh_gfx_glNormal3sv,
    NULL, // nh_gfx_glNormalPointer,
    NULL, // nh_gfx_glOrtho,
    NULL, // nh_gfx_glPassThrough,
    NULL, // nh_gfx_glPixelMapfv,
    NULL, // nh_gfx_glPixelMapuiv,
    NULL, // nh_gfx_glPixelMapusv,
    NULL, // nh_gfx_glPixelStoref,
    NULL, // nh_gfx_glPixelStorei,
    NULL, // nh_gfx_glPixelTransferf,
    NULL, // nh_gfx_glPixelTransferi,
    NULL, // nh_gfx_glPixelZoom,
    NULL, // nh_gfx_glPointSize,
    NULL, // nh_gfx_glPolygonMode,
    NULL, // nh_gfx_glPolygonOffset,
    NULL, // nh_gfx_glPolygonStipple,
    NULL, // nh_gfx_glPrioritizeTextures,
    NULL, // nh_gfx_glPushAttrib,
    NULL, // nh_gfx_glPopAttrib,
    NULL, // nh_gfx_glPushClientAttrib,
    NULL, // nh_gfx_glPopClientAttrib,
    NULL, // nh_gfx_glPushMatrix,
    NULL, // nh_gfx_glPopMatrix,
    NULL, // nh_gfx_glPushName,
    NULL, // nh_gfx_glPopName,
    NULL, // nh_gfx_glRasterPos2d,
    NULL, // nh_gfx_glRasterPos2f,
    NULL, // nh_gfx_glRasterPos2i,
    NULL, // nh_gfx_glRasterPos2s,
    NULL, // nh_gfx_glRasterPos3d,
    NULL, // nh_gfx_glRasterPos3f,
    NULL, // nh_gfx_glRasterPos3i,
    NULL, // nh_gfx_glRasterPos3s,
    NULL, // nh_gfx_glRasterPos4d,
    NULL, // nh_gfx_glRasterPos4f,
    NULL, // nh_gfx_glRasterPos4i,
    NULL, // nh_gfx_glRasterPos4s,
    NULL, // nh_gfx_glRasterPos2dv,
    NULL, // nh_gfx_glRasterPos2fv,
    NULL, // nh_gfx_glRasterPos2iv,
    NULL, // nh_gfx_glRasterPos2sv,
    NULL, // nh_gfx_glRasterPos3dv,
    NULL, // nh_gfx_glRasterPos3fv,
    NULL, // nh_gfx_glRasterPos3iv,
    NULL, // nh_gfx_glRasterPos3sv,
    NULL, // nh_gfx_glRasterPos4dv,
    NULL, // nh_gfx_glRasterPos4fv,
    NULL, // nh_gfx_glRasterPos4iv,
    NULL, // nh_gfx_glRasterPos4sv,
    NULL, // nh_gfx_glReadBuffer,
    NULL, // nh_gfx_glReadPixels,
    NULL, // nh_gfx_glRectd,
    NULL, // nh_gfx_glRectf,
    NULL, // nh_gfx_glRecti,
    NULL, // nh_gfx_glRects,
    NULL, // nh_gfx_glRectdv,
    NULL, // nh_gfx_glRectfv,
    NULL, // nh_gfx_glRectiv,
    NULL, // nh_gfx_glRectsv,
    NULL, // nh_gfx_glRenderMode,
    NULL, // nh_gfx_glRotated,
    NULL, // nh_gfx_glRotatef,
    NULL, // nh_gfx_glScaled,
    NULL, // nh_gfx_glScalef,
    nh_gfx_glScissor,
    NULL, // nh_gfx_glSelectBuffer,
    NULL, // nh_gfx_glShadeModel,
    nh_gfx_glShaderSource,
    NULL, // nh_gfx_glStencilFunc,
    NULL, // nh_gfx_glStencilMask,
    NULL, // nh_gfx_glStencilOp,
    NULL, // nh_gfx_glTexCoord1d,
    NULL, // nh_gfx_glTexCoord1f,
    NULL, // nh_gfx_glTexCoord1i,
    NULL, // nh_gfx_glTexCoord1s,
    NULL, // nh_gfx_glTexCoord2d,
    NULL, // nh_gfx_glTexCoord2i,
    NULL, // nh_gfx_glTexCoord2s,
    NULL, // nh_gfx_glTexCoord3d,
    NULL, // nh_gfx_glTexCoord3f,
    NULL, // nh_gfx_glTexCoord3i,
    NULL, // nh_gfx_glTexCoord3s,
    NULL, // nh_gfx_glTexCoord4d,
    NULL, // nh_gfx_glTexCoord4f,
    NULL, // nh_gfx_glTexCoord4i,
    NULL, // nh_gfx_glTexCoord4s,
    NULL, // nh_gfx_glTexCoord1dv,
    NULL, // nh_gfx_glTexCoord1fv,
    NULL, // nh_gfx_glTexCoord1iv,
    NULL, // nh_gfx_glTexCoord1sv,
    NULL, // nh_gfx_glTexCoord2dv,
    NULL, // nh_gfx_glTexCoord2fv,
    NULL, // nh_gfx_glTexCoord2iv,
    NULL, // nh_gfx_glTexCoord2sv,
    NULL, // nh_gfx_glTexCoord3dv,
    NULL, // nh_gfx_glTexCoord3fv,
    NULL, // nh_gfx_glTexCoord3iv,
    NULL, // nh_gfx_glTexCoord3sv,
    NULL, // nh_gfx_glTexCoord4dv,
    NULL, // nh_gfx_glTexCoord4fv,
    NULL, // nh_gfx_glTexCoord4iv,
    NULL, // nh_gfx_glTexCoord4sv,
    NULL, // nh_gfx_glTexCoordPointer,
    NULL, // nh_gfx_glTexEnvf,
    NULL, // nh_gfx_glTexEnvi,
    NULL, // nh_gfx_glTexEnvfv,
    NULL, // nh_gfx_glTexEnviv,
    NULL, // nh_gfx_glTexGend,
    NULL, // nh_gfx_glTexGenf,
    NULL, // nh_gfx_glTexGeni,
    NULL, // nh_gfx_glTexGendv,
    NULL, // nh_gfx_glTexGenfv,
    NULL, // nh_gfx_glTexGeniv,
    NULL, // nh_gfx_glTexImage1D,
    nh_gfx_glTexImage2D,
    NULL, // nh_gfx_glTexParameterf,
    nh_gfx_glTexParameteri,
    NULL, // nh_gfx_glTexParameterfv,
    NULL, // nh_gfx_glTexParameteriv,
    NULL, // nh_gfx_glTexSubImage1D,
    NULL, // nh_gfx_glTexSubImage2D,
    NULL, // nh_gfx_glTranslated,
    NULL, // nh_gfx_glTranslatef,
    nh_gfx_glUniform1i,
    nh_gfx_glUniform3f,
    nh_gfx_glUseProgram,
    NULL, // nh_gfx_glVertex2d,
    NULL, // nh_gfx_glVertex2f,
    NULL, // nh_gfx_glVertex2i,
    NULL, // nh_gfx_glVertex2s,
    NULL, // nh_gfx_glVertex3d,
    NULL, // nh_gfx_glVertex3f,
    NULL, // nh_gfx_glVertex3i,
    NULL, // nh_gfx_glVertex3s,
    NULL, // nh_gfx_glVertex4d,
    NULL, // nh_gfx_glVertex4f,
    NULL, // nh_gfx_glVertex4i,
    NULL, // nh_gfx_glVertex4s,
    NULL, // nh_gfx_glVertex2dv,
    NULL, // nh_gfx_glVertex2fv,
    NULL, // nh_gfx_glVertex2iv,
    NULL, // nh_gfx_glVertex2sv,
    NULL, // nh_gfx_glVertex3dv,
    NULL, // nh_gfx_glVertex3fv,
    NULL, // nh_gfx_glVertex3iv,
    NULL, // nh_gfx_glVertex3sv,
    NULL, // nh_gfx_glVertex4dv,
    NULL, // nh_gfx_glVertex4fv,
    NULL, // nh_gfx_glVertex4iv,
    NULL, // nh_gfx_glVertex4sv,
    nh_gfx_glVertexAttribPointer,
    NULL, // nh_gfx_glVertexPointer,
    nh_gfx_glViewport,
};

// EXECUTE COMMAND =================================================================================

NH_API_RESULT nh_gfx_executeOpenGLCommand(
    nh_gfx_OpenGLCommand *Command_p)
{
    return commands_p[Command_p->type](Command_p);
}

