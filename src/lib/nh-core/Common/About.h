#ifndef NH_CORE_ABOUT_H
#define NH_CORE_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nh-core nh-core
 *  @brief The nh-core library implements fundamentals used throughout netzhaut.
 *  @ingroup lib
 */

/** @defgroup lib_nh-core_version Version
 *  @ingroup lib_nh-core
 */
/** @defgroup lib_nh-core_macros Macros 
 *  @ingroup lib_nh-core
 */
/** @defgroup lib_nh-core_enums Enumerations
 *  @ingroup lib_nh-core
 */
/** @defgroup lib_nh-core_structs Data Structures
 *  @ingroup lib_nh-core
 */
/** @defgroup lib_nh-core_functions Functions
 *  @ingroup lib_nh-core
 */
/** @defgroup lib_nh-core_typedefs Typedefs
 *  @ingroup lib_nh-core
 */
/** @defgroup lib_nh-core_vars Variables
 *  @ingroup lib_nh-core
 */

/** @addtogroup lib_nh-core_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_CORE_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_CORE_MAJOR_VERSION 1

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_CORE_MINOR_VERSION 6

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_CORE_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_CORE_VERSION_P[4];

/** @} */

#endif
