// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Object.h"
#include "Realm.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdlib.h>

// LIFECYCLE FUNCTIONS ====================================================================

/**
 * https://tc39.es/ecma262/#sec-ordinaryobjectcreate
 * * @param Prototype_p The object to use as [[Prototype]]. Can be NULL.
 * @param InternalSlotsList A list of additional slot IDs needed (e.g., for Date/Array)
 */
nh_ecmascript_Object *nh_ecmascript_ordinaryObjectCreate(
    nh_ecmascript_Object *Prototype_p,
    nh_core_List *InternalSlotsList,
    nh_ecmascript_Realm *Realm_p) 
{
    nh_ecmascript_Object *O_p = malloc(sizeof(nh_ecmascript_Object));
    if (!O_p) return NULL;

    O_p->Methods_p = &NH_ECMASCRIPT_METHODS_ORDINARY;
    O_p->Shape_p = Realm_p->EmptyObjectShape_p;

    // Initialize standard slots
    O_p->Slots_p[NH_ECMASCRIPT_SLOT_PROTOTYPE] = (Prototype_p) 
        ? nh_ecmascript_makeObject(Prototype_p) 
        : nh_ecmascript_makeNull();
    O_p->Slots_p[NH_ECMASCRIPT_SLOT_EXTENSIBLE] = nh_ecmascript_makeBool(true);

    // 5. Allocate Property Storage
    // Since the initial shape is empty, we can defer this or allocate a small buffer
    O_p->Properties_p = NULL; 

    return O_p;
}

// https://tc39.es/ecma262/#sec-ordinaryfunctioncreate
nh_ecmascript_Object *nh_ecmascript_ordinaryFunctionCreate(
    nh_ecmascript_Object *prototype,
    nh_ecmascript_ASTNode *sourceCode, // Or bytecode
    nh_ecmascript_Environment *env,
    nh_ecmascript_Realm *realm)
{
    // 1. Create the base object
    // If prototype is NULL, the spec says use %Function.prototype%
//    nh_ecmascript_Object *proto = prototype ? prototype : realm->intrinsics.functionPrototype;
//    nh_ecmascript_Object *F = nh_ecmascript_ordinaryObjectCreate(proto, NULL, 4);
//
//    // 2. Set Internal Slots specific to JS functions
//    F->type = NH_ECMASCRIPT_OBJECT_FUNCTION_ORDINARY;
////    F->ecmascriptCode = sourceCode;
//
//    nh_ecmascript_InternalSlot *realmSlot = (nh_ecmascript_InternalSlot*)nh_core_allocate(sizeof(nh_ecmascript_InternalSlot));
//    realmSlot->type = NH_ECMASCRIPT_INTERNAL_SLOT_CURRENT_REALM;
//    realmSlot->p = realm;
//    nh_core_appendToList(&F->InternalSlots, realmSlot);
//
//    nh_ecmascript_InternalSlot *envSlot = (nh_ecmascript_InternalSlot*)nh_core_allocate(sizeof(nh_ecmascript_InternalSlot));
//    envSlot->type = NH_ECMASCRIPT_INTERNAL_SLOT_ENVIRONMENT;
//    envSlot->p = realm;
//    nh_core_appendToList(&F->InternalSlots, envSlot);

    // 3. Ordinary functions are always extensible
//    F->extensible = true;

    // 4. Note: length and name are usually set by the caller (the parser/interpreter)
    // based on the function declaration.

//    return F;
return NULL;
}

/**
 * https://tc39.es/ecma262/#sec-createbuiltinfunction
 * @param steps The C function pointer (the native implementation)
 * @param InternalSlotsList Any extra state needed (beyond Call/Construct/Realm)
 * @param Prototype_p Usually %Function.prototype%
 */
nh_ecmascript_Object *nh_ecmascript_createBuiltinFunction(
    void *steps_p, 
    nh_core_List *InternalSlotsList,
    nh_ecmascript_Realm *Realm_p,
    nh_ecmascript_Object *Prototype_p)
{
    nh_ecmascript_Object *F_p = malloc(sizeof(nh_ecmascript_Object));
    if (!F_p) return NULL;

    F_p->Methods_p = &NH_ECMASCRIPT_METHODS_FUNCTION;
    F_p->Shape_p = Realm_p->EmptyFunctionShape_p;

    // Initialize the Slots
    F_p->Slots_p[NH_ECMASCRIPT_SLOT_PROTOTYPE]      = nh_ecmascript_makeObject(Prototype_p);
    F_p->Slots_p[NH_ECMASCRIPT_SLOT_EXTENSIBLE]     = nh_ecmascript_makeBool(true);
    F_p->Slots_p[NH_ECMASCRIPT_SLOT_FUNC_CALL]      = nh_ecmascript_makeInternalPointer(steps_p);
    F_p->Slots_p[NH_ECMASCRIPT_SLOT_FUNC_CONSTRUCT] = nh_ecmascript_makeUndefined();
    F_p->Slots_p[NH_ECMASCRIPT_SLOT_FUNC_REALM]     = nh_ecmascript_makeObject(Realm_p);

    // Properties array (length, name, etc. will be added via DefineOwnProperty later)
    F_p->Properties_p = NULL;

    return F_p;
}

