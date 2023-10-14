#ifndef NH_MAKE_SOURCE_H
#define NH_MAKE_SOURCE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhmake_structs
 *  @{
 */

    typedef struct nh_make_Source {
        nh_make_SourceContext *Context_p;
        NH_BYTE *path_p;
    } nh_make_Source;

    typedef struct nh_make_SourceArray {
        int length;
        nh_make_Source *Sources_p;
    } nh_make_SourceArray;

/** @} */

/** @addtogroup lib_nhmake_typedefs
 *  @{
 */

    typedef nh_make_SourceContextArray *(*nh_make_getSourceContextArray_f)(
        nh_make_Runtime *Runtime_p
    );
    
    typedef nh_make_SourceContext *(*nh_make_getSourceContext_f)(
        nh_make_Runtime *Runtime_p, NH_BYTE *name_p, NH_MAKE_SOURCE_CONTEXT type
    );

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    void nh_make_initSourceArray(
        nh_make_SourceArray *Array_p
    );

    NH_MAKE_RESULT nh_make_addSource(
        nh_make_Runtime *Runtime_p, nh_make_SourceArray *SourceArray_p, nh_make_Function *Function_p,
        NH_MAKE_SOURCE_CONTEXT type
    );

    void nh_make_initSourceContextArray(
        nh_make_SourceContextArray *Array_p
    );

    void nh_make_getIndentAfterSourceContext(
        NH_BYTE *name_p, NH_BYTE *buffer_p, int size, nh_make_SourceContextArray *Array_p
    );

    nh_make_SourceContextArray *nh_make_getSourceContextArray(
        nh_make_Runtime *Runtime_p
    );

    NH_MAKE_RESULT nh_make_addDefaultSourceContext(
        nh_make_Runtime *Runtime_p
    );

    NH_MAKE_RESULT nh_make_addSourceContext(
        nh_make_Runtime *Runtime_p, nh_make_Function *Function_p, NH_MAKE_SOURCE_CONTEXT type, int offset
    );

    NH_MAKE_RESULT nh_make_addCompileArguments(
        nh_make_Runtime *Runtime_p, nh_make_Function *Function_p, NH_MAKE_SOURCE_CONTEXT type
    );
    
    NH_MAKE_RESULT nh_make_addLinkArguments(
        nh_make_Runtime *Runtime_p, nh_make_Function *Function_p, NH_MAKE_SOURCE_CONTEXT type
    );

/** @} */

#endif
