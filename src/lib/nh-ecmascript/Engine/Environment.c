// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Environment.h"
#include "TestAndCompare.h"

#include "../StaticSemantics/BoundNames.h"
#include "../StaticSemantics/DeclaredNames.h"
#include "../StaticSemantics/ScopedDeclarations.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>

// DECLARATIVE ENVIRONMENT =========================================================================

nh_ecmascript_Environment *nh_ecmascript_newDeclarativeEnvironment(
    nh_ecmascript_Environment *Outer_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Environment *Environment_p = nh_core_allocate(sizeof(nh_ecmascript_Environment));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, Environment_p)

    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p = nh_core_allocate(sizeof(nh_ecmascript_DeclarativeEnvironment));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, DeclarativeEnvironment_p)

    DeclarativeEnvironment_p->Bindings = nh_core_initList(16);

    Environment_p->Handle_p = DeclarativeEnvironment_p;
    Environment_p->Outer_p  = Outer_p;
    Environment_p->type     = NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE;

NH_ECMASCRIPT_END(Environment_p)
}

static bool nh_ecmascript_declarativeEnvironmentHasBinding(
    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    for (int i = 0; i < DeclarativeEnvironment_p->Bindings.size; ++i) {
        nh_ecmascript_Binding *Binding_p = DeclarativeEnvironment_p->Bindings.pp[i];
        if (!strcmp(Binding_p->Name_p->p, Name_p->p)) {NH_ECMASCRIPT_END(true)}
    }

NH_ECMASCRIPT_END(false)
}

static nh_ecmascript_Completion nh_ecmascript_declarativeEnvironmentCreateMutableBinding(
    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p, nh_encoding_UTF8String *Name_p, bool deletable)
{
NH_ECMASCRIPT_BEGIN()

    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
    }

    nh_ecmascript_Binding *NewBinding_p = nh_core_allocate(sizeof(nh_ecmascript_Binding));
    NewBinding_p->_mutable    = true;
    NewBinding_p->deletable   = deletable;
    NewBinding_p->initialized = false;
    NewBinding_p->Name_p      = Name_p;
    NewBinding_p->Value       = nh_ecmascript_wrapUndefined();

    nh_core_appendToList(&DeclarativeEnvironment_p->Bindings, NewBinding_p);

NH_ECMASCRIPT_END(nh_ecmascript_normalEmptyCompletion())
}

static nh_ecmascript_Completion nh_ecmascript_declarativeEnvironmentCreateImmutableBinding(
    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p, nh_encoding_UTF8String *Name_p, bool strict)
{
NH_ECMASCRIPT_BEGIN()

    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
    }

    nh_ecmascript_Binding *NewBinding_p = nh_core_allocate(sizeof(nh_ecmascript_Binding));
    NewBinding_p->_mutable    = false;
    NewBinding_p->strict      = strict;
    NewBinding_p->initialized = false;
    NewBinding_p->Name_p      = Name_p;
    NewBinding_p->Value       = nh_ecmascript_wrapUndefined();

    nh_core_appendToList(&DeclarativeEnvironment_p->Bindings, NewBinding_p);

NH_ECMASCRIPT_END(nh_ecmascript_normalEmptyCompletion())
}

static nh_ecmascript_Completion nh_ecmascript_declarativeEnvironmentInitializeBinding(
    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Binding *Binding_p = NULL;
    for (int i = 0; i < DeclarativeEnvironment_p->Bindings.size; ++i) {
        Binding_p = DeclarativeEnvironment_p->Bindings.pp[i];
        if (!strcmp(Binding_p->Name_p->p, Name_p->p)) {break;}
    }

    if (Binding_p == NULL) {NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())}

    if (!Binding_p->initialized) {
        Binding_p->Value = Value;
        Binding_p->initialized = true;
    }

NH_ECMASCRIPT_END(nh_ecmascript_normalEmptyCompletion())
}

// OBJECT ENVIRONMENT ==============================================================================

