// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "API.h"

#include "../Common/IndexMap.h"

#include "../../nh-core/Util/HashMap.h"

// COMMAND NAMES ===================================================================================

const char *NH_GFX_OPENGL_COMMAND_NAMES_PP[] = {
    "glAccum",
    "glActiveTexture",
    "glAddSwapHintRectWIN",
    "glAlphaFunc",
    "glAreTexturesResident",
    "glArrayElement",
    "glAttachShader",
    "glBegin",
    "glEnd",
    "glBindBuffer",
    "glBindTexture",
    "glBindVertexArray",
    "glBitmap",
    "glBlendFunc",
    "glBufferData",
    "glCallList",
    "glCallLists",
    "glClear",
    "glClearAccum",
    "glClearColor",
    "glClearDepth",
    "glClearIndex",
    "glClearStencil",
    "glClipPlane",
    "glColor3b",
    "glColor3d",
    "glColor3f",
    "glColor3i",
    "glColor3s",
    "glColor3ub",
    "glColor3ui",
    "glColor3us",
    "glColor4b",
    "glColor4d",
    "glColor4f",
    "glColor4i",
    "glColor4s",
    "glColor4ub",
    "glColor4ui",
    "glColor4us",
    "glColor3bv",
    "glColor3dv",
    "glColor3fv",
    "glColor3iv",
    "glColor3sv",
    "glColor3ubv",
    "glColor3uiv",
    "glColor3usv",
    "glColor4bv",
    "glColor4dv",
    "glColor4fv",
    "glColor4iv",
    "glColor4sv",
    "glColor4ubv",
    "glColor4uiv",
    "glColor4usv",
    "glColorMask",
    "glColorMaterial",
    "glColorPointer",
    "glColorTableEXT",
    "glColorSubTableEXT",
    "glCompileShader",
    "glCopyPixels",
    "glCopyTexImage1D",
    "glCopyTexImage2D",
    "glCopyTexSubImage1D",
    "glCopyTexSubImage2D",
    "glCreateProgram",
    "glCreateShader",
    "glCullFace",
    "glDeleteLists",
    "glDeleteTextures",
    "glDepthFunc",
    "glDepthMask",
    "glDepthRange",
    "glDrawArrays",
    "glDrawBuffer",
    "glDrawElements",
    "glDrawPixels",
    "glEdgeFlag",
    "glEdgeFlagv",
    "glEdgeFlagPointer",
    "glEnable",
    "glEnableVertexAttribArray",
    "glDisable",
    "glEnableClientState",
    "glDisableClientState",
    "glEvalCoord1d",
    "glEvalCoord1f",
    "glEvalCoord2d",
    "glEvalCoord2f",
    "glEvalCoord1dv",
    "glEvalCoord2dv",
    "glEvalCoord1fv",
    "glEvalCoord2fv",
    "glEvalMesh1",
    "glEvalMesh2",
    "glEvalPoint1", 
    "glEvalPoint2", 
    "glFeedbackBuffer",
    "glFinish",
    "glFlush",
    "glFogf",
    "glFogi",
    "glFogfv",
    "glFogiv",
    "glFrontFace",
    "glFrustum",
    "glGenBuffers",
    "glGenLists",
    "glGenTextures",
    "glGenVertexArrays",
    "glGetBooleanv",
    "glGetClipPlane",
    "glGetColorTableEXT",
    "glGetColorTableParameterfvEXT",
    "glGetColorTableParameterivEXT",
    "glGetDoublev",
    "glGetError",
    "glGetFloatv",
    "glGetIntegerv",
    "glGetLightfv",
    "glGetLightiv",
    "glGetMapdv",
    "glGetMapfv",
    "glGetMapiv",
    "glGetMaterialfv",
    "glGetMaterialiv",
    "glGetPixelMapfv",
    "glGetPixelMapuiv",
    "glGetPixelMapusv",
    "glGetPointerv",
    "glGetPolygonStipple",
    "glGetString",
    "glGetTexEnvfv",
    "glGetTexEnviv",
    "glGetTexGendv",
    "glGetTexGenfv",
    "glGetTexGeniv",
    "glGetTexImage",
    "glGetTexLevelParameterfv",
    "glGetTexLevelParameteriv",
    "glGetTexParameterfv",
    "glGetTexParameteriv",
    "glGetUniformLocation",
    "glHint",
    "glIndexd",
    "glIndexf",
    "glIndexi",
    "glIndexs",
    "glIndexdv",
    "glIndexfv",
    "glIndexiv",
    "glIndexsv",
    "glIndexMask",
    "glIndexPointer",
    "glInitNames",
    "glInterleavedArrays",
    "glIsEnabled",
    "glIsList",
    "glIsTexture",
    "glLightf",
    "glLighti",
    "glLightfv",
    "glLightiv",
    "glLightModelf",
    "glLightModelfv",
    "glLightModeli",
    "glLightModeliv",
    "glLineStipple",
    "glLineWidth",
    "glLinkProgram",
    "glListBase",
    "glLoadIdentity",
    "glLoadMatrixd",
    "glLoadMatrixf",
    "glLoadName",
    "glLogicOp",
    "glMap1d",
    "glMap1f",
    "glMap2d",
    "glMap2f",
    "glMapGrid1d",
    "glMapGrid1f",
    "glMapGrid2d",
    "glMapGrid2f",
    "glMaterialf",
    "glMateriali",
    "glMaterialfv",
    "glMaterialiv",
    "glMatrixMode",
    "glMultMatrixd",
    "glMultMatrixf",
    "glNewList",
    "glEndList",
    "glNormal3b",
    "glNormal3d",
    "glNormal3f",
    "glNormal3i",
    "glNormal3s",
    "glNormal3bv",
    "glNormal3dv",
    "glNormal3fv",
    "glNormal3iv",
    "glNormal3sv",
    "glNormalPointer",
    "glOrtho",
    "glPassThrough",
    "glPixelMapfv",
    "glPixelMapuiv",
    "glPixelMapusv",
    "glPixelStoref",
    "glPixelStorei",
    "glPixelTransferf",
    "glPixelTransferi",
    "glPixelZoom",
    "glPointSize",
    "glPolygonMode",
    "glPolygonOffset",
    "glPolygonStipple",
    "glPrioritizeTextures",
    "glPushAttrib",
    "glPopAttrib",
    "glPushClientAttrib",
    "glPopClientAttrib",
    "glPushMatrix",
    "glPopMatrix",
    "glPushName",
    "glPopName",
    "glRasterPos2d",
    "glRasterPos2f",
    "glRasterPos2i",
    "glRasterPos2s",
    "glRasterPos3d",
    "glRasterPos3f",
    "glRasterPos3i",
    "glRasterPos3s",
    "glRasterPos4d",
    "glRasterPos4f",
    "glRasterPos4i",
    "glRasterPos4s",
    "glRasterPos2dv",
    "glRasterPos2fv",
    "glRasterPos2iv",
    "glRasterPos2sv",
    "glRasterPos3dv",
    "glRasterPos3fv",
    "glRasterPos3iv",
    "glRasterPos3sv",
    "glRasterPos4dv",
    "glRasterPos4fv",
    "glRasterPos4iv",
    "glRasterPos4sv",
    "glReadBuffer",
    "glReadPixels",
    "glRectd",
    "glRectf",
    "glRecti",
    "glRects",
    "glRectdv",
    "glRectfv",
    "glRectiv",
    "glRectsv",
    "glRenderMode",
    "glRotated",
    "glRotatef",
    "glScaled",
    "glScalef",
    "glScissor",
    "glSelectBuffer",
    "glShadeModel",
    "glShaderSource",
    "glStencilFunc",
    "glStencilMask",
    "glStencilOp",
    "glTexCoord1d",
    "glTexCoord1f",
    "glTexCoord1i",
    "glTexCoord1s",
    "glTexCoord2d",
    "glTexCoord2i",
    "glTexCoord2s",
    "glTexCoord3d",
    "glTexCoord3f",
    "glTexCoord3i",
    "glTexCoord3s",
    "glTexCoord4d",
    "glTexCoord4f",
    "glTexCoord4i",
    "glTexCoord4s",
    "glTexCoord1dv",
    "glTexCoord1fv",
    "glTexCoord1iv",
    "glTexCoord1sv",
    "glTexCoord2dv",
    "glTexCoord2fv",
    "glTexCoord2iv",
    "glTexCoord2sv",
    "glTexCoord3dv",
    "glTexCoord3fv",
    "glTexCoord3iv",
    "glTexCoord3sv",
    "glTexCoord4dv",
    "glTexCoord4fv",
    "glTexCoord4iv",
    "glTexCoord4sv", 
    "glTexCoordPointer",
    "glTexEnvf",
    "glTexEnvi",
    "glTexEnvfv",
    "glTexEnviv",
    "glTexGend",
    "glTexGenf",
    "glTexGeni",
    "glTexGendv",
    "glTexGenfv",
    "glTexGeniv",
    "glTexImage1D",
    "glTexImage2D",
    "glTexParameterf",
    "glTexParameteri",
    "glTexParameterfv",
    "glTexParameteriv",
    "glTexSubImage1D",
    "glTexSubImage2D",
    "glTranslated",
    "glTranslatef",
    "glUniform1i",
    "glUniform3f",
    "glUseProgram",
    "glVertex2d", 
    "glVertex2f",
    "glVertex2i",
    "glVertex2s",
    "glVertex3d",
    "glVertex3f",
    "glVertex3i",
    "glVertex3s",
    "glVertex4d",
    "glVertex4f",
    "glVertex4i",
    "glVertex4s", 
    "glVertex2dv", 
    "glVertex2fv",
    "glVertex2iv",
    "glVertex2sv",
    "glVertex3dv",
    "glVertex3fv",
    "glVertex3iv",
    "glVertex3sv",
    "glVertex4dv",
    "glVertex4fv",
    "glVertex4iv",
    "glVertex4sv", 
    "glVertexAttribPointer",
    "glVertexPointer",
    "glViewport",
};

