/* AUTO-GENERATED — DO NOT EDIT */
#include "PropertyMethods.gen.h"
#include <stddef.h>

const char *nh_propertyMethodFullNames[] = {
    "array_from",
    "array_isArray",
    "array_of",
    "booleanprototype_toString",
    "booleanprototype_valueOf",
    "object_assign",
    "object_create",
    "object_defineProperties",
    "object_defineProperty",
    "object_entries",
    "object_freeze",
    "object_getOwnPropertyDescriptor",
    "object_getOwnPropertyDescriptors",
    "object_getPrototypeOf",
    "object_is",
    "object_keys",
    "object_preventExtensions",
    "object_seal",
    "object_setPrototypeOf",
    "object_values",
    "objectprototype___defineGetter__",
    "objectprototype___defineSetter__",
    "objectprototype___lookupGetter__",
    "objectprototype___lookupSetter__",
    "objectprototype_hasOwnProperty",
    "objectprototype_isPrototypeOf",
    "objectprototype_propertyIsEnumerable",
    "objectprototype_toLocaleString",
    "objectprototype_toString",
    "objectprototype_valueOf",
};

nh_PropertyMethodFn nh_propertyMethodDispatch[] = {
    nh_ecmascript_array_from,
    nh_ecmascript_array_isArray,
    nh_ecmascript_array_of,
    nh_ecmascript_booleanprototype_toString,
    nh_ecmascript_booleanprototype_valueOf,
    nh_ecmascript_object_assign,
    nh_ecmascript_object_create,
    nh_ecmascript_object_defineProperties,
    nh_ecmascript_object_defineProperty,
    nh_ecmascript_object_entries,
    nh_ecmascript_object_freeze,
    nh_ecmascript_object_getOwnPropertyDescriptor,
    nh_ecmascript_object_getOwnPropertyDescriptors,
    nh_ecmascript_object_getPrototypeOf,
    nh_ecmascript_object_is,
    nh_ecmascript_object_keys,
    nh_ecmascript_object_preventExtensions,
    nh_ecmascript_object_seal,
    nh_ecmascript_object_setPrototypeOf,
    nh_ecmascript_object_values,
    nh_ecmascript_objectprototype___defineGetter__,
    nh_ecmascript_objectprototype___defineSetter__,
    nh_ecmascript_objectprototype___lookupGetter__,
    nh_ecmascript_objectprototype___lookupSetter__,
    nh_ecmascript_objectprototype_hasOwnProperty,
    nh_ecmascript_objectprototype_isPrototypeOf,
    nh_ecmascript_objectprototype_propertyIsEnumerable,
    nh_ecmascript_objectprototype_toLocaleString,
    nh_ecmascript_objectprototype_toString,
    nh_ecmascript_objectprototype_valueOf,
};

const size_t nh_propertyMethodCount = sizeof(nh_propertyMethodFullNames) / sizeof(nh_propertyMethodFullNames[0]);

nh_PropertyMethodFn nh_ecmascript_lookupPropertyMethod(const char *fullName) {
    for (size_t i = 0; i < nh_propertyMethodCount; i++) {
        if (strcmp(nh_propertyMethodFullNames[i], fullName) == 0) {
            return nh_propertyMethodDispatch[i];
        }
    }
    return NULL;
}

