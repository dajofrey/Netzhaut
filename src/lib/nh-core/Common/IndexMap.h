#ifndef NH_CORE_INDEXMAP_H
#define NH_CORE_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"
#include "../Util/HashMap.h"

#endif

/** @addtogroup lib_nh-core_structs
 *  @{
 */

    typedef struct nh_IndexMap {
        nh_HashMap MediaTypes;
    } nh_IndexMap;

/** @} */

/** @addtogroup lib_nh-core_vars
 *  @{
 */

    extern nh_IndexMap NH_INDEXMAP;

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_createIndexMap(
    );

    void nh_core_freeIndexMap(
    );

/** @} */

#endif 
