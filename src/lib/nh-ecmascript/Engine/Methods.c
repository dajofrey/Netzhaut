// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Methods.h"
#include "Realm.h"
#include "Shape.h"
#include "PropertyDescriptor.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdlib.h>

// FUNCTIONS =====================================================================================

nh_ecmascript_Completion nh_ecmascript_ordinaryGetPrototypeOf(
    nh_ecmascript_Object *F)
{
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_ordinarySetPrototypeOf(
    nh_ecmascript_Object *F, nh_ecmascript_Object *V)
{
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_ordinaryIsExtensible(
    nh_ecmascript_Object *F)
{
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_ordinaryPreventExtensions(
    nh_ecmascript_Object *F)
{
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_ordinaryGetOwnProperty(
    nh_ecmascript_Object *F,
    char *p,
    nh_ecmascript_PropertyDescriptor *Property_p)
{
    return nh_ecmascript_normalEmptyCompletion();
}

// https://tc39.es/ecma262/#sec-ordinarydefineownproperty
nh_ecmascript_Completion nh_ecmascript_ordinaryDefineOwnProperty(
    nh_ecmascript_Object *Object_p,
    char *name_p,
    nh_ecmascript_PropertyDescriptor *Desc_p)
{
    // 1. Get the current index of the property from the Shape
    int index = nh_ecmascript_getIndexFromShape(Object_p->Shape_p, name_p);

    // 2. If the property doesn't exist yet, we add it
    if (index == -1) {
        // A. Check [[Extensible]] (Slot 1)
        if (!nh_ecmascript_toBool(Object_p->Slots[NH_ECMASCRIPT_SLOT_EXTENSIBLE])) {
            return nh_ecmascript_throwTypeError();
        }

        // B. Find or Create a new Shape for this transition
        // This moves Object_p->Shape_p to a new Shape that includes P
        uint8_t attrs = nh_ecmascript_getAttributeFlags(Desc_p);
        nh_ecmascript_Shape *NewShape_p = nh_ecmascript_transitionShape(Object_p->Shape_p, name_p, attrs, 0); // TODO
        Object_p->Shape_p = NewShape_p;

        // C. Get the new index (usually the last index in the new shape)
        index = NewShape_p->property_count - 1;

        // D. Ensure Properties_p is large enough
        Object_p->Properties_p = realloc(Object_p->Properties_p, sizeof(nh_ecmascript_Value) * NewShape_p->property_count);
    }

    // 3. Validation Logic (Simplified Spec Compliance)
    // In a full implementation, you'd compare Desc_p against the existing property
    // (checking [[Configurable]], [[Writable]], etc.)

    // 4. Update the actual value in the flat array
    Object_p->Properties_p[index] = Desc_p->Value;

    return nh_ecmascript_normalEmptyCompletion();
}

// https://tc39.es/ecma262/#sec-ordinarydefineownproperty
nh_ecmascript_Completion nh_ecmascript_arrayDefineOwnProperty(
    nh_ecmascript_Object *Object_p,
    char *P,
    nh_ecmascript_PropertyDescriptor *Desc_p)
{
    return nh_ecmascript_normalEmptyCompletion();
}

// https://tc39.es/ecma262/#sec-ordinaryhasproperty
bool nh_ecmascript_ordinaryHasProperty(
    nh_ecmascript_Object *Object_p, 
    char *p) 
{
    return false;
}

// The actual logic: OrdinaryGet(O, P, Receiver)
static nh_ecmascript_Completion nh_ecmascript_ordinaryGet(
    nh_ecmascript_Object *Object_p, 
    char *P, 
    nh_ecmascript_Object *Receiver_p) 
{
    return nh_ecmascript_normalEmptyCompletion();
}

// https://tc39.es/ecma262/#sec-ordinaryset
static nh_ecmascript_Completion nh_ecmascript_ordinarySet(
    nh_ecmascript_Object *Object_p, 
    char *P, 
    nh_ecmascript_Value V, 
    nh_ecmascript_Object *Receiver_p) 
{
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_ordinaryDelete(
    nh_ecmascript_Object *F, char *p)
{
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_ordinaryOwnPropertyKeys(
    nh_ecmascript_Object *F)
{
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_functionCall(
    nh_ecmascript_Object *F, 
    nh_ecmascript_Value thisArgument, 
    nh_core_List *argumentsList)
{
    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_functionConstruct(
    nh_ecmascript_Object *F, 
    nh_core_List argumentsList,
    nh_ecmascript_Object *newTarget)
{
    return nh_ecmascript_normalEmptyCompletion();
}

// DATA ===========================================================================================

const nh_ecmascript_ObjectMethods NH_ECMASCRIPT_METHODS_ORDINARY = {
    .getPrototypeOf    = nh_ecmascript_ordinaryGetPrototypeOf,
    .setPrototypeOf    = nh_ecmascript_ordinarySetPrototypeOf,
    .isExtensible      = nh_ecmascript_ordinaryIsExtensible,
    .preventExtensions = nh_ecmascript_ordinaryPreventExtensions,
    .getOwnProperty    = nh_ecmascript_ordinaryGetOwnProperty,
    .defineOwnProperty = nh_ecmascript_ordinaryDefineOwnProperty,
    .hasProperty       = nh_ecmascript_ordinaryHasProperty,
    .get               = nh_ecmascript_ordinaryGet,
    .set               = nh_ecmascript_ordinarySet,
    .delete            = nh_ecmascript_ordinaryDelete,
    .ownPropertyKeys   = nh_ecmascript_ordinaryOwnPropertyKeys,
    .call              = NULL,
    .construct         = NULL
};

const nh_ecmascript_ObjectMethods NH_ECMASCRIPT_METHODS_FUNCTION = {
    .getPrototypeOf    = nh_ecmascript_ordinaryGetPrototypeOf,
    .setPrototypeOf    = nh_ecmascript_ordinarySetPrototypeOf,
    .isExtensible      = nh_ecmascript_ordinaryIsExtensible,
    .preventExtensions = nh_ecmascript_ordinaryPreventExtensions,
    .getOwnProperty    = nh_ecmascript_ordinaryGetOwnProperty,
    .defineOwnProperty = nh_ecmascript_ordinaryDefineOwnProperty,
    .hasProperty       = nh_ecmascript_ordinaryHasProperty,
    .get               = nh_ecmascript_ordinaryGet,
    .set               = nh_ecmascript_ordinarySet,
    .delete            = nh_ecmascript_ordinaryDelete,
    .ownPropertyKeys   = nh_ecmascript_ordinaryOwnPropertyKeys,
    .call              = nh_ecmascript_functionCall,      // Standard 10.2.1
    .construct         = nh_ecmascript_functionConstruct  // Standard 10.2.2
};

const nh_ecmascript_ObjectMethods NH_ECMASCRIPT_METHODS_ARRAY = {
    .getPrototypeOf    = nh_ecmascript_ordinaryGetPrototypeOf,
    .setPrototypeOf    = nh_ecmascript_ordinarySetPrototypeOf,
    .isExtensible      = nh_ecmascript_ordinaryIsExtensible,
    .preventExtensions = nh_ecmascript_ordinaryPreventExtensions,
    .getOwnProperty    = nh_ecmascript_ordinaryGetOwnProperty,
    .defineOwnProperty = nh_ecmascript_arrayDefineOwnProperty, // Exotic 10.4.2.1
    .hasProperty       = nh_ecmascript_ordinaryHasProperty,
    .get               = nh_ecmascript_ordinaryGet,
    .set               = nh_ecmascript_ordinarySet,
    .delete            = nh_ecmascript_ordinaryDelete,
    .ownPropertyKeys   = nh_ecmascript_ordinaryOwnPropertyKeys,
    .call              = NULL,
    .construct         = NULL
};
