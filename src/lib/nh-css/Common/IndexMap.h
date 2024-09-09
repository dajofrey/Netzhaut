#ifndef NH_CSS_INDEXMAP_H
#define NH_CSS_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "../../nh-core/Util/HashMap.h"

#endif

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_IndexMap {
        nh_HashMap Colors;
        nh_HashMap Properties;
    } nh_css_IndexMap;

/** @} */

/** @addtogroup lib_nh-css_vars
 *  @{
 */

    extern nh_css_IndexMap NH_CSS_INDEXMAP;

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    NH_CSS_RESULT nh_css_createIndexMap(
    );

    void nh_css_freeIndexMap(
    );

/** @} */

#endif 
