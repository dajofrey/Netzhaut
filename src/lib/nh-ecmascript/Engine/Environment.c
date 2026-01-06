// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Environment.h"
#include "TestAndCompare.h"
#include "PropertyDescriptor.h"

#include "../Interpreter/DeclaredNames.h"
#include "../StaticSemantics/ScopedDeclarations.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>

// DECLARATIVE ENVIRONMENT =========================================================================


//nh_ecmascript_Environment *nh_ecmascript_newDeclarativeEnvironment(
//    nh_ecmascript_Environment *Outer_p)
//{
//    nh_ecmascript_Environment *Environment_p = (nh_ecmascript_Environment*)nh_core_allocate(sizeof(nh_ecmascript_Environment));
//    NH_CORE_CHECK_MEM_2(NULL, Environment_p)
//
//    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p = (nh_ecmascript_DeclarativeEnvironment*)nh_core_allocate(sizeof(nh_ecmascript_DeclarativeEnvironment));
//    NH_CORE_CHECK_MEM_2(NULL, DeclarativeEnvironment_p)
//
//    DeclarativeEnvironment_p->Bindings = nh_core_initList(16);
//
//    Environment_p->Handle_p = DeclarativeEnvironment_p;
//    Environment_p->Outer_p  = Outer_p;
//    Environment_p->type     = NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE;
//
//    return Environment_p;
//}
//
//static bool nh_ecmascript_declarativeEnvironmentHasBinding(
//    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p, nh_encoding_UTF8String *Name_p)
//{
//    for (int i = 0; i < DeclarativeEnvironment_p->Bindings.size; ++i) {
//        nh_ecmascript_Binding *Binding_p = DeclarativeEnvironment_p->Bindings.pp[i];
//        if (!strcmp(Binding_p->Name_p->p, Name_p->p)) {return true;}
//    }
//
//    return false;
//}
//
//static nh_ecmascript_Completion nh_ecmascript_declarativeEnvironmentCreateMutableBinding(
//    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p, nh_encoding_UTF8String *Name_p, bool deletable)
//{
//    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
//        return nh_ecmascript_throwTypeError();
//    }
//
//    nh_ecmascript_Binding *NewBinding_p = (nh_ecmascript_Binding*)nh_core_allocate(sizeof(nh_ecmascript_Binding));
//    NewBinding_p->_mutable    = true;
//    NewBinding_p->deletable   = deletable;
//    NewBinding_p->initialized = false;
//    NewBinding_p->Name_p      = Name_p;
//    NewBinding_p->Value       = nh_ecmascript_wrapUndefined();
//
//    nh_core_appendToList(&DeclarativeEnvironment_p->Bindings, NewBinding_p);
//
//    return nh_ecmascript_normalEmptyCompletion();
//}
//
//static nh_ecmascript_Completion nh_ecmascript_declarativeEnvironmentCreateImmutableBinding(
//    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p, nh_encoding_UTF8String *Name_p, bool strict)
//{
//    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
//        return nh_ecmascript_throwTypeError();
//    }
//
//    nh_ecmascript_Binding *NewBinding_p = (nh_ecmascript_Binding*)nh_core_allocate(sizeof(nh_ecmascript_Binding));
//    NewBinding_p->_mutable    = false;
//    NewBinding_p->strict      = strict;
//    NewBinding_p->initialized = false;
//    NewBinding_p->Name_p      = Name_p;
//    NewBinding_p->Value       = nh_ecmascript_wrapUndefined();
//
//    nh_core_appendToList(&DeclarativeEnvironment_p->Bindings, NewBinding_p);
//
//    return nh_ecmascript_normalEmptyCompletion();
//}
//
//static nh_ecmascript_Completion nh_ecmascript_declarativeEnvironmentInitializeBinding(
//    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value)
//{
//    nh_ecmascript_Binding *Binding_p = NULL;
//    for (int i = 0; i < DeclarativeEnvironment_p->Bindings.size; ++i) {
//        Binding_p = DeclarativeEnvironment_p->Bindings.pp[i];
//        if (!strcmp(Binding_p->Name_p->p, Name_p->p)) {break;}
//    }
//
//    if (Binding_p == NULL) {return nh_ecmascript_throwTypeError();}
//
//    if (!Binding_p->initialized) {
//        Binding_p->Value = Value;
//        Binding_p->initialized = true;
//    }
//
//    return nh_ecmascript_normalEmptyCompletion();
//}
//
//// OBJECT ENVIRONMENT ==============================================================================
//
//nh_ecmascript_Environment *nh_ecmascript_newObjectEnvironment(
//    nh_ecmascript_Object *Object_p, nh_ecmascript_Environment *Outer_p)
//{
//    nh_ecmascript_Environment *Environment_p = (nh_ecmascript_Environment*)nh_core_allocate(sizeof(nh_ecmascript_Environment));
//    NH_CORE_CHECK_MEM_2(NULL, Environment_p)
//
//    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p = (nh_ecmascript_ObjectEnvironment*)nh_core_allocate(sizeof(nh_ecmascript_ObjectEnvironment));
//    NH_CORE_CHECK_MEM_2(NULL, ObjectEnvironment_p)
//
//    ObjectEnvironment_p->BindingObject_p = Object_p;
//    ObjectEnvironment_p->withEnvironment = false;
//
//    Environment_p->Handle_p = ObjectEnvironment_p;
//    Environment_p->Outer_p = Outer_p;
//    Environment_p->type = NH_ECMASCRIPT_ENVIRONMENT_OBJECT;
//
//    return Environment_p;
//}
//
//static nh_ecmascript_Completion nh_ecmascript_objectEnvironmentCreateMutableBinding(
//    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p, nh_encoding_UTF8String *Name_p, bool deletable)
//{
//    nh_ecmascript_Object *Bindings_p = ObjectEnvironment_p->BindingObject_p;
//
//    nh_ecmascript_PropertyDescriptor Descriptor;
//    Descriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
//    Descriptor.enumerable   = true;
//    Descriptor.configurable = deletable;
//    Descriptor.Fields.Data.writable = true;
//    Descriptor.Fields.Data.Value    = nh_ecmascript_wrapUndefined();
//
//    return nh_ecmascript_abstractDefinePropertyOrThrow(Bindings_p, nh_ecmascript_wrapString(Name_p), Descriptor);
//}
//
//static nh_ecmascript_Completion nh_ecmascript_objectEnvironmentSetMutableBinding(
//    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value, bool _throw)
//{
//    nh_ecmascript_Object *Bindings_p = ObjectEnvironment_p->BindingObject_p;
//    bool stillExists = nh_ecmascript_abstractHasProperty(Bindings_p, nh_ecmascript_wrapString(Name_p));
//    if (!stillExists && _throw) {return nh_ecmascript_throwReferenceError();}
//
//    return nh_ecmascript_abstractSet(Bindings_p, nh_ecmascript_wrapString(Name_p), Value, _throw);
//}
//
//static nh_ecmascript_Completion nh_ecmascript_objectEnvironmentInitializeBinding(
//    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value)
//{
//    return nh_ecmascript_objectEnvironmentSetMutableBinding(ObjectEnvironment_p, Name_p, Value, false);
//}
//
//static bool nh_ecmascript_objectEnvironmentHasBinding(
//    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p, nh_encoding_UTF8String *Name_p)
//{
//    nh_ecmascript_Object *Bindings_p = ObjectEnvironment_p->BindingObject_p;
//    bool foundBinding = nh_ecmascript_abstractHasProperty(Bindings_p, nh_ecmascript_wrapString(Name_p));
//    if (!foundBinding) {return false;}
//    if (!ObjectEnvironment_p->withEnvironment) {return false;}
//
//// Let unscopables be ? Get(bindings, @@unscopables).
//// If Type(unscopables) is Object, then
//// 
////     Let blocked be ! ToBoolean(? Get(unscopables, N)).
////     If blocked is true, return false.
//
//    return true;
//}

