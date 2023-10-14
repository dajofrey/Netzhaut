#ifndef NH_CORE_ABOUT_H
#define NH_CORE_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nhcore nhcore
 *  @brief The nhcore library implements fundamentals used throughout netzhaut.
 *  @ingroup lib
 */

/** @defgroup lib_nhcore_version Version
 *  @ingroup lib_nhcore
 */
/** @defgroup lib_nhcore_macros Macros 
 *  @ingroup lib_nhcore
 */
/** @defgroup lib_nhcore_enums Enumerations
 *  @ingroup lib_nhcore
 */
/** @defgroup lib_nhcore_structs Data Structures
 *  @ingroup lib_nhcore
 */
/** @defgroup lib_nhcore_functions Functions
 *  @ingroup lib_nhcore
 */
/** @defgroup lib_nhcore_typedefs Typedefs
 *  @ingroup lib_nhcore
 */
/** @defgroup lib_nhcore_vars Variables
 *  @ingroup lib_nhcore
 */

/** @addtogroup lib_nhcore_version
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
