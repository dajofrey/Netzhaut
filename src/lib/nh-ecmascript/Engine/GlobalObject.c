// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "GlobalObject.h"
#include <string.h>

// DATA ============================================================================================

static const int lookup_p[] = {
     0
};

static const char *defaultGlobalBindings_pp[] = {
// values
    "globalThis",
    "Infinity",
    "NaN",
    "undefined",
// functions
    "eval",
    "isFinite",
    "isNaN",
    "parseFloat",
    "parseInt",
// constructors
    "Array",
    "ArrayBuffer",
    "BigInt",
    "BigInt64Array",
    "BigUint64Array",
    "Boolean",
    "DataView",
    "Date",
    "Error",
    "EvalError",
    "FinalizationRegistry",
    "Float32Array",
    "Float64Array", 
    "Function",
    "Int8Array",
    "Int16Array",
    "Int32Array",
    "Map",
    "Number",
    "Object",
    "Promise",
    "Proxy",
    "RangeError",
    "ReferenceError",
    "RegExp",
    "Set",
    "SharedArrayBuffer",
    "String",
    "Symbol",
    "SyntaxError",
    "TypeError",
    "Uint8Array",
    "Uint8ClampedArray",
    "Uint16Array",
    "Uint32Array",
    "URIError",
    "WeakMap",
    "WeakRef",
    "WeakSet",
// objects
    "Atomics",
    "JSON",
    "Math",
    "Reflect",
};

// CREATE/FREE =====================================================================================

