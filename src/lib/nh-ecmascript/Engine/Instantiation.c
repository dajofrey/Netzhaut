// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Instantiation.h"
#include "TestAndCompare.h"
#include "OrdinaryFunctionObject.h"
#include "ExecutionContext.h"

#include "../Common/Macros.h"

#include "../StaticSemantics/BoundNames.h"
#include "../StaticSemantics/DeclaredNames.h"
#include "../StaticSemantics/ScopedDeclarations.h"
#include "../StaticSemantics/Misc.h"

#include "../../nh-core/System/Memory.h"

#include <stdio.h>
#include <string.h>

// INSTANTIATE FUNCTION OBJECT =====================================================================

nh_ecmascript_Object *nh_ecmascript_instantiateFunctionObject(
    nh_ecmascript_Environment *Scope_p, nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION :
            NH_ECMASCRIPT_END(nh_ecmascript_instantiateOrdinaryFunctionObject(Scope_p, Node_p))
        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_DECLARATION :
            break;
        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_DECLARATION :
            break;
        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_DECLARATION :
            break;
    }

NH_ECMASCRIPT_END(NULL)
}

// INSTANTIATE ORDINARY FUNCTION OBJECT ============================================================

// https://tc39.es/ecma262/#sec-runtime-semantics-instantiateordinaryfunctionobject
nh_ecmascript_Object *nh_ecmascript_instantiateOrdinaryFunctionObject(
    nh_ecmascript_Environment *Scope_p, nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Node_p->type != NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION) {NH_ECMASCRIPT_END(NULL)}

    if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) 
    {
        nh_encoding_UTF8String *Name_p = &Node_p->Value_p->String;

        // TODO Let sourceText be the source text matched by FunctionDeclaration.

        nh_ecmascript_Object *Function_p = nh_ecmascript_ordinaryFunctionCreate(
            &nh_ecmascript_getCurrentRealm()->Intrinsics.Function.Prototype, NULL, Node_p->Children.pp[1],
            Node_p->Children.pp[2], NH_ECMASCRIPT_THIS_MODE_NON_LEXICAL, Scope_p
        );

        nh_ecmascript_setFunctionName(Function_p, nh_ecmascript_wrapString(Name_p), NULL);
        nh_ecmascript_makeConstructor(Function_p, false, NULL);

        NH_ECMASCRIPT_END(Function_p)
    }
    else {

        nh_ecmascript_String Name;
        Name.p = "default";
        Name.length  = 7;

        // TODO Let sourceText be the source text matched by FunctionDeclaration.

        nh_ecmascript_Object *Function_p = nh_ecmascript_ordinaryFunctionCreate(
            &nh_ecmascript_getCurrentRealm()->Intrinsics.Function.Prototype, NULL, Node_p->Children.pp[1],
            Node_p->Children.pp[2], NH_ECMASCRIPT_THIS_MODE_NON_LEXICAL, Scope_p
        );

        nh_ecmascript_setFunctionName(Function_p, nh_ecmascript_wrapString(&Name), NULL);
        nh_ecmascript_makeConstructor(Function_p, false, NULL);

        NH_ECMASCRIPT_END(Function_p)
    }

NH_ECMASCRIPT_END(NULL)
}

// GLOBAL DECLARATION INSTANTIATION ================================================================

static bool nh_ecmascript_nameInList(
    nh_List *Names_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    bool inList = false;
    for (int j = 0; j < Names_p->size; ++j) {
        nh_encoding_UTF8String *Compare_p = Names_p->pp[j];
        if (!strcmp(Name_p->p, Compare_p->p)) {
            inList = true;
            break;
        }
    }

NH_ECMASCRIPT_END(inList)
}