// GLOBAL ENVIRONMENT ==============================================================================

nh_ecmascript_Environment *nh_ecmascript_newGlobalEnvironment(
    nh_ecmascript_Object *GlobalObject_p, 
    nh_ecmascript_Object *ThisValue_p) 
{
    nh_ecmascript_Environment *Env_p = (nh_ecmascript_Environment*)nh_core_allocate(sizeof(nh_ecmascript_Environment));
    NH_CORE_CHECK_MEM_2(NULL, Env_p)

    // 1. Set the type to GLOBAL (Spec 9.1.1.4)
    Env_p->type = NH_ECMASCRIPT_ENVIRONMENT_GLOBAL;

    // 2. Setup the Object Record
    // This record "wraps" the global object. When the interpreter looks for 'Object',
    // it will check if GlobalObject_p has a property named 'Object'.
    Env_p->records.global.objectRecord = GlobalObject_p;

    // 3. Setup the Declarative Record
    // This is essentially a fresh HashMap for 'let' and 'const' variables.
//    nh_core_initializeHashMap(&Env_p->records.global.declarativeRecord, 16);

    // 4. Set the [[GlobalThisValue]] (Spec 9.1.1.4.10)
    // Usually, this is just the Global Object itself.
    Env_p->records.global.globalThisValue = nh_ecmascript_makeObject(ThisValue_p);

    // 5. Global Environments have no outer environment
    Env_p->outer = NULL;

    return Env_p;
}

