#ifndef NH_DOCS_CHANGELOG_CONTEXT_H
#define NH_DOCS_CHANGELOG_CONTEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhdocs_typedefs
 *  @{
 */

    typedef nh_docs_Context *(*nh_docs_getContext_f)(
        nh_docs_ContextArray *Array_p, long unsigned int index
    );

    typedef nh_docs_Context *(*nh_docs_getContextFromDescription_f)(
        nh_docs_ContextArray *Array_p, NH_BYTE *name_p, NH_DOCS_CONTEXT_E type
    );
    
    typedef nh_docs_ContextArray *(*nh_docs_getContextArray_f)(
        nh_docs_Changelog *Changelog_p
    );

    typedef NH_DOCS_RESULT (*nh_docs_freeContextArray_f)(
        nh_docs_ContextArray *ContextArray_p
    );

/** @} */

/** @addtogroup lib_nhdocs_functions
 *  @{
 */

    nh_docs_Context *nh_docs_getContext(
        nh_docs_ContextArray *Array_p, long unsigned int index
    );
    
    nh_docs_ContextArray *nh_docs_getContextArray(
        nh_docs_Changelog *Changelog_p
    );

    NH_DOCS_RESULT nh_docs_freeContextArray(
        nh_docs_ContextArray *ContextArray_p
    );

/** @} */

#endif // NH_DOCS_CHANGELOG_CONTEXT_H
