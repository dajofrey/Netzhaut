// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "TestAndCompare.h"

// FUNCTIONS =======================================================================================

// https://tc39.es/ecma262/#sec-samevalue
bool nh_ecmascript_sameValue(
    nh_ecmascript_Value x, 
    nh_ecmascript_Value y) 
{
    // 1. If types are different, return false
    if (x.tag != y.tag) return false;

    // 2. If Number, handle IEEE 754 quirks
    if (x.tag == NH_ECMASCRIPT_VALUE_NUMBER) {
        double dx = x.p.number;
        double dy = y.p.number;
        
        // Handle NaN (NaN is the only value not equal to itself)
//        if (isnan(dx) && isnan(dy)) return true;
        
        // Handle +0 vs -0
        if (dx == 0 && dy == 0) {
//            return signbit(dx) == signbit(dy);
        }
        
        return dx == dy;
    }

    // 3. SameValueNonNumeric (Strings, Objects, Booleans)
    return nh_ecmascript_sameValueNonNumeric(x, y);
}

// https://tc39.es/ecma262/#sec-samevaluenonnumeric
bool nh_ecmascript_sameValueNonNumeric(
    nh_ecmascript_Value x, 
    nh_ecmascript_Value y) 
{
    switch (x.tag) {
        case NH_ECMASCRIPT_VALUE_UNDEFINED:
        case NH_ECMASCRIPT_VALUE_NULL:
            return true;
        case NH_ECMASCRIPT_VALUE_STRING:
            // Since you likely use a String Table/Interning:
            return x.p.string == y.p.string; 
            // If not interning: return strcmp(x.p.string, y.p.string) == 0;
        case NH_ECMASCRIPT_VALUE_BOOLEAN:
            return x.p.boolean == y.p.boolean;
        case NH_ECMASCRIPT_VALUE_OBJECT:
            return x.p.object == y.p.object; // Reference equality
        default:
            return false;
    }
}

bool nh_ecmascript_isUndefined(
    nh_ecmascript_Value Value)
{
    return Value.tag == NH_ECMASCRIPT_VALUE_UNDEFINED;
}

bool nh_ecmascript_isObject(
    nh_ecmascript_Value Value)
{
    return Value.tag == NH_ECMASCRIPT_VALUE_OBJECT;
}

// https://tc39.es/ecma262/#sec-iscallable
bool nh_ecmascript_isCallable(nh_ecmascript_Value v) 
{
    if (v.tag != NH_ECMASCRIPT_VALUE_OBJECT) return false;
    
    // Check if it's one of your function types
//    return (v.p.object->type == NH_ECMASCRIPT_OBJECT_FUNCTION_BUILTIN || 
//            v.p.object->type == NH_ECMASCRIPT_OBJECT_FUNCTION_ORDINARY);

return false;
}

// https://tc39.es/ecma262/#sec-isconstructor
bool nh_ecmascript_isConstructor(nh_ecmascript_Value v) 
{
    if (v.tag != NH_ECMASCRIPT_VALUE_OBJECT) return false;
    
    // Not all functions are constructors (e.g., Arrow Functions)
    // You should add an 'isConstructor' flag to your nh_ecmascript_Object struct
//    return v.p.object->isConstructor;
return false;
}

// https://tc39.es/ecma262/#sec-requireobjectcoercible
nh_ecmascript_Completion nh_ecmascript_requireObjectCoercible(
    nh_ecmascript_Value v, 
    nh_ecmascript_Realm *Realm_p) 
{
    if (v.tag == NH_ECMASCRIPT_VALUE_UNDEFINED || v.tag == NH_ECMASCRIPT_VALUE_NULL) {
        return nh_ecmascript_throwTypeError("Value is not coercible to an object", Realm_p);
    }
    return nh_ecmascript_normalCompletion(v);
}

// https://tc39.es/ecma262/#sec-isextensible-obj
nh_ecmascript_Completion nh_ecmascript_isExtensible(
    nh_ecmascript_Object *O_p,
    nh_ecmascript_Realm *Realm_p) 
{
    // In a system with a dispatch table (Option B from before):
    // return O_p->ops->isExtensible(O_p, Realm_p);

    // Using your current Option A approach (Hardcoded Ordinary):
//    return nh_ecmascript_ordinaryIsExtensible(O_p, Realm_p);

return nh_ecmascript_normalEmptyCompletion();
}

/**
 * https://tc39.es/ecma262/#sec-isarray
 */
bool nh_ecmascript_isArray(
    nh_ecmascript_Value argument, 
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. If Type(argument) is not Object, return false.
    // (Using your tag check logic)
    if (argument.tag != NH_ECMASCRIPT_VALUE_OBJECT || argument.p.object == NULL) {
        return false;
    }

    nh_ecmascript_Object *O_p = argument.p.object;

    // 2. If O_p is an Array exotic object, return true.
    // We identify "Array-ness" by checking if it uses the Array method table.
    if (O_p->Methods_p == &NH_ECMASCRIPT_METHODS_ARRAY) {
        return true;
    }

    // 3. If O_p is a Proxy exotic object, handle recursion.
    // Proxies also have their own specific method table.
//    if (O_p->Methods_p == &NH_ECMASCRIPT_METHODS_PROXY) {
//        // According to 7.2.2, if it's a proxy, we check the target.
//        // The target is stored in an internal slot for proxies.
//        nh_ecmascript_Value target = O_p->Slots[NH_ECMASCRIPT_SLOT_PROXY_TARGET];
//        return nh_ecmascript_isArray(target, Realm_p);
//    }

    return false;
}