bool nh_ecmascript_canDeclareGlobalFunction(
    nh_ecmascript_Environment *env, 
    const char *name, 
    nh_ecmascript_Realm *Realm_p)
{
    nh_ecmascript_Object *globalObj = env->records.global.objectRecord;
    
    // You need a GetOwnProperty internal call here
    nh_ecmascript_PropertyDescriptor desc;
//    bool found = nh_ecmascript_getOwnProperty(globalObj, name, &desc, Realm_p);
//
//    if (!found) {
//        nh_ecmascript_Completion extensible = nh_ecmascript_isExtensible(globalObj, Realm_p);
//        return extensible.value.p.boolean;
//    }
//
//    if (desc.configurable) return true;
//    
//    // If it's a data property, it must be writable and enumerable
//    if (desc.type == NH_ECMASCRIPT_PROPERTY_DATA && desc.writable && desc.enumerable) {
//        return true;
//    }

    return false;
}

bool nh_ecmascript_canDeclareGlobalVar(
    nh_ecmascript_Environment *env, 
    const char *name, 
    nh_ecmascript_Realm *Realm_p)
{
    nh_ecmascript_Object *globalObj = env->records.global.objectRecord;
    
    // 1. If property exists, it's fine (re-declaration is allowed for var)
//    if (nh_ecmascript_hasProperty(globalObj, name, Realm_p)) return true;

    // 2. If it doesn't exist, we must be able to add it
    nh_ecmascript_Completion extensible = nh_ecmascript_isExtensible(globalObj, Realm_p);
    return extensible.Value.p.boolean;
}

nh_ecmascript_Completion nh_ecmascript_createGlobalVarBinding(
    nh_ecmascript_Environment *env, 
    const char *name, 
    bool deletable, 
    nh_ecmascript_Realm *Realm_p)
{
//    nh_ecmascript_Object *globalObj = env->records.global.objectRecord;
//    
//    if (!nh_ecmascript_hasProperty(globalObj, name, Realm_p)) {
//        // Create the property on the actual object (e.g. window.x)
//        nh_ecmascript_createDataPropertyOrThrow(
//            globalObj, name, nh_ecmascript_makeUndefined(), 
//            true, true, deletable, Realm_p
//        );
//    }

    // Keep track of the name in your VarNames list for the Global Environment
    // (You'll need to add a List to your records.global struct for this)
    return nh_ecmascript_normalEmptyCompletion();
}

bool nh_ecmascript_hasRestrictedGlobalProperty(
    nh_ecmascript_Environment *env, 
    const char *name, 
    nh_ecmascript_Realm *Realm_p)
{
//    nh_ecmascript_PropertyDescriptor desc;
//    bool found = nh_ecmascript_getOwnProperty(env->records.global.objectRecord, name, &desc, Realm_p);
//    
//    if (!found) return false;
//    return !desc.configurable;
return false;
}

