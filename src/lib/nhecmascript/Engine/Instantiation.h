#ifndef NH_ECMASCRIPT_INSTANTIATION_H
#define NH_ECMASCRIPT_INSTANTIATION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Environment.h"

#endif

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

    // https://tc39.es/ecma262/#sec-globaldeclarationinstantiation
    nh_ecmascript_Completion nh_ecmascript_globalDeclarationInstantiation(
        nh_ecmascript_ParseNode *ScriptBody_p, nh_ecmascript_Environment *GlobalEnvironment_p
    );

    nh_ecmascript_Object *nh_ecmascript_instantiateFunctionObject(
        nh_ecmascript_Environment *Scope_p, nh_ecmascript_ParseNode *Node_p
    );

    nh_ecmascript_Object *nh_ecmascript_instantiateOrdinaryFunctionObject(
        nh_ecmascript_Environment *Scope_p, nh_ecmascript_ParseNode *Node_p
    ); 

/** @} */

#endif
