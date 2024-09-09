#ifndef NH_NETWORK_H
#define NH_NETWORK_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-core.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @defgroup api_nh-network nh-network
 *  @brief TODO.
 */

/** @addtogroup api_nh-network
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_NETWORK_RESULT {
        NH_NETWORK_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_NETWORK_ERROR_NULL_POINTER,
        NH_NETWORK_ERROR_BAD_STATE,
        NH_NETWORK_ERROR_MEMORY_ALLOCATION,
    } NH_NETWORK_RESULT;

/** @} */

#endif // NH_NETWORK_H
