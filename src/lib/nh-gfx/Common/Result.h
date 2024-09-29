#ifndef NH_GFX_COMMON_RESULT_H
#define NH_GFX_COMMON_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @{ */

    extern const char *NH_API_RESULTS_PP[];
    extern unsigned int NH_API_RESULTS_PP_COUNT;

/** @} */
/** @{ */

    NH_API_RESULT nh_vk_getResult(
        int result
    );

/** @} */

#endif // NH_GFX_COMMON_RESULT_H
