#ifndef NH_GFX_COMMON_ABOUT_H
#define NH_GFX_COMMON_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @defgroup lib_nh-gfx nh-gfx
  * @ingroup lib
  * @brief The nh-gfx library implements the ECMA-262 engine.
  */

/** @defgroup lib_nh-gfx_version Version
 *  @ingroup lib_nh-gfx
 */
/** @defgroup lib_nh-gfx_macros Macros 
 *  @ingroup lib_nh-gfx
 */
/** @defgroup lib_nh-gfx_enums Enumerations
 *  @ingroup lib_nh-gfx
 */
/** @defgroup lib_nh-gfx_structs Data Structures
 *  @ingroup lib_nh-gfx
 */
/** @defgroup lib_nh-gfx_functions Functions
 *  @ingroup lib_nh-gfx
 */
/** @defgroup lib_nh-gfx_typedefs Typedefs
 *  @ingroup lib_nh-gfx
 */
/** @defgroup lib_nh-gfx_vars Variables
 *  @ingroup lib_nh-gfx
 */

/** @addtogroup lib_nh-gfx_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_API_GRAPHICS_BACKEND_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_GFX_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_GFX_MINOR_VERSION 8

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_GFX_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_GFX_VERSION_P[4];

/** @} */

#endif // NH_GFX_COMMON_ABOUT_H
