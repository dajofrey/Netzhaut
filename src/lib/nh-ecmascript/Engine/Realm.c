// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Realm.h"
#include "ExecutionContext.h"
#include "Object.h"
#include "Value.h"
#include "PropertyDescriptor.h"

#include "../Intrinsics/Intrinsics.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <ctype.h>
#include <dlfcn.h>

// FUNCTIONS ======================================================================================

// Helper to map template string keys to array indices
static int nh_ecmascript_getSlotIndex(const char *key) {
    if (key == NULL) return -1;

    /* --- Function Object Slots --- */
    if (strcmp(key, "[[Call]]") == 0)              return NH_ECMASCRIPT_SLOT_CALL;
    if (strcmp(key, "[[Construct]]") == 0)         return NH_ECMASCRIPT_SLOT_CONSTRUCT;
    if (strcmp(key, "[[FormalParameters]]") == 0)  return NH_ECMASCRIPT_SLOT_FORMAL_PARAMETERS;
    if (strcmp(key, "[[ECMAScriptCode]]") == 0)    return NH_ECMASCRIPT_SLOT_ECMASCRIPT_CODE;
    if (strcmp(key, "[[ConstructorKind]]") == 0)   return NH_ECMASCRIPT_SLOT_CONSTRUCTOR_KIND;
    if (strcmp(key, "[[Realm]]") == 0)             return NH_ECMASCRIPT_SLOT_REALM;
    if (strcmp(key, "[[ScriptOrModule]]") == 0)    return NH_ECMASCRIPT_SLOT_SCRIPT_OR_MODULE;
    if (strcmp(key, "[[Environment]]") == 0)       return NH_ECMASCRIPT_SLOT_ENVIRONMENT;

    /* --- Common State Slots --- */
    if (strcmp(key, "[[Extensible]]") == 0)        return NH_ECMASCRIPT_SLOT_EXTENSIBLE;
    if (strcmp(key, "[[PrivateElements]]") == 0)   return NH_ECMASCRIPT_SLOT_PRIVATE_ELEMENTS;
    if (strcmp(key, "[[Prototype]]") == 0)         return NH_ECMASCRIPT_SLOT_PROTOTYPE;

    /* --- Built-in Specific Slots --- */
    if (strcmp(key, "[[DateValue]]") == 0)         return NH_ECMASCRIPT_SLOT_DATE_VALUE;
    if (strcmp(key, "[[ErrorData]]") == 0)         return NH_ECMASCRIPT_SLOT_ERROR_DATA;
    if (strcmp(key, "[[ArrayLength]]") == 0)       return NH_ECMASCRIPT_SLOT_ARRAY_LENGTH;
    if (strcmp(key, "[[MapData]]") == 0)           return NH_ECMASCRIPT_SLOT_MAP_DATA;
    if (strcmp(key, "[[SetData]]") == 0)           return NH_ECMASCRIPT_SLOT_SET_DATA;
    if (strcmp(key, "[[PromiseState]]") == 0)      return NH_ECMASCRIPT_SLOT_PROMISE_STATE;
    if (strcmp(key, "[[PromiseResult]]") == 0)     return NH_ECMASCRIPT_SLOT_PROMISE_RESULT;

    return -1;
}

