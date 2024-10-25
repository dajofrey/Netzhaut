// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Object.h"

#include "../Engine/OrdinaryObject.h"
#include "../Engine/BuiltinFunctionObject.h"

#include <string.h>

// INTERNAL METHODS ================================================================================

// https://tc39.es/ecma262/#sec-immutable-prototype-exotic-objects-setprototypeof-v
static NH_ECMASCRIPT_BOOLEAN _nh_ecmascript_setPrototypeOf(
    nh_ecmascript_Object *This_p)
{
    return false;
}

// DATA ============================================================================================

static nh_ecmascript_InternalMethods ObjectPrototypeInternalMethods = {
    .getPrototypeOf_f    = nh_ecmascript_ordinaryGetPrototypeOf,
    .setPrototypeOf_f    = _nh_ecmascript_setPrototypeOf,
    .isExtensible_f      = nh_ecmascript_ordinaryIsExtensible,
    .preventExtensions_f = nh_ecmascript_ordinaryPreventExtensions,
    .getOwnProperty_f    = nh_ecmascript_ordinaryGetOwnProperty,
    .defineOwnProperty_f = nh_ecmascript_ordinaryDefineOwnProperty,
    .hasProperty_f       = nh_ecmascript_ordinaryHasProperty,
    .get_f               = nh_ecmascript_ordinaryGet,
    .set_f               = nh_ecmascript_ordinarySet,
    .delete_f            = nh_ecmascript_ordinaryDelete,
    .ownPropertyKeys_f   = nh_ecmascript_ordinaryOwnPropertyKeys,
    .call_f              = NULL,
    .construct_f         = NULL,
};

static const char *objectPropertyNames_pp[] = {
// constructor properties
    "length",
    "assign",
    "create",
    "defineProperties",
    "defineProperty",
    "entries",
    "freeze",
    "fromEntries",
    "getOwnPropertyDescriptor",
    "getOwnPropertyDescriptors",
    "getOwnPropertyNames",
    "getOwnPropertySymbols",
    "getPrototypeOf",
    "is",
    "isExtensible",
    "isFrozen",
    "isSealed",
    "keys",
    "preventExtensions",
    "seal",
    "setPrototypeOf",
    "values",
// prototype properties
    "constructor",
    "hasOwnProperty",
    "isPrototypeOf",
    "propertyIsEnumerable",
    "toLocaleString",
    "toString",
    "valueOf",
};

// CONSTRUCTOR =====================================================================================

