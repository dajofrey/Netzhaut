#ifndef NH_ECMASCRIPT_BOUND_NAMES_H
#define NH_ECMASCRIPT_BOUND_NAMES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Engine/Parser.h"

#endif

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

// correponds to https://tc39.es/ecma262/#sec-syntax-directed-operations-scope-analysis

    nh_List nh_ecmascript_getBoundNames(
        nh_ecmascript_ParseNode *Node_p
    );

    void nh_ecmascript_freeBoundNames(
        nh_List *Names_p
    );

/** @} */

#endif
