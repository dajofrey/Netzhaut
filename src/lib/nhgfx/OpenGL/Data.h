#ifndef NH_GFX_OPENGL_DATA_H
#define NH_GFX_OPENGL_DATA_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#define NETZHAUT_OPENGL
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhgfx_enums
 *  @{
 */

    typedef enum NH_GFX_OPENGL_DATA_E {
        NH_GFX_OPENGL_DATA_GLBOOLEAN,
        NH_GFX_OPENGL_DATA_GLBYTE,
        NH_GFX_OPENGL_DATA_GLUBYTE,
        NH_GFX_OPENGL_DATA_GLSHORT,
        NH_GFX_OPENGL_DATA_GLUSHORT,
        NH_GFX_OPENGL_DATA_GLINT,
        NH_GFX_OPENGL_DATA_GLUINT,
        NH_GFX_OPENGL_DATA_GLFIXED,
        NH_GFX_OPENGL_DATA_GLINT64,
        NH_GFX_OPENGL_DATA_GLUINT64,
        NH_GFX_OPENGL_DATA_GLSIZEI,
        NH_GFX_OPENGL_DATA_GLENUM,
        NH_GFX_OPENGL_DATA_GLINTPTR,
        NH_GFX_OPENGL_DATA_GLSIZEIPTR,
        NH_GFX_OPENGL_DATA_GLSYNC,
        NH_GFX_OPENGL_DATA_GLBITFIELD,
        NH_GFX_OPENGL_DATA_GLHALF,
        NH_GFX_OPENGL_DATA_GLFLOAT,
        NH_GFX_OPENGL_DATA_GLCLAMPF,
        NH_GFX_OPENGL_DATA_GLDOUBLE,
        NH_GFX_OPENGL_DATA_GLCLAMPD,
        NH_GFX_OPENGL_DATA_POINTER,
    } NH_GFX_OPENGL_DATA_E;

/** @} */

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

    typedef struct nh_opengl_Data {
        NH_GFX_OPENGL_DATA_E type;
	void *p;
        void *pp;
        int size;
        NH_BOOL autoFree;
    } nh_opengl_Data;

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    nh_opengl_Data *nh_opengl_glboolean(
        nh_opengl_Data *Data_p, GLboolean data
    );
    
    nh_opengl_Data *nh_opengl_glbyte(
        nh_opengl_Data *Data_p, GLbyte *data_p, int length
    );

    nh_opengl_Data *nh_opengl_glubyte(
        nh_opengl_Data *Data_p, GLubyte *data_p, int length
    );
    
    nh_opengl_Data *nh_opengl_glint(
        nh_opengl_Data *Data_p, GLint data
    );
    
    nh_opengl_Data *nh_opengl_gluint(
        nh_opengl_Data *Data_p, GLuint data
    );
    
    nh_opengl_Data *nh_opengl_glfloat(
        nh_opengl_Data *Data_p, GLfloat data
    );

    nh_opengl_Data *nh_opengl_glsizei(
        nh_opengl_Data *Data_p, GLsizei data
    );

    nh_opengl_Data *nh_opengl_glsizeiptr(
        nh_opengl_Data *Data_p, GLsizeiptr data
    );

    nh_opengl_Data *nh_opengl_glchar(
        nh_opengl_Data *Data_p, GLchar *data_p, int length, GLchar **data_pp
    );

    nh_opengl_Data *nh_opengl_glenum(
        nh_opengl_Data *Data_p, GLenum data
    );

    nh_opengl_Data *nh_opengl_pointer(
        nh_opengl_Data *Data_p, void *p
    );

    nh_opengl_Data nh_opengl_initData(
    );

    nh_opengl_Data *nh_opengl_disableDataAutoFree(
        nh_opengl_Data *Data_p
    );

    void nh_opengl_freeData(
        nh_opengl_Data *Data_p
    );

/** @} */

#endif
