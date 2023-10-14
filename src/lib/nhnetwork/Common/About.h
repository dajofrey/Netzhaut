#ifndef NH_NETWORK_ABOUT_H
#define NH_NETWORK_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nhnetwork nhnetwork
 *  @ingroup lib
 *  @brief The nhnetwork library implements network routines.
 */

/** @defgroup lib_nhnetwork_version Version
 *  @ingroup lib_nhnetwork
 */
/** @defgroup lib_nhnetwork_macros Macros 
 *  @ingroup lib_nhnetwork
 */
/** @defgroup lib_nhnetwork_enums Enumerations
 *  @ingroup lib_nhnetwork
 */
/** @defgroup lib_nhnetwork_structs Data Structures
 *  @ingroup lib_nhnetwork
 */
/** @defgroup lib_nhnetwork_functions Functions
 *  @ingroup lib_nhnetwork
 */
/** @defgroup lib_nhnetwork_typedefs Typedefs
 *  @ingroup lib_nhnetwork
 */
/** @defgroup lib_nhnetwork_vars Variables
 *  @ingroup lib_nhnetwork
 */

/** @addtogroup lib_nhnetwork_version
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