static NH_API_RESULT nh_ecmascript_createIntrinsics(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    // PASS 1: Allocate all "Headers" and "Slots"
    for (int i = 0; i < Templates_p->size; ++i) {
        nh_ecmascript_IntrinsicTemplate *T = Templates_p->pp[i];
        
        // A. Determine if it's a function by checking for [[Call]] in InternalSlots
        bool isCallable = false;
        void *nativeSteps_p = NULL;
        for (int s = 0; s < T->InternalSlots.size; s++) {
            nh_ecmascript_KeyValuePair *slot = T->InternalSlots.pp[s];
            if (strcmp(slot->key, "[[Call]]") == 0) {
                isCallable = true;
                nativeSteps_p = nh_ecmascript_getNativeSteps(slot->value->data.string_p);
                break;
            }
        }

        // B. Create the Object instance
        nh_ecmascript_Object *O_p;
        if (isCallable) {
            O_p = nh_ecmascript_createBuiltinFunction(nativeSteps_p, Realm_p, NULL);
        } else {
            O_p = nh_ecmascript_ordinaryObjectCreate(NULL, Realm_p);
        }

        // C. Register in Realm
        nh_core_addToHashMap(&Realm_p->Intrinsics, T->name, O_p);
    }

    // PASS 2: Wiring and Properties
    for (int i = 0; i < Templates_p->size; ++i) {
        nh_ecmascript_IntrinsicTemplate *T = Templates_p->pp[i];
        nh_ecmascript_Object *O_p = nh_core_getFromHashMap(&Realm_p->Intrinsics, T->name);

        // 1. Resolve slots 
        for (int s = 0; s < T->InternalSlots.size; s++) {
            nh_ecmascript_KeyValuePair *slot_template = T->InternalSlots.pp[s];
            
            // 1. Get the destination index in the flat Slots array
            int index = nh_ecmascript_getSlotIndex(slot_template->key);
            
            if (index != -1) {
                // 2. Resolve the value (Intrinsic ref, Boolean, Number, etc.)
                nh_ecmascript_Value Value = nh_ecmascript_resolveTemplateValue(
                    slot_template->value, Realm_p
                );
        
                // 3. Special handling for [[Call]]/[[Construct]] 
                // These were already partially handled in Pass 1, but we ensure 
                // the Slots is fully synced here.
                if (strcmp(slot_template->key, "[[Call]]") == 0 || 
                    strcmp(slot_template->key, "[[Construct]]") == 0) {
                    
                    void *ptr = nh_ecmascript_getNativeSteps(slot_template->value->data.string_p);
                    O_p->Slots[index] = nh_ecmascript_makeInternalPointer(ptr);
                } else {
                    // 4. Standard slot assignment
                    O_p->Slots[index] = Value;
                }
            }
        }
puts("ok");
exit(0);
        // 2. Define Public Properties
        for (int p = 0; p < T->Properties.size; p++) {
            nh_ecmascript_KeyValuePair *prop = T->Properties.pp[p];
            
            nh_ecmascript_Value Value = nh_ecmascript_resolveTemplateValue(prop->value, Realm_p);
            
            nh_ecmascript_PropertyDescriptor Descriptor = {
                .Value = Value, 
                .flags.hasWritable = true, .flags.writable = true,
                .flags.hasEnumerable = true, .flags.enumerable = false,
                .flags.hasConfigurable = true, .flags.configurable = true
            };

            // This evolves the Shape and populates Properties_p
            O_p->Methods_p->defineOwnProperty(O_p, prop->key, &Descriptor);
        }
    }

    return NH_API_SUCCESS;
}

static void nh_ecmascript_setDefaultGlobalBindings(
    nh_ecmascript_Realm *Realm_p)
{
//    nh_ecmascript_Object *Global_p = Realm_p->GlobalObject_p;
//
//    // 2. Iterate through your Intrinsics HashMap
//    // We want to expose the "Constructors" (Object, Array, etc.) 
//    // but usually NOT the prototypes (ObjectPrototype, ArrayPrototype) 
//    // because those aren't global variables in JS.
//
//    unsigned int length = nh_core_getHashMapLength(&Realm_p->Intrinsics);
//
//    for (int i = 0; i < length; ++i) {
//        char *name = nh_core_getKeyFromHashMapUsingIndex(&Realm_p->Intrinsics, i);
//        nh_ecmascript_Object *Intrinsic_p = nh_core_getValueFromHashMapUsingIndex(&Realm_p->Intrinsics, i);
//
//        // Skip internal names like "ObjectPrototype" or "ArrayPrototype"
//        // You only want to expose the actual constructor names.
//        if (strstr(name, "Prototype") != NULL) {
//            continue;
//        }
//
//        // 3. Define the property on the Global Object
//        // Spec: Writable=true, Enumerable=false, Configurable=true
//        nh_ecmascript_createDataProperty(
//            Global_p,
//            name,
//            nh_ecmascript_makeObject(Intrinsic_p),
//            true,  // Writable
//            false, // Enumerable
//            true   // Configurable
//        );
//    }
//
//    // 4. Special Case: The 'globalThis' / 'window' / 'self' binding
//    // In a browser, window.window === window.
//    nh_ecmascript_createDataProperty(
//        Global_p,
//        "globalThis",
//        nh_ecmascript_makeObject(Global_p),
//        true, false, true
//    );
}

