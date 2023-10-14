#ifndef NH_ENCODING_INDEXMAP_H
#define NH_ENCODING_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nhcore/Util/HashMap.h"

#endif

/** @addtogroup lib_nhencoding_structs
 *  @{
 */

    typedef struct nh_encoding_IndexMap {
        nh_HashMap Labels;
    } nh_encoding_IndexMap;

/** @} */

/** @addtogroup lib_nhencoding_vars
 *  @{
 */

    extern nh_encoding_IndexMap NH_ENCODING_INDEXMAP;

/** @} */

/** @addtogroup lib_nhencoding_functions
 *  @{
 */

    NH_ENCODING_RESULT nh_encoding_createIndexMap(
    );

    void nh_encoding_freeIndexMap(
    );

/** @} */

#endif 
