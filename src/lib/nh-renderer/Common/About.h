#ifndef NH_RENDERER_ABOUT_H
#define NH_RENDERER_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nh-renderer nh-renderer
 *  @ingroup lib
 *  @brief The nh-renderer library implements HTML parsing and the HTML event loop.
 */

/** @defgroup lib_nh-renderer_version Version
 *  @ingroup lib_nh-renderer
 */
/** @defgroup lib_nh-renderer_macros Macros 
 *  @ingroup lib_nh-renderer
 */
/** @defgroup lib_nh-renderer_enums Enumerations
 *  @ingroup lib_nh-renderer
 */
/** @defgroup lib_nh-renderer_structs Data Structures
 *  @ingroup lib_nh-renderer
 */
/** @defgroup lib_nh-renderer_functions Functions
 *  @ingroup lib_nh-renderer
 */
/** @defgroup lib_nh-renderer_typedefs Typedefs
 *  @ingroup lib_nh-renderer
 */
/** @defgroup lib_nh-renderer_vars Variables
 *  @ingroup lib_nh-renderer
 */

/** @addtogroup lib_nh-renderer_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_RENDERER_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_RENDERER_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_RENDERER_MINOR_VERSION 2

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_RENDERER_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_RENDERER_VERSION_P[4];

/** @} */

#endif