//bool nh_ecmascript_hasVarDeclaration(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p)
//{
//    for (int i = 0; i < GlobalEnvironment_p->VarNames.size; ++i) {
//        if (!strcmp(((nh_encoding_UTF8String*)GlobalEnvironment_p->VarNames.pp[i])->p, Name_p->p)) {
//            return true;
//        }
//    }
//
//    return false;
//}
//
//bool nh_ecmascript_hasLexicalDeclaration(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p)
//{
//    // TODO 
//    return false;
//}
//
//
//static nh_ecmascript_Completion nh_ecmascript_globalEnvironmentCreateMutableBinding(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p, bool deletable)
//{
//    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p = GlobalEnvironment_p->DeclarativeEnvironment_p->Handle_p;
//    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
//        return nh_ecmascript_throwTypeError();
//    } 
//
//    return nh_ecmascript_declarativeEnvironmentCreateMutableBinding(DeclarativeEnvironment_p, Name_p, deletable);
//}
//
//static nh_ecmascript_Completion nh_ecmascript_globalEnvironmentCreateImmutableBinding(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p, bool strict)
//{
//    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p = GlobalEnvironment_p->DeclarativeEnvironment_p->Handle_p;
//    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
//        return nh_ecmascript_throwTypeError();
//    } 
//
//    return nh_ecmascript_declarativeEnvironmentCreateImmutableBinding(DeclarativeEnvironment_p, Name_p, strict);
//}
//
//static bool nh_ecmascript_globalEnvironmentHasBinding(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p)
//{
//    nh_ecmascript_DeclarativeEnvironment *DeclarativeEnvironment_p = GlobalEnvironment_p->DeclarativeEnvironment_p->Handle_p;
//    if (nh_ecmascript_declarativeEnvironmentHasBinding(DeclarativeEnvironment_p, Name_p)) {
//        return true;
//    }
//
//    nh_ecmascript_ObjectEnvironment *ObjectEnvironment_p = GlobalEnvironment_p->ObjectEnvironment_p->Handle_p;
//
//    return nh_ecmascript_objectEnvironmentHasBinding(ObjectEnvironment_p, Name_p);
//}
//

bool nh_ecmascript_hasBinding(nh_ecmascript_Environment *env, const char *name)
{
//    switch (env->type) {
//        case NH_ECMASCRIPT_ENV_DECLARATIVE:
//            return nh_ecmascript_findBinding(env, name) != NULL;
//
//        case NH_ECMASCRIPT_ENV_OBJECT:
//            // Object environments (like 'with') check the actual object
//            return nh_ecmascript_hasProperty(env->records.object.Object_p, name, NULL);
//
//        case NH_ECMASCRIPT_ENV_GLOBAL:
//            // Global checks declarative first, then the global object
//            if (nh_ecmascript_findGlobalDeclarativeBinding(env, name)) return true;
//            return nh_ecmascript_hasProperty(env->records.global.objectRecord, name, NULL);
//    }
    return false;
}

