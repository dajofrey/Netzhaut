// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Function.h"

#include "../Common/Macros.h"

#include <string.h>

// INTERNAL METHODS ================================================================================

//// https://tc39.es/ecma262/#sec-immutable-prototype-exotic-objects-setprototypeof-v
//static NH_ECMASCRIPT_BOOLEAN _nh_ecmascript_setPrototypeOf(
//    nh_ecmascript_Object *This_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
//NH_ECMASCRIPT_END(NH_FALSE)
//}

//// DATA ============================================================================================
//
//static nh_ecmascript_InternalMethods FunctionPrototypeInternalMethods = {
//    .getPrototypeOf_f    = nh_ecmascript_ordinaryGetPrototypeOf,
////    .setPrototypeOf_f    = _nh_ecmascript_setPrototypeOf,
//    .isExtensible_f      = nh_ecmascript_ordinaryIsExtensible,
//    .preventExtensions_f = nh_ecmascript_ordinaryPreventExtensions,
//    .getOwnProperty_f    = nh_ecmascript_ordinaryGetOwnProperty,
//    .defineOwnProperty_f = nh_ecmascript_ordinaryDefineOwnProperty,
//    .hasProperty_f       = nh_ecmascript_ordinaryHasProperty,
//    .get_f               = nh_ecmascript_ordinaryGet,
//    .set_f               = nh_ecmascript_ordinarySet,
//    .delete_f            = nh_ecmascript_ordinaryDelete,
//    .ownPropertyKeys_f   = nh_ecmascript_ordinaryOwnPropertyKeys,
////    .call_f              = NULL,
////    .construct_f         = NULL,
//};
//
//static const char *cProps_pp[] = {
//    "length",
//    "assign",
//    "create",
//    "defineProperties",
//    "defineProperty",
//    "entries",
//    "freeze",
//    "fromEntries",
//    "getOwnPropertyDescriptor",
//    "getOwnPropertyDescriptors",
//    "getOwnPropertyNames",
//    "getOwnPropertySymbols",
//    "getPrototypeOf",
//    "is",
//    "isExtensible",
//    "isFrozen",
//    "isSealed",
//    "keys",
//    "preventExtensions",
//    "seal",
//    "setPrototypeOf",
//    "values",
//};
//
//static const char *pProps_pp[] = {
//    "constructor",
//    "hasOwnProperty",
//    "isPrototypeOf",
//    "propertyIsEnumerable",
//    "toLocaleString",
//    "toString",
//    "valueOf",
//};
//
//// BUILTIN OBJECT CONSTRUCTOR FUNCTIONS ============================================================
//
////static nh_ecmascript_Any nh_ecmascript_assign(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_create(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_defineProperties(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_defineProperty(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_entries(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_freeze(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_fromEntries(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_getOwnPropertyDescriptor(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_getOwnPropertyDescriptors(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_getOwnPropertyNames(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_getOwnPropertySymbols(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_getPrototypeOf(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_is(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_isExtensible(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_isFrozen(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_isSealed(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_keys(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_preventExtensions(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_seal(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_setPrototypeOf(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_values(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
//
//// BUILTIN OBJECT PROTOTYPE FUNCTIONS ==============================================================
//
////static nh_ecmascript_Any nh_ecmascript_hasOwnProperty(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_isPrototypeOf(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_propertyIsEnumerable(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_toLocaleString(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any _nh_ecmascript_toString(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static nh_ecmascript_Any nh_ecmascript_valueOf(
////    nh_ecmascript_Object *This_p, nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
//
//// CREATE ==========================================================================================
//
//static NH_ECMASCRIPT_RESULT nh_ecmascript_createIntrinsicFunctionConstructor(
//    nh_ecmascript_IntrinsicFunction *Function_p, nh_ecmascript_Realm *Realm_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
////    Object_p->Constructor.Properties = nh_ecmascript_initProperties(22);
//////    Object_p->Prototype.InternalSlots = nh_core_initInternalSlots(ordinaryInternalSlotLookUp_p);
//////    Object_p->Prototype.InternalSlots.values_pp[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE] = NH_TRUE;
//////    Object_p->Prototype.InternalSlots.values_pp[NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE] = NULL;
//////    Object_p->Prototype.InternalMethods_p = ObjectPrototypeInternalMethods;
////
////    Object_p->Constructor.InternalSlots.values_pp[
////        Object_p->Constructor.InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE]
////    ] = (void*)NH_FALSE;
////    Object_p->Constructor.InternalSlots.values_pp[
////        Object_p->Constructor.InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE]
////    ] = &Realm_p->Intrinsics.Function.Prototype;
////
//////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createBuiltinFunction(
//////        &Object_p->Constructor, nh_ecmascript_construct_f, NULL, Realm_p, NULL 
//////    ))
////
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Assign, cProps_pp[1], nh_ecmascript_assign, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Create, cProps_pp[2], nh_ecmascript_create, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->DefineProperties, cProps_pp[3], nh_ecmascript_defineProperties, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->DefineProperty, cProps_pp[4], nh_ecmascript_defineProperty, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Entries, cProps_pp[5], nh_ecmascript_entries, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Freeze, cProps_pp[6], nh_ecmascript_freeze, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->FromEntries, cProps_pp[7], nh_ecmascript_fromEntries, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetOwnPropertyDescriptor, cProps_pp[8], nh_ecmascript_getOwnPropertyDescriptor, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetOwnPropertyDescriptors, cProps_pp[9], nh_ecmascript_getOwnPropertyDescriptors, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetOwnPropertyNames, cProps_pp[10], nh_ecmascript_getOwnPropertyNames, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetOwnPropertySymbols, cProps_pp[11], nh_ecmascript_getOwnPropertySymbols, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetPrototypeOf, cProps_pp[12], nh_ecmascript_getPrototypeOf, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Is, cProps_pp[13], nh_ecmascript_is, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->IsExtensible, cProps_pp[14], nh_ecmascript_isExtensible, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->IsFrozen, cProps_pp[15], nh_ecmascript_isFrozen, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->IsSealed, cProps_pp[16], nh_ecmascript_isSealed, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Keys, cProps_pp[17], nh_ecmascript_keys, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->PreventExtensions, cProps_pp[18], nh_ecmascript_preventExtensions, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Seal, cProps_pp[19], nh_ecmascript_seal, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->SetPrototypeOf, cProps_pp[20], nh_ecmascript_setPrototypeOf, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Values, cProps_pp[21], nh_ecmascript_values, Realm_p
////    ))
//
//NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
//}
//
//static NH_ECMASCRIPT_RESULT nh_ecmascript_createIntrinsicFunctionPrototype(
//    nh_ecmascript_IntrinsicFunction *Function_p, nh_ecmascript_Realm *Realm_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
////    Object_p->Prototype.Properties = nh_ecmascript_initProperties(7);
////    Object_p->Prototype.InternalSlots = nh_ecmascript_initInternalSlots(NULL, -1);
////    Object_p->Prototype.InternalMethods_p = &ObjectPrototypeInternalMethods;
////
////    Object_p->Prototype.InternalSlots.values_pp[
////        Object_p->Prototype.InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE]
////    ] = (void*)NH_TRUE;
////    Object_p->Prototype.InternalSlots.values_pp[
////        Object_p->Prototype.InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE]
////    ] = NULL;
////
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicDataProperty(
////        &Object_p->Prototype, pProps_pp[0], NH_FALSE, NH_FALSE, NH_FALSE, NH_ECMASCRIPT_TYPE_OBJECT,
////        &Realm_p->Intrinsics.Object.Constructor
////    ))
////
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->HasOwnProperty, pProps_pp[1], nh_ecmascript_hasOwnProperty, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->IsPrototypeOf, pProps_pp[2], nh_ecmascript_isPrototypeOf, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->PropertyIsEnumerable, pProps_pp[3], nh_ecmascript_propertyIsEnumerable, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->ToLocaleString, pProps_pp[4], nh_ecmascript_toLocaleString, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->ToString, pProps_pp[5], _nh_ecmascript_toString, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->ValueOf, pProps_pp[6], nh_ecmascript_valueOf, Realm_p
////    ))
//
//NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
//}
//
//NH_ECMASCRIPT_RESULT nh_ecmascript_createIntrinsicFunction(
//    nh_ecmascript_IntrinsicFunction *Function_p, nh_ecmascript_Realm *Realm_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
//    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicFunctionConstructor(Function_p, Realm_p))
//    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicFunctionPrototype(Function_p, Realm_p))
//
//NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
//}
//
//NH_ECMASCRIPT_RESULT nh_ecmascript_freeIntrinsicFunction(
//    nh_ecmascript_IntrinsicFunction *Function_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
//    // TODO
//
//NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
//}
//
