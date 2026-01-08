#ifndef NH_ECMASCRIPT_ENGINE_OBJECT_H
#define NH_ECMASCRIPT_ENGINE_OBJECT_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Completion.h"
#include "Value.h"
#include "Methods.h"
#include "Shape.h"

#include "../Parser/AST.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/HashMap.h"

// TYPES ============================================================

typedef struct nh_ecmascript_Environment nh_ecmascript_Environment;
typedef struct nh_ecmascript_PropertyDescriptor nh_ecmascript_PropertyDescriptor;

typedef enum NH_ECMASCRIPT_SLOT_E {
    /* --- Function Object Slots (10.2) --- */
    NH_ECMASCRIPT_SLOT_CALL = 0,                // [[Call]]
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

    NH_ECMASCRIPT_SLOT_COUNT,
} NH_ECMASCRIPT_SLOT_E;

typedef struct nh_ecmascript_Object {
    nh_ecmascript_Shape *Shape_p;
    nh_ecmascript_ObjectMethods *Methods_p; // Handle [[Get]], [[Set]], etc.
    nh_ecmascript_Value Slots[NH_ECMASCRIPT_SLOT_COUNT]; // Fixed indexing 
    nh_ecmascript_Value *Properties_p; // Ordered according to the Shape
} nh_ecmascript_Object;

// FUNCTIONS =======================================================

nh_ecmascript_Object *nh_ecmascript_ordinaryObjectCreate(
    nh_ecmascript_Object *proto_p,
    nh_ecmascript_Realm *Realm_p 
);

nh_ecmascript_Object *nh_ecmascript_ordinaryFunctionCreate(
    nh_ecmascript_Object *prototype,
    nh_ecmascript_ASTNode *sourceCode, // Or bytecode
    nh_ecmascript_Environment *env,
    nh_ecmascript_Realm *realm
);

nh_ecmascript_Object *nh_ecmascript_createBuiltinFunction(
    void *steps_p, 
    nh_ecmascript_Realm *Realm_p,
    nh_ecmascript_Object *Prototype_p
);

#endif
