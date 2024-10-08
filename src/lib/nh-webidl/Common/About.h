#ifndef NH_WEBIDL_ABOUT_H
#define NH_WEBIDL_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nh-webidl nh-webidl
 *  @ingroup lib
 *  @brief The nh-webidl library implements the ECMA-262 engine.
 */

/** @defgroup lib_nh-webidl_version Version
 *  @ingroup lib_nh-webidl
 */
/** @defgroup lib_nh-webidl_macros Macros 
 *  @ingroup lib_nh-webidl
 */
/** @defgroup lib_nh-webidl_enums Enumerations
 *  @ingroup lib_nh-webidl
 */
/** @defgroup lib_nh-webidl_structs Data Structures
 *  @ingroup lib_nh-webidl
 */
/** @defgroup lib_nh-webidl_functions Functions
 *  @ingroup lib_nh-webidl
 */
/** @defgroup lib_nh-webidl_typedefs Typedefs
 *  @ingroup lib_nh-webidl
 */
/** @defgroup lib_nh-webidl_vars Variables
 *  @ingroup lib_nh-webidl
 */

/** @addtogroup lib_nh-webidl_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_WEBIDL_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_WEBIDL_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_WEBIDL_MINOR_VERSION 1

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_WEBIDL_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_WEBIDL_VERSION_P[4];

/** @} */

#endif
