#ifndef NH_DOM_ABOUT_H
#define NH_DOM_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nh-dom nh-dom
 *  @ingroup lib
 *  @brief The nh-dom library implements the DOM standard. 
 *  DOM defines a platform-neutral model for events, aborting activities, and node trees.
 */

/** @defgroup lib_nh-dom_version Version
 *  @ingroup lib_nh-dom
 */
/** @defgroup lib_nh-dom_macros Macros 
 *  @ingroup lib_nh-dom
 */
/** @defgroup lib_nh-dom_enums Enumerations
 *  @ingroup lib_nh-dom
 */
/** @defgroup lib_nh-dom_structs Data Structures
 *  @ingroup lib_nh-dom
 */
/** @defgroup lib_nh-dom_functions Functions
 *  @ingroup lib_nh-dom
 */
/** @defgroup lib_nh-dom_typedefs Typedefs
 *  @ingroup lib_nh-dom
 */
/** @defgroup lib_nh-dom_vars Variables
 *  @ingroup lib_nh-dom
 */

/** @addtogroup lib_nh-dom_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_DOM_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_DOM_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_DOM_MINOR_VERSION 4

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_DOM_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_DOM_VERSION_P[4];

/** @} */

#endif
