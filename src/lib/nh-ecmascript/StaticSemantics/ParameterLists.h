#ifndef NH_ECMASCRIPT_PARAMETER_LISTS_H
#define NH_ECMASCRIPT_PARAMETER_LISTS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

#include "../Parser/CST.h"

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    int nh_ecmascript_getExpectedArgumentCount(
        nh_ecmascript_ParseNode *Node_p
    );

/** @} */

#endif