nh_ecmascript_Environment *nh_ecmascript_newObjectEnvironment(
    nh_ecmascript_Object *Object_p, nh_ecmascript_Environment *Outer_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Environment *Environment_p = nh_core_allocate(sizeof(nh_ecmascript_Environment));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, Environment_p)

    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p = nh_core_allocate(sizeof(nh_ecmascript_ObjectEnvironment));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, ObjectEnvironment_p)

    ObjectEnvironment_p->BindingObject_p = Object_p;
    ObjectEnvironment_p->withEnvironment = false;

    Environment_p->Handle_p = ObjectEnvironment_p;
    Environment_p->Outer_p = Outer_p;
    Environment_p->type = NH_ECMASCRIPT_ENVIRONMENT_OBJECT;

NH_ECMASCRIPT_END(Environment_p)
}

static nh_ecmascript_Completion nh_ecmascript_objectEnvironmentCreateMutableBinding(
    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p, nh_encoding_UTF8String *Name_p, bool deletable)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Object *Bindings_p = ObjectEnvironment_p->BindingObject_p;

    nh_ecmascript_PropertyDescriptor Descriptor;
    Descriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
    Descriptor.enumerable   = true;
    Descriptor.configurable = deletable;
    Descriptor.Fields.Data.writable = true;
    Descriptor.Fields.Data.Value    = nh_ecmascript_wrapUndefined();

NH_ECMASCRIPT_END(nh_ecmascript_abstractDefinePropertyOrThrow(Bindings_p, nh_ecmascript_wrapString(Name_p), Descriptor))
}

static nh_ecmascript_Completion nh_ecmascript_objectEnvironmentSetMutableBinding(
    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value, bool _throw)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Object *Bindings_p = ObjectEnvironment_p->BindingObject_p;
    bool stillExists = nh_ecmascript_abstractHasProperty(Bindings_p, nh_ecmascript_wrapString(Name_p));
    if (!stillExists && _throw) {NH_ECMASCRIPT_END(nh_ecmascript_throwReferenceError())}

NH_ECMASCRIPT_END(nh_ecmascript_abstractSet(Bindings_p, nh_ecmascript_wrapString(Name_p), Value, _throw))
}

static nh_ecmascript_Completion nh_ecmascript_objectEnvironmentInitializeBinding(
    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_ecmascript_objectEnvironmentSetMutableBinding(ObjectEnvironment_p, Name_p, Value, false))
}

static bool nh_ecmascript_objectEnvironmentHasBinding(
    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Object *Bindings_p = ObjectEnvironment_p->BindingObject_p;
    bool foundBinding = nh_ecmascript_abstractHasProperty(Bindings_p, nh_ecmascript_wrapString(Name_p));
    if (!foundBinding) {NH_ECMASCRIPT_END(false)}
    if (!ObjectEnvironment_p->withEnvironment) {NH_ECMASCRIPT_END(false)}

// Let unscopables be ? Get(bindings, @@unscopables).
// If Type(unscopables) is Object, then
// 
//     Let blocked be ! ToBoolean(? Get(unscopables, N)).
//     If blocked is true, return false.

NH_ECMASCRIPT_END(true)
}

// GLOBAL ENVIRONMENT ==============================================================================

nh_ecmascript_Environment *nh_ecmascript_newGlobalEnvironment(
    nh_ecmascript_Object *GlobalObject_p, nh_ecmascript_Object *ThisValue_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Environment *Abstract_p = nh_core_allocate(sizeof(nh_ecmascript_Environment));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, Abstract_p)

    nh_ecmascript_Environment *ObjectEnv_p = nh_ecmascript_newObjectEnvironment(GlobalObject_p, NULL);
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, ObjectEnv_p)

    nh_ecmascript_Environment *DeclarativeEnv_p = nh_ecmascript_newDeclarativeEnvironment(NULL);
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, DeclarativeEnv_p)

    nh_ecmascript_GlobalEnvironment *GlobalEnv_p = nh_core_allocate(sizeof(nh_ecmascript_GlobalEnvironment));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, GlobalEnv_p)

    GlobalEnv_p->ObjectEnvironment_p = ObjectEnv_p;
    GlobalEnv_p->GlobalThisValue_p = ThisValue_p;
    GlobalEnv_p->DeclarativeEnvironment_p = DeclarativeEnv_p;
    GlobalEnv_p->VarNames = nh_core_initList(32);

    Abstract_p->Handle_p = GlobalEnv_p;
    Abstract_p->Outer_p = NULL;
    Abstract_p->type = NH_ECMASCRIPT_ENVIRONMENT_GLOBAL;