size_t NH_GFX_OPENGL_COMMAND_NAMES_PP_COUNT = 
    sizeof(NH_GFX_OPENGL_COMMAND_NAMES_PP) / sizeof(NH_GFX_OPENGL_COMMAND_NAMES_PP[0]);

// COMMAND PARAMETER COUNTS ========================================================================

const int NH_GFX_OPENGL_COMMAND_PARAMETER_COUNTS_P[] = {
    2, // glAccum
    1, // glActiveTexture
    4, // glAddSwapHintRectWIN
    2, // glAlphaFunc
    3, // glAreTexturesResident
    1, // glArrayElement
    2, // glAttachShader
    1, // glBegin
    0, // glEnd
    2, // glBindBuffer
    2, // glBindTexture
    1, // glBindVertexArray
    7, // glBitmap
    2, // glBlendFunc
    4, // glBufferData
    1, // glCallList
    3, // glCallLists
    1, // glClear
    4, // glClearAccum
    4, // glClearColor
    1, // glClearDepth
    1, // glClearIndex
    1, // glClearStencil
    2, // glClipPlane
    3, // glColor3b
    3, // glColor3d
    3, // glColor3f
    3, // glColor3i
    3, // glColor3s
    3, // glColor3ub
    3, // glColor3ui
    3, // glColor3us
    4, // glColor4b
    4, // glColor4d
    4, // glColor4f
    4, // glColor4i
    4, // glColor4s
    4, // glColor4ub
    4, // glColor4ui
    4, // glColor4us
    1, // glColor3bv
    1, // glColor3dv
    1, // glColor3fv
    1, // glColor3iv
    1, // glColor3sv
    1, // glColor3ubv
    1, // glColor3uiv
    1, // glColor3usv
    1, // glColor4bv
    1, // glColor4dv
    1, // glColor4fv
    1, // glColor4iv
    1, // glColor4sv
    1, // glColor4ubv
    1, // glColor4uiv
    1, // glColor4usv
    4, // glColorMask
    2, // glColorMaterial
    4, // glColorPointer
    6, // glColorTableEXT
    6, // glColorSubTableEXT
    1, // glCompileShader
    5, // glCopyPixels
    7, // glCopyTexImage1D
    8, // glCopyTexImage2D
    6, // glCopyTexSubImage1D
    8, // glCopyTexSubImage2D
    0, // glCreateProgram
    1, // glCreateShader
    1, // glCullFace
    2, // glDeleteLists
    2, // glDeleteTextures
    1, // glDepthFunc
    1, // glDepthMask
    2, // glDepthRange
    3, // glDrawArrays
    1, // glDrawBuffer
    4, // glDrawElements
    5, // glDrawPixels
    1, // glEdgeFlag
    1, // glEdgeFlagv
    2, // glEdgeFlagPointer
    1, // glEnable
    1, // glEnableVertexAttribArray
    1, // glDisable
    1, // glEnableClientState
    1, // glDisableClientState
    1, // glEvalCoord1d
    1, // glEvalCoord1f
    2, // glEvalCoord2d
    2, // glEvalCoord2f
    1, // glEvalCoord1dv
    1, // glEvalCoord2dv
    1, // glEvalCoord1fv
    1, // glEvalCoord2fv
    3, // glEvalMesh1
    5, // glEvalMesh2
    1, // glEvalPoint1
    2, // glEvalPoint2
    3, // glFeedbackBuffer
    0, // glFinish
    0, // glFlush
    2, // glFogf
    2, // glFogi
    2, // glFogfv
    2, // glFogiv
    1, // glFrontFace
    6, // glFrustum
    2, // glGenBuffers
    1, // glGenLists
    2, // glGenTextures
    2, // glGenVertexArrays
    2, // glGetBooleanv
    2, // glGetClipPlane
    4, // glGetColorTableEXT
    3, // glGetColorTableParameterfvEXT
    3, // glGetColorTableParameterivEXT
    2, // glGetDoublev
    0, // glGetError
    2, // glGetFloatv
    2, // glGetIntegerv
    3, // glGetLightfv
    3, // glGetLightiv
    3, // glGetMapdv
    3, // glGetMapfv
    3, // glGetMapiv
    3, // glGetMaterialfv
    3, // glGetMaterialiv
    2, // glGetPixelMapfv
    2, // glGetPixelMapuiv
    2, // glGetPixelMapusv
    2, // glGetPointerv
    1, // glGetPolygonStipple
    1, // glGetString
    3, // glGetTexEnvfv
    3, // glGetTexEnviv
    3, // glGetTexGendv
    3, // glGetTexGenfv
    3, // glGetTexGeniv
    5, // glGetTexImage
    4, // glGetTexLevelParameterfv
    4, // glGetTexLevelParameteriv
    3, // glGetTexParameterfv
    3, // glGetTexParameteriv
    2, // glGetUniformLocation
    2, // glHint
    1, // glIndexd
    1, // glIndexf
    1, // glIndexi
    1, // glIndexs
    1, // glIndexdv
    1, // glIndexfv
    1, // glIndexiv
    1, // glIndexsv
    1, // glIndexMask
    3, // glIndexPointer
    0, // glInitNames
    3, // glInterleavedArrays
    1, // glIsEnabled
    1, // glIsList
    1, // glIsTexture
    3, // glLightf
    3, // glLighti
    3, // glLightfv
    3, // glLightiv
    2, // glLightModelf
    2, // glLightModelfv
    2, // glLightModeli
    2, // glLightModeliv
    2, // glLineStipple
    1, // glLineWidth
    1, // glLinkProgram
    1, // glListBase
    0, // glLoadIdentity
    1, // glLoadMatrixd
    1, // glLoadMatrixf
    1, // glLoadName
    1, // glLogicOp
    6, // glMap1d
    6, // glMap1f
    10, // glMap2d
    10, // glMap2f
    3, // glMapGrid1d
    3, // glMapGrid1f
    6, // glMapGrid2d
    6, // glMapGrid2f
    3, // glMaterialf
    3, // glMateriali
    3, // glMaterialfv
    3, // glMaterialiv
    1, // glMatrixMode
    1, // glMultMatrixd
    1, // glMultMatrixf
    2, // glNewList
    0, // glEndList
    3, // glNormal3b
    3, // glNormal3d
    3, // glNormal3f
    3, // glNormal3i
    3, // glNormal3s
    1, // glNormal3bv
    1, // glNormal3dv
    1, // glNormal3fv
    1, // glNormal3iv
    1, // glNormal3sv
    3, // glNormalPointer
    6, // glOrtho
    1, // glPassThrough
    3, // glPixelMapfv
    3, // glPixelMapuiv
    3, // glPixelMapusv
    2, // glPixelStoref
    2, // glPixelStorei
    2, // glPixelTransferf
    2, // glPixelTransferi
    2, // glPixelZoom
    1, // glPointSize
    2, // glPolygonMode
    2, // glPolygonOffset
    1, // glPolygonStipple
    3, // glPrioritizeTextures
    1, // glPushAttrib
    0, // glPopAttrib
    1, // glPushClientAttrib
    0, // glPopClientAttrib
    0, // glPushMatrix
    0, // glPopMatrix
    1, // glPushName
    0, // glPopName
    2, // glRasterPos2d
    2, // glRasterPos2f
    2, // glRasterPos2i
    2, // glRasterPos2s
    3, // glRasterPos3d
    3, // glRasterPos3f
    3, // glRasterPos3i
    3, // glRasterPos3s
    4, // glRasterPos4d
    4, // glRasterPos4f
    4, // glRasterPos4i
    4, // glRasterPos4s
    1, // glRasterPos2dv
    1, // glRasterPos2fv
    1, // glRasterPos2iv
    1, // glRasterPos2sv
    1, // glRasterPos3dv
    1, // glRasterPos3fv
    1, // glRasterPos3iv
    1, // glRasterPos3sv
    1, // glRasterPos4dv
    1, // glRasterPos4fv
    1, // glRasterPos4iv
    1, // glRasterPos4sv
    1, // glReadBuffer
    7, // glReadPixels
    4, // glRectd
    4, // glRectf
    4, // glRecti
    4, // glRects
    2, // glRectdv
    2, // glRectfv
    2, // glRectiv
    2, // glRectsv
    1, // glRenderMode
    4, // glRotated
    4, // glRotatef
    3, // glScaled
    3, // glScalef
    4, // glScissor
    2, // glSelectBuffer
    1, // glShadeModel
    4, // glShaderSource
    3, // glStencilFunc
    1, // glStencilMask
    3, // glStencilOp
    1, // glTexCoord1d
    1, // glTexCoord1f
    1, // glTexCoord1i
    1, // glTexCoord1s
    2, // glTexCoord2d
    2, // glTexCoord2i
    2, // glTexCoord2s
    3, // glTexCoord3d
    3, // glTexCoord3f
    3, // glTexCoord3i
    3, // glTexCoord3s
    4, // glTexCoord4d
    4, // glTexCoord4f
    4, // glTexCoord4i
    4, // glTexCoord4s
    1, // glTexCoord1dv
    1, // glTexCoord1fv
    1, // glTexCoord1iv
    1, // glTexCoord1sv
    1, // glTexCoord2dv
    1, // glTexCoord2fv
    1, // glTexCoord2iv
    1, // glTexCoord2sv
    1, // glTexCoord3dv
    1, // glTexCoord3fv
    1, // glTexCoord3iv
    1, // glTexCoord3sv
    1, // glTexCoord4dv
    1, // glTexCoord4fv
    1, // glTexCoord4iv
    1, // glTexCoord4sv
    4, // glTexCoordPointer
    3, // glTexEnvf
    3, // glTexEnvi
    3, // glTexEnvfv
    3, // glTexEnviv
    3, // glTexGend
    3, // glTexGenf
    3, // glTexGeni
    3, // glTexGendv
    3, // glTexGenfv
    3, // glTexGeniv
    8, // glTexImage1D
    9, // glTexImage2D
    3, // glTexParameterf
    3, // glTexParameteri
    3, // glTexParameterfv
    3, // glTexParameteriv
    7, // glTexSubImage1D
    9, // glTexSubImage2D
    3, // glTranslated
    3, // glTranslatef
    2, // glUniform1i
    4, // glUniform3f
    1, // glUseProgram
    2, // glVertex2d
    2, // glVertex2f
    2, // glVertex2i
    2, // glVertex2s
    3, // glVertex3d
    3, // glVertex3f
    3, // glVertex3i
    3, // glVertex3s
    4, // glVertex4d
    4, // glVertex4f
    4, // glVertex4i
    4, // glVertex4s
    1, // glVertex2dv
    1, // glVertex2fv
    1, // glVertex2iv
    1, // glVertex2sv
    4, // glVertex3dv
    4, // glVertex3fv
    4, // glVertex3iv
    4, // glVertex3sv
    1, // glVertex4dv
    1, // glVertex4fv
    1, // glVertex4iv
    1, // glVertex4sv
    6, // glVertexAttribPointer
    4, // glVertexPointer
    4, // glViewport
};

size_t NH_GFX_OPENGL_COMMAND_PARAMETER_COUNTS_P_COUNT = 
    sizeof(NH_GFX_OPENGL_COMMAND_PARAMETER_COUNTS_P) / sizeof(NH_GFX_OPENGL_COMMAND_PARAMETER_COUNTS_P[0]);

// GET TYPE ========================================================================================

NH_GFX_OPENGL_COMMAND_E nh_gfx_getOpenGLCommandType(
    char *name_p)
{
    NH_GFX_OPENGL_COMMAND_E *index_p = nh_core_getFromHashMap(&NH_GFX_INDEXMAP.OpenGLCommandNames, name_p);
    if (index_p == NULL) {return NH_GFX_OPENGL_COMMAND_UNDEFINED;}
    return *index_p;
}

int nh_gfx_getOpenGLCommandParameterCount(
    NH_GFX_OPENGL_COMMAND_E type)
{
    return NH_GFX_OPENGL_COMMAND_PARAMETER_COUNTS_P[type];
}

