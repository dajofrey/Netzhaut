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

// nh_opengl_glAccum

static NH_API_RESULT nh_opengl_glActiveTexture(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glActiveTexture(texture: %d)\n",*((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    glActiveTexture(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glAddSwapHintRectWIN
// nh_opengl_glAlphaFunc
// nh_opengl_glAreTexturesResident
// nh_opengl_glArrayElement

static NH_API_RESULT nh_opengl_glAttachShader(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glAttachShader(program: %d, shader: %d)\n",
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));

    glAttachShader(
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glBegin
// nh_opengl_glEnd

static NH_API_RESULT nh_opengl_glBindBuffer(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glBindBuffer(target: %d, buffer: %d)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));

    glBindBuffer(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_opengl_glBindTexture(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glBindTexture(target: %d, texture: %d)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));

    glBindTexture(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_opengl_glBindVertexArray(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glBindVertexArray(array: %d)\n",
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    glBindVertexArray(
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glBitmap

static NH_API_RESULT nh_opengl_glBlendFunc(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glBlendFunc(sfactor: %d, dfactor: %d)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));

    glBlendFunc(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_opengl_glBufferData(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glBufferData(target: %d, size: %d, data: %p, usage: %d)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
       *(GLsizeiptr*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p,
      (const void*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p,
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));

    glBufferData(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
       *(GLsizeiptr*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p,
      (const void*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p,
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glCallList
// nh_opengl_glCallLists

static NH_API_RESULT nh_opengl_glClear(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glClear(mask: %d)\n",
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    glClear(
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glClearAccum

static NH_API_RESULT nh_opengl_glClearColor(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glClearColor(red: %f, green: %f, blue: %f, alpha: %f)\n",
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));

    glClearColor(
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glClearDepth
// nh_opengl_glClearIndex
// nh_opengl_glClearStencil
// nh_opengl_glClipPlane
// nh_opengl_glColor3b
// nh_opengl_glColor3d
// nh_opengl_glColor3f
// nh_opengl_glColor3i
// nh_opengl_glColor3s
// nh_opengl_glColor3ub
// nh_opengl_glColor3ui
// nh_opengl_glColor3us
// nh_opengl_glColor4b
// nh_opengl_glColor4d
// nh_opengl_glColor4f
// nh_opengl_glColor4i
// nh_opengl_glColor4s
// nh_opengl_glColor4ub
// nh_opengl_glColor4ui
// nh_opengl_glColor4us
// nh_opengl_glColor3bv
// nh_opengl_glColor3dv
// nh_opengl_glColor3fv
// nh_opengl_glColor3iv
// nh_opengl_glColor3sv
// nh_opengl_glColor3ubv
// nh_opengl_glColor3uiv
// nh_opengl_glColor3usv
// nh_opengl_glColor4bv
// nh_opengl_glColor4dv
// nh_opengl_glColor4fv
// nh_opengl_glColor4iv
// nh_opengl_glColor4sv
// nh_opengl_glColor4ubv
// nh_opengl_glColor4uiv
// nh_opengl_glColor4usv

static NH_API_RESULT nh_opengl_glColorMask(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glColorMask(red: %d, green: %d, blue: %d, alpha: %d)\n",
        *((GLboolean*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLboolean*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLboolean*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLboolean*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));
 
     glColorMask(
        *((GLboolean*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLboolean*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLboolean*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLboolean*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glColorMaterial
// nh_opengl_glColorPointer
// nh_opengl_glColorTableEXT
// nh_opengl_glColorSubTableEXT

static NH_API_RESULT nh_opengl_glCompileShader(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glCompileShader(shader: %d)\n",
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    glCompileShader(
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glCopyPixels
// nh_opengl_glCopyTexImage1D
// nh_opengl_glCopyTexImage2D
// nh_opengl_glCopyTexSubImage1D
// nh_opengl_glCopyTexSubImage2D

static NH_API_RESULT nh_opengl_glCreateProgram(
    nh_opengl_Command *Command_p)
{
    nh_opengl_gluint(&Command_p->Result, glCreateProgram());
    nh_gfx_logOpenGLCommand("glCreateProgram(void) -> %d\n", *((GLuint*)Command_p->Result.p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_opengl_glCreateShader(
    nh_opengl_Command *Command_p)
{
    nh_opengl_gluint(&Command_p->Result, 
        glCreateShader(
            *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p)));

    nh_gfx_logOpenGLCommand("glCreateShader(shaderType: %d) -> %d\n", 
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLuint*)Command_p->Result.p));

    return NH_API_SUCCESS;
}

// nh_opengl_glCullFace
// nh_opengl_glDeleteLists
// nh_opengl_glDeleteTextures

static NH_API_RESULT nh_opengl_glDepthFunc(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glDepthFunc(func: %d)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));
 
    glDepthFunc(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glDepthMask
// nh_opengl_glDepthRange

static NH_API_RESULT nh_opengl_glDrawArrays(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glDrawArrays(mode: %d, first: %d, count: %d)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p));

    glDrawArrays(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glDrawBuffer

static NH_API_RESULT nh_opengl_glDrawElements(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glDrawElements(mode: %d, count: %d, type: %d, indices: %p)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        (const void*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p);

    glDrawElements(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        (const void*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p);

    return NH_API_SUCCESS;
}

// nh_opengl_glDrawPixels
// nh_opengl_glEdgeFlag
// nh_opengl_glEdgeFlagv
// nh_opengl_glEdgeFlagPointer

static NH_API_RESULT nh_opengl_glEnable(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glEnable(cap: %d)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));
 
    glEnable(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_opengl_glEnableVertexAttribArray(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glEnableVertexAttribArray(index: %d)\n",
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));
 
    glEnableVertexAttribArray(
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_opengl_glDisable(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glDisable(cap: %d)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));
 
    glDisable(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glEnableClientState
// nh_opengl_glDisableClientState
// nh_opengl_glEvalCoord1d
// nh_opengl_glEvalCoord1f
// nh_opengl_glEvalCoord2d
// nh_opengl_glEvalCoord2f
// nh_opengl_glEvalCoord1dv
// nh_opengl_glEvalCoord2dv
// nh_opengl_glEvalCoord1fv
// nh_opengl_glEvalCoord2fv
// nh_opengl_glEvalMesh1
// nh_opengl_glEvalMesh2
// nh_opengl_glEvalPoint1
// nh_opengl_glEvalPoint2
// nh_opengl_glFeedbackBuffer
// nh_opengl_glFinish
// nh_opengl_glFlush
// nh_opengl_glFogf
// nh_opengl_glFogi
// nh_opengl_glFogfv
// nh_opengl_glFogiv
// nh_opengl_glFrontFace
// nh_opengl_glFrustum

static NH_API_RESULT nh_opengl_glGenBuffers(
    nh_opengl_Command *Command_p)
{
    glGenBuffers(
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p);

    char b[255] = {};
    memset(b, 0, 255);
     
    sprintf(b, "glGenBuffers(n: %d, buffers: %p) -> (",
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p);
     
    for (int i = 0; i < *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p); ++i) {
        if (i) {sprintf(b+strlen(b), ", ");}
        sprintf(b+strlen(b), "%d", ((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p)[i]);
    }
    sprintf(b+strlen(b), ")\n");
    nh_gfx_logOpenGLCommand(b);

    return NH_API_SUCCESS;
}

// nh_opengl_glGenLists

static NH_API_RESULT nh_opengl_glGenTextures(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glGenTextures(n: %d, textures: %p)\n",
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p);
 
    glGenTextures(
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_opengl_glGenVertexArrays(
    nh_opengl_Command *Command_p)
{
    glGenVertexArrays(
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p);

    char b[255] = {};
    memset(b, 0, 255);
     
    sprintf(b, "glGenVertexArrays(n: %d, arrays: %p) -> (",
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
           (GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p);
     
    for (int i = 0; i < *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p); ++i) {
        if (i) {sprintf(b+strlen(b), ", ");}
        sprintf(b+strlen(b), "%d", ((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p)[i]);
    }

    sprintf(b+strlen(b), ")\n");
    nh_gfx_logOpenGLCommand(b);

    return NH_API_SUCCESS;
}

// nh_opengl_glGetBooleanv
// nh_opengl_glGetClipPlane
// nh_opengl_glGetColorTableEXT
// nh_opengl_glGetColorTableParameterfvEXT
// nh_opengl_glGetColorTableParameterivEXT
// nh_opengl_glGetDoublev
// nh_opengl_glGetError
// nh_opengl_glGetFloatv
// nh_opengl_glGetIntegerv
// nh_opengl_glGetLightfv
// nh_opengl_glGetLightiv
// nh_opengl_glGetMapdv
// nh_opengl_glGetMapfv
// nh_opengl_glGetMapiv
// nh_opengl_glGetMaterialfv
// nh_opengl_glGetMaterialiv
// nh_opengl_glGetPixelMapfv
// nh_opengl_glGetPixelMapuiv
// nh_opengl_glGetPixelMapusv
// nh_opengl_glGetPointerv
// nh_opengl_glGetPolygonStipple
// nh_opengl_glGetString
// nh_opengl_glGetTexEnvfv
// nh_opengl_glGetTexEnviv
// nh_opengl_glGetTexGendv
// nh_opengl_glGetTexGenfv
// nh_opengl_glGetTexGeniv
// nh_opengl_glGetTexImage
// nh_opengl_glGetTexLevelParameterfv
// nh_opengl_glGetTexLevelParameteriv
// nh_opengl_glGetTexParameterfv
// nh_opengl_glGetTexParameteriv

static NH_API_RESULT nh_opengl_glGetUniformLocation(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glGetUniformLocation(program: %d, name: %s)\n",
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((const GLchar**)((nh_opengl_Data*)Command_p->Arguments.pp[1])->pp));
 
    nh_opengl_glint(&Command_p->Result,
        glGetUniformLocation(
            *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
            *((const GLchar**)((nh_opengl_Data*)Command_p->Arguments.pp[1])->pp)));

    return NH_API_SUCCESS;
}

// nh_opengl_glHint
// nh_opengl_glIndexd
// nh_opengl_glIndexf
// nh_opengl_glIndexi
// nh_opengl_glIndexs
// nh_opengl_glIndexdv
// nh_opengl_glIndexfv
// nh_opengl_glIndexiv
// nh_opengl_glIndexsv
// nh_opengl_glIndexMask
// nh_opengl_glIndexPointer
// nh_opengl_glInitNames
// nh_opengl_glInterleavedArrays
// nh_opengl_glIsEnabled
// nh_opengl_glIsList
// nh_opengl_glIsTexture
// nh_opengl_glLightf
// nh_opengl_glLighti
// nh_opengl_glLightfv
// nh_opengl_glLightiv
// nh_opengl_glLightModelf
// nh_opengl_glLightModelfv
// nh_opengl_glLightModeli
// nh_opengl_glLightModeliv
// nh_opengl_glLineStipple
// nh_opengl_glLineWidth

static NH_API_RESULT nh_opengl_glLinkProgram(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glLinkProgram(program: %d)\n",
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));
 
    glLinkProgram(
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glListBase
// nh_opengl_glLoadIdentity
// nh_opengl_glLoadMatrixd
// nh_opengl_glLoadMatrixf
// nh_opengl_glLoadName
// nh_opengl_glLogicOp
// nh_opengl_glMap1d
// nh_opengl_glMap1f
// nh_opengl_glMap2d
// nh_opengl_glMap2f
// nh_opengl_glMapGrid1d
// nh_opengl_glMapGrid1f
// nh_opengl_glMapGrid2d
// nh_opengl_glMapGrid2f
// nh_opengl_glMaterialf
// nh_opengl_glMateriali
// nh_opengl_glMaterialfv
// nh_opengl_glMaterialiv
// nh_opengl_glMatrixMode
// nh_opengl_glMultMatrixd
// nh_opengl_glMultMatrixf
// nh_opengl_glNewList
// nh_opengl_glEndList
// nh_opengl_glNormal3b
// nh_opengl_glNormal3d
// nh_opengl_glNormal3f
// nh_opengl_glNormal3i
// nh_opengl_glNormal3s
// nh_opengl_glNormal3bv
// nh_opengl_glNormal3dv
// nh_opengl_glNormal3fv
// nh_opengl_glNormal3iv
// nh_opengl_glNormal3sv
// nh_opengl_glNormalPointer
// nh_opengl_glOrtho
// nh_opengl_glPassThrough
// nh_opengl_glPixelMapfv
// nh_opengl_glPixelMapuiv
// nh_opengl_glPixelMapusv
// nh_opengl_glPixelStoref
// nh_opengl_glPixelStorei
// nh_opengl_glPixelTransferf
// nh_opengl_glPixelTransferi
// nh_opengl_glPixelZoom
// nh_opengl_glPointSize
// nh_opengl_glPolygonMode
// nh_opengl_glPolygonOffset
// nh_opengl_glPolygonStipple
// nh_opengl_glPrioritizeTextures
// nh_opengl_glPushAttrib
// nh_opengl_glPopAttrib
// nh_opengl_glPushClientAttrib
// nh_opengl_glPopClientAttrib
// nh_opengl_glPushMatrix
// nh_opengl_glPopMatrix
// nh_opengl_glPushName
// nh_opengl_glPopName
// nh_opengl_glRasterPos2d
// nh_opengl_glRasterPos2f
// nh_opengl_glRasterPos2i
// nh_opengl_glRasterPos2s
// nh_opengl_glRasterPos3d
// nh_opengl_glRasterPos3f
// nh_opengl_glRasterPos3i
// nh_opengl_glRasterPos3s
// nh_opengl_glRasterPos4d
// nh_opengl_glRasterPos4f
// nh_opengl_glRasterPos4i
// nh_opengl_glRasterPos4s
// nh_opengl_glRasterPos2dv
// nh_opengl_glRasterPos2fv
// nh_opengl_glRasterPos2iv
// nh_opengl_glRasterPos2sv
// nh_opengl_glRasterPos3dv
// nh_opengl_glRasterPos3fv
// nh_opengl_glRasterPos3iv
// nh_opengl_glRasterPos3sv
// nh_opengl_glRasterPos4dv
// nh_opengl_glRasterPos4fv
// nh_opengl_glRasterPos4iv
// nh_opengl_glRasterPos4sv
// nh_opengl_glReadBuffer
// nh_opengl_glReadPixels
// nh_opengl_glRectd
// nh_opengl_glRectf
// nh_opengl_glRecti
// nh_opengl_glRects
// nh_opengl_glRectdv
// nh_opengl_glRectfv
// nh_opengl_glRectiv
// nh_opengl_glRectsv
// nh_opengl_glRenderMode
// nh_opengl_glRotated
// nh_opengl_glRotatef
// nh_opengl_glScaled
// nh_opengl_glScalef

static NH_API_RESULT nh_opengl_glScissor(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glScissor(x: %d, y: %d, width: %d, height: %d)\n",
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));
 
    glScissor(
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glSelectBuffer
// nh_opengl_glShadeModel

static NH_API_RESULT nh_opengl_glShaderSource(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glShaderSource(shader: %d, count: %d, string: %p, length: %d)\n",
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
          (const GLchar**)((nh_opengl_Data*)Command_p->Arguments.pp[2])->pp,
          (const GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p);
 
    glShaderSource(
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
          (const GLchar**)((nh_opengl_Data*)Command_p->Arguments.pp[2])->pp,
          (const GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p);

    return NH_API_SUCCESS;
}

// nh_opengl_glStencilFunc
// nh_opengl_glStencilMask
// nh_opengl_glStencilOp
// nh_opengl_glTexCoord1d
// nh_opengl_glTexCoord1f
// nh_opengl_glTexCoord1i
// nh_opengl_glTexCoord1s
// nh_opengl_glTexCoord2d
// nh_opengl_glTexCoord2i
// nh_opengl_glTexCoord2s
// nh_opengl_glTexCoord3d
// nh_opengl_glTexCoord3f
// nh_opengl_glTexCoord3i
// nh_opengl_glTexCoord3s
// nh_opengl_glTexCoord4d
// nh_opengl_glTexCoord4f
// nh_opengl_glTexCoord4i
// nh_opengl_glTexCoord4s
// nh_opengl_glTexCoord1dv
// nh_opengl_glTexCoord1fv
// nh_opengl_glTexCoord1iv
// nh_opengl_glTexCoord1sv
// nh_opengl_glTexCoord2dv
// nh_opengl_glTexCoord2fv
// nh_opengl_glTexCoord2iv
// nh_opengl_glTexCoord2sv
// nh_opengl_glTexCoord3dv
// nh_opengl_glTexCoord3fv
// nh_opengl_glTexCoord3iv
// nh_opengl_glTexCoord3sv
// nh_opengl_glTexCoord4dv
// nh_opengl_glTexCoord4fv
// nh_opengl_glTexCoord4iv
// nh_opengl_glTexCoord4sv
// nh_opengl_glTexCoordPointer
// nh_opengl_glTexEnvf
// nh_opengl_glTexEnvi
// nh_opengl_glTexEnvfv
// nh_opengl_glTexEnviv
// nh_opengl_glTexGend
// nh_opengl_glTexGenf
// nh_opengl_glTexGeni
// nh_opengl_glTexGendv
// nh_opengl_glTexGenfv
// nh_opengl_glTexGeniv
// nh_opengl_glTexImage1D

static NH_API_RESULT nh_opengl_glTexImage2D(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glTexImage2D(target: %d, level: %d, internalformat: %d, width: %d, height: %d, border: %d, format: %d, type: %d, data: %p)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[4])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[5])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[6])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[7])->p),
        *((const void**)((nh_opengl_Data*)Command_p->Arguments.pp[8])->pp));
 
    glTexImage2D(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[4])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[5])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[6])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[7])->p),
        *((const void**)((nh_opengl_Data*)Command_p->Arguments.pp[8])->pp));

    return NH_API_SUCCESS;
}

// nh_opengl_glTexParameterf

static NH_API_RESULT nh_opengl_glTexParameteri(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glTexParameteri(target: %d, pname: %d, param: %d)\n",
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p));
 
    glTexParameteri(
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glTexParameterfv
// nh_opengl_glTexParameteriv
// nh_opengl_glTexSubImage1D
// nh_opengl_glTexSubImage2D
// nh_opengl_glTranslated
// nh_opengl_glTranslatef

static NH_API_RESULT nh_opengl_glUniform1i(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glUniform1i(location: %d, v0: %d)\n",
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));
 
    glUniform1i(
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_opengl_glUniform3f(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glUniform3f(location: %d, v0: %f, v1: %f, v2: %f)\n",
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));
 
    glUniform3f(
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLfloat*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_opengl_glUseProgram(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glUseProgram(program: %d)\n",
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));
 
    glUseProgram(
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p));

    return NH_API_SUCCESS;
}

// nh_opengl_glVertex2d
// nh_opengl_glVertex2f
// nh_opengl_glVertex2i
// nh_opengl_glVertex2s
// nh_opengl_glVertex3d
// nh_opengl_glVertex3f
// nh_opengl_glVertex3i
// nh_opengl_glVertex3s
// nh_opengl_glVertex4d
// nh_opengl_glVertex4f
// nh_opengl_glVertex4i
// nh_opengl_glVertex4s
// nh_opengl_glVertex2dv
// nh_opengl_glVertex2fv
// nh_opengl_glVertex2iv
// nh_opengl_glVertex2sv
// nh_opengl_glVertex3dv
// nh_opengl_glVertex3fv
// nh_opengl_glVertex3iv
// nh_opengl_glVertex3sv
// nh_opengl_glVertex4dv
// nh_opengl_glVertex4fv
// nh_opengl_glVertex4iv
// nh_opengl_glVertex4sv

static NH_API_RESULT nh_opengl_glVertexAttribPointer(
    nh_opengl_Command *Command_p)
{
    void *p = NULL;
    if ((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[5])->p) {
        p = *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[5])->p);
    }

    glVertexAttribPointer(
        *((GLuint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
         *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLenum*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
     *((GLboolean*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p),
       *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[4])->p), p);

    return NH_API_SUCCESS;
}

// nh_opengl_glVertexPointer

static NH_API_RESULT nh_opengl_glViewport(
    nh_opengl_Command *Command_p)
{
    nh_gfx_logOpenGLCommand("glViewport(x: %d, y: %d, width: %d, height: %d)\n",
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));
 
    glViewport(
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[0])->p),
        *((GLint*)((nh_opengl_Data*)Command_p->Arguments.pp[1])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[2])->p),
        *((GLsizei*)((nh_opengl_Data*)Command_p->Arguments.pp[3])->p));

    return NH_API_SUCCESS;
}