NH_ECMASCRIPT_END(Abstract_p)
}

bool nh_ecmascript_canDeclareGlobalFunction(
    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p = GlobalEnvironment_p->ObjectEnvironment_p->Handle_p;
    nh_ecmascript_Object *GlobalObject_p = ObjectEnvironment_p->BindingObject_p;

    nh_ecmascript_PropertyDescriptor Descriptor = 
        GlobalObject_p->InternalMethods_p->getOwnProperty_f(GlobalObject_p, nh_ecmascript_wrapString(Name_p));

    if (Descriptor.type == -1) {

    }
    if (Descriptor.configurable) {NH_ECMASCRIPT_END(true)}
    if (nh_ecmascript_isDataDescriptor(Descriptor)) {
        if (Descriptor.Fields.Data.writable && Descriptor.enumerable) {
            NH_ECMASCRIPT_END(true)
        } 
    }

NH_ECMASCRIPT_END(false)
}

nh_ecmascript_Completion nh_ecmascript_createGlobalFunctionBinding(
    nh_encoding_UTF8String *Name_p, nh_ecmascript_Any V, NH_ECMASCRIPT_BOOLEAN configurable)
{
NH_ECMASCRIPT_BEGIN()

    // TODO

NH_ECMASCRIPT_END(nh_ecmascript_normalEmptyCompletion())
}

bool nh_ecmascript_canDeclareGlobalVar(
    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p = GlobalEnvironment_p->ObjectEnvironment_p->Handle_p;
    nh_ecmascript_Object *GlobalObject_p = ObjectEnvironment_p->BindingObject_p;

    bool hasProperty = nh_ecmascript_abstractHasOwnProperty(GlobalObject_p, nh_ecmascript_wrapString(Name_p));

    if (hasProperty) {NH_ECMASCRIPT_END(true)}

NH_ECMASCRIPT_END((bool)nh_ecmascript_isExtensible(GlobalObject_p).Value.Payload.handle_p)
}

nh_ecmascript_Completion nh_ecmascript_createGlobalVarBinding(
    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN deletable)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p = GlobalEnvironment_p->ObjectEnvironment_p->Handle_p;
    nh_ecmascript_Object *GlobalObject_p = ObjectEnvironment_p->BindingObject_p;

    bool hasProperty = nh_ecmascript_abstractHasOwnProperty(GlobalObject_p, nh_ecmascript_wrapString(Name_p));
    bool extensible = nh_ecmascript_abstractIsExtensible(GlobalObject_p);

    if (!hasProperty && extensible) {
        nh_ecmascript_objectEnvironmentCreateMutableBinding(ObjectEnvironment_p, Name_p, deletable);
        nh_ecmascript_objectEnvironmentInitializeBinding(ObjectEnvironment_p, Name_p, nh_ecmascript_wrapUndefined());
    }

    nh_List *VarDeclaredNames_p = &GlobalEnvironment_p->VarNames;
    
    bool containsName = false;
    for (int i = 0; i < VarDeclaredNames_p->size; ++i) {
        if (!strcmp(Name_p->p, ((nh_encoding_UTF8String*)VarDeclaredNames_p->pp[i])->p)) {
            containsName = true;
            break;
        }
    } 

    if (!containsName) {nh_core_appendToList(VarDeclaredNames_p, Name_p);}

NH_ECMASCRIPT_END(nh_ecmascript_normalEmptyCompletion())
}

bool nh_ecmascript_hasVarDeclaration(
    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    for (int i = 0; i < GlobalEnvironment_p->VarNames.size; ++i) {
        if (!strcmp(((nh_encoding_UTF8String*)GlobalEnvironment_p->VarNames.pp[i])->p, Name_p->p)) {
            NH_ECMASCRIPT_END(true)
        }
    }

NH_ECMASCRIPT_END(false)
}

bool nh_ecmascript_hasLexicalDeclaration(
    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

// TODO 

NH_ECMASCRIPT_END(false)
}

