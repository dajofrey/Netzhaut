#ifndef NH_TERMINAL_ABOUT_H
#define NH_TERMINAL_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nhterminal nhterminal
 *  @ingroup lib
 *  @brief The nhterminal library implements rendering of nhtty Pseudo Terminals. 
 */

/** @defgroup lib_nhterminal_version Version
 *  @ingroup lib_nhterminal
 */
/** @defgroup lib_nhterminal_macros Macros 
 *  @ingroup lib_nhterminal
 */
/** @defgroup lib_nhterminal_enums Enumerations
 *  @ingroup lib_nhterminal
 */
/** @defgroup lib_nhterminal_structs Data Structures
 *  @ingroup lib_nhterminal
 */
/** @defgroup lib_nhterminal_functions Functions
 *  @ingroup lib_nhterminal
 */
/** @defgroup lib_nhterminal_typedefs Typedefs
 *  @ingroup lib_nhterminal
 */
/** @defgroup lib_nhterminal_vars Variables
 *  @ingroup lib_nhterminal
 */

/** @addtogroup lib_nhterminal_version 
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_TERMINAL_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_TERMINAL_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_TERMINAL_MINOR_VERSION 7

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_TERMINAL_PATCH_VERSION 2

    /**
     * Version array.
     */
    extern int NH_TERMINAL_VERSION_P[4];

/** @} */

#endif
