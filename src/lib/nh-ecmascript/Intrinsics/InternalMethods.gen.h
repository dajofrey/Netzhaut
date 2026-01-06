#pragma once

#include "../Engine/Object.h"
#include "../Engine/Value.h"
#include "../Engine/Completion.h"
#include "../Engine/Realm.h"

typedef nh_ecmascript_Completion (*nh_InternalMethodFn)(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);
nh_ecmascript_Completion nh_ecmascript_arrayConstructorCall(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_arrayConstructorConstruct(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_booleanConstructorCall(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_booleanConstructorConstruct(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_functionConstructorCall(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_functionConstructorConstruct(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_objectConstructorCall(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_objectConstructorConstruct(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Completion nh_ecmascript_throwTypeError(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

extern nh_InternalMethodFn
    nh_internalMethodDispatch[NH_IMPL_COUNT];
