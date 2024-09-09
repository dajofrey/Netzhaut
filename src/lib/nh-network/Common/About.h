#ifndef NH_NETWORK_ABOUT_H
#define NH_NETWORK_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nh-network nh-network
 *  @ingroup lib
 *  @brief The nh-network library implements network routines.
 */

/** @defgroup lib_nh-network_version Version
 *  @ingroup lib_nh-network
 */
/** @defgroup lib_nh-network_macros Macros 
 *  @ingroup lib_nh-network
 */
/** @defgroup lib_nh-network_enums Enumerations
 *  @ingroup lib_nh-network
 */
/** @defgroup lib_nh-network_structs Data Structures
 *  @ingroup lib_nh-network
 */
/** @defgroup lib_nh-network_functions Functions
 *  @ingroup lib_nh-network
 */
/** @defgroup lib_nh-network_typedefs Typedefs
 *  @ingroup lib_nh-network
 */
/** @defgroup lib_nh-network_vars Variables
 *  @ingroup lib_nh-network
 */

/** @addtogroup lib_nh-network_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_NETWORK_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_NETWORK_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_NETWORK_MINOR_VERSION 1

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_NETWORK_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_NETWORK_VERSION_P[4];

/** @} */

#endif
