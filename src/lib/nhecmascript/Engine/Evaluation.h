#ifndef NH_ECMASCRIPT_EVALUATION_H
#define NH_ECMASCRIPT_EVALUATION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Completion.h"
#include "Parser.h"

#endif

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

    nh_ecmascript_Completion nh_ecmascript_evaluateScriptBody(
        nh_ecmascript_ParseNode *Node_p
    );

/** @} */

#endif