nh_ecmascript_Completion nh_ecmascript_createMutableBinding(
    nh_ecmascript_Environment *env, const char *name, bool deletable)
{
//    if (env->type == NH_ECMASCRIPT_ENV_DECLARATIVE) {
//        nh_ecmascript_Binding b = {
//            ._mutable = true,
//            .deletable = deletable,
//            .initialized = false,
//            .Name_p = nh_encoding_createUTF8String(name),
//            .Value = nh_ecmascript_makeUndefined()
//        };
//        nh_core_addToList(&env->records.declarative.Bindings, &b);
//        return nh_ecmascript_normalEmptyCompletion();
//    }
    
    // Global and Object environments typically use DefineOwnProperty under the hood
    // ... logic for Global/Object ...
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_initializeBinding(
    nh_ecmascript_Environment *env, const char *name, nh_ecmascript_Value val)
{
//    if (env->type == NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE) {
//        nh_ecmascript_Binding *b = nh_ecmascript_findBinding(env, name);
//        if (b) {
//            b->Value = val;
//            b->initialized = true;
//            return nh_ecmascript_normalEmptyCompletion();
//        }
//    }
    // Handle Global/Object initialization
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_setMutableBinding(
    nh_ecmascript_Environment *env, 
    const char *name, 
    nh_ecmascript_Value val, 
    bool strict, 
    nh_ecmascript_Realm *Realm_p)
{
//    switch (env->type) {
//        case NH_ECMASCRIPT_ENV_DECLARATIVE: {
//            nh_ecmascript_Binding *b = nh_ecmascript_findBinding(env, name);
//            if (!b) return nh_ecmascript_throwReferenceError("Binding not found", Realm_p);
//            if (!b->initialized) return nh_ecmascript_throwReferenceError("TDZ", Realm_p);
//            if (!b->_mutable) {
//                if (strict) return nh_ecmascript_throwTypeError("Assigning to const", Realm_p);
//                return nh_ecmascript_normalEmptyCompletion();
//            }
//            b->Value = val;
//            return nh_ecmascript_normalEmptyCompletion();
//        }
//
//        case NH_ECMASCRIPT_ENV_OBJECT:
//            return nh_ecmascript_set(env->records.object.Object_p, name, val, Realm_p);
//
//        case NH_ECMASCRIPT_ENV_GLOBAL:
//            // 1. Check declarative (let/const)
//            nh_ecmascript_Binding *db = nh_ecmascript_findGlobalDeclarativeBinding(env, name);
//            if (db) {
//                // Same logic as declarative block above
//                db->Value = val; 
//                return nh_ecmascript_normalEmptyCompletion();
//            }
//            // 2. Fallback to global object
//            return nh_ecmascript_set(env->records.global.objectRecord, name, val, Realm_p);
//    }
    return nh_ecmascript_normalEmptyCompletion();
}

static nh_ecmascript_Binding* nh_ecmascript_findBinding(nh_ecmascript_Environment *env, const char *name)
{
//    // Iterate through your core list
//    for (size_t i = 0; i < nh_core_getListSize(&env->records.declarative.Bindings); i++) {
//        nh_ecmascript_Binding *b = nh_core_getElement(&env->records.declarative.Bindings, i);
//        if (strcmp(b->Name_p->p, name) == 0) {
//            return b;
//        }
//    }
    return NULL;
}

// https://tc39.es/ecma262/#sec-getbindingvalue
nh_ecmascript_Completion nh_ecmascript_getBindingValue(
    nh_ecmascript_Environment *env,
    const char *name,
    bool strict,
    nh_ecmascript_Realm *Realm_p)
{
//    switch (env->type) {
//        
//        case NH_ECMASCRIPT_ENV_DECLARATIVE: {
//            // 1. Search the list for the binding
//            nh_ecmascript_Binding *binding = nh_ecmascript_findBinding(env, name);
//            
//            // 2. If not found, this is an internal engine error (GetValue should have found it)
//            if (!binding) return nh_ecmascript_throwReferenceError("Binding not found", Realm_p);
//
//            // 3. Check for "Temporal Dead Zone" (let/const declared but not initialized)
//            if (!binding->initialized) {
//                return nh_ecmascript_throwReferenceError("Variable used before initialization", Realm_p);
//            }
//
//            return nh_ecmascript_normalCompletion(binding->Value);
//        }
//
//        case NH_ECMASCRIPT_ENV_OBJECT: {
//            // Object environments (like 'with' blocks) look up properties on an actual object
//            nh_ecmascript_Object *O = env->records.object.Object_p;
//            
//            // Note: If property doesn't exist, the spec says return undefined (or handle 'with' rules)
//            bool has = nh_ecmascript_hasProperty(O, name, Realm_p);
//            if (!has) {
//                if (strict) return nh_ecmascript_throwReferenceError("Property not found", Realm_p);
//                return nh_ecmascript_normalCompletion(nh_ecmascript_makeUndefined());
//            }
//
//            return nh_ecmascript_get(O, name, Realm_p);
//        }
//
//        case NH_ECMASCRIPT_ENV_GLOBAL: {
//            // Global environments are special: they check both a declarative list (let/const) 
//            // and an object record (var/functions on the window/global object).
//            
//            // 1. Check declarative part (e.g., 'let x' at top level)
//            nh_ecmascript_Binding *decl = nh_ecmascript_findGlobalDeclarativeBinding(env, name);
//            if (decl) {
//                if (!decl->initialized) return nh_ecmascript_throwReferenceError("TDZ", Realm_p);
//                return nh_ecmascript_normalCompletion(decl->Value);
//            }
//
//            // 2. Check object part (e.g., 'var x' or 'window.x')
//            return nh_ecmascript_get(env->records.global.objectRecord, name, Realm_p);
//        }
//    }

    return nh_ecmascript_normalEmptyCompletion();
}

// https://tc39.es/ecma262/#sec-createglobalfunctionbinding
nh_ecmascript_Completion nh_ecmascript_createGlobalFunctionBinding(
    nh_ecmascript_Environment *env,
    const char *name,
    nh_ecmascript_Value V,
    bool configurable,
    nh_ecmascript_Realm *Realm_p)
{
//    nh_ecmascript_Object *globalObj = env->records.global.objectRecord;
//    
//    // 1. Prepare a Descriptor for the new function
//    nh_ecmascript_PropertyDescriptor desc;
//    memset(&desc, 0, sizeof(desc)); // Initialize all presence bits to 0
//    
//    desc.Value = V;
//    desc.presence.hasValue = 1;
//    
//    desc.writable = true;
//    desc.presence.hasWritable = 1;
//    
//    desc.enumerable = true;
//    desc.presence.hasEnumerable = 1;
//    
//    desc.configurable = configurable;
//    desc.presence.hasConfigurable = 1;
//
//    // 2. Check if the property already exists
//    nh_ecmascript_PropertyDescriptor existing;
//    bool found = nh_ecmascript_getOwnProperty(globalObj, name, &existing, Realm_p);
//
//    if (!found) {
//        // Create new property on global object
//        return nh_ecmascript_defineOwnProperty(globalObj, name, desc, Realm_p);
//    } 
//    
//    // 3. If it exists but is configurable, we update its value
//    if (existing.configurable) {
//        return nh_ecmascript_defineOwnProperty(globalObj, name, desc, Realm_p);
//    }
//
//    // 4. Special case: If it's not configurable but writable, we just set the value
//    // This happens with legacy vars/functions on the global object
//    return nh_ecmascript_set(globalObj, name, V, Realm_p);
return nh_ecmascript_normalEmptyCompletion();
}

// https://tc39.es/ecma262/#sec-getthisbinding
nh_ecmascript_Value nh_ecmascript_getThisBinding(
    nh_ecmascript_Environment *env,
    nh_ecmascript_Realm *Realm_p)
{
//    switch (env->type) {
//        case NH_ECMASCRIPT_ENV_GLOBAL:
//            // Global environment has a dedicated slot for this (usually the Global Object)
//            return env->records.global.globalThisValue;
//
//        case NH_ECMASCRIPT_ENV_OBJECT:
//            // Object environments (like 'with') don't provide a 'this' binding.
//            // The spec says they return undefined, as the search continues outward.
//            return nh_ecmascript_makeUndefined();
//
//        case NH_ECMASCRIPT_ENV_DECLARATIVE: {
//            // Functional environments are a subset of declarative ones.
//            // If this is a function environment, it stores the 'this' value used during the call.
//            if (env->type == NH_ECMASCRIPT_ENV_FUNCTION) {
//                // You'll need to add 'thisValue' to your function record struct
//                return env->records.function.thisValue;
//            }
//            
//            // For block scopes {}, 'this' is inherited from the outer environment
//            if (env->outer != NULL) {
//                return nh_ecmascript_getThisBinding(env->outer, Realm_p);
//            }
//            return nh_ecmascript_makeUndefined();
//        }
//    }
    return nh_ecmascript_makeUndefined();
}
