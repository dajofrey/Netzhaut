#ifndef NH_ECMASCRIPT_ABOUT_H
#define NH_ECMASCRIPT_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nhecmascript nhecmascript
 *  @ingroup lib
 *  @brief The nhecmascript library implements the ECMAScript Language Specification (ECMA-262).
 */

/** @defgroup lib_nhecmascript_version Version
 *  @ingroup lib_nhecmascript
 */
/** @defgroup lib_nhecmascript_macros Macros 
 *  @ingroup lib_nhecmascript
 */
/** @defgroup lib_nhecmascript_enums Enumerations
 *  @ingroup lib_nhecmascript
 */
/** @defgroup lib_nhecmascript_structs Data Structures
 *  @ingroup lib_nhecmascript
 */
/** @defgroup lib_nhecmascript_functions Functions
 *  @ingroup lib_nhecmascript
 */
/** @defgroup lib_nhecmascript_typedefs Typedefs
 *  @ingroup lib_nhecmascript
 */
/** @defgroup lib_nhecmascript_vars Variables
 *  @ingroup lib_nhecmascript
 */

/** @addtogroup lib_nhecmascript_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_ECMASCRIPT_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_ECMASCRIPT_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_ECMASCRIPT_MINOR_VERSION 1

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_ECMASCRIPT_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_ECMASCRIPT_VERSION_P[4];

/** @} */

#endif
