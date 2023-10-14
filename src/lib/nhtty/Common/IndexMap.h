#ifndef NH_TTY_INDEXMAP_H
#define NH_TTY_INDEXMAP_H

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

    typedef struct nh_tty_IndexMap {
        nh_HashMap SettingNames;
    } nh_tty_IndexMap;

/** @} */

/** @addtogroup lib_nhcore_vars
 *  @{
 */

    extern nh_tty_IndexMap NH_TTY_INDEXMAP;

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_TTY_RESULT nh_tty_createIndexMap(
    );

    void nh_tty_freeIndexMap(
    );

/** @} */

#endif // NH_TTY_INDEXMAP_H 
