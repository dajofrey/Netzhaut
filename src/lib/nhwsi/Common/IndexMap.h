#ifndef NH_WSI_INDEXMAP_H
#define NH_WSI_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "../../nhcore/Util/HashMap.h"

#endif

/** @addtogroup lib_nhcore_structs
 *  @{
 */

    typedef struct nh_wsi_IndexMap {
        nh_HashMap SettingNames;
    } nh_wsi_IndexMap;

/** @} */

/** @addtogroup lib_nhcore_vars
 *  @{
 */

    extern nh_wsi_IndexMap NH_WSI_INDEXMAP;

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_WSI_RESULT_E nh_wsi_createIndexMap(
    );

    void nh_wsi_freeIndexMap(
    );

/** @} */

#endif // NH_WSI_INDEXMAP_H 
