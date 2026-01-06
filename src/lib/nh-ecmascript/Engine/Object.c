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

// LIFECYCLE FUNCTIONS ====================================================================

// https://tc39.es/ecma262/#sec-ordinaryobjectcreate
nh_ecmascript_Object *nh_ecmascript_ordinaryObjectCreate(
    nh_ecmascript_Object *proto_p, 
    void *internalSlots_p, // Reserved for exotic data
    int initialCapacity) 
{
    // 1. Allocate the object structure
    nh_ecmascript_Object *obj_p = nh_core_allocate(sizeof(nh_ecmascript_Object));
    if (obj_p == NULL) return NULL;

    // 2. Set the [[Prototype]] internal slot
    obj_p->Prototype_p = proto_p;

    // 3. Initialize the property storage
    // If capacity is -1, use a sensible default (e.g., 8)
    int capacity = (initialCapacity < 0) ? 8 : initialCapacity;
    nh_core_createHashMap(&obj_p->Properties, capacity);

    // 4. Set the Extensible flag (Spec 9.1.11 step 3)
//    obj_p->extensible = true;

    // 5. Assign internal slots if provided
//    obj_p->internalSlots_p = internalSlots_p;

    return obj_p;
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

// https://tc39.es/ecma262/#sec-createbuiltinfunction
nh_ecmascript_Object *nh_ecmascript_createBuiltinFunction(
    void *handler, // Your native function pointer
    int length,
    const char *name,
    nh_ecmascript_Realm *Realm_p,
    nh_ecmascript_Object *prototype)
{
    // 1. Determine prototype (Default to %Function.prototype%)
    nh_ecmascript_Object *proto = (prototype != NULL) ? prototype : 
        nh_core_getFromHashMap(&Realm_p->Intrinsics, "FunctionPrototype");

    // 2. Allocate and initialize basic object
    nh_ecmascript_Object *func = nh_core_allocate(sizeof(nh_ecmascript_Object));
    func->type = NH_ECMASCRIPT_OBJECT_FUNCTION; // or your specific enum
    func->Prototype_p = proto;
    func->isCallable = true;
    func->isConstructor = false; // Built-ins usually aren't constructors unless specified
    func->isExotic = false;
    
    // 3. Set the "Internal Slots" that you hardcoded for speed
    func->nativeCall = handler;
    func->functionLength = length;

    // 4. Initialize dynamic containers
    func->Properties = nh_core_createHashMap();
    func->InternalSlots = nh_core_initList(2); // Start small

    // 5. Add "special" internal slots to the dynamic list if they aren't in the struct
    // Example: [[Realm]]
    nh_ecmascript_InternalSlot *realmSlot = (nh_ecmascript_InternalSlot*)nh_core_allocate(sizeof(nh_ecmascript_InternalSlot));
//    realmSlot->type = NH_ECMASCRIPT_INTERNAL_SLOT_CURRENT_REALM;
//    realmSlot->p = Realm_p;
//    nh_core_appendToList(&func->InternalSlots, realmSlot);

    // 6. Set JS-visible properties ("name", "length")
    // These are actual entries in the Properties hashmap, not internal slots
    nh_ecmascript_createDataProperty(
        func, "length", nh_ecmascript_makeNumber(length), 
        false, false, true
    );
    nh_ecmascript_createDataProperty(
        func, "name", nh_ecmascript_makeString(name), 
        false, false, true
    );

    return func;
}

// INTERNAL FUNCTIONS =======================================================================

nh_ecmascript_Completion nh_ecmascript_call(
    nh_ecmascript_Object *F, 
    nh_ecmascript_Value thisArgument, 
    nh_core_List argumentsList, 
    nh_ecmascript_Realm *Realm_p)
{
//    // 1. Prepare the New Execution Context
//    nh_ecmascript_ExecutionContext *calleeContext = nh_ecmascript_allocateExecutionContext();
//    calleeContext->Function_p = F;
//    calleeContext->Realm_p = Realm_p;
//
//    // 2. Create the Function Environment (Section 8.1.2.1)
//    nh_ecmascript_Environment *localEnv = nh_core_allocate(sizeof(nh_ecmascript_Environment));
//    localEnv->type = NH_ECMASCRIPT_ENVIRONMENT_FUNCTION;
//    localEnv->outer = F->environment; // The scope where function was DEFINED (Closure)
//    
//    // 3. Bind 'this' (simplified logic for non-arrow functions)
//    localEnv->records.function.thisValue = thisArgument;
//    localEnv->records.function.functionObject = F;
//
//    // 4. Update the Context to use this new Environment
//    calleeContext->LexicalEnvironment_p = localEnv;
//    calleeContext->VariableEnvironment_p = localEnv;
//
//    // 5. Push context onto the stack
//    nh_ecmascript_pushExecutionContext(calleeContext);
//
//    // 6. Perform "FunctionDeclarationInstantiation" (Binding arguments to names)
//    nh_ecmascript_instantiateFunctionVariables(localEnv, F, argumentsList);

    // 7. Execute the body
//    nh_ecmascript_Completion result = nh_ecmascript_evaluateBody(F->code);

    // 8. Clean up
//    nh_ecmascript_popExecutionContext();
    // (Free calleeContext and localEnv if not handled by GC)

    return nh_ecmascript_normalEmptyCompletion();
}

// The actual logic: OrdinaryGet(O, P, Receiver)
static nh_ecmascript_Completion nh_ecmascript_ordinaryGet(
    nh_ecmascript_Object *O_p, 
    const char *P, 
    nh_ecmascript_Object *Receiver_p,
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. Search the current object's HashMap
    nh_ecmascript_Property *prop = nh_core_getFromHashMap(&O_p->Properties, P);

//    // 2. If not found, walk the prototype chain
//    if (prop == NULL) {
//        if (O_p->prototype == NULL) {
//            return nh_ecmascript_normalCompletion(nh_ecmascript_makeUndefined());
//        }
//        return nh_ecmascript_ordinaryGet(O_p->prototype, P, Receiver_p, Realm_p);
//    }
//
//    // 3. If Data Property: return value
//    if (!prop->isAccessor) {
//        return nh_ecmascript_normalCompletion(prop->value);
//    }
//
//    // 4. If Accessor Property: call the getter
//    if (prop->get == NULL) {
//        return nh_ecmascript_normalCompletion(nh_ecmascript_makeUndefined());
//    }
    
    // Call(getter, Receiver)
//    return nh_ecmascript_call(prop->get, nh_ecmascript_makeObject(Receiver_p), NULL, 0, Realm_p);
return nh_ecmascript_normalEmptyCompletion();
}

// High-level wrapper: Get(O, P)
nh_ecmascript_Completion nh_ecmascript_get(
    nh_ecmascript_Object *O_p, 
    const char *P, 
    nh_ecmascript_Realm *Realm_p) 
{
    return nh_ecmascript_ordinaryGet(O_p, P, O_p, Realm_p);
}

// https://tc39.es/ecma262/#sec-ordinaryset
static nh_ecmascript_Completion nh_ecmascript_ordinarySet(
    nh_ecmascript_Object *O_p, 
    const char *P, 
    nh_ecmascript_Value V, 
    nh_ecmascript_Object *Receiver_p,
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. Look for existing property
    nh_ecmascript_Property *prop = nh_core_getFromHashMap(&O_p->Properties, P);

//    // 2. If not found, check prototype
//    if (prop == NULL) {
//        if (O_p->prototype != NULL) {
//            return nh_ecmascript_ordinarySet(O_p->prototype, P, V, Receiver_p, Realm_p);
//        } else {
//            // Create a new data property if we hit the end of the chain
//            nh_ecmascript_createDataProperty(Receiver_p, P, V, true, true, true);
//            return nh_ecmascript_normalCompletion(nh_ecmascript_makeBoolean(true));
//        }
//    }
//
//    // 3. If Data Property
//    if (!prop->isAccessor) {
//        if (!prop->writable) return nh_ecmascript_normalCompletion(nh_ecmascript_makeBoolean(false));
//        
//        // If the property exists on the Receiver, update it. 
//        // If it was found on a proto, we create a new property on the Receiver (shadowing).
//        if (O_p == Receiver_p) {
//            prop->value = V;
//        } else {
//            nh_ecmascript_createDataProperty(Receiver_p, P, V, true, true, true);
//        }
//        return nh_ecmascript_normalCompletion(nh_ecmascript_makeBoolean(true));
//    }
//
//    // 4. If Accessor Property
//    if (prop->set == NULL) return nh_ecmascript_normalCompletion(nh_ecmascript_makeBoolean(false));
    
    // Call(setter, Receiver, « V »)
//    return nh_ecmascript_call(prop->set, nh_ecmascript_makeObject(Receiver_p), &V, 1, Realm_p);

return nh_ecmascript_normalEmptyCompletion();

}

// High-level wrapper: Set(O, P, V)
nh_ecmascript_Completion nh_ecmascript_set(
    nh_ecmascript_Object *O_p, 
    const char *P, 
    nh_ecmascript_Value Value, 
    nh_ecmascript_Realm *Realm_p) 
{
    return nh_ecmascript_ordinarySet(O_p, P, Value, O_p, Realm_p);
}

// https://tc39.es/ecma262/#sec-ordinaryhasproperty
bool nh_ecmascript_hasProperty(
    nh_ecmascript_Object *O_p, 
    const char *P, 
    nh_ecmascript_Realm *Realm_p) 
{
//    bool hasOwn = nh_core_hashMapHasKey(&O_p->Properties, P);
//    if (hasOwn) return nh_ecmascript_normalCompletion(nh_ecmascript_makeBoolean(true));
//
//    if (O_p->prototype != NULL) {
//        return nh_ecmascript_hasProperty(O_p->prototype, P, Realm_p);
//    }

    return false;
}

// https://tc39.es/ecma262/#sec-ordinarydefineownproperty
nh_ecmascript_Completion nh_ecmascript_defineOwnProperty(
    nh_ecmascript_Object *O_p,
    const char *P,
    nh_ecmascript_PropertyDescriptor *Desc_p,
    nh_ecmascript_Realm *Realm_p)
{
//    // 1. Get the current property state
//    nh_ecmascript_PropertyDescriptor current;
//    bool found = nh_ecmascript_getOwnProperty(O_p, P, &current, Realm_p);
//
//    // 2. Check if the object is extensible
//    nh_ecmascript_Completion extensibleComp = nh_ecmascript_isExtensible(O_p, Realm_p);
//    bool extensible = extensibleComp.value.p.boolean;
//
//    // 3. Logic for New Property
//    if (!found) {
//        if (!extensible) {
//            return nh_ecmascript_throwTypeError("Cannot define property on non-extensible object", Realm_p);
//        }
//        
//        // Create a new internal property struct and save it
//        return nh_ecmascript_createNewProperty(O_p, P, Desc, Realm_p);
//    }
//
//    // 4. Logic for Existing Property (Validation)
//    // If it's not configurable, the user is very limited in what they can change
//    if (!current.configurable) {
//        if (Desc.presence.hasConfigurable && Desc.configurable) {
//            return nh_ecmascript_throwTypeError("Cannot make non-configurable property configurable", Realm_p);
//        }
//        if (Desc.presence.hasEnumerable && (Desc.enumerable != current.enumerable)) {
//            return nh_ecmascript_throwTypeError("Cannot change enumerable of non-configurable property", Realm_p);
//        }
//    }
//
//    // 5. Apply the changes to your internal storage
//    return nh_ecmascript_updateExistingProperty(O_p, P, Desc, Realm_p);

    return nh_ecmascript_normalEmptyCompletion();
}
