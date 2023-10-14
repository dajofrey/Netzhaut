#ifndef NH_RENDERER_ABOUT_H
#define NH_RENDERER_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nhrenderer nhrenderer
 *  @ingroup lib
 *  @brief The nhrenderer library implements HTML parsing and the HTML event loop.
 */

/** @defgroup lib_nhrenderer_version Version
 *  @ingroup lib_nhrenderer
 */
/** @defgroup lib_nhrenderer_macros Macros 
 *  @ingroup lib_nhrenderer
 */
/** @defgroup lib_nhrenderer_enums Enumerations
 *  @ingroup lib_nhrenderer
 */
/** @defgroup lib_nhrenderer_structs Data Structures
 *  @ingroup lib_nhrenderer
 */
/** @defgroup lib_nhrenderer_functions Functions
 *  @ingroup lib_nhrenderer
 */
/** @defgroup lib_nhrenderer_typedefs Typedefs
 *  @ingroup lib_nhrenderer
 */
/** @defgroup lib_nhrenderer_vars Variables
 *  @ingroup lib_nhrenderer
 */

/** @addtogroup lib_nhrenderer_version
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