// PROPERTY FUNCTIONS =============================================================================

static nh_ecmascript_Property* nh_ecmascript_newProperty(
    bool isAccessor)
{
    nh_ecmascript_Property *prop = (nh_ecmascript_Property*)nh_core_allocate(sizeof(nh_ecmascript_Property));
    if (!prop) return NULL;

    prop->isAccessor = isAccessor;
    prop->enumerable = false;   // Spec default
    prop->configurable = false; // Spec default

    if (isAccessor) {
        prop->accessor.get = NULL;
        prop->accessor.set = NULL;
    } else {
        prop->data.value = nh_ecmascript_makeUndefined();
        prop->data.writable = false; // Spec default
    }

    return prop;
}

void nh_ecmascript_createDataProperty(
    nh_ecmascript_Object *Obj_p,
    const char *Key,
    nh_ecmascript_Value Value,
    bool Writable,
    bool Enumerable,
    bool Configurable)
{
    nh_ecmascript_Property *NewProp_p = nh_ecmascript_newProperty(false);

    NewProp_p->data.value = Value;
    NewProp_p->data.writable = Writable;
    NewProp_p->enumerable = Enumerable;
    NewProp_p->configurable = Configurable;

    // Assuming your HashMap key is the string property name
    nh_core_addToHashMap(Obj_p->Properties, Key, NewProp_p);
}

void nh_ecmascript_createAccessorProperty(
    nh_ecmascript_Object *Obj_p,
    const char *Key,
    nh_ecmascript_Object *GetterObj_p, // Can be NULL
    nh_ecmascript_Object *SetterObj_p, // Can be NULL
    bool Enumerable,
    bool Configurable)
{
    nh_ecmascript_Property *NewProp_p = nh_ecmascript_newProperty(true);

    NewProp_p->accessor.get = GetterObj_p;
    NewProp_p->accessor.set = SetterObj_p;
    NewProp_p->enumerable   = Enumerable;
    NewProp_p->configurable = Configurable;

    nh_core_addToHashMap(Obj_p->Properties, Key, NewProp_p);
}

// HELPER ===========================================================================

// https://tc39.es/ecma262/#sec-setfunctionlength
bool nh_ecmascript_setFunctionLength(nh_ecmascript_Object *F_p, int length) 
{
    // Attributes: { [[Value]]: length, [[Writable]]: false, [[Enumerable]]: false, [[Configurable]]: true }
    nh_ecmascript_createDataProperty(
        F_p, 
        "length", 
        nh_ecmascript_makeNumber(length), 
        false, 
        false, 
        true
    );
    return true;
}

// https://tc39.es/ecma262/#sec-setfunctionname
bool nh_ecmascript_setFunctionName(
    nh_ecmascript_Object *F_p,
    nh_ecmascript_Value name,
    const char *prefix) 
{
    // 1. Assert: F_p is an extensible object with no "name" property (internal logic).
    
    // 2. Prepare the final name string
    char *finalName = NULL;
    const char *rawName = name.p.string; // Assuming name is a String value

    if (prefix != NULL) {
        // Length: prefix + space + rawName + null terminator
        size_t len = strlen(prefix) + 1 + strlen(rawName) + 1;
        finalName = malloc(len);
        if (finalName == NULL) return false;

        // Construct: "prefix name"
        sprintf(finalName, "%s %s", prefix, rawName);
    } else {
        size_t len = strlen(rawName) + 1;
        finalName = malloc(len);
        if (finalName == NULL) return false;

        strcpy(finalName, rawName);
    }

    // 3. Set the "name" property
    // Attributes: { [[Value]]: name, [[Writable]]: false, [[Enumerable]]: false, [[Configurable]]: true }
    // Note: We use nh_ecmascript_makeString which should take ownership or copy the buffer.
    nh_ecmascript_createDataProperty(
        F_p, 
        "name", 
        nh_ecmascript_makeString(finalName), 
        false, // Writable
        false, // Enumerable
        true   // Configurable
    );

    free(finalName);
    return true; // TODO
}
