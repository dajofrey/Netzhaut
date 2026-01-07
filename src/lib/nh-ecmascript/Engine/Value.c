// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Value.h"
#include "Realm.h"
#include "Object.h"
#include "Completion.h"

#include <math.h>
#include <string.h>

// FUNCTIONS =========================================================================================

// https://tc39.es/ecma262/#sec-toobject
nh_ecmascript_Completion nh_ecmascript_toObject(
    nh_ecmascript_Value Value, 
    nh_ecmascript_Realm *Realm_p) 
{
    switch (Value.tag) {
        case NH_ECMASCRIPT_VALUE_UNDEFINED:
        case NH_ECMASCRIPT_VALUE_NULL:
            return nh_ecmascript_throwTypeError("Cannot convert undefined or null to object", Realm_p);

        case NH_ECMASCRIPT_VALUE_BOOLEAN:
            // Create a new Boolean Object with [[BooleanData]] internal slot
            // You'll need to implement Boolean constructor first
            return nh_ecmascript_throwTypeError("Boolean boxing not yet implemented", Realm_p);

        case NH_ECMASCRIPT_VALUE_NUMBER:
            // Create a new Number Object
            return nh_ecmascript_throwTypeError("Number boxing not yet implemented", Realm_p);

        case NH_ECMASCRIPT_VALUE_STRING:
            // Create a new String Object
            return nh_ecmascript_throwTypeError("String boxing not yet implemented", Realm_p);

        case NH_ECMASCRIPT_VALUE_OBJECT:
            // If it's already an object, just return it
            return nh_ecmascript_normalCompletion(Value);

        case NH_ECMASCRIPT_VALUE_SYMBOL:
            return nh_ecmascript_throwTypeError("Symbol boxing not yet implemented", Realm_p);

        default:
            return nh_ecmascript_throwTypeError("Unknown type", Realm_p);
    }
}

bool nh_ecmascript_toBool(
    nh_ecmascript_Value v) 
{
    switch (v.tag) {
        case NH_ECMASCRIPT_VALUE_UNDEFINED:
        case NH_ECMASCRIPT_VALUE_NULL:
            return false;
            
        case NH_ECMASCRIPT_VALUE_BOOLEAN:
            return v.p.boolean;
            
        case NH_ECMASCRIPT_VALUE_NUMBER:
            // false if 0, -0, or NaN. Otherwise true.
            if (v.p.number == 0.0 || isnan(v.p.number)) return false;
            return true;
            
        case NH_ECMASCRIPT_VALUE_STRING:
            // false if empty string, otherwise true.
            // (Assumes your string implementation has a length check)
            return strlen(v.p.string) > 0;
            
        case NH_ECMASCRIPT_VALUE_OBJECT:
        case NH_ECMASCRIPT_VALUE_SYMBOL:
            return true;
            
        default:
            return false;
    }
}

// https://tc39.es/ecma262/#sec-tostring
nh_ecmascript_Completion nh_ecmascript_toString(
    nh_ecmascript_Value v, 
    nh_ecmascript_Realm *Realm_p) 
{
    switch (v.tag) {
        case NH_ECMASCRIPT_VALUE_UNDEFINED:
            return nh_ecmascript_normalCompletion(nh_ecmascript_makeString("undefined"));
        case NH_ECMASCRIPT_VALUE_NULL:
            return nh_ecmascript_normalCompletion(nh_ecmascript_makeString("null"));
        case NH_ECMASCRIPT_VALUE_BOOLEAN:
            return nh_ecmascript_normalCompletion(nh_ecmascript_makeString(v.p.boolean ? "true" : "false"));
        case NH_ECMASCRIPT_VALUE_NUMBER:
//            // You'll need a helper like nh_core_formatNumberToString
//            return nh_ecmascript_normalCompletion(nh_ecmascript_formatNumber(v.p.number));
        case NH_ECMASCRIPT_VALUE_STRING:
            return nh_ecmascript_normalCompletion(v);
        case NH_ECMASCRIPT_VALUE_SYMBOL:
            return nh_ecmascript_throwTypeError("Cannot convert a Symbol value to a string", Realm_p);
        case NH_ECMASCRIPT_VALUE_OBJECT: {
            nh_ecmascript_Completion prim = nh_ecmascript_toPrimitive(v, NH_ECMASCRIPT_PREFERRED_TYPE_STRING, Realm_p);
            if (prim.type == NH_ECMASCRIPT_COMPLETION_THROW) return prim;
            return nh_ecmascript_toString(prim.Value, Realm_p);
        }
        default:
            return nh_ecmascript_throwTypeError("Unknown type in toString", Realm_p);
    }
}

// https://tc39.es/ecma262/#sec-topropertykey
nh_ecmascript_Completion nh_ecmascript_toPropertyKey(
    nh_ecmascript_Value v, 
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. Let key be ? ToPrimitive(v, string).
    nh_ecmascript_Completion keyComp = nh_ecmascript_toPrimitive(v, NH_ECMASCRIPT_PREFERRED_TYPE_STRING, Realm_p);
    if (keyComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return keyComp;
    
    nh_ecmascript_Value key = keyComp.Value;

    // 2. If Type(key) is Symbol, return key.
    if (key.tag == NH_ECMASCRIPT_VALUE_SYMBOL) {
        return nh_ecmascript_normalCompletion(key);
    }

    // 3. Return ! ToString(key).
    return nh_ecmascript_toString(key, Realm_p);
}

// https://tc39.es/ecma262/#sec-ordinarytoprimitive
static nh_ecmascript_Completion nh_ecmascript_ordinaryToPrimitive(
    nh_ecmascript_Object *O_p, 
    NH_ECMASCRIPT_PREFERRED_TYPE_E hint, 
    nh_ecmascript_Realm *Realm_p) 
{
    const char *methodNames[2];
    if (hint == NH_ECMASCRIPT_PREFERRED_TYPE_STRING) {
        methodNames[0] = "toString";
        methodNames[1] = "valueOf";
    } else {
        methodNames[0] = "valueOf";
        methodNames[1] = "toString";
    }

    for (int i = 0; i < 2; i++) {
        // Use your nh_ecmascript_get to find the method
        nh_ecmascript_Completion methodComp = nh_ecmascript_get(O_p, methodNames[i], Realm_p);
        if (methodComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return methodComp;
        
        nh_ecmascript_Value method = methodComp.Value;
        if (method.tag == NH_ECMASCRIPT_VALUE_OBJECT) { // Simplified check for "IsCallable"
//             nh_ecmascript_Completion callComp = nh_ecmascript_call(
//                 method.p.object, nh_ecmascript_makeObject(O_p), NULL, 0, Realm_p
//             );
//             if (callComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return callComp;
//             if (callComp.Value.tag != NH_ECMASCRIPT_VALUE_OBJECT) return callComp;
        }
    }

    return nh_ecmascript_throwTypeError("Cannot convert object to primitive value", Realm_p);
}

// https://tc39.es/ecma262/#sec-toprimitive
nh_ecmascript_Completion nh_ecmascript_toPrimitive(
    nh_ecmascript_Value v, 
    NH_ECMASCRIPT_PREFERRED_TYPE_E hint, 
    nh_ecmascript_Realm *Realm_p) 
{
    if (v.tag != NH_ECMASCRIPT_VALUE_OBJECT) {
        return nh_ecmascript_normalCompletion(v);
    }

    // 1. Check for Symbol.toPrimitive (simplified: assuming not present for now)
    // 2. Fallback to OrdinaryToPrimitive
    return nh_ecmascript_ordinaryToPrimitive(v.p.object, hint, Realm_p);
}
