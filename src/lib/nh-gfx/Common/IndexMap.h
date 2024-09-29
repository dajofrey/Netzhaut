#ifndef NH_GFX_COMMON_INDEXMAP_H
#define NH_GFX_COMMON_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"
#include "../../nh-core/Util/HashMap.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_gfx_IndexMap {
        nh_HashMap OpenGLCommandNames;
    } nh_gfx_IndexMap;

/** @} */

/** @addtogroup lib_nh-gfx_vars
 *  @{
 */

    extern nh_gfx_IndexMap NH_GFX_INDEXMAP;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_API_RESULT nh_gfx_createIndexMap(
    );

    void nh_gfx_freeIndexMap(
    );

/** @} */

#endif // NH_GFX_COMMON_INDEXMAP_H 
