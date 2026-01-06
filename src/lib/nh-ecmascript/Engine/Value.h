#ifndef NH_ECMASCRIPT_ENGINE_VALUE_H
#define NH_ECMASCRIPT_ENGINE_VALUE_H

#include <stdbool.h>
#include <stdint.h>

// STRUCTS ===============================================================================

// Forward declaration of the Object struct
typedef struct nh_ecmascript_Completion nh_ecmascript_Completion;
typedef struct nh_ecmascript_Object nh_ecmascript_Object;
typedef struct nh_ecmascript_Realm nh_ecmascript_Realm;

typedef enum NH_ECMASCRIPT_PREFERRED_TYPE_E {
    NH_ECMASCRIPT_PREFERRED_TYPE_NONE = 0,
    NH_ECMASCRIPT_PREFERRED_TYPE_STRING,
    NH_ECMASCRIPT_PREFERRED_TYPE_NUMBER
} NH_ECMASCRIPT_PREFERRED_TYPE_E;

typedef enum NH_ECMASCRIPT_VALUE_E {
    NH_ECMASCRIPT_VALUE_UNDEFINED = 0,
    NH_ECMASCRIPT_VALUE_NULL,
    NH_ECMASCRIPT_VALUE_BOOLEAN,
    NH_ECMASCRIPT_VALUE_NUMBER,
    NH_ECMASCRIPT_VALUE_STRING,
    NH_ECMASCRIPT_VALUE_SYMBOL,
    NH_ECMASCRIPT_VALUE_OBJECT,
    NH_ECMASCRIPT_VALUE_INTERNAL_POINTER,
    NH_ECMASCRIPT_VALUE_REFERNCE,
} NH_ECMASCRIPT_VALUE_E;

typedef struct nh_ecmascript_Value {
    NH_ECMASCRIPT_VALUE_E tag;
    union {
        bool boolean;
        double number;
        void *string; // Pointer to your internal string representation
        void *symbol; // Pointer to symbol description/id
        void *internalPointer;
        void *reference;
        struct nh_ecmascript_Object *object;
    } p;
} nh_ecmascript_Value;

// FUNCTIONS =======================================================================

bool nh_ecmascript_toBoolean(
    nh_ecmascript_Value Value
);

nh_ecmascript_Completion nh_ecmascript_toObject(
    nh_ecmascript_Value Value, 
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_toPrimitive(
    nh_ecmascript_Value v,
    NH_ECMASCRIPT_PREFERRED_TYPE_E hint,
    nh_ecmascript_Realm *Realm_p
);

static inline nh_ecmascript_Value nh_ecmascript_makeInternalPointer(void *ptr) {
    nh_ecmascript_Value v;
    v.tag = NH_ECMASCRIPT_VALUE_INTERNAL_POINTER;
    v.p.internalPointer = ptr; // Ensure your union has this field!
    return v;
}

static inline nh_ecmascript_Value nh_ecmascript_makeUndefined() {
    return (nh_ecmascript_Value){ .tag = NH_ECMASCRIPT_VALUE_UNDEFINED };
}

static inline nh_ecmascript_Value nh_ecmascript_makeNull() {
    return (nh_ecmascript_Value){ .tag = NH_ECMASCRIPT_VALUE_NULL };
}

static inline nh_ecmascript_Value nh_ecmascript_makeNumber(double n) {
    return (nh_ecmascript_Value){ .tag = NH_ECMASCRIPT_VALUE_NUMBER, .p.number = n };
}

static inline nh_ecmascript_Value nh_ecmascript_makeBoolean(bool b) {
    return (nh_ecmascript_Value){ .tag = NH_ECMASCRIPT_VALUE_BOOLEAN, .p.boolean = b };
}

static inline nh_ecmascript_Value nh_ecmascript_makeObject(struct nh_ecmascript_Object *obj) {
    return (nh_ecmascript_Value){ .tag = NH_ECMASCRIPT_VALUE_OBJECT, .p.object = obj };
}

static inline nh_ecmascript_Value nh_ecmascript_makeString(char *string_p) {
    return (nh_ecmascript_Value){ .tag = NH_ECMASCRIPT_VALUE_STRING, .p.string = string_p };
}

#endif
