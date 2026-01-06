// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Realm.h"
#include "ExecutionContext.h"
#include "BuiltinFunctionObject.h"
#include "OrdinaryObject.h"

#include "../Intrinsics/Parser.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// FUNCTIONS ======================================================================================

static NH_API_RESULT nh_ecmascript_allocateIntrinsicObjects(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    for (int i = 0; i < Templates_p->size; ++i) {
        nh_ecmascript_IntrinsicTemplate *Template_p = Templates_p->pp[i];

        nh_ecmascript_Object *Object_p = nh_core_allocate(sizeof(nh_ecmascript_Object));
        Object_p->type = Template_p->kind;
        Object_p->isCallable = Template_p->isCallable;
        Object_p->isConstructor = Template_p->isConstructor;
        Object_p->isExotic = Template_p->isExotic;
        Object_p->Prototype_p = NULL;

        nh_core_addToHashMap(&Realm_p->Intrinsics, Template_p->name, Object_p);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_ecmascript_linkIntrinsicPrototypes(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    for (int i = 0; i < Templates_p->size; ++i) {
        nh_ecmascript_IntrinsicTemplate *Template_p = Templates_p->pp[i];
        nh_ecmascript_Object *Object_p = nh_core_getFromHashMap(&Realm_p->Intrinsics, Template_p->name);
        if (Template_p->prototype_p != NULL) {
            Object_p->Prototype_p = nh_core_getFromHashMap(&Realm_p->Intrinsics, Template_p->prototype_p);
        }
    }
    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_ecmascript_initializeIntrinsicInternalSlots(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    for (int i = 0; i < Templates_p->size; ++i) {
        nh_ecmascript_IntrinsicTemplate *Template_p = Templates_p->pp[i];
        nh_ecmascript_Object *Object_p = nh_core_getFromHashMap(&Realm_p->Intrinsics, Template_p->name);
        if (Object_p == NULL) {return NH_API_ERROR_BAD_STATE;}

        Object_p->InternalSlots = nh_core_initList(Template_p->InternalSlotTemplates.size);

        for (int j = 0; j < Template_p->InternalSlots.size; ++j) {
            nh_ecmascript_InternalSlotTemplate *InternalSlotTemplate_p = Template_p->InternalSlotTemplates.pp[j];
            nh_ecmascript_InternalSlot *InternalSlot_p = nh_core_allocate(sizeof(nh_ecmascript_InternalSlot));
            nh_core_appendToList(&Object_p->InternalSlots, InternalSlot_p);

            InternalSlot_p->type = InternalSlotTemplate_p->type;

            // TODO private elements "All objects have an internal slot named [[PrivateElements]]

            switch (InternalSlotTemplate_p->type) {
                case NH_ECMASCRIPT_INTERNAL_SLOT_NULL :
                    break; 
                case NH_ECMASCRIPT_INTERNAL_SLOT_UNDEFINED :
                    break; 
                case NH_ECMASCRIPT_INTERNAL_SLOT_BOOLEAN :
                   InternalSlot_p->boolean = InternalSlotTemplate_p->boolean;
                   break; 
                case NH_ECMASCRIPT_INTERNAL_SLOT_NUMBER :
                   InternalSlot_p->number = InternalSlotTemplate_p->number;
                   break; 
                case NH_ECMASCRIPT_INTERNAL_CURRENT_REALM :
                   InternalSlot_p->p = Realm_p;
                   break; 
                case NH_ECMASCRIPT_INTERNAL_INTRINSIC_REF :
                   break; 
                default :
                   return NH_API_ERROR_BAD_STATE;
            }
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_ecmascript_attachInternalMethods(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    for (int i = 0; i < Templates_p->size; ++i) {
        nh_ecmascript_IntrinsicTemplate *Template_p = Templates_p->pp[i];
        nh_ecmascript_Object *Object_p = nh_core_getFromHashMap(&Realm_p->Intrinsics, Template_p->name);
        Object_p->Methods = Template_p->Methods;
    }
    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_ecmascript_defineIntrinsicProperties(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    for (int i = 0; i < Templates_p->size; ++i) {
        nh_ecmascript_IntrinsicTemplate *Template_p = Templates_p->pp[i];
        nh_ecmascript_Object *Object_p = nh_core_getFromHashMap(&Realm_p->Intrinsics, Template_p->name);

        if (!Object_p) continue;

        for (int j = 0; j < Template_p->Properties.size; ++j) {
            nh_ecmascript_IntrinsicPropertyTemplate *PropTemp_p = Template_p->Properties.pp[j];
            nh_ecmascript_Value val;

            // 1. Check if this is a Built-in Function (has length, no static value)
            if (PropTemp_p->length >= 0 && !PropTemp_p->hasStaticValue) {
                // Construct the lookup name: e.g., "Object_assign"
                char fullName[256];
                snprintf(fullName, sizeof(fullName), "%s_%s", Template_p->name, PropTemp_p->name);

                // Create the function object
                nh_ecmascript_Object *Func_p = nh_ecmascript_createBuiltinFunction(
                    PropTemp_p->name, 
                    PropTemp_p->length, 
                    nh_ecmascript_lookupNativeHandler(fullName), 
                    Realm_p
                );
                val = nh_ecmascript_makeObject(Func_p);
            } 
            // 2. Otherwise, it is a static data value (like Number.MAX_VALUE or a Prototype pointer)
            else {
                val = PropTemp_p->staticValue;
            }

            // 3. Define the property on the Intrinsic Object
            // Intrinsic properties are usually: Writable=true, Enumerable=false, Configurable=true
            nh_ecmascript_createDataProperty(
                Object_p, 
                PropTemp_p->name, 
                val, 
                PropTemp_p->writable, 
                PropTemp_p->enumerable, 
                PropTemp_p->configurable
            );
        }
    }

    return NH_API_SUCCESS;
}

// corresponds to https://tc39.es/ecma262/#sec-createintrinsics
static NH_API_RESULT nh_ecmascript_createIntrinsics(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    Realm_p->Intrinsics = nh_core_createHashMap();

    NH_CORE_CHECK(nh_ecmascript_allocateIntrinsicObjects(Templates_p, Realm_p))
    NH_CORE_CHECK(nh_ecmascript_linkIntrinsicPrototypes(Templates_p, Realm_p))
    NH_CORE_CHECK(nh_ecmascript_initializeIntrinsicInternalSlots(Templates_p, Realm_p))
    NH_CORE_CHECK(nh_ecmascript_attachInternalMethods(Templates_p, Realm_p))
    NH_CORE_CHECK(nh_ecmascript_defineIntrinsicProperties(Templates_p, Realm_p))

    return NH_API_SUCCESS;
}

static void nh_ecmascript_freeIntrinsics(
    nh_ecmascript_Intrinsics Intrinsics)
{
    return;
}

static void nh_ecmascript_setDefaultGlobalBindings(
    nh_ecmascript_Realm *Realm_p)
{
    nh_ecmascript_Object *Global_p = Realm_p->GlobalObject_p;

    // 2. Iterate through your Intrinsics HashMap
    // We want to expose the "Constructors" (Object, Array, etc.) 
    // but usually NOT the prototypes (ObjectPrototype, ArrayPrototype) 
    // because those aren't global variables in JS.

    nh_core_HashMapIterator it = nh_core_getHashMapIterator(&Realm_p->Intrinsics);
    while (nh_core_hashMapNext(&it)) {
        const char *name = it.key;
        nh_ecmascript_Object *Intrinsic_p = it.value;

        // Skip internal names like "ObjectPrototype" or "ArrayPrototype"
        // You only want to expose the actual constructor names.
        if (strstr(name, "Prototype") != NULL) {
            continue;
        }

        // 3. Define the property on the Global Object
        // Spec: Writable=true, Enumerable=false, Configurable=true
        nh_ecmascript_createDataProperty(
            Global_p,
            name,
            nh_ecmascript_makeObject(Intrinsic_p),
            true,  // Writable
            false, // Enumerable
            true   // Configurable
        );
    }

    // 4. Special Case: The 'globalThis' / 'window' / 'self' binding
    // In a browser, window.window === window.
    nh_ecmascript_createDataProperty(
        Global_p,
        "globalThis",
        nh_ecmascript_makeObject(Global_p),
        true, false, true
    );
}

// corresponds to https://www.262.ecma-international.org/11.0/index.html#sec-initializehostdefinedrealm
// with no modifications to the default global object
nh_ecmascript_Realm *nh_ecmascript_initializeRealm(
    nh_ecmascript_Agent *Agent_p)
{
    nh_ecmascript_Realm *Realm_p = (nh_ecmascript_Realm*)nh_core_allocate(sizeof(nh_ecmascript_Realm));
    NH_CORE_CHECK_MEM_2(NULL, Realm_p)

    NH_CORE_CHECK_2(NULL, nh_ecmascript_createIntrinsics(&Agent_p->Cluster_p->Runtime_p->IntrinsicTemplates, Realm_p))

    Realm_p->Agent_p = Agent_p; // aka agent signifier in the spec
    Realm_p->GlobalObject_p = NULL;
    Realm_p->GlobalEnvironment_p = NULL;

    nh_ecmascript_ExecutionContext *NewContext_p = nh_ecmascript_allocateExecutionContext();
    NH_CORE_CHECK_MEM_2(NULL, NewContext_p)

    NewContext_p->Function_p = NULL;
    NewContext_p->Realm_p    = Realm_p;
    NewContext_p->ScriptOrModule.isScript = -1;
    NewContext_p->ScriptOrModule.handle_p = NULL;

    nh_core_pushStack(&Agent_p->ExecutionContextStack, NewContext_p);

    nh_ecmascript_Object *ObjProto_p = nh_core_getFromHashMap(&Realm_p->Intrinsics, "ObjectPrototype");
    
    // Create an ordinary object with ObjectPrototype as [[Prototype]]
    nh_ecmascript_Object *GlobalObject_p = nh_ecmascript_ordinaryObjectCreate(ObjProto_p, NULL, -1);
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
    NewContext_p->VariableEnvironment = Realm_p->GlobalEnvironment_p;
    NewContext_p->LexicalEnvironment  = Realm_p->GlobalEnvironment_p;

    return Realm_p;
}
