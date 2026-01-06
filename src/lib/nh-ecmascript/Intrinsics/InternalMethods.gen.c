/* AUTO-GENERATED — DO NOT EDIT */
#include "InternalMethods.h"
#include <stddef.h>

const char *nh_internalMethodNames[] = {
    "ArrayConstructorCall",
    "ArrayConstructorConstruct",
    "BooleanConstructorCall",
    "BooleanConstructorConstruct",
    "FunctionConstructorCall",
    "FunctionConstructorConstruct",
    "ObjectConstructorCall",
    "ObjectConstructorConstruct",
    "ThrowTypeError",
};

nh_InternalMethodFn nh_internalMethodDispatch[] = {
    nh_ecmascript_arrayConstructorCall,
    nh_ecmascript_arrayConstructorConstruct,
    nh_ecmascript_booleanConstructorCall,
    nh_ecmascript_booleanConstructorConstruct,
    nh_ecmascript_functionConstructorCall,
    nh_ecmascript_functionConstructorConstruct,
    nh_ecmascript_objectConstructorCall,
    nh_ecmascript_objectConstructorConstruct,
    nh_ecmascript_throwTypeError,
};

const size_t nh_internalMethodCount =
    sizeof(nh_internalMethodNames) / sizeof(nh_internalMethodNames[0]);
