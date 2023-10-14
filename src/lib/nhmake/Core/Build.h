#ifndef NH_MAKE_BUILD_H
#define NH_MAKE_BUILD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Parser/Functions.h"

#endif

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    NH_MAKE_RESULT nh_make_build(
        nh_make_Runtime *Runtime_p, NH_BYTE *name_p
    ); 

/** @} */

#endif
