#ifndef NH_API_H
#define NH_API_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-core.h"
#include "nh-renderer.h"
#include "nh-html.h"
#include "nh-ecmascript.h"

#endif

/** @{ */

    /**
     * The API version is used for documenting backwards-incompatible API changes. 
     */
    #define NETZHAUT_API_VERSION 0

    /**
     * The major version is used for documenting the completion of big functionalities. 
     */
    #define NETZHAUT_MAJOR_VERSION 2

    /**
     * The minor version is used for documenting the addition of small functionalities or consequential changes. 
     */
    #define NETZHAUT_MINOR_VERSION 7

    /**
     * The patch version is used for documenting bugfixes or non-consequential changes. 
     */
    #define NETZHAUT_PATCH_VERSION 1

/** @} */
/** @{ */

    /**
     * Version array. 
     */
    extern int NETZHAUT_VERSION_P[4];

/** @} */

#endif // NH_API_H

