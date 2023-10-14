#ifndef NH_MAKE_OPTIONS_H
#define NH_MAKE_OPTIONS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Source.h"

#include "../Common/Includes.h"
#include "../Parser/Parser.h"
#include "../Parser/Variables.h"

#endif

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    NH_MAKE_RESULT nh_make_addBuildOption(
        nh_make_Runtime *Runtime_p, NH_BYTE *name_p
    );

    NH_MAKE_RESULT nh_make_addTestOption(
        nh_make_Runtime *Runtime_p, NH_BYTE *name_p
    );

    NH_MAKE_RESULT nh_make_parseOption(
        nh_make_Runtime *Runtime_p, int optionLength, NH_BYTE **argv_pp, int *advance_p
    );

/** @} */

#endif
