#ifndef NH_ECMASCRIPT_ENGINE_OBJECT_H
#define NH_ECMASCRIPT_ENGINE_OBJECT_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Completion.h"
#include "Value.h"

#include "../Parser/AST.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/HashMap.h"

// TYPES ============================================================

typedef struct nh_ecmascript_Environment nh_ecmascript_Environment;
typedef struct nh_ecmascript_PropertyDescriptor nh_ecmascript_PropertyDescriptor;

typedef struct nh_ecmascript_Property {
    bool enumerable;
    bool configurable;
    bool isAccessor; 

    union {
        // https://www.262.ecma-international.org/11.0/index.html#table-2
        struct {
            nh_ecmascript_Value value;
            bool writable;
        } data;
        // https://www.262.ecma-international.org/11.0/index.html#table-3
        struct {
            struct nh_ecmascript_Object *get; 
            struct nh_ecmascript_Object *set; 
        } accessor;
    };
} nh_ecmascript_Property;

typedef enum NH_ECMASCRIPT_SLOT_ID_E {
    /* --- Internal Methods (9.1) --- */
    NH_ECMASCRIPT_SLOT_GET_PROTOTYPE_OF,    // [[GetPrototypeOf]]
    NH_ECMASCRIPT_SLOT_SET_PROTOTYPE_OF,    // [[SetPrototypeOf]]
    NH_ECMASCRIPT_SLOT_IS_EXTENSIBLE,       // [[IsExtensible]]
    NH_ECMASCRIPT_SLOT_PREVENT_EXTENSIONS,  // [[PreventExtensions]]
    NH_ECMASCRIPT_SLOT_GET_OWN_PROPERTY,    // [[GetOwnProperty]]
    NH_ECMASCRIPT_SLOT_DEFINE_OWN_PROPERTY, // [[DefineOwnProperty]]
    NH_ECMASCRIPT_SLOT_HAS_PROPERTY,        // [[HasProperty]]
    NH_ECMASCRIPT_SLOT_GET,                 // [[Get]]
    NH_ECMASCRIPT_SLOT_SET,                 // [[Set]]
    NH_ECMASCRIPT_SLOT_DELETE,              // [[Delete]]
    NH_ECMASCRIPT_SLOT_OWN_PROPERTY_KEYS,   // [[OwnPropertyKeys]]

    /* --- Function Object Slots (10.2) --- */
    NH_ECMASCRIPT_SLOT_CALL,                // [[Call]]
    NH_ECMASCRIPT_SLOT_CONSTRUCT,           // [[Construct]]
    NH_ECMASCRIPT_SLOT_FORMAL_PARAMETERS,   // [[FormalParameters]]
    NH_ECMASCRIPT_SLOT_ECMASCRIPT_CODE,     // [[ECMAScriptCode]]
    NH_ECMASCRIPT_SLOT_CONSTRUCTOR_KIND,    // [[ConstructorKind]] (base or derived)
    NH_ECMASCRIPT_SLOT_REALM,               // [[Realm]]
    NH_ECMASCRIPT_SLOT_SCRIPT_OR_MODULE,    // [[ScriptOrModule]]
    NH_ECMASCRIPT_SLOT_ENVIRONMENT,         // [[Environment]] (Lexical environment)

    /* --- Common State Slots --- */
    NH_ECMASCRIPT_SLOT_EXTENSIBLE,          // [[Extensible]] (Boolean)
    NH_ECMASCRIPT_SLOT_PRIVATE_ELEMENTS,    // [[PrivateElements]]
    NH_ECMASCRIPT_SLOT_PROTOTYPE,           // [[Prototype]] (Usually in struct, but good for exotic)
    
    /* --- Built-in Specific Slots --- */
    NH_ECMASCRIPT_SLOT_DATE_VALUE,          // [[DateValue]]
    NH_ECMASCRIPT_SLOT_ERROR_DATA,          // [[ErrorData]]
    NH_ECMASCRIPT_SLOT_ARRAY_LENGTH,        // [[ArrayLength]] (For exotic array behavior)
    NH_ECMASCRIPT_SLOT_MAP_DATA,            // [[MapData]]
    NH_ECMASCRIPT_SLOT_SET_DATA,            // [[SetData]]
    NH_ECMASCRIPT_SLOT_PROMISE_STATE,       // [[PromiseState]]
    NH_ECMASCRIPT_SLOT_PROMISE_RESULT,      // [[PromiseResult]]

} NH_ECMASCRIPT_SLOT_ID_E;

