#ifndef NH_TERMINAL_INDEXMAP_H
#define NH_TERMINAL_INDEXMAP_H

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

    typedef struct nh_terminal_IndexMap {
        nh_HashMap SettingNames;
    } nh_terminal_IndexMap;

/** @} */

/** @addtogroup lib_nhcore_vars
 *  @{
 */

    extern nh_terminal_IndexMap NH_TERMINAL_INDEXMAP;

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_TERMINAL_RESULT nh_terminal_createIndexMap(
    );

    void nh_terminal_freeIndexMap(
    );

/** @} */

#endif // NH_TERMINAL_INDEXMAP_H 
