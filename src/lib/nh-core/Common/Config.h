#ifndef NH_CORE_COMMON_CONFIG_H
#define NH_CORE_COMMON_CONFIG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"
#include "Includes.h"

#endif

/** @addtogroup lib_nh-core_enums
 *  @{
 */

    typedef enum NH_CORE_SETTING_E {
        NH_CORE_SETTING_LOADER_UNLOAD,
        NH_CORE_SETTING_E_COUNT,
    } NH_CORE_SETTING_E;

/** @} */

/** @addtogroup lib_nh-core_structs
 *  @{
 */

    typedef struct nh_core_Config {
        NH_BOOL loaderUnload;
    } nh_core_Config;

/** @} */


/** @addtogroup lib_nh-core_functions
 *  @{
 */

    nh_core_Config nh_core_getConfig(
    );

/** @} */

#endif // NH_CORE_COMMON_CONFIG_H
