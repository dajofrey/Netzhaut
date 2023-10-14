#ifndef NH_ECMASCRIPT_MISCELLANEOUS_STATIC_SEMANTICS_H
#define NH_ECMASCRIPT_MISCELLANEOUS_STATIC_SEMANTICS_H

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

    NH_BOOL nh_ecmascript_isAnonymousFunctionDefinition(
        nh_ecmascript_ParseNode *Node_p
    );

    nh_encoding_UTF8String *nh_ecmascript_getStringValue(
        nh_ecmascript_ParseNode *Node_p
    );

    NH_BOOL nh_ecmascript_isConstantDeclaration(
        nh_ecmascript_ParseNode *Node_p
    );

/** @} */

#endif
