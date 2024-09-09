#ifndef NH_ENCODING_ABOUT_H
#define NH_ENCODING_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nh-encoding nh-encoding
 *  @ingroup lib
 *  @brief The nh-encoding library implements the ECMA-262 engine.
 */

/** @defgroup lib_nh-encoding_version Version
 *  @ingroup lib_nh-encoding
 */
/** @defgroup lib_nh-encoding_macros Macros 
 *  @ingroup lib_nh-encoding
 */
/** @defgroup lib_nh-encoding_enums Enumerations
 *  @ingroup lib_nh-encoding
 */
/** @defgroup lib_nh-encoding_structs Data Structures
 *  @ingroup lib_nh-encoding
 */
/** @defgroup lib_nh-encoding_functions Functions
 *  @ingroup lib_nh-encoding
 */
/** @defgroup lib_nh-encoding_typedefs Typedefs
 *  @ingroup lib_nh-encoding
 */
/** @defgroup lib_nh-encoding_vars Variables
 *  @ingroup lib_nh-encoding
 */

/** @addtogroup lib_nh-encoding_version 
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_ENCODING_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_ENCODING_MAJOR_VERSION 1

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_ENCODING_MINOR_VERSION 3

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_ENCODING_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_ENCODING_VERSION_P[4];

/** @} */

#endif
