#ifndef NH_URL_ABOUT_H
#define NH_URL_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nh-url nh-url
 *  @ingroup lib
 *  @brief The nh-url library implements the WHATWG URL standard.
 */

/** @defgroup lib_nh-url_version Version
 *  @ingroup lib_nh-url
 */
/** @defgroup lib_nh-url_macros Macros 
 *  @ingroup lib_nh-url
 */
/** @defgroup lib_nh-url_enums Enumerations
 *  @ingroup lib_nh-url
 */
/** @defgroup lib_nh-url_structs Data Structures
 *  @ingroup lib_nh-url
 */
/** @defgroup lib_nh-url_functions Functions
 *  @ingroup lib_nh-url
 */
/** @defgroup lib_nh-url_typedefs Typedefs
 *  @ingroup lib_nh-url
 */
/** @defgroup lib_nh-url_vars Variables
 *  @ingroup lib_nh-url
 */

/** @addtogroup lib_nh-url_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_URL_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_URL_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_URL_MINOR_VERSION 2

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_URL_PATCH_VERSION 1

    /**
     * Version array.
     */
    extern int NH_URL_VERSION_P[4];

/** @} */

#endif
