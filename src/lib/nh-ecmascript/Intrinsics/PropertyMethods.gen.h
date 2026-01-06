/* AUTO-GENERATED — DO NOT EDIT */
#pragma once
#include "../Engine/Object.h"
#include "../Engine/Value.h"
#include "../Engine/Completion.h"
#include "../Engine/Realm.h"

typedef nh_ecmascript_Completion (*nh_PropertyMethodFn)(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);
nh_ecmascript_Completion nh_ecmascript_array_from(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_array_isArray(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_array_of(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_booleanprototype_toString(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_booleanprototype_valueOf(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_assign(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_create(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_defineProperties(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_defineProperty(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_entries(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_freeze(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_getOwnPropertyDescriptor(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_getOwnPropertyDescriptors(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_getPrototypeOf(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_is(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_keys(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_preventExtensions(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_seal(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_setPrototypeOf(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_object_values(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype___defineGetter__(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype___defineSetter__(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype___lookupGetter__(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype___lookupSetter__(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype_hasOwnProperty(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype_isPrototypeOf(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype_propertyIsEnumerable(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype_toLocaleString(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype_toString(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);
nh_ecmascript_Completion nh_ecmascript_objectprototype_valueOf(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);

nh_PropertyMethodFn nh_ecmascript_lookupPropertyMethod(const char *fullName); 

