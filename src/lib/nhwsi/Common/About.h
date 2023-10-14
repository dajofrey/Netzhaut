#ifndef NH_WSI_ABOUT_H
#define NH_WSI_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nhwsi nhwsi
 *  @ingroup lib
 *  @brief The nhwsi library implements window system related routines.
 */

/** @defgroup lib_nhwsi_version Version
 *  @ingroup lib_nhwsi
 */
/** @defgroup lib_nhwsi_macros Macros 
 *  @ingroup lib_nhwsi
 */
/** @defgroup lib_nhwsi_enums Enumerations
 *  @ingroup lib_nhwsi
 */
/** @defgroup lib_nhwsi_structs Data Structures
 *  @ingroup lib_nhwsi
 */
/** @defgroup lib_nhwsi_functions Functions
 *  @ingroup lib_nhwsi
 */
/** @defgroup lib_nhwsi_typedefs Typedefs
 *  @ingroup lib_nhwsi
 */
/** @defgroup lib_nhwsi_vars Variables
 *  @ingroup lib_nhwsi
 */

/** @addtogroup lib_nhwsi_version
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes. 
     */
    #define NH_WSI_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities. 
     */
    #define NH_WSI_MAJOR_VERSION 0

    /**
     * The minor version is used for the addition of small functionalities or consequential changes. 
     */
    #define NH_WSI_MINOR_VERSION 5

    /**
     * The patch version is used for bugfixes or non-consequential changes. 
     */
    #define NH_WSI_PATCH_VERSION 0

    /**
     * Version array.
     */
    extern int NH_WSI_VERSION_P[4];

/** @} */

#endif