// corresponds to https://www.262.ecma-international.org/11.0/index.html#sec-initializehostdefinedrealm
// with no modifications to the default global object
nh_ecmascript_Realm *nh_ecmascript_initializeRealm(
    nh_ecmascript_Agent *Agent_p)
{
    nh_ecmascript_Realm *Realm_p = (nh_ecmascript_Realm*)nh_core_allocate(sizeof(nh_ecmascript_Realm));
    NH_CORE_CHECK_MEM_2(NULL, Realm_p)

    Realm_p->EmptyObjectShape_p = nh_ecmascript_createRootShape();
    Realm_p->Agent_p = Agent_p; // aka agent signifier in the spec
    Realm_p->GlobalObject_p = NULL;
    Realm_p->GlobalEnvironment_p = NULL;
    Realm_p->Intrinsics = nh_core_createHashMap();

    NH_CORE_CHECK_2(NULL, nh_ecmascript_createIntrinsics(&Agent_p->Cluster_p->Runtime_p->IntrinsicTemplates, Realm_p))

    nh_ecmascript_ExecutionContext *NewContext_p = nh_ecmascript_allocateExecutionContext();
    NH_CORE_CHECK_MEM_2(NULL, NewContext_p)

    NewContext_p->Function_p = NULL;
    NewContext_p->Realm_p    = Realm_p;
    NewContext_p->ScriptOrModule.isScript = -1;
    NewContext_p->ScriptOrModule.handle_p = NULL;

    nh_core_pushStack(&Agent_p->ExecutionContextStack, NewContext_p);

    nh_ecmascript_Object *ObjProto_p = nh_core_getFromHashMap(&Realm_p->Intrinsics, "ObjectPrototype");
    
    // Create an ordinary object with ObjectPrototype as [[Prototype]]
    nh_ecmascript_Object *GlobalObject_p = nh_ecmascript_ordinaryObjectCreate(ObjProto_p, NULL);
    NH_CORE_CHECK_MEM_2(NULL, GlobalObject_p)

    Realm_p->GlobalObject_p = GlobalObject_p;

    // 2. Set up the Global Environment (Spec 9.1.1.4)
    // The Global Environment contains both the Object Record (Global Object properties)
    // and the Declarative Record (top-level 'let', 'const', 'class' bindings).
    Realm_p->GlobalEnvironment_p = nh_ecmascript_newGlobalEnvironment(GlobalObject_p, GlobalObject_p);
    NH_CORE_CHECK_MEM_2(NULL, Realm_p->GlobalEnvironment_p)

    // 3. Set Default Global Bindings (Spec 9.1.1.4.11)
    // This populates the Global Object with properties like "Object", "Array", "Math", etc.
    nh_ecmascript_setDefaultGlobalBindings(Realm_p);

    // 4. Update the current Execution Context
    // Now that the environment is ready, the context must point to it.
    NewContext_p->VariableEnvironment_p = Realm_p->GlobalEnvironment_p;
    NewContext_p->LexicalEnvironment_p  = Realm_p->GlobalEnvironment_p;

    return Realm_p;
}
