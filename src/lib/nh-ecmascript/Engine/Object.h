#ifndef NH_ECMASCRIPT_ENGINE_OBJECT_H
#define NH_ECMASCRIPT_ENGINE_OBJECT_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Completion.h"
#include "Value.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/HashMap.h"

// TYPES ============================================================

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

typedef enum NH_ECMASCRIPT_INTERNAL_SLOT_E { 
    NH_ECMASCRIPT_INTERNAL_SLOT_NULL,
    NH_ECMASCRIPT_INTERNAL_SLOT_UNDEFINED,
    NH_ECMASCRIPT_INTERNAL_SLOT_BOOLEAN,
    NH_ECMASCRIPT_INTERNAL_SLOT_NUMBER,
} NH_ECMASCRIPT_INTERNAL_SLOT_E;

typedef enum NH_ECMASCRIPT_OBJECT_E { 
    NH_ECMASCRIPT_OBJECT_ORDINARY,
    NH_ECMASCRIPT_OBJECT_FUNCTION,
    NH_ECMASCRIPT_OBJECT_ARRAY,
} NH_ECMASCRIPT_OBJECT_E;

typedef struct nh_ecmascript_InternalSlot {
    NH_ECMASCRIPT_INTERNAL_SLOT_E type;
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
    // own properties
    nh_core_HashMap Properties; // of type nh_ecmascript_Property
    // for function objects
    void *nativeCall;
    void *nativeConstruct;
    unsigned int functionLength;
} nh_ecmascript_Object;

// FUNCTIONS =======================================================

void nh_ecmascript_createDataPropertyOrThrow(
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
    nh_ecmascript_NativeHandler handler,
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

bool nh_ecmascript_defineOwnProperty(
    nh_ecmascript_Object *O_p,
    const char *P,
    nh_ecmascript_PropertyDescriptor *Desc_p,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_hasProperty(
    nh_ecmascript_Object *O_p,
    const char *P,
    nh_ecmascript_Realm *Realm_p
);

#endif