// https://tc39.es/ecma262/#sec-globaldeclarationinstantiation
nh_ecmascript_Completion nh_ecmascript_globalDeclarationInstantiation(
    nh_ecmascript_ParseNode *ScriptBody_p, nh_ecmascript_Environment *GlobalEnvironment_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_List LexNames = nh_ecmascript_getLexicallyDeclaredNames(ScriptBody_p);
    nh_List VarNames = nh_ecmascript_getVarDeclaredNames(ScriptBody_p);

    for (int i = 0; i < LexNames.size; ++i) 
    {
        if (nh_ecmascript_hasVarDeclaration(GlobalEnvironment_p->Handle_p, LexNames.pp[i])) {
            NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
        }
        if (nh_ecmascript_hasLexicalDeclaration(GlobalEnvironment_p->Handle_p, LexNames.pp[i])) {
            NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
        }
        bool hasRestrictedGlobal = nh_ecmascript_hasRestrictedGlobalProperty(GlobalEnvironment_p->Handle_p, LexNames.pp[i]);
        if (hasRestrictedGlobal) {
            NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
        }
    }

    for (int i = 0; i < VarNames.size; ++i) 
    {
        if (nh_ecmascript_hasLexicalDeclaration(GlobalEnvironment_p->Handle_p, VarNames.pp[i])) {
            NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
        }
    }

    nh_List VarDeclarations = nh_ecmascript_getVarScopedDeclarations(ScriptBody_p);
    nh_List FunctionsToInitialize = nh_core_initList(16);
    nh_List DeclaredFunctionNames = nh_core_initList(16);

    for (int i = VarDeclarations.size - 1; i >= 0; --i) 
    {
        nh_ecmascript_ParseNode *Declaration_p = VarDeclarations.pp[i];

        if (Declaration_p->type != NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION
        &&  Declaration_p->type != NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING
        &&  Declaration_p->type != NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) {

            nh_List BoundNames = nh_ecmascript_getBoundNames(Declaration_p);
            nh_encoding_UTF8String *Name_p = BoundNames.pp[BoundNames.size - 1];

            if (!nh_ecmascript_nameInList(&DeclaredFunctionNames, Name_p)) {
                bool fnDefinable = nh_ecmascript_canDeclareGlobalFunction(GlobalEnvironment_p->Handle_p, Name_p);
                if (!fnDefinable) {NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())}
                nh_core_appendToList(&DeclaredFunctionNames, Name_p);
                nh_core_prependToList(&FunctionsToInitialize, Declaration_p);
            }

            nh_core_freeList(&BoundNames, false);
        }
    }

    nh_List DeclaredVarNames = nh_core_initList(16);
    for (int i = 0; i < VarDeclarations.size; ++i) 
    {
        nh_ecmascript_ParseNode *Declaration_p = VarDeclarations.pp[i];

        if (Declaration_p->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION
        ||  Declaration_p->type == NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING
        ||  Declaration_p->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) {

            nh_List BoundNames = nh_ecmascript_getBoundNames(Declaration_p);

            for (int j = 0; j < BoundNames.size; ++j) 
            {
                nh_encoding_UTF8String *Name_p = BoundNames.pp[j];

                if (!nh_ecmascript_nameInList(&DeclaredFunctionNames, Name_p)) {
                    bool vnDefinable = nh_ecmascript_canDeclareGlobalVar(GlobalEnvironment_p->Handle_p, Name_p);
                    if (!vnDefinable) {NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())}
                    if (!nh_ecmascript_nameInList(&DeclaredVarNames, Name_p)) {
                        nh_core_appendToList(&DeclaredVarNames, Name_p);
                    }
                }
            }

            nh_core_freeList(&BoundNames, false);
        }
    } 

    // TODO Annex B.3.3.2 adds additional steps at this point.

    nh_List LexDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(ScriptBody_p);

    for (int i = 0; i < LexDeclarations.size; ++i) 
    {
        nh_ecmascript_ParseNode *Declaration_p = LexDeclarations.pp[i];
        nh_List BoundNames = nh_ecmascript_getBoundNames(Declaration_p);

        for (int j = 0; j < BoundNames.size; ++j) {
            nh_encoding_UTF8String *Name_p = BoundNames.pp[j];
            if (nh_ecmascript_isConstantDeclaration(Declaration_p)) {
                nh_ecmascript_createImmutableBinding(GlobalEnvironment_p, Name_p, true);
            } else {
                nh_ecmascript_createMutableBinding(GlobalEnvironment_p, Name_p, false);
            }
        }

        nh_core_freeList(&BoundNames, false);
    }

    for (int i = 0; i < FunctionsToInitialize.size; ++i) 
    {
        nh_ecmascript_ParseNode *Function_p = FunctionsToInitialize.pp[i];
        nh_List BoundNames = nh_ecmascript_getBoundNames(Function_p);
        nh_encoding_UTF8String *Name_p = BoundNames.pp[BoundNames.size - 1];
        nh_ecmascript_Object *FunctionObject_p = nh_ecmascript_instantiateFunctionObject(GlobalEnvironment_p, Function_p);
        nh_ecmascript_createGlobalFunctionBinding(Name_p, nh_ecmascript_wrapObject(FunctionObject_p), false);
        nh_core_freeList(&BoundNames, false);
    }

    for (int i = 0; i < DeclaredVarNames.size; ++i) {
        nh_ecmascript_createGlobalVarBinding(GlobalEnvironment_p->Handle_p, DeclaredVarNames.pp[i], false);
    }

    nh_core_freeList(&LexNames, false);
    nh_core_freeList(&VarNames, false);
    nh_core_freeList(&DeclaredVarNames, false);
    nh_core_freeList(&DeclaredFunctionNames, false);
    nh_core_freeList(&FunctionsToInitialize, false);
    nh_core_freeList(&VarDeclarations, false);
    nh_core_freeList(&LexDeclarations, false);

NH_ECMASCRIPT_END(nh_ecmascript_normalEmptyCompletion())
}

