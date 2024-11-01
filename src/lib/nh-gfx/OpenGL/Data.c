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

nh_opengl_Data nh_opengl_initData()
{
    nh_opengl_Data Data;
    Data.type = 0;
    Data.size = 0;
    Data.p = NULL;
    Data.pp = NULL;
    Data.autoFree = true;
    return Data;
}

static nh_opengl_Data *nh_opengl_createData(
    nh_opengl_Data *Data_p, NH_GFX_OPENGL_DATA_E type, int size, char *data_p, char **data_pp)
{
    if (!Data_p) {
        Data_p = nh_core_allocate(sizeof(nh_opengl_Data));
        NH_CORE_CHECK_MEM_2(NULL, Data_p)
        *Data_p = nh_opengl_initData();
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

nh_opengl_Data *nh_opengl_disableDataAutoFree(
    nh_opengl_Data *Data_p)
{
    Data_p->autoFree = false;
    return Data_p;
}

void nh_opengl_freeData(
    nh_opengl_Data *Data_p)
{
    if (!Data_p) {return ;}
    if (Data_p->p != NULL && Data_p->size > 0) {nh_core_free(Data_p->p);}
    nh_core_free(Data_p);
    return;
}

// DATA ============================================================================================

nh_opengl_Data *nh_opengl_glboolean(
    nh_opengl_Data *Data_p, GLboolean data)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLBOOLEAN, sizeof(GLboolean), (char*)&data, NULL);
}

nh_opengl_Data *nh_opengl_glbyte(
    nh_opengl_Data *Data_p, GLbyte *data_p, int length)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLBYTE, sizeof(GLbyte)*length, (char*)data_p, NULL);
}

nh_opengl_Data *nh_opengl_glubyte(
    nh_opengl_Data *Data_p, GLubyte *data_p, int length)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLUBYTE, sizeof(GLubyte)*length, (unsigned char*)data_p, NULL);
}

nh_opengl_Data *nh_opengl_glint(
    nh_opengl_Data *Data_p, GLint data)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLINT, sizeof(GLint), (char*)&data, NULL);
}

nh_opengl_Data *nh_opengl_gluint(
    nh_opengl_Data *Data_p, GLuint data)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLUINT, sizeof(GLuint), (char*)&data, NULL);
}

nh_opengl_Data *nh_opengl_glfloat(
    nh_opengl_Data *Data_p, GLfloat data)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLFLOAT, sizeof(GLfloat), (char*)&data, NULL);
}

nh_opengl_Data *nh_opengl_glsizei(
    nh_opengl_Data *Data_p, GLsizei data)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLSIZEI, sizeof(GLsizei), (char*)&data, NULL);
}

nh_opengl_Data *nh_opengl_glsizeiptr(
    nh_opengl_Data *Data_p, GLsizeiptr data)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLSIZEIPTR, sizeof(GLsizeiptr), (char*)&data, NULL);
}

nh_opengl_Data *nh_opengl_glchar(
    nh_opengl_Data *Data_p, GLchar *data_p, int length, GLchar **data_pp)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLBYTE, length, (char*)data_p, (char**)data_pp);
}

nh_opengl_Data *nh_opengl_glenum(
    nh_opengl_Data *Data_p, GLenum data)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLENUM, sizeof(GLenum), (char*)&data, NULL);
}

nh_opengl_Data *nh_opengl_pointer(
    nh_opengl_Data *Data_p, void *p)
{
    return nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_POINTER, 0, p, NULL);
}

