#ifndef NH_MAKE_ABOUT_H
#define NH_MAKE_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nhmake nhmake
 *  @brief The nhmake library implements the build/versioning system used by netzhaut.
 */
/** @defgroup selfmakeAPI selfmake API 
 *  @brief The API code-documentation for selfmake. 
 */

/** @defgroup lib_nhmake_changelog Changelog
 *  @ingroup nhmake
 */
/** @defgroup lib_nhmake_macros Macros 
 *  @ingroup nhmake
 */
/** @defgroup lib_nhmake_enums Enumerations
 *  @ingroup nhmake
 */
/** @defgroup lib_nhmake_structs Data Structures
 *  @ingroup nhmake
 */
/** @defgroup lib_nhmake_functions Functions
 *  @ingroup nhmake
 */
/** @defgroup lib_nhmake_typedefs Typedefs
 *  @ingroup nhmake
 */
/** @defgroup lib_nhmake_vars Variables
 *  @ingroup nhmake
 */

/** @defgroup selfmakeAPIEnums Enumerations
 *  @ingroup nhmakeAPI
 */
/** @defgroup selfmakeAPIStructs Data Structures
 *  @ingroup nhmakeAPI
 */
/** @defgroup selfmakeAPIFunctions Functions
 *  @ingroup nhmakeAPI
 */
/** @defgroup selfmakeAPITypedefs Typedefs
 *  @ingroup nhmakeAPI
 */
/** @defgroup selfmakeAPIVars Variables
 *  @ingroup nhmakeAPI
 */

/** @addtogroup lib_nhmake_changelog
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes.
     */
    #define NH_MAKE_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities.
     */
    #define NH_MAKE_MAJOR_VERSION 2

    /**
     * The minor version is used for the addition of small functionalities.
     */
    #define NH_MAKE_MINOR_VERSION 6

    /**
     * The patch version is used for bugfixes or non-consequential changes.
     */
    #define NH_MAKE_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_MAKE_VERSION_P[4];

/** @} */

#endif