static nh_ecmascript_Any nh_ecmascript_construct(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

// BUILTIN OBJECT CONSTRUCTOR FUNCTIONS ============================================================

static nh_ecmascript_Any nh_ecmascript_assign(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_create(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_defineProperties(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_defineProperty(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_entries(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_freeze(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_fromEntries(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_getOwnPropertyDescriptor(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_getOwnPropertyDescriptors(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_getOwnPropertyNames(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_getOwnPropertySymbols(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_getPrototypeOf(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_is(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_isExtensible(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_isFrozen(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_isSealed(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_keys(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_preventExtensions(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_seal(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_setPrototypeOf(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_values(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

// BUILTIN OBJECT PROTOTYPE FUNCTIONS ==============================================================

static nh_ecmascript_Any nh_ecmascript_hasOwnProperty(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_isPrototypeOf(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_propertyIsEnumerable(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_toLocaleString(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any _nh_ecmascript_toString(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

static nh_ecmascript_Any nh_ecmascript_valueOf(
    nh_ecmascript_Object *This_p, nh_core_List Arguments)
{
    return nh_ecmascript_wrapUndefined();
}

// CREATE ==========================================================================================

typedef nh_ecmascript_Any (*builtin_f)(nh_ecmascript_Object *This_p, nh_core_List Arguments);

static nh_ecmascript_Property *nh_ecmascript_getObjectProperty(
    nh_ecmascript_IntrinsicObject *Object_p, int i, builtin_f *function_pp)
{
    Object_p->Properties_p[i].type = NH_ECMASCRIPT_PROPERTY_DATA;
    Object_p->Properties_p[i].enumerable   = false;
    Object_p->Properties_p[i].configurable = false;
    Object_p->Properties_p[i].Fields.Data.writable = false;

    nh_ecmascript_Any Value;
    Value.type = NH_ECMASCRIPT_TYPE_OBJECT;
    Value.handle_p = NULL;

    switch (i)
    {
        case  0 : break;
        case  1 : Value.handle_p = &Object_p->Assign; break;
        case  2 : Value.handle_p = &Object_p->Create; break;
        case  3 : Value.handle_p = &Object_p->DefineProperties; break;
        case  4 : Value.handle_p = &Object_p->DefineProperty; break;
        case  5 : Value.handle_p = &Object_p->Entries; break;
        case  6 : Value.handle_p = &Object_p->Freeze; break;
        case  7 : Value.handle_p = &Object_p->FromEntries; break;
        case  8 : Value.handle_p = &Object_p->GetOwnPropertyDescriptor; break;
        case  9 : Value.handle_p = &Object_p->GetOwnPropertyDescriptors; break;
        case 10 : Value.handle_p = &Object_p->GetOwnPropertyNames; break;
        case 11 : Value.handle_p = &Object_p->GetOwnPropertySymbols; break;
        case 12 : Value.handle_p = &Object_p->GetPrototypeOf; break;
        case 13 : Value.handle_p = &Object_p->Is; break;
        case 14 : Value.handle_p = &Object_p->IsExtensible; break;
        case 15 : Value.handle_p = &Object_p->IsFrozen; break;
        case 16 : Value.handle_p = &Object_p->IsSealed; break;
        case 17 : Value.handle_p = &Object_p->Keys; break;
        case 18 : Value.handle_p = &Object_p->PreventExtensions; break;
        case 19 : Value.handle_p = &Object_p->Seal; break;
        case 20 : Value.handle_p = &Object_p->SetPrototypeOf; break;
        case 21 : Value.handle_p = &Object_p->Values; break;
        case 22 : Value.handle_p = &Object_p->Constructor; break;
        case 23 : Value.handle_p = &Object_p->HasOwnProperty; break;
        case 24 : Value.handle_p = &Object_p->IsPrototypeOf; break;
        case 25 : Value.handle_p = &Object_p->PropertyIsEnumerable; break;
        case 26 : Value.handle_p = &Object_p->ToLocaleString; break;
        case 27 : Value.handle_p = &Object_p->ToString; break;
        case 28 : Value.handle_p = &Object_p->ValueOf; break;
    }

    Object_p->Properties_p[i].Fields.Data.Value = Value;
    *function_pp = NULL;

    switch (i)
    {
        case  0 : break;
        case  1 : *function_pp = nh_ecmascript_assign; break;
        case  2 : *function_pp = nh_ecmascript_create; break;
        case  3 : *function_pp = nh_ecmascript_defineProperties; break;
        case  4 : *function_pp = nh_ecmascript_defineProperty; break;
        case  5 : *function_pp = nh_ecmascript_entries; break;
        case  6 : *function_pp = nh_ecmascript_freeze; break;
        case  7 : *function_pp = nh_ecmascript_fromEntries; break;
        case  8 : *function_pp = nh_ecmascript_getOwnPropertyDescriptor; break;
        case  9 : *function_pp = nh_ecmascript_getOwnPropertyDescriptors; break;
        case 10 : *function_pp = nh_ecmascript_getOwnPropertyNames; break;
        case 11 : *function_pp = nh_ecmascript_getOwnPropertySymbols; break;
        case 12 : *function_pp = nh_ecmascript_getPrototypeOf; break;
        case 13 : *function_pp = nh_ecmascript_is; break;
        case 14 : *function_pp = nh_ecmascript_isExtensible; break;
        case 15 : *function_pp = nh_ecmascript_isFrozen; break;
        case 16 : *function_pp = nh_ecmascript_isSealed; break;
        case 17 : *function_pp = nh_ecmascript_keys; break;
        case 18 : *function_pp = nh_ecmascript_preventExtensions; break;
        case 19 : *function_pp = nh_ecmascript_seal; break;
        case 20 : *function_pp = nh_ecmascript_setPrototypeOf; break;
        case 21 : *function_pp = nh_ecmascript_values; break;
        case 22 : break;
        case 23 : *function_pp = nh_ecmascript_hasOwnProperty; break;
        case 24 : *function_pp = nh_ecmascript_isPrototypeOf; break;
        case 25 : *function_pp = nh_ecmascript_propertyIsEnumerable; break;
        case 26 : *function_pp = nh_ecmascript_toLocaleString; break;
        case 27 : *function_pp = _nh_ecmascript_toString; break;
        case 28 : *function_pp = nh_ecmascript_valueOf; break;
    }

    return &Object_p->Properties_p[i];
}

NH_API_RESULT nh_ecmascript_createIntrinsicObject(
    nh_ecmascript_IntrinsicObject *Object_p, nh_ecmascript_Realm *Realm_p)
{
    Object_p->Prototype.Properties = nh_core_initList(8);
    Object_p->Prototype.InternalSlots = nh_ecmascript_initInternalSlots(NULL, 0);
    Object_p->Prototype.InternalMethods_p = &ObjectPrototypeInternalMethods;

    nh_ecmascript_setInternalSlot(&Object_p->Prototype.InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE, (void*)true);
    nh_ecmascript_setInternalSlot(&Object_p->Prototype.InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE, NULL);

    NH_CORE_CHECK_NULL(nh_ecmascript_createBuiltinFunctionObject(
        &Object_p->Constructor, nh_ecmascript_construct, NULL, 0, Realm_p, &Realm_p->Intrinsics.Function.Prototype 
    ))

    for (int i = 0; i < sizeof(objectPropertyNames_pp) / sizeof(objectPropertyNames_pp[0]); ++i) 
    {
        nh_ecmascript_Object *ConstructorOrPrototype_p = i < 22 ? &Object_p->Constructor : &Object_p->Prototype;

        Object_p->Keys_p[i].p = (char*)objectPropertyNames_pp[i];
        Object_p->Keys_p[i].length  = strlen(objectPropertyNames_pp[i]);

        builtin_f function_p = NULL;
        nh_ecmascript_Property *Property_p = nh_ecmascript_getObjectProperty(Object_p, i, &function_p);
        Property_p->Key = nh_ecmascript_wrapString(&Object_p->Keys_p[i]);

        if (function_p != NULL) {
            NH_CORE_CHECK_NULL(nh_ecmascript_createBuiltinFunctionObject(
                Property_p->Fields.Data.Value.handle_p, function_p, NULL, 0, Realm_p, NULL 
            ))
        }

        nh_core_appendToList(&ConstructorOrPrototype_p->Properties, Property_p);
    }

    return NH_API_SUCCESS;
}
