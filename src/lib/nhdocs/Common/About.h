#ifndef NH_DOCS_COMMON_ABOUT_H
#define NH_DOCS_COMMON_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @defgroup lib_nhdocs nhdocs
 *  @brief TODO.
 */

/** @defgroup lib_nhdocs_changelog Changelog
 *  @ingroup nhdocs
 */
/** @defgroup lib_nhdocs_macros Macros 
 *  @ingroup nhdocs
 */
/** @defgroup lib_nhdocs_enums Enumerations
 *  @ingroup nhdocs
 */
/** @defgroup lib_nhdocs_structs Data Structures
 *  @ingroup nhdocs
 */
/** @defgroup lib_nhdocs_functions Functions
 *  @ingroup nhdocs
 */
/** @defgroup lib_nhdocs_typedefs Typedefs
 *  @ingroup nhdocs
 */
/** @defgroup lib_nhdocs_vars Variables
 *  @ingroup nhdocs
 */

/** @addtogroup lib_nhdocs_changelog
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes.
     */
    #define NH_DOCS_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities.
     */
    #define NH_DOCS_MAJOR_VERSION 2

    /**
     * The minor version is used for the addition of small functionalities.
     */
    #define NH_DOCS_MINOR_VERSION 6

    /**
     * The patch version is used for bugfixes or non-consequential changes.
     */
    #define NH_DOCS_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_DOCS_VERSION_P[4];

/** @} */

#endif // NH_DOCS_COMMON_ABOUT_H
