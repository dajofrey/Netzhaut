#ifndef NH_URL_TEST_H
#define NH_URL_TEST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#endif

/** @addtogroup lib_nhurl_functions
 *  @{
 */

    int nh_url_testHostParser(
        int arguments, nh_make_TestArgument *Arguments_p, NH_BYTE *output_p, int maxOutputLength
    );

    int nh_url_testURLParser(
        int arguments, nh_make_TestArgument *Arguments_p, NH_BYTE *output_p, int maxOutputLength
    );

/** @} */

#endif // NH_URL_TEST_H 
