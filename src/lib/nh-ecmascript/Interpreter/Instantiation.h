#ifndef NH_ECMASCRIPT_INTERPRETER_INSTANTIATION_H
#define NH_ECMASCRIPT_INTERPRETER_INSTANTIATION_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Parser/AST.h"
#include "../Engine/Environment.h"

// https://tc39.es/ecma262/#sec-globaldeclarationinstantiation
nh_ecmascript_Completion nh_ecmascript_globalDeclarationInstantiation(
    nh_ecmascript_ASTNode *ScriptBody_p, nh_ecmascript_Environment *GlobalEnvironment_p
);

nh_ecmascript_Object *nh_ecmascript_instantiateFunctionObject(
    nh_ecmascript_Environment *Scope_p, nh_ecmascript_ParseNode *Node_p
);

nh_ecmascript_Object *nh_ecmascript_instantiateOrdinaryFunctionObject(
    nh_ecmascript_Environment *Scope_p, nh_ecmascript_ParseNode *Node_p
); 

#endif
