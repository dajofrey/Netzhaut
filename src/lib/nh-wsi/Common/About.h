#ifndef NH_WSI_ABOUT_H
#define NH_WSI_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @defgroup lib_nh-wsi nh-wsi
 *  @ingroup lib
 *  @brief The nh-wsi library implements window system related routines.
 */

/** @defgroup lib_nh-wsi_version Version
 *  @ingroup lib_nh-wsi
 */
/** @defgroup lib_nh-wsi_macros Macros 
 *  @ingroup lib_nh-wsi
 */
/** @defgroup lib_nh-wsi_enums Enumerations
 *  @ingroup lib_nh-wsi
 */
/** @defgroup lib_nh-wsi_structs Data Structures
 *  @ingroup lib_nh-wsi
 */
/** @defgroup lib_nh-wsi_functions Functions
 *  @ingroup lib_nh-wsi
 */
/** @defgroup lib_nh-wsi_typedefs Typedefs
 *  @ingroup lib_nh-wsi
 */
/** @defgroup lib_nh-wsi_vars Variables
 *  @ingroup lib_nh-wsi
 */

/** @addtogroup lib_nh-wsi_version
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
