#ifndef NH_CSS_ABOUT_H
#define NH_CSS_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nhcss nhcss
 *  @ingroup lib
 *  @brief The nhcss library implements the CSS based renderer.
 */

/** @defgroup lib_nhcss_version Version
 *  @ingroup lib_nhcss
 */
/** @defgroup lib_nhcss_macros Macros 
 *  @ingroup lib_nhcss
 */
/** @defgroup lib_nhcss_enums Enumerations
 *  @ingroup lib_nhcss
 */
/** @defgroup lib_nhcss_structs Data Structures
 *  @ingroup lib_nhcss
 */
/** @defgroup lib_nhcss_functions Functions
 *  @ingroup lib_nhcss
 */
/** @defgroup lib_nhcss_typedefs Typedefs
 *  @ingroup lib_nhcss
 */
/** @defgroup lib_nhcss_vars Variables
 *  @ingroup lib_nhcss
 */

/** @addtogroup lib_nhcss_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_CSS_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_CSS_MAJOR_VERSION 3

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_CSS_MINOR_VERSION 6

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_CSS_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_CSS_VERSION_P[4];

/** @} */

#endif
