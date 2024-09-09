#ifndef NH_HTML_ABOUT_H
#define NH_HTML_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nh-html nh-html
 *  @ingroup lib
 *  @brief The nh-html library implements HTML parsing and the HTML event loop.
 */

/** @defgroup lib_nh-html_version Version
 *  @ingroup lib_nh-html
 */
/** @defgroup lib_nh-html_macros Macros 
 *  @ingroup lib_nh-html
 */
/** @defgroup lib_nh-html_enums Enumerations
 *  @ingroup lib_nh-html
 */
/** @defgroup lib_nh-html_structs Data Structures
 *  @ingroup lib_nh-html
 */
/** @defgroup lib_nh-html_functions Functions
 *  @ingroup lib_nh-html
 */
/** @defgroup lib_nh-html_typedefs Typedefs
 *  @ingroup lib_nh-html
 */
/** @defgroup lib_nh-html_vars Variables
 *  @ingroup lib_nh-html
 */

/** @addtogroup lib_nh-html_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_HTML_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_HTML_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_HTML_MINOR_VERSION 3

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_HTML_PATCH_VERSION 0

    /**
     * Version array. 
     */
    extern int NH_HTML_VERSION_P[4];

/** @} */

#endif
