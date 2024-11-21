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

/** @addtogroup lib_nh-gfx_enums
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

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_gfx_OpenGLData {
        NH_GFX_OPENGL_DATA_E type;
	void *p;
        void *pp;
        int size;
        bool autoFree;
    } nh_gfx_OpenGLData;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    nh_gfx_OpenGLData *nh_gfx_glboolean(
        nh_gfx_OpenGLData *Data_p, GLboolean data
    );
    
    nh_gfx_OpenGLData *nh_gfx_glbyte(
        nh_gfx_OpenGLData *Data_p, GLbyte *data_p, int length
    );

    nh_gfx_OpenGLData *nh_gfx_glubyte(
        nh_gfx_OpenGLData *Data_p, GLubyte *data_p, int length
    );
    
    nh_gfx_OpenGLData *nh_gfx_glint(
        nh_gfx_OpenGLData *Data_p, GLint data
    );
    
    nh_gfx_OpenGLData *nh_gfx_gluint(
        nh_gfx_OpenGLData *Data_p, GLuint data
    );
    
    nh_gfx_OpenGLData *nh_gfx_glfloat(
        nh_gfx_OpenGLData *Data_p, GLfloat data
    );

    nh_gfx_OpenGLData *nh_gfx_glsizei(
        nh_gfx_OpenGLData *Data_p, GLsizei data
    );

    nh_gfx_OpenGLData *nh_gfx_glsizeiptr(
        nh_gfx_OpenGLData *Data_p, GLsizeiptr data
    );

    nh_gfx_OpenGLData *nh_gfx_glchar(
        nh_gfx_OpenGLData *Data_p, GLchar *data_p, int length, GLchar **data_pp
    );

    nh_gfx_OpenGLData *nh_gfx_glenum(
        nh_gfx_OpenGLData *Data_p, GLenum data
    );

    nh_gfx_OpenGLData *nh_gfx_glpointer(
        nh_gfx_OpenGLData *Data_p, void *p
    );

    nh_gfx_OpenGLData nh_gfx_initOpenGLData(
    );

    nh_gfx_OpenGLData *nh_gfx_disableOpenGLDataAutoFree(
        nh_gfx_OpenGLData *Data_p
    );

    void nh_gfx_freeOpenGLData(
        nh_gfx_OpenGLData *Data_p
    );

/** @} */

#endif
