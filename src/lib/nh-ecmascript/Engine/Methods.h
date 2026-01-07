#ifndef NH_ECMASCRIPT_ENGINE_METHODS_H
#define NH_ECMASCRIPT_ENGINE_METHODS_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Completion.h"
#include "Value.h"

#include "../../nh-core/Util/List.h"

typedef struct nh_ecmascript_Object nh_ecmascript_Object;
typedef struct nh_ecmascript_PropertyDescriptor nh_ecmascript_PropertyDescriptor;

typedef struct nh_ecmascript_ObjectMethods {
    // https://tc39.es/ecma262/#sec-ordinary-and-exotic-objects-behaviours
    nh_ecmascript_Completion (*getPrototypeOf)(nh_ecmascript_Object *Object_p);
    nh_ecmascript_Completion (*setPrototypeOf)(nh_ecmascript_Object *Object_p, nh_ecmascript_Object *V);
    nh_ecmascript_Completion (*isExtensible)(nh_ecmascript_Object *Object_p);
    nh_ecmascript_Completion (*preventExtensions)(nh_ecmascript_Object *Object_p);
    nh_ecmascript_Completion (*getOwnProperty)(nh_ecmascript_Object *Object_p, char *p, nh_ecmascript_PropertyDescriptor *desc);
    nh_ecmascript_Completion (*defineOwnProperty)(nh_ecmascript_Object *Object_p, char *p, nh_ecmascript_PropertyDescriptor *desc);
    nh_ecmascript_Completion (*hasProperty)(nh_ecmascript_Object *Object_p, char *p);
    nh_ecmascript_Completion (*get)(nh_ecmascript_Object *Object_p, const char *p, nh_ecmascript_Object *Receiver_p);
    nh_ecmascript_Completion (*set)(nh_ecmascript_Object *Object_p, char *p, nh_ecmascript_Value V, nh_ecmascript_Object *Receiver_p);
    nh_ecmascript_Completion (*delete)(nh_ecmascript_Object *Object_p, char *p);
    nh_ecmascript_Completion (*ownPropertyKeys)(nh_ecmascript_Object *Object_p);
    // https://tc39.es/ecma262/#sec-ecmascript-function-objects
    nh_ecmascript_Completion (*call)(nh_ecmascript_Object *Object_p, nh_ecmascript_Value thisArgument, nh_core_List argumentsList);
    nh_ecmascript_Completion (*construct)(nh_ecmascript_Object *Object_p, nh_core_List argumentsList, nh_ecmascript_Object *newTarget);
} nh_ecmascript_ObjectMethods;

extern const nh_ecmascript_ObjectMethods NH_ECMASCRIPT_METHODS_ORDINARY;
extern const nh_ecmascript_ObjectMethods NH_ECMASCRIPT_METHODS_FUNCTION;
extern const nh_ecmascript_ObjectMethods NH_ECMASCRIPT_METHODS_ARRAY;

#endif