// COMMANDS LIST ===================================================================================

typedef NH_API_RESULT (*nh_opengl_command_f)(
    nh_opengl_Command *Command_p    
);

nh_opengl_command_f commands_p[] = {
    NULL, // nh_opengl_glAccum,
    nh_opengl_glActiveTexture,
    NULL, // nh_opengl_glAddSwapHintRectWIN,
    NULL, // nh_opengl_glAlphaFunc,
    NULL, // nh_opengl_glAreTexturesResident,
    NULL, // nh_opengl_glArrayElement,
    nh_opengl_glAttachShader,
    NULL, // nh_opengl_glBegin,
    NULL, // nh_opengl_glEnd,
    nh_opengl_glBindBuffer,
    nh_opengl_glBindTexture,
    nh_opengl_glBindVertexArray,
    NULL, // nh_opengl_glBitmap,
    nh_opengl_glBlendFunc,
    nh_opengl_glBufferData,
    NULL, // nh_opengl_glCallList,
    NULL, // nh_opengl_glCallLists,
    nh_opengl_glClear,
    NULL, // nh_opengl_glClearAccum,
    nh_opengl_glClearColor,
    NULL, // nh_opengl_glClearDepth,
    NULL, // nh_opengl_glClearIndex,
    NULL, // nh_opengl_glClearStencil,
    NULL, // nh_opengl_glClipPlane,
    NULL, // nh_opengl_glColor3b,
    NULL, // nh_opengl_glColor3d,
    NULL, // nh_opengl_glColor3f,
    NULL, // nh_opengl_glColor3i,
    NULL, // nh_opengl_glColor3s,
    NULL, // nh_opengl_glColor3ub,
    NULL, // nh_opengl_glColor3ui,
    NULL, // nh_opengl_glColor3us,
    NULL, // nh_opengl_glColor4b,
    NULL, // nh_opengl_glColor4d,
    NULL, // nh_opengl_glColor4f,
    NULL, // nh_opengl_glColor4i,
    NULL, // nh_opengl_glColor4s,
    NULL, // nh_opengl_glColor4ub,
    NULL, // nh_opengl_glColor4ui,
    NULL, // nh_opengl_glColor4us,
    NULL, // nh_opengl_glColor3bv,
    NULL, // nh_opengl_glColor3dv,
    NULL, // nh_opengl_glColor3fv,
    NULL, // nh_opengl_glColor3iv,
    NULL, // nh_opengl_glColor3sv,
    NULL, // nh_opengl_glColor3ubv,
    NULL, // nh_opengl_glColor3uiv,
    NULL, // nh_opengl_glColor3usv,
    NULL, // nh_opengl_glColor4bv,
    NULL, // nh_opengl_glColor4dv,
    NULL, // nh_opengl_glColor4fv,
    NULL, // nh_opengl_glColor4iv,
    NULL, // nh_opengl_glColor4sv,
    NULL, // nh_opengl_glColor4ubv,
    NULL, // nh_opengl_glColor4uiv,
    NULL, // nh_opengl_glColor4usv,
    nh_opengl_glColorMask,
    NULL, // nh_opengl_glColorMaterial,
    NULL, // nh_opengl_glColorPointer,
    NULL, // nh_opengl_glColorTableEXT,
    NULL, // nh_opengl_glColorSubTableEXT,
    nh_opengl_glCompileShader,
    NULL, // nh_opengl_glCopyPixels,
    NULL, // nh_opengl_glCopyTexImage1D,
    NULL, // nh_opengl_glCopyTexImage2D,
    NULL, // nh_opengl_glCopyTexSubImage1D,
    NULL, // nh_opengl_glCopyTexSubImage2D,
    nh_opengl_glCreateProgram,
    nh_opengl_glCreateShader,
    NULL, // nh_opengl_glCullFace,
    NULL, // nh_opengl_glDeleteLists,
    NULL, // nh_opengl_glDeleteTextures,
    nh_opengl_glDepthFunc,
    NULL, // nh_opengl_glDepthMask,
    NULL, // nh_opengl_glDepthRange,
    nh_opengl_glDrawArrays,
    NULL, // nh_opengl_glDrawBuffer,
    nh_opengl_glDrawElements,
    NULL, // nh_opengl_glDrawPixels,
    NULL, // nh_opengl_glEdgeFlag,
    NULL, // nh_opengl_glEdgeFlagv,
    NULL, // nh_opengl_glEdgeFlagPointer,
    nh_opengl_glEnable,
    nh_opengl_glEnableVertexAttribArray,
    nh_opengl_glDisable,
    NULL, // nh_opengl_glEnableClientState,
    NULL, // nh_opengl_glDisableClientState,
    NULL, // nh_opengl_glEvalCoord1d,
    NULL, // nh_opengl_glEvalCoord1f,
    NULL, // nh_opengl_glEvalCoord2d,
    NULL, // nh_opengl_glEvalCoord2f,
    NULL, // nh_opengl_glEvalCoord1dv,
    NULL, // nh_opengl_glEvalCoord2dv,
    NULL, // nh_opengl_glEvalCoord1fv,
    NULL, // nh_opengl_glEvalCoord2fv,
    NULL, // nh_opengl_glEvalMesh1,
    NULL, // nh_opengl_glEvalMesh2,
    NULL, // nh_opengl_glEvalPoint1,
    NULL, // nh_opengl_glEvalPoint2,
    NULL, // nh_opengl_glFeedbackBuffer,
    NULL, // nh_opengl_glFinish,
    NULL, // nh_opengl_glFlush,
    NULL, // nh_opengl_glFogf,
    NULL, // nh_opengl_glFogi,
    NULL, // nh_opengl_glFogfv,
    NULL, // nh_opengl_glFogiv,
    NULL, // nh_opengl_glFrontFace,
    NULL, // nh_opengl_glFrustum,
    nh_opengl_glGenBuffers,
    NULL, // nh_opengl_glGenLists,
    nh_opengl_glGenTextures,
    nh_opengl_glGenVertexArrays,
    NULL, // nh_opengl_glGetBooleanv,
    NULL, // nh_opengl_glGetClipPlane,
    NULL, // nh_opengl_glGetColorTableEXT,
    NULL, // nh_opengl_glGetColorTableParameterfvEXT,
    NULL, // nh_opengl_glGetColorTableParameterivEXT,
    NULL, // nh_opengl_glGetDoublev,
    NULL, // nh_opengl_glGetError,
    NULL, // nh_opengl_glGetFloatv,
    NULL, // nh_opengl_glGetIntegerv,
    NULL, // nh_opengl_glGetLightfv,
    NULL, // nh_opengl_glGetLightiv,
    NULL, // nh_opengl_glGetMapdv,
    NULL, // nh_opengl_glGetMapfv,
    NULL, // nh_opengl_glGetMapiv,
    NULL, // nh_opengl_glGetMaterialfv,
    NULL, // nh_opengl_glGetMaterialiv,
    NULL, // nh_opengl_glGetPixelMapfv,
    NULL, // nh_opengl_glGetPixelMapuiv,
    NULL, // nh_opengl_glGetPixelMapusv,
    NULL, // nh_opengl_glGetPointerv,
    NULL, // nh_opengl_glGetPolygonStipple,
    NULL, // nh_opengl_glGetString,
    NULL, // nh_opengl_glGetTexEnvfv,
    NULL, // nh_opengl_glGetTexEnviv,
    NULL, // nh_opengl_glGetTexGendv,
    NULL, // nh_opengl_glGetTexGenfv,
    NULL, // nh_opengl_glGetTexGeniv,
    NULL, // nh_opengl_glGetTexImage,
    NULL, // nh_opengl_glGetTexLevelParameterfv,
    NULL, // nh_opengl_glGetTexLevelParameteriv,
    NULL, // nh_opengl_glGetTexParameterfv,
    NULL, // nh_opengl_glGetTexParameteriv,
    nh_opengl_glGetUniformLocation,
    NULL, // nh_opengl_glHint,
    NULL, // nh_opengl_glIndexd,
    NULL, // nh_opengl_glIndexf,
    NULL, // nh_opengl_glIndexi,
    NULL, // nh_opengl_glIndexs,
    NULL, // nh_opengl_glIndexdv,
    NULL, // nh_opengl_glIndexfv,
    NULL, // nh_opengl_glIndexiv,
    NULL, // nh_opengl_glIndexsv,
    NULL, // nh_opengl_glIndexMask,
    NULL, // nh_opengl_glIndexPointer,
    NULL, // nh_opengl_glInitNames,
    NULL, // nh_opengl_glInterleavedArrays,
    NULL, // nh_opengl_glIsEnabled,
    NULL, // nh_opengl_glIsList,
    NULL, // nh_opengl_glIsTexture,
    NULL, // nh_opengl_glLightf,
    NULL, // nh_opengl_glLighti,
    NULL, // nh_opengl_glLightfv,
    NULL, // nh_opengl_glLightiv,
    NULL, // nh_opengl_glLightModelf,
    NULL, // nh_opengl_glLightModelfv,
    NULL, // nh_opengl_glLightModeli,
    NULL, // nh_opengl_glLightModeliv,
    NULL, // nh_opengl_glLineStipple,
    NULL, // nh_opengl_glLineWidth,
    nh_opengl_glLinkProgram,
    NULL, // nh_opengl_glListBase,
    NULL, // nh_opengl_glLoadIdentity,
    NULL, // nh_opengl_glLoadMatrixd,
    NULL, // nh_opengl_glLoadMatrixf,
    NULL, // nh_opengl_glLoadName,
    NULL, // nh_opengl_glLogicOp,
    NULL, // nh_opengl_glMap1d,
    NULL, // nh_opengl_glMap1f,
    NULL, // nh_opengl_glMap2d,
    NULL, // nh_opengl_glMap2f,
    NULL, // nh_opengl_glMapGrid1d,
    NULL, // nh_opengl_glMapGrid1f,
    NULL, // nh_opengl_glMapGrid2d,
    NULL, // nh_opengl_glMapGrid2f,
    NULL, // nh_opengl_glMaterialf,
    NULL, // nh_opengl_glMateriali,
    NULL, // nh_opengl_glMaterialfv,
    NULL, // nh_opengl_glMaterialiv,
    NULL, // nh_opengl_glMatrixMode,
    NULL, // nh_opengl_glMultMatrixd,
    NULL, // nh_opengl_glMultMatrixf,
    NULL, // nh_opengl_glNewList,
    NULL, // nh_opengl_glEndList,
    NULL, // nh_opengl_glNormal3b,
    NULL, // nh_opengl_glNormal3d,
    NULL, // nh_opengl_glNormal3f,
    NULL, // nh_opengl_glNormal3i,
    NULL, // nh_opengl_glNormal3s,
    NULL, // nh_opengl_glNormal3bv,
    NULL, // nh_opengl_glNormal3dv,
    NULL, // nh_opengl_glNormal3fv,
    NULL, // nh_opengl_glNormal3iv,
    NULL, // nh_opengl_glNormal3sv,
    NULL, // nh_opengl_glNormalPointer,
    NULL, // nh_opengl_glOrtho,
    NULL, // nh_opengl_glPassThrough,
    NULL, // nh_opengl_glPixelMapfv,
    NULL, // nh_opengl_glPixelMapuiv,
    NULL, // nh_opengl_glPixelMapusv,
    NULL, // nh_opengl_glPixelStoref,
    NULL, // nh_opengl_glPixelStorei,
    NULL, // nh_opengl_glPixelTransferf,
    NULL, // nh_opengl_glPixelTransferi,
    NULL, // nh_opengl_glPixelZoom,
    NULL, // nh_opengl_glPointSize,
    NULL, // nh_opengl_glPolygonMode,
    NULL, // nh_opengl_glPolygonOffset,
    NULL, // nh_opengl_glPolygonStipple,
    NULL, // nh_opengl_glPrioritizeTextures,
    NULL, // nh_opengl_glPushAttrib,
    NULL, // nh_opengl_glPopAttrib,
    NULL, // nh_opengl_glPushClientAttrib,
    NULL, // nh_opengl_glPopClientAttrib,
    NULL, // nh_opengl_glPushMatrix,
    NULL, // nh_opengl_glPopMatrix,
    NULL, // nh_opengl_glPushName,
    NULL, // nh_opengl_glPopName,
    NULL, // nh_opengl_glRasterPos2d,
    NULL, // nh_opengl_glRasterPos2f,
    NULL, // nh_opengl_glRasterPos2i,
    NULL, // nh_opengl_glRasterPos2s,
    NULL, // nh_opengl_glRasterPos3d,
    NULL, // nh_opengl_glRasterPos3f,
    NULL, // nh_opengl_glRasterPos3i,
    NULL, // nh_opengl_glRasterPos3s,
    NULL, // nh_opengl_glRasterPos4d,
    NULL, // nh_opengl_glRasterPos4f,
    NULL, // nh_opengl_glRasterPos4i,
    NULL, // nh_opengl_glRasterPos4s,
    NULL, // nh_opengl_glRasterPos2dv,
    NULL, // nh_opengl_glRasterPos2fv,
    NULL, // nh_opengl_glRasterPos2iv,
    NULL, // nh_opengl_glRasterPos2sv,
    NULL, // nh_opengl_glRasterPos3dv,
    NULL, // nh_opengl_glRasterPos3fv,
    NULL, // nh_opengl_glRasterPos3iv,
    NULL, // nh_opengl_glRasterPos3sv,
    NULL, // nh_opengl_glRasterPos4dv,
    NULL, // nh_opengl_glRasterPos4fv,
    NULL, // nh_opengl_glRasterPos4iv,
    NULL, // nh_opengl_glRasterPos4sv,
    NULL, // nh_opengl_glReadBuffer,
    NULL, // nh_opengl_glReadPixels,
    NULL, // nh_opengl_glRectd,
    NULL, // nh_opengl_glRectf,
    NULL, // nh_opengl_glRecti,
    NULL, // nh_opengl_glRects,
    NULL, // nh_opengl_glRectdv,
    NULL, // nh_opengl_glRectfv,
    NULL, // nh_opengl_glRectiv,
    NULL, // nh_opengl_glRectsv,
    NULL, // nh_opengl_glRenderMode,
    NULL, // nh_opengl_glRotated,
    NULL, // nh_opengl_glRotatef,
    NULL, // nh_opengl_glScaled,
    NULL, // nh_opengl_glScalef,
    nh_opengl_glScissor,
    NULL, // nh_opengl_glSelectBuffer,
    NULL, // nh_opengl_glShadeModel,
    nh_opengl_glShaderSource,
    NULL, // nh_opengl_glStencilFunc,
    NULL, // nh_opengl_glStencilMask,
    NULL, // nh_opengl_glStencilOp,
    NULL, // nh_opengl_glTexCoord1d,
    NULL, // nh_opengl_glTexCoord1f,
    NULL, // nh_opengl_glTexCoord1i,
    NULL, // nh_opengl_glTexCoord1s,
    NULL, // nh_opengl_glTexCoord2d,
    NULL, // nh_opengl_glTexCoord2i,
    NULL, // nh_opengl_glTexCoord2s,
    NULL, // nh_opengl_glTexCoord3d,
    NULL, // nh_opengl_glTexCoord3f,
    NULL, // nh_opengl_glTexCoord3i,
    NULL, // nh_opengl_glTexCoord3s,
    NULL, // nh_opengl_glTexCoord4d,
    NULL, // nh_opengl_glTexCoord4f,
    NULL, // nh_opengl_glTexCoord4i,
    NULL, // nh_opengl_glTexCoord4s,
    NULL, // nh_opengl_glTexCoord1dv,
    NULL, // nh_opengl_glTexCoord1fv,
    NULL, // nh_opengl_glTexCoord1iv,
    NULL, // nh_opengl_glTexCoord1sv,
    NULL, // nh_opengl_glTexCoord2dv,
    NULL, // nh_opengl_glTexCoord2fv,
    NULL, // nh_opengl_glTexCoord2iv,
    NULL, // nh_opengl_glTexCoord2sv,
    NULL, // nh_opengl_glTexCoord3dv,
    NULL, // nh_opengl_glTexCoord3fv,
    NULL, // nh_opengl_glTexCoord3iv,
    NULL, // nh_opengl_glTexCoord3sv,
    NULL, // nh_opengl_glTexCoord4dv,
    NULL, // nh_opengl_glTexCoord4fv,
    NULL, // nh_opengl_glTexCoord4iv,
    NULL, // nh_opengl_glTexCoord4sv,
    NULL, // nh_opengl_glTexCoordPointer,
    NULL, // nh_opengl_glTexEnvf,
    NULL, // nh_opengl_glTexEnvi,
    NULL, // nh_opengl_glTexEnvfv,
    NULL, // nh_opengl_glTexEnviv,
    NULL, // nh_opengl_glTexGend,
    NULL, // nh_opengl_glTexGenf,
    NULL, // nh_opengl_glTexGeni,
    NULL, // nh_opengl_glTexGendv,
    NULL, // nh_opengl_glTexGenfv,
    NULL, // nh_opengl_glTexGeniv,
    NULL, // nh_opengl_glTexImage1D,
    nh_opengl_glTexImage2D,
    NULL, // nh_opengl_glTexParameterf,
    nh_opengl_glTexParameteri,
    NULL, // nh_opengl_glTexParameterfv,
    NULL, // nh_opengl_glTexParameteriv,
    NULL, // nh_opengl_glTexSubImage1D,
    NULL, // nh_opengl_glTexSubImage2D,
    NULL, // nh_opengl_glTranslated,
    NULL, // nh_opengl_glTranslatef,
    nh_opengl_glUniform1i,
    nh_opengl_glUniform3f,
    nh_opengl_glUseProgram,
    NULL, // nh_opengl_glVertex2d,
    NULL, // nh_opengl_glVertex2f,
    NULL, // nh_opengl_glVertex2i,
    NULL, // nh_opengl_glVertex2s,
    NULL, // nh_opengl_glVertex3d,
    NULL, // nh_opengl_glVertex3f,
    NULL, // nh_opengl_glVertex3i,
    NULL, // nh_opengl_glVertex3s,
    NULL, // nh_opengl_glVertex4d,
    NULL, // nh_opengl_glVertex4f,
    NULL, // nh_opengl_glVertex4i,
    NULL, // nh_opengl_glVertex4s,
    NULL, // nh_opengl_glVertex2dv,
    NULL, // nh_opengl_glVertex2fv,
    NULL, // nh_opengl_glVertex2iv,
    NULL, // nh_opengl_glVertex2sv,
    NULL, // nh_opengl_glVertex3dv,
    NULL, // nh_opengl_glVertex3fv,
    NULL, // nh_opengl_glVertex3iv,
    NULL, // nh_opengl_glVertex3sv,
    NULL, // nh_opengl_glVertex4dv,
    NULL, // nh_opengl_glVertex4fv,
    NULL, // nh_opengl_glVertex4iv,
    NULL, // nh_opengl_glVertex4sv,
    nh_opengl_glVertexAttribPointer,
    NULL, // nh_opengl_glVertexPointer,
    nh_opengl_glViewport,
};

// EXECUTE COMMAND =================================================================================

NH_API_RESULT nh_opengl_executeCommand(
    nh_opengl_Command *Command_p)
{
    return commands_p[Command_p->type](Command_p);
}

