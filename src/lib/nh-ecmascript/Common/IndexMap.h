#ifndef NH_ECMASCRIPT_INDEXMAP_H
#define NH_ECMASCRIPT_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "../../nh-core/Util/HashMap.h"

#endif

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_IndexMap {
        nh_core_HashMap ParseNodeNames;
        nh_core_HashMap ReservedWords;
    } nh_ecmascript_IndexMap;

/** @} */

/** @addtogroup lib_nh-ecmascript_vars
 *  @{
 */

    extern nh_ecmascript_IndexMap NH_ECMASCRIPT_INDEXMAP;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    NH_API_RESULT nh_ecmascript_createIndexMap(
    );

    void nh_ecmascript_freeIndexMap(
    );

/** @} */

#endif 
