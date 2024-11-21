// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Data.h"

#include "../../nh-core/System/Memory.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// CREATE ==========================================================================================

nh_gfx_OpenGLData nh_gfx_initOpenGLData()
{
    nh_gfx_OpenGLData Data;
    Data.type = 0;
    Data.size = 0;
    Data.p = NULL;
    Data.pp = NULL;
    Data.autoFree = true;
    return Data;
}

static nh_gfx_OpenGLData *nh_gfx_createOpenGLData(
    nh_gfx_OpenGLData *Data_p, NH_GFX_OPENGL_DATA_E type, int size, char *data_p, char **data_pp)
{
    if (!Data_p) {
        Data_p = nh_core_allocate(sizeof(nh_gfx_OpenGLData));
        NH_CORE_CHECK_MEM_2(NULL, Data_p)
        *Data_p = nh_gfx_initOpenGLData();
        Data_p->autoFree = true;
    }

    Data_p->type = type;
    Data_p->size = size;
    Data_p->p = Data_p->size ? nh_core_allocate(Data_p->size+1) : data_p;
    Data_p->pp = data_pp;

    if (Data_p->size) {
        NH_CORE_CHECK_MEM_2(NULL, Data_p->p)
        memset(Data_p->p, 0, Data_p->size+1);
        memcpy(Data_p->p, data_p, Data_p->size);
    }

    return Data_p;
}

nh_gfx_OpenGLData *nh_gfx_disableOpenGLDataAutoFree(
    nh_gfx_OpenGLData *Data_p)
{
    Data_p->autoFree = false;
    return Data_p;
}

void nh_gfx_freeOpenGLData(
    nh_gfx_OpenGLData *Data_p)
{
    if (!Data_p) {return ;}
    if (Data_p->p != NULL && Data_p->size > 0) {nh_core_free(Data_p->p);}
    nh_core_free(Data_p);
    return;
}

// DATA ============================================================================================

nh_gfx_OpenGLData *nh_gfx_glboolean(
    nh_gfx_OpenGLData *Data_p, GLboolean data)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLBOOLEAN, sizeof(GLboolean), (char*)&data, NULL);
}

nh_gfx_OpenGLData *nh_gfx_glbyte(
    nh_gfx_OpenGLData *Data_p, GLbyte *data_p, int length)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLBYTE, sizeof(GLbyte)*length, (char*)data_p, NULL);
}

nh_gfx_OpenGLData *nh_gfx_glubyte(
    nh_gfx_OpenGLData *Data_p, GLubyte *data_p, int length)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLUBYTE, sizeof(GLubyte)*length, (unsigned char*)data_p, NULL);
}

nh_gfx_OpenGLData *nh_gfx_glint(
    nh_gfx_OpenGLData *Data_p, GLint data)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLINT, sizeof(GLint), (char*)&data, NULL);
}

nh_gfx_OpenGLData *nh_gfx_gluint(
    nh_gfx_OpenGLData *Data_p, GLuint data)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLUINT, sizeof(GLuint), (char*)&data, NULL);
}

nh_gfx_OpenGLData *nh_gfx_glfloat(
    nh_gfx_OpenGLData *Data_p, GLfloat data)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLFLOAT, sizeof(GLfloat), (char*)&data, NULL);
}

nh_gfx_OpenGLData *nh_gfx_glsizei(
    nh_gfx_OpenGLData *Data_p, GLsizei data)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLSIZEI, sizeof(GLsizei), (char*)&data, NULL);
}

nh_gfx_OpenGLData *nh_gfx_glsizeiptr(
    nh_gfx_OpenGLData *Data_p, GLsizeiptr data)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLSIZEIPTR, sizeof(GLsizeiptr), (char*)&data, NULL);
}

nh_gfx_OpenGLData *nh_gfx_glchar(
    nh_gfx_OpenGLData *Data_p, GLchar *data_p, int length, GLchar **data_pp)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLBYTE, length, (char*)data_p, (char**)data_pp);
}

nh_gfx_OpenGLData *nh_gfx_glenum(
    nh_gfx_OpenGLData *Data_p, GLenum data)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_GLENUM, sizeof(GLenum), (char*)&data, NULL);
}

nh_gfx_OpenGLData *nh_gfx_glpointer(
    nh_gfx_OpenGLData *Data_p, void *p)
{
    return nh_gfx_createOpenGLData(Data_p, NH_GFX_OPENGL_DATA_POINTER, 0, p, NULL);
}

