#ifndef NH_ECMASCRIPT_INDEXMAP_H
#define NH_ECMASCRIPT_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "../../nhcore/Util/HashMap.h"

#endif

/** @addtogroup lib_nhecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_IndexMap {
        nh_HashMap ParseNodeNames;
        nh_HashMap ReservedWords;
    } nh_ecmascript_IndexMap;

/** @} */

/** @addtogroup lib_nhecmascript_vars
 *  @{
 */

    extern nh_ecmascript_IndexMap NH_ECMASCRIPT_INDEXMAP;

/** @} */

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

    NH_ECMASCRIPT_RESULT nh_ecmascript_createIndexMap(
    );

    void nh_ecmascript_freeIndexMap(
    );

/** @} */

#endif 
