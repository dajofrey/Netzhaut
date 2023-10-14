#ifndef NH_GFX_COMMON_CONFIG_H
#define NH_GFX_COMMON_CONFIG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

    typedef struct nh_gfx_Config {
        double gamma;
    } nh_gfx_Config;

/** @} */

/** @addtogroup lib_nhcore_vars
 *  @{
 */

    extern nh_gfx_Config NH_GFX_CONFIG;

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    void _nh_gfx_initConfig(
    );

/** @} */

#endif // NH_GFX_COMMON_CONFIG_H
