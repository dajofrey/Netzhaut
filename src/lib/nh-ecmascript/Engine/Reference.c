// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Reference.h"
#include "ExecutionContext.h"
#include "../Interpreter/Instantiation.h"

#include "../Parser/Lexer.h"
#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdio.h>

// FUNCTIONS ======================================================================================

nh_ecmascript_Reference nh_ecmascript_initReference()
{
    nh_ecmascript_Reference ref;
    ref.baseType = NH_ECMASCRIPT_REF_BASE_UNRESOLVABLE;
    ref.ReferencedName = nh_ecmascript_makeUndefined();
    ref.strict = false;
    ref.ThisValue.hasValue = false;
    return ref;
}

nh_ecmascript_Reference nh_ecmascript_getIdentifierReference(
    nh_ecmascript_Environment *env, 
    nh_ecmascript_Value name, 
    bool strict)
{
    // 1. If env is NULL, we've reached the end of the chain without finding the binding
    if (env == NULL) {
        nh_ecmascript_Reference ref = nh_ecmascript_initReference();
        ref.ReferencedName = name;
        ref.strict = strict;
        return ref;
    }

    // 2. Check if the current environment has the binding
    if (nh_ecmascript_hasBinding(env, name.p.string)) {
        nh_ecmascript_Reference ref;
        ref.baseType = NH_ECMASCRIPT_REF_BASE_ENVIRONMENT;
        ref.Base.Environment_p = env;
        ref.ReferencedName = name;
        ref.strict = strict;
        ref.ThisValue.hasValue = false;
        return ref;
    }

    // 3. Recurse to outer environment
    return nh_ecmascript_getIdentifierReference(env->Outer_p, name, strict);
}

bool nh_ecmascript_isPropertyReference(nh_ecmascript_Reference ref)
{
    return (ref.baseType == NH_ECMASCRIPT_REF_BASE_OBJECT || 
            ref.baseType == NH_ECMASCRIPT_REF_BASE_PRIMITIVE);
}

// https://tc39.es/ecma262/#sec-getvalue
nh_ecmascript_Completion nh_ecmascript_getValue(
    nh_ecmascript_Reference ref, 
    nh_ecmascript_Realm *Realm_p)
{
    // 1. Handle Unresolvable (Throws ReferenceError)
    if (ref.baseType == NH_ECMASCRIPT_REF_BASE_UNRESOLVABLE) {
        return nh_ecmascript_throwReferenceError("Variable is not defined", Realm_p);
    }

    // 2. Handle Property References (obj.prop or "string".length)
    if (nh_ecmascript_isPropertyReference(ref)) {
        nh_ecmascript_Object *baseObj;

        if (ref.baseType == NH_ECMASCRIPT_REF_BASE_PRIMITIVE) {
            // Autoboxing: convert primitive to temporary object (e.g. "a".length)
            baseObj = nh_ecmascript_toObject(ref.Base.Primitive, Realm_p);
        } else {
            baseObj = ref.Base.Object_p;
        }

        // Use the 'get' internal method we implemented in Object.c
        return nh_ecmascript_get(baseObj, ref.ReferencedName.p.string, Realm_p);
    } 
    
    // 3. Handle Environment References (local/global variables)
    else {
        return nh_ecmascript_getBindingValue(
            ref.Base.Environment_p, 
            ref.ReferencedName.p.string, 
            ref.strict,
            Realm_p
        );
    }
}

// https://tc39.es/ecma262/#sec-putvalue
nh_ecmascript_Completion nh_ecmascript_putValue(
    nh_ecmascript_Reference ref, 
    nh_ecmascript_Value value,
    nh_ecmascript_Realm *Realm_p)
{
    // 1. Handle Unresolvable
    if (ref.baseType == NH_ECMASCRIPT_REF_BASE_UNRESOLVABLE) {
        if (ref.strict) {
            return nh_ecmascript_throwReferenceError("Assigning to undefined variable", Realm_p);
        }
        // In non-strict mode, assigning to unresolvable creates a global variable
        return nh_ecmascript_set(Realm_p->globalObject, ref.ReferencedName.p.string, value, Realm_p);
    }

    // 2. Handle Property References
    if (nh_ecmascript_isPropertyReference(ref)) {
        nh_ecmascript_Object *baseObj;
        
        if (ref.baseType == NH_ECMASCRIPT_REF_BASE_PRIMITIVE) {
            baseObj = nh_ecmascript_toObject(ref.Base.Primitive, Realm_p);
        } else {
            baseObj = ref.Base.Object_p;
        }

        return nh_ecmascript_set(baseObj, ref.ReferencedName.p.string, value, Realm_p);
    }

    // 3. Handle Environment References
    else {
        return nh_ecmascript_setMutableBinding(
            ref.Base.Environment_p,
            ref.ReferencedName.p.string,
            value,
            ref.strict,
            Realm_p
        );
    }
}