typedef enum NH_ECMASCRIPT_SLOT_TYPE_E { 
    NH_ECMASCRIPT_SLOT_TYPE_NULL,
    NH_ECMASCRIPT_SLOT_TYPE_UNDEFINED,
    NH_ECMASCRIPT_SLOT_TYPE_BOOLEAN,
    NH_ECMASCRIPT_SLOT_TYPE_NUMBER,
    NH_ECMASCRIPT_SLOT_TYPE_PRIVATE_ELEMENTS,
    NH_ECMASCRIPT_SLOT_TYPE_INTRINSIC_REF,
    NH_ECMASCRIPT_SLOT_TYPE_CURRENT_REALM,
    NH_ECMASCRIPT_SLOT_TYPE_ENVIRONMENT,
    NH_ECMASCRIPT_SLOT_TYPE_METHOD,
    NH_ECMASCRIPT_SLOT_TYPE_SCRIPT_OR_MODULE,
} NH_ECMASCRIPT_SLOT_TYPE_E;

typedef enum NH_ECMASCRIPT_OBJECT_E { 
    NH_ECMASCRIPT_OBJECT_ORDINARY,
    NH_ECMASCRIPT_OBJECT_FUNCTION,
    NH_ECMASCRIPT_OBJECT_ARRAY,
} NH_ECMASCRIPT_OBJECT_E;

typedef struct nh_ecmascript_InternalSlot {
    NH_ECMASCRIPT_SLOT_ID_E id;        // What is this?
    NH_ECMASCRIPT_SLOT_TYPE_E type;
    union {
        bool boolean;
        double number;
        void *p;
    };
} nh_ecmascript_InternalSlot;

typedef struct nh_ecmascript_Object {
    NH_ECMASCRIPT_OBJECT_E type;
    struct nh_ecmascript_Object *Prototype_p;
    bool isCallable;
    bool isConstructor;
    bool isExotic;
    nh_core_List InternalSlots;
    nh_core_HashMap Properties; // of type nh_ecmascript_Property
    // for function objects
    void *nativeCall;
    void *nativeConstruct;
    unsigned int functionLength;
} nh_ecmascript_Object;

// FUNCTIONS =======================================================

void nh_ecmascript_createDataProperty(
    nh_ecmascript_Object *Obj_p,
    const char *Key,
    nh_ecmascript_Value Value,
    bool Writable,
    bool Enumerable,
    bool Configurable
);

void nh_ecmascript_createAccessorProperty(
    nh_ecmascript_Object *Obj_p,
    const char *Key,
    nh_ecmascript_Object *GetterObj_p, // Can be NULL
    nh_ecmascript_Object *SetterObj_p, // Can be NULL
    bool Enumerable,
    bool Configurable
);

nh_ecmascript_Object *nh_ecmascript_ordinaryObjectCreate(
    nh_ecmascript_Object *proto_p,
    void *internalSlots_p, // Reserved for exotic data
    int initialCapacity
);

nh_ecmascript_Object *nh_ecmascript_ordinaryFunctionCreate(
    nh_ecmascript_Object *prototype,
    nh_ecmascript_ASTNode *sourceCode, // Or bytecode
    nh_ecmascript_Environment *env,
    nh_ecmascript_Realm *realm
);

nh_ecmascript_Object *nh_ecmascript_createBuiltinFunction(
    void *handler,
    int length,
    const char *name,
    nh_ecmascript_Realm *Realm_p,
    nh_ecmascript_Object *prototype
);

nh_ecmascript_Completion nh_ecmascript_get(
    nh_ecmascript_Object *O_p,
    const char *P,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_set(
    nh_ecmascript_Object *O_p,
    const char *P,
    nh_ecmascript_Value V,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_defineOwnProperty(
    nh_ecmascript_Object *O_p,
    const char *P,
    nh_ecmascript_PropertyDescriptor *Desc_p,
    nh_ecmascript_Realm *Realm_p
);

bool nh_ecmascript_hasProperty(
    nh_ecmascript_Object *O_p,
    const char *P,
    nh_ecmascript_Realm *Realm_p
);

#endif
