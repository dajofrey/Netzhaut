#ifndef NH_MAKE_ABOUT_H
#define NH_MAKE_ABOUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

#endif

/** @defgroup bin_nhmake nhmake
 *  @ingroup bin
 *  @brief The build/development automisation tool used for netzhaut across all systems.
 *  It's based on <a href="https://selfmake.netzwerkz.org">selfmake</a>.
 */

/** @defgroup bin_nhmake_functions Functions
 *  @ingroup bin_nhmake
 */
/** @defgroup bin_nhmake_changelog Changelog
 *  @ingroup bin_nhmake
 */

/** @addtogroup bin_nhmake_changelog 
 *  @{
 */

    /**
     * The API version is used for backwards-incompatible API changes.
     */
    #define NH_MAKE_API_VERSION 0

    /**
     * The major version is used for the completion of big functionalities.
     */
    #define NH_MAKE_MAJOR_VERSION 1

    /**
     * The minor version is used for the addition of small functionalities or consequential changes.
     */
    #define NH_MAKE_MINOR_VERSION 2

    /**
     * The patch version is used for bugfixes or non-consequential changes.
     */
    #define NH_MAKE_PATCH_VERSION 0

/** @} */

#endif
