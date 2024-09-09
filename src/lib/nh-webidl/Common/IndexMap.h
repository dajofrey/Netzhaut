#ifndef NH_WEBIDL_INDEXMAP_H
#define NH_WEBIDL_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "../../nh-core/Util/HashMap.h"

#endif

/** @addtogroup lib_nh-webidl_structs
 *  @{
 */

    typedef struct nh_webidl_IndexMap {
        nh_HashMap FragmentNames;
        nh_HashMap FragmentFunctionNames;
        nh_HashMap ParseNodeNames;
    } nh_webidl_IndexMap;

/** @} */

/** @addtogroup lib_nh-webidl_vars
 *  @{
 */

    extern nh_webidl_IndexMap NH_WEBIDL_INDEXMAP;

/** @} */

/** @addtogroup lib_nh-webidl_functions
 *  @{
 */

    NH_WEBIDL_RESULT nh_webidl_createIndexMap(
    );

    void nh_webidl_freeIndexMap(
    );

/** @} */

#endif 
