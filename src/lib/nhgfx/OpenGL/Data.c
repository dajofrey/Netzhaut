// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Data.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// CREATE ==========================================================================================

nh_opengl_Data nh_opengl_initData()
{
NH_GFX_BEGIN()

    nh_opengl_Data Data;
    Data.type = 0;
    Data.size = 0;
    Data.p = NULL;
    Data.pp = NULL;
    Data.autoFree = NH_TRUE;

NH_GFX_END(Data)
}

static nh_opengl_Data *nh_opengl_createData(
    nh_opengl_Data *Data_p, NH_GFX_OPENGL_DATA_E type, int size, NH_BYTE *data_p, NH_BYTE **data_pp)
{
NH_GFX_BEGIN()

    if (!Data_p) {
        Data_p = nh_core_allocate(sizeof(nh_opengl_Data));
        NH_GFX_CHECK_MEM_2(NULL, Data_p)
        *Data_p = nh_opengl_initData();
        Data_p->autoFree = NH_TRUE;
    }

    Data_p->type = type;
    Data_p->size = size;
    Data_p->p = Data_p->size ? nh_core_allocate(Data_p->size+1) : data_p;
    Data_p->pp = data_pp;

    if (Data_p->size) {
        NH_GFX_CHECK_MEM_2(NULL, Data_p->p)
        memset(Data_p->p, 0, Data_p->size+1);
        memcpy(Data_p->p, data_p, Data_p->size);
    }

NH_GFX_END(Data_p)
}

nh_opengl_Data *nh_opengl_disableDataAutoFree(
    nh_opengl_Data *Data_p)
{
NH_GFX_BEGIN()

    Data_p->autoFree = NH_FALSE;

NH_GFX_END(Data_p)
}

void nh_opengl_freeData(
    nh_opengl_Data *Data_p)
{
NH_GFX_BEGIN()

    if (!Data_p) {NH_GFX_SILENT_END()}
    if (Data_p->p != NULL && Data_p->size > 0) {nh_core_free(Data_p->p);}
    nh_core_free(Data_p);

NH_GFX_SILENT_END()
}

// DATA ============================================================================================

nh_opengl_Data *nh_opengl_glboolean(
    nh_opengl_Data *Data_p, GLboolean data)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLBOOLEAN, sizeof(GLboolean), (NH_BYTE*)&data, NULL))
}

nh_opengl_Data *nh_opengl_glbyte(
    nh_opengl_Data *Data_p, GLbyte *data_p, int length)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLBYTE, sizeof(GLbyte)*length, (NH_BYTE*)data_p, NULL))
}

nh_opengl_Data *nh_opengl_glubyte(
    nh_opengl_Data *Data_p, GLubyte *data_p, int length)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLUBYTE, sizeof(GLubyte)*length, (NH_UNSIGNED_BYTE*)data_p, NULL))
}

nh_opengl_Data *nh_opengl_glint(
    nh_opengl_Data *Data_p, GLint data)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLINT, sizeof(GLint), (NH_BYTE*)&data, NULL))
}

nh_opengl_Data *nh_opengl_gluint(
    nh_opengl_Data *Data_p, GLuint data)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLUINT, sizeof(GLuint), (NH_BYTE*)&data, NULL))
}

nh_opengl_Data *nh_opengl_glfloat(
    nh_opengl_Data *Data_p, GLfloat data)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLFLOAT, sizeof(GLfloat), (NH_BYTE*)&data, NULL))
}

nh_opengl_Data *nh_opengl_glsizei(
    nh_opengl_Data *Data_p, GLsizei data)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLSIZEI, sizeof(GLsizei), (NH_BYTE*)&data, NULL))
}

nh_opengl_Data *nh_opengl_glsizeiptr(
    nh_opengl_Data *Data_p, GLsizeiptr data)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLSIZEIPTR, sizeof(GLsizeiptr), (NH_BYTE*)&data, NULL))
}

nh_opengl_Data *nh_opengl_glchar(
    nh_opengl_Data *Data_p, GLchar *data_p, int length, GLchar **data_pp)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLBYTE, length, (NH_BYTE*)data_p, (NH_BYTE**)data_pp))
}

nh_opengl_Data *nh_opengl_glenum(
    nh_opengl_Data *Data_p, GLenum data)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_GLENUM, sizeof(GLenum), (NH_BYTE*)&data, NULL))
}

nh_opengl_Data *nh_opengl_pointer(
    nh_opengl_Data *Data_p, void *p)
{
NH_GFX_BEGIN()
NH_GFX_END(nh_opengl_createData(Data_p, NH_GFX_OPENGL_DATA_POINTER, 0, p, NULL))
}

