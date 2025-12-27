// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "BuiltinFunctionObject.h"
#include "OrdinaryObject.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>

// BUILTIN FUNCTION ================================================================================

static nh_ecmascript_Completion nh_ecmascript_builtinCall(
    nh_ecmascript_Object *This_p, nh_ecmascript_Any ThisArgument, nh_core_List ArgumentsList)
{
    return nh_ecmascript_normalEmptyCompletion();
}

static nh_ecmascript_Object *nh_ecmascript_builtinConstruct(
    nh_core_List Arguments, nh_ecmascript_Object *Target_p)
{
    return NULL;
}

// DATA ============================================================================================

static nh_ecmascript_InternalMethods BuiltinFunctionObjectInternalMethods = {
    .getPrototypeOf_f    = nh_ecmascript_ordinaryGetPrototypeOf,
    .setPrototypeOf_f    = nh_ecmascript_ordinarySetPrototypeOf,
    .isExtensible_f      = nh_ecmascript_ordinaryIsExtensible,
    .preventExtensions_f = nh_ecmascript_ordinaryPreventExtensions,
    .getOwnProperty_f    = nh_ecmascript_ordinaryGetOwnProperty,
    .defineOwnProperty_f = nh_ecmascript_ordinaryDefineOwnProperty,
    .hasProperty_f       = nh_ecmascript_ordinaryHasProperty,
    .get_f               = nh_ecmascript_ordinaryGet,
    .set_f               = nh_ecmascript_ordinarySet,
    .delete_f            = nh_ecmascript_ordinaryDelete,
    .ownPropertyKeys_f   = nh_ecmascript_ordinaryOwnPropertyKeys,
    .call_f              = nh_ecmascript_builtinCall,
    .construct_f         = nh_ecmascript_builtinConstruct,
};

static int builtinFunctionObjectLookup_p[] = {
     0,  1,  2,  3,  4
};

// CREATE ==========================================================================================

// https://tc39.es/ecma262/#sec-createbuiltinfunction
nh_ecmascript_Object *nh_ecmascript_createBuiltinFunctionObject(
    nh_ecmascript_Object *FunctionObject_p, nh_ecmascript_Any (*call_f)(nh_ecmascript_Object *This_p, nh_core_List Arguments),
    int *lookup_p, int lookupLength, nh_ecmascript_Realm *Realm_p, nh_ecmascript_Object *Prototype_p)
{
    if (Prototype_p == NULL) {
        Prototype_p = &Realm_p->Intrinsics.Function.Prototype;
    }

    if (FunctionObject_p == NULL) {
        FunctionObject_p = (nh_ecmascript_Object*)nh_core_allocate(sizeof(nh_ecmascript_Object));
        NH_CORE_CHECK_MEM_2(NULL, FunctionObject_p)
    }

    if (lookup_p == NULL || lookupLength < 5) {
        lookup_p = builtinFunctionObjectLookup_p;
        lookupLength = 5;
    }

    if (lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_REALM] < 0
    ||  lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE] < 0
    ||  lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE] < 0
    ||  lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_BUILTIN] < 0
    ||  lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_INITIAL_NAME] < 0) {
        return NULL;
    }

//    FunctionObject_p->InternalSlots     = nh_ecmascript_initInternalSlots(lookup_p, lookupLength);
//    FunctionObject_p->Properties        = nh_core_initList(8);
//    FunctionObject_p->InternalMethods_p = &BuiltinFunctionObjectInternalMethods;
//
//    nh_ecmascript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_REALM, Realm_p);
//    nh_ecmascript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE, Prototype_p);
//    nh_ecmascript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE, (void*)true);
//    nh_ecmascript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_INITIAL_NAME, NULL);
//    nh_ecmascript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_BUILTIN, call_f);

    return FunctionObject_p;
}