static NH_API_RESULT nh_ecmascript_createGlobalObjectValueProperties(
    nh_ecmascript_Object *Object_p, nh_ecmascript_Realm *Realm_p)
{
    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_ecmascript_createGlobalObjectFunctionProperties(
    nh_ecmascript_Object *Object_p, nh_ecmascript_Realm *Realm_p)
{
    return NH_API_SUCCESS;
}

static nh_ecmascript_Property *nh_ecmascript_getGlobalProperty(
    nh_ecmascript_Realm *Realm_p, int i)
{
    Realm_p->Intrinsics.Properties_p[i].type = NH_ECMASCRIPT_PROPERTY_DATA;
    Realm_p->Intrinsics.Properties_p[i].enumerable   = false;
    Realm_p->Intrinsics.Properties_p[i].configurable = false;
    Realm_p->Intrinsics.Properties_p[i].Fields.Data.writable = false;

    nh_ecmascript_Any Value;
    Value.type = NH_ECMASCRIPT_TYPE_OBJECT;
    Value.handle_p = NULL;

    switch (i) 
    {
          case 0 :
          case 1 : Value.type = NH_ECMASCRIPT_TYPE_INFINITY; break;
          case 2 : Value.type = NH_ECMASCRIPT_TYPE_NOT_A_NUMBER; break;
          case 3 : Value.type = NH_ECMASCRIPT_TYPE_UNDEFINED; break;
          case 4 :
          case 5 :
          case 6 :
          case 7 :
          case 8 : break;
//        case  9 : Any.handle_p = &Realm_p->Intrinsics.Array.Constructor; break;
//        case 10 : Any.handle_p = &Realm_p->Intrinsics.ArrayBuffer.Constructor; break;
//        case 11 : Any.handle_p = &Realm_p->Intrinsics.BigInt.Constructor; break;
//        case 12 : Any.handle_p = &Realm_p->Intrinsics.BigInt64Array.Constructor; break;
//        case 13 : Any.handle_p = &Realm_p->Intrinsics.BigUint64Array.Constructor; break;
//        case 14 : Any.handle_p = &Realm_p->Intrinsics.Boolean.Constructor; break;
//        case 15 : Any.handle_p = &Realm_p->Intrinsics.DataView.Constructor; break;
//        case 16 : Any.handle_p = &Realm_p->Intrinsics.Date.Constructor; break;
//        case 17 : Any.handle_p = &Realm_p->Intrinsics.Error.Constructor; break;
//        case 18 : Any.handle_p = &Realm_p->Intrinsics.EvalError.Constructor; break;
//        case 19 : Any.handle_p = &Realm_p->Intrinsics.FinalizationRegistry.Constructor; break;
//        case 20 : Any.handle_p = &Realm_p->Intrinsics.Float32Array.Constructor; break;
//        case 21 : Any.handle_p = &Realm_p->Intrinsics.Float64Array.Constructor; break;
        case 22 : Value.handle_p = &Realm_p->Intrinsics.Function.Constructor; break;
//        case 23 : Any.handle_p = &Realm_p->Intrinsics.Int8Array.Constructor; break;
//        case 24 : Any.handle_p = &Realm_p->Intrinsics.Int16Array.Constructor; break;
//        case 25 : Any.handle_p = &Realm_p->Intrinsics.Int32Array.Constructor; break;
//        case 26 : Any.handle_p = &Realm_p->Intrinsics.Map.Constructor; break;
//        case 27 : Any.handle_p = &Realm_p->Intrinsics.Number.Constructor; break;
        case 28 : Value.handle_p = &Realm_p->Intrinsics.Object.Constructor; break;
//        case 29 : Any.handle_p = &Realm_p->Intrinsics.Promise.Constructor; break;
//        case 30 : Any.handle_p = &Realm_p->Intrinsics.Proxy.Constructor; break;
//        case 31 : Any.handle_p = &Realm_p->Intrinsics.RangeError.Constructor; break;
//        case 32 : Any.handle_p = &Realm_p->Intrinsics.ReferenceError.Constructor; break;
//        case 33 : Any.handle_p = &Realm_p->Intrinsics.RegExp.Constructor; break;
//        case 34 : Any.handle_p = &Realm_p->Intrinsics.Set.Constructor; break;
//        case 35 : Any.handle_p = &Realm_p->Intrinsics.SharedArrayBuffer.Constructor; break;
//        case 36 : Any.handle_p = &Realm_p->Intrinsics.String.Constructor; break;
//        case 37 : Any.handle_p = &Realm_p->Intrinsics.Symbol.Constructor; break;
//        case 38 : Any.handle_p = &Realm_p->Intrinsics.SyntaxError.Constructor; break;
//        case 39 : Any.handle_p = &Realm_p->Intrinsics.TypeError.Constructor; break;
//        case 40 : Any.handle_p = &Realm_p->Intrinsics.Uint8Array.Constructor; break;
//        case 41 : Any.handle_p = &Realm_p->Intrinsics.Uint8ClampedArray.Constructor; break;
//        case 42 : Any.handle_p = &Realm_p->Intrinsics.Uint16Array.Constructor; break;
//        case 43 : Any.handle_p = &Realm_p->Intrinsics.Uint32Array.Constructor; break;
//        case 44 : Any.handle_p = &Realm_p->Intrinsics.URIError.Constructor; break;
//        case 45 : Any.handle_p = &Realm_p->Intrinsics.WeakMap.Constructor; break;
//        case 46 : Any.handle_p = &Realm_p->Intrinsics.WeakRef.Constructor; break;
//        case 47 : Any.handle_p = &Realm_p->Intrinsics.WeakSet.Constructor; break;
          case 48 :
          case 49 :
          case 50 :
          case 51 : break;
    }

    Realm_p->Intrinsics.Properties_p[i].Fields.Data.Value = Value;

    return &Realm_p->Intrinsics.Properties_p[i];
}

// corresponds to https://tc39.es/ecma262/#sec-setdefaultglobalbindings
nh_ecmascript_Object *nh_ecmascript_setDefaultGlobalBindings(
    nh_ecmascript_Realm *Realm_p)
{
    nh_ecmascript_Object *Global_p = Realm_p->GlobalObject_p;

    for (int i = 0; i < sizeof(defaultGlobalBindings_pp) / sizeof(defaultGlobalBindings_pp[0]); ++i) 
    {
        Realm_p->Intrinsics.Keys_p[i].p = (char*)defaultGlobalBindings_pp[i];
        Realm_p->Intrinsics.Keys_p[i].length  = strlen(defaultGlobalBindings_pp[i]);

        nh_ecmascript_Property *Property_p = nh_ecmascript_getGlobalProperty(Realm_p, i);
        Property_p->Key = nh_ecmascript_wrapString(&Realm_p->Intrinsics.Keys_p[i]);

        nh_core_appendToList(&Global_p->Properties, Property_p);
    }

    return Global_p;
}

NH_API_RESULT nh_ecmascript_freeDefaultGlobalBindings(
    nh_ecmascript_Object *Object_p)
{
    // TODO
    return NH_API_SUCCESS;
}
