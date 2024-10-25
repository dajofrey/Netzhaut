#ifndef NH_ECMASCRIPT_SCOPED_DECLARATIONS_H
#define NH_ECMASCRIPT_SCOPED_DECLARATIONS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Engine/Parser.h"

#endif

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    nh_core_List nh_ecmascript_getLexicallyScopedDeclarations(
        nh_ecmascript_ParseNode *Node_p
    );
    
    nh_core_List nh_ecmascript_getTopLevelLexicallyScopedDeclarations(
        nh_ecmascript_ParseNode *Node_p
    );

    nh_core_List nh_ecmascript_getVarScopedDeclarations(
        nh_ecmascript_ParseNode *Node_p
    );

    nh_core_List nh_ecmascript_getTopLevelVarScopedDeclarations(
        nh_ecmascript_ParseNode *Node_p
    );

/** @} */

#endif