bool nh_ecmascript_hasRestrictedGlobalProperty(
    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

// TODO 

NH_ECMASCRIPT_END(false)
}

static nh_ecmascript_Completion nh_ecmascript_globalEnvironmentCreateMutableBinding(
    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p, bool deletable)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p = GlobalEnvironment_p->DeclarativeEnvironment_p->Handle_p;
    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
    } 

NH_ECMASCRIPT_END(nh_ecmascript_declarativeEnvironmentCreateMutableBinding(DeclarativeEnvironment_p, Name_p, deletable))
}

static nh_ecmascript_Completion nh_ecmascript_globalEnvironmentCreateImmutableBinding(
    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p, bool strict)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p = GlobalEnvironment_p->DeclarativeEnvironment_p->Handle_p;
    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
    } 

NH_ECMASCRIPT_END(nh_ecmascript_declarativeEnvironmentCreateImmutableBinding(DeclarativeEnvironment_p, Name_p, strict))
}

static bool nh_ecmascript_globalEnvironmentHasBinding(
    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p = GlobalEnvironment_p->DeclarativeEnvironment_p->Handle_p;
    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
        NH_ECMASCRIPT_END(true)
    }

    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p = GlobalEnvironment_p->ObjectEnvironment_p->Handle_p;

NH_ECMASCRIPT_END(nh_ecmascript_objectEnvironmentHasBinding(ObjectEnvironment_p, Name_p))
}

// ABSTRACT ========================================================================================

bool nh_ecmascript_hasBinding(
    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_OBJECT :
            NH_ECMASCRIPT_END(nh_ecmascript_objectEnvironmentHasBinding(
                Environment_p->Handle_p, Name_p
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL :
            NH_ECMASCRIPT_END(nh_ecmascript_globalEnvironmentHasBinding(
                Environment_p->Handle_p, Name_p
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE :
            NH_ECMASCRIPT_END(nh_ecmascript_declarativeEnvironmentHasBinding(
                Environment_p->Handle_p, Name_p
            ))
    }

NH_ECMASCRIPT_END(false)
}

nh_ecmascript_Completion nh_ecmascript_createMutableBinding(
    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, bool deletable)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL :
            NH_ECMASCRIPT_END(nh_ecmascript_globalEnvironmentCreateMutableBinding(
                Environment_p->Handle_p, Name_p, deletable
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE :
            NH_ECMASCRIPT_END(nh_ecmascript_declarativeEnvironmentCreateMutableBinding(
                Environment_p->Handle_p, Name_p, deletable
            ))
    }

NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
}

nh_ecmascript_Completion nh_ecmascript_createImmutableBinding(
    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, bool strict)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL :
            NH_ECMASCRIPT_END(nh_ecmascript_globalEnvironmentCreateImmutableBinding(
                Environment_p->Handle_p, Name_p, strict
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE :
            NH_ECMASCRIPT_END(nh_ecmascript_declarativeEnvironmentCreateImmutableBinding(
                Environment_p->Handle_p, Name_p, strict
            ))
    }

NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
}

nh_ecmascript_Completion nh_ecmascript_initializeBinding(
    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL : break;
        case NH_ECMASCRIPT_ENVIRONMENT_OBJECT : 
            NH_ECMASCRIPT_END(nh_ecmascript_objectEnvironmentInitializeBinding(
                Environment_p->Handle_p, Name_p, Value 
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE :
            NH_ECMASCRIPT_END(nh_ecmascript_declarativeEnvironmentInitializeBinding(
                Environment_p->Handle_p, Name_p, Value 
            ))
    }

NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
}

nh_ecmascript_Completion nh_ecmascript_setMutableBinding(
    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value, bool _throw)
{
NH_ECMASCRIPT_BEGIN()

    switch (Environment_p->type)
    {
        case NH_ECMASCRIPT_ENVIRONMENT_GLOBAL : break;
        case NH_ECMASCRIPT_ENVIRONMENT_OBJECT : 
            NH_ECMASCRIPT_END(nh_ecmascript_objectEnvironmentSetMutableBinding(
                Environment_p->Handle_p, Name_p, Value, _throw 
            ))
        case NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE : break;
    }

NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())
}

