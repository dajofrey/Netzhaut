#ifndef NH_HTML_INDEXMAP_H
#define NH_HTML_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "../../nh-core/Util/HashMap.h"

#endif

/** @addtogroup lib_nh-html_structs
 *  @{
 */

    typedef struct nh_html_IndexMap {
        nh_core_HashMap Tags;
    } nh_html_IndexMap;

/** @} */

/** @addtogroup lib_nh-html_vars
 *  @{
 */

    extern nh_html_IndexMap NH_HTML_INDEXMAP;

/** @} */

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    NH_API_RESULT nh_html_createIndexMap(
    );

    void nh_html_freeIndexMap(
    );

/** @} */

#endif 
