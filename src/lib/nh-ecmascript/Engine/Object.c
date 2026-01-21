// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Object.h"
#include "Realm.h"
#include "PropertyDescriptor.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

/**
 * https://tc39.es/ecma262/#sec-ordinaryobjectcreate
 * * @param Prototype_p The object to use as [[Prototype]]. Can be NULL.
 * @param InternalSlotsList A list of additional slot IDs needed (e.g., for Date/Array)
 */
nh_ecmascript_Object *nh_ecmascript_ordinaryObjectCreate(
    nh_ecmascript_Object *Prototype_p,
    nh_ecmascript_Realm *Realm_p) 
{
    nh_ecmascript_Object *O_p = malloc(sizeof(nh_ecmascript_Object));
    if (!O_p) return NULL;

    O_p->Methods_p = &NH_ECMASCRIPT_METHODS_ORDINARY;
    O_p->Shape_p = Realm_p->EmptyObjectShape_p;

    // Initialize standard slots
    O_p->Slots[NH_ECMASCRIPT_SLOT_PROTOTYPE] = (Prototype_p) 
        ? nh_ecmascript_makeObject(Prototype_p) 
        : nh_ecmascript_makeNull();
    O_p->Slots[NH_ECMASCRIPT_SLOT_EXTENSIBLE] = nh_ecmascript_makeBool(true);

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
    nh_ecmascript_Realm *Realm_p)
{
    nh_ecmascript_Object *F_p = malloc(sizeof(nh_ecmascript_Object));
    if (!F_p) return NULL;

    F_p->Methods_p = &NH_ECMASCRIPT_METHODS_FUNCTION;
    F_p->Shape_p = Realm_p->EmptyObjectShape_p;

    // Initialize the Slots
    F_p->Slots[NH_ECMASCRIPT_SLOT_PROTOTYPE]       = nh_ecmascript_makeObject(prototype);
    F_p->Slots[NH_ECMASCRIPT_SLOT_EXTENSIBLE]      = nh_ecmascript_makeBool(true);
    F_p->Slots[NH_ECMASCRIPT_SLOT_ECMASCRIPT_CODE] = nh_ecmascript_makeInternalPointer(sourceCode);
    F_p->Slots[NH_ECMASCRIPT_SLOT_CONSTRUCT]       = nh_ecmascript_makeUndefined();
    F_p->Slots[NH_ECMASCRIPT_SLOT_ENVIRONMENT]     = nh_ecmascript_makeInternalPointer(env);

    return F_p;
}

/**
 * https://tc39.es/ecma262/#sec-createbuiltinfunction
 * @param steps The C function pointer (the native implementation)
 * @param InternalSlotsList Any extra state needed (beyond Call/Construct/Realm)
 * @param Prototype_p Usually %Function.prototype%
 */
nh_ecmascript_Object *nh_ecmascript_createBuiltinFunction(
    void *steps_p, 
    nh_ecmascript_Realm *Realm_p,
    nh_ecmascript_Object *Prototype_p)
{
    nh_ecmascript_Object *F_p = malloc(sizeof(nh_ecmascript_Object));
    if (!F_p) return NULL;

    F_p->Methods_p = &NH_ECMASCRIPT_METHODS_FUNCTION;
    F_p->Shape_p = Realm_p->EmptyObjectShape_p;

    // Initialize the Slots
    F_p->Slots[NH_ECMASCRIPT_SLOT_PROTOTYPE]  = nh_ecmascript_makeObject(Prototype_p);
    F_p->Slots[NH_ECMASCRIPT_SLOT_EXTENSIBLE] = nh_ecmascript_makeBool(true);
    F_p->Slots[NH_ECMASCRIPT_SLOT_CALL]       = nh_ecmascript_makeInternalPointer(steps_p);
    F_p->Slots[NH_ECMASCRIPT_SLOT_CONSTRUCT]  = nh_ecmascript_makeUndefined();
    F_p->Slots[NH_ECMASCRIPT_SLOT_REALM]      = nh_ecmascript_makeInternalPointer(Realm_p);

    // Properties array (length, name, etc. will be added via DefineOwnProperty later)
    F_p->Properties_p = NULL;

    return F_p;
}

void nh_ecmascript_defineDataProperty(
    nh_ecmascript_Object *O_p,
    const char *key_p,
    nh_ecmascript_Value value,
    uint8_t attributes)
{
    // 1. Create the Property Descriptor (Spec 6.2.4)
    // We assume a Data Property here (Value + Writable/Enumerable/Configurable)
    nh_ecmascript_PropertyDescriptor desc = {0};
    desc.Value = value;
    
    desc.flags.hasValue        = true;
    desc.flags.hasWritable     = true;
    desc.flags.hasEnumerable   = true;
    desc.flags.hasConfigurable = true;

    // Attributes bitmask mapping (usually: 1=Writable, 2=Enumerable, 4=Configurable)
    desc.flags.writable     = (attributes & NH_ECMASCRIPT_ATTR_WRITABLE);
    desc.flags.enumerable   = (attributes & NH_ECMASCRIPT_ATTR_ENUMERABLE);
    desc.flags.configurable = (attributes & NH_ECMASCRIPT_ATTR_CONFIGURABLE);

    // 2. Dispatch to the object's internal [[DefineOwnProperty]]
    // This will trigger Shape transitions and storage allocation
    O_p->Methods_p->defineOwnProperty(O_p, key_p, &desc);
}

void nh_ecmascript_defineAccessorProperty(
    nh_ecmascript_Object *O_p,
    const char *key_p,
    nh_ecmascript_Object *Getter_p, // Can be NULL
    nh_ecmascript_Object *Setter_p, // Can be NULL
    uint8_t attributes)
{
    // 1. Create the Property Descriptor (Spec 6.2.4)
    nh_ecmascript_PropertyDescriptor desc = {0};
    
    // An Accessor Descriptor has [[Get]] and [[Set]], but NO [[Value]] or [[Writable]]
    desc.flags.hasGet          = true;
    desc.flags.hasSet          = true;
    desc.flags.hasEnumerable   = true;
    desc.flags.hasConfigurable = true;

    desc.Get = Getter_p;
    desc.Set = Setter_p;

    // Attributes bitmask mapping
    desc.flags.enumerable   = (attributes & NH_ECMASCRIPT_ATTR_ENUMERABLE);
    desc.flags.configurable = (attributes & NH_ECMASCRIPT_ATTR_CONFIGURABLE);

    // 2. Dispatch to the object's internal [[DefineOwnProperty]]
    O_p->Methods_p->defineOwnProperty(O_p, key_p, &desc);
}
