#ifndef NH_ECMASCRIPT_ENGINE_REALM_H
#define NH_ECMASCRIPT_ENGINE_REALM_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Agent.h"
#include "ObjectType.h"
#include "Environment.h"

#include "../Common/Includes.h"
#include "../../nh-core/Util/Stack.h"

// STRUCTS ================================================================================

typedef struct nh_ecmascript_IntrinsicObject {
    // internal
    nh_ecmascript_String Keys_p[29];
    nh_ecmascript_Property Properties_p[29];
    // main objects
    nh_ecmascript_Object Constructor;
    nh_ecmascript_Object Prototype;
    // object constructor builtin function objects
    nh_ecmascript_Object Assign;
    nh_ecmascript_Object Create;
    nh_ecmascript_Object DefineProperties;
    nh_ecmascript_Object DefineProperty;
    nh_ecmascript_Object Entries;
    nh_ecmascript_Object Freeze;
    nh_ecmascript_Object FromEntries;
    nh_ecmascript_Object GetOwnPropertyDescriptor;
    nh_ecmascript_Object GetOwnPropertyDescriptors;
    nh_ecmascript_Object GetOwnPropertyNames;
    nh_ecmascript_Object GetOwnPropertySymbols;
    nh_ecmascript_Object GetPrototypeOf;
    nh_ecmascript_Object Is;
    nh_ecmascript_Object IsExtensible;
    nh_ecmascript_Object IsFrozen;
    nh_ecmascript_Object IsSealed;
    nh_ecmascript_Object Keys;
    nh_ecmascript_Object PreventExtensions;
    nh_ecmascript_Object Seal;
    nh_ecmascript_Object SetPrototypeOf;
    nh_ecmascript_Object Values;
    // object prototype  builtin function objects
    nh_ecmascript_Object HasOwnProperty;
    nh_ecmascript_Object IsPrototypeOf;
    nh_ecmascript_Object PropertyIsEnumerable;
    nh_ecmascript_Object ToLocaleString;
    nh_ecmascript_Object ToString;
    nh_ecmascript_Object ValueOf;
} nh_ecmascript_IntrinsicObject;

typedef struct nh_ecmascript_IntrinsicFunction {
    nh_ecmascript_Object Constructor;
    nh_ecmascript_Object Prototype;
} nh_ecmascript_IntrinsicFunction;

typedef struct nh_ecmascript_Intrinsics {
    // internal
    nh_ecmascript_String Keys_p[52];
    nh_ecmascript_Property Properties_p[52];
    // global object properties
//        nh_ecmascript_Object AggregateError;
//        nh_ecmascript_Object Array;
//        nh_ecmascript_Object ArrayBuffer;
//        nh_ecmascript_Object ArrayBufferPrototype;
//        nh_ecmascript_Object ArrayIteratorPrototype;
//        nh_ecmascript_Object ArrayPrototype;
//        nh_ecmascript_Object ArrayProto_entries;
//        nh_ecmascript_Object ArrayProto_forEach;
//        nh_ecmascript_Object ArrayProto_keys;
//        nh_ecmascript_Object ArrayProto_values;
//        nh_ecmascript_Object AsyncFromSyncIteratorPrototype;
//        nh_ecmascript_Object AsyncFunction;
//        nh_ecmascript_Object AsyncFunctionPrototype;
//        nh_ecmascript_Object AsyncGenerator;
//        nh_ecmascript_Object AsyncGeneratorFunction;
//        nh_ecmascript_Object AsyncGeneratorPrototype;
//        nh_ecmascript_Object AsyncIteratorPrototype;
//        nh_ecmascript_Object Atomics;
//        nh_ecmascript_Object BigInt;
//        nh_ecmascript_Object BigInt64Array;
//        nh_ecmascript_Object BigUint64Array;
//        nh_ecmascript_Object Boolean;
//        nh_ecmascript_Object BooleanPrototype;
//        nh_ecmascript_Object DataView;
//        nh_ecmascript_Object DataViewPrototype;
//        nh_ecmascript_Object Date;
//        nh_ecmascript_Object DatePrototype;
//        nh_ecmascript_Object decodeURI;
//        nh_ecmascript_Object decodeURIComponent;
//        nh_ecmascript_Object encodeURI;
//        nh_ecmascript_Object encodeURIComponent;
//        nh_ecmascript_Object Error;
//        nh_ecmascript_Object ErrorPrototype;
//        nh_ecmascript_Object eval;
//        nh_ecmascript_Object EvalError;
//        nh_ecmascript_Object EvalErrorPrototype;
//        nh_ecmascript_Object Float32Array;
//        nh_ecmascript_Object Float32ArrayPrototype;
//        nh_ecmascript_Object Float64Array;
//        nh_ecmascript_Object Float64ArrayPrototype;
//        nh_ecmascript_Object ForInIteratorPrototype;
        nh_ecmascript_IntrinsicFunction Function;
//        nh_ecmascript_Object FunctionPrototype;
//        nh_ecmascript_Object Generator;
//        nh_ecmascript_Object GeneratorFunction;
//        nh_ecmascript_Object GeneratorPrototype;
//        nh_ecmascript_Object Int8Array;
//        nh_ecmascript_Object Int8ArrayPrototype;
//        nh_ecmascript_Object Int16Array;
//        nh_ecmascript_Object Int16ArrayPrototype;
//        nh_ecmascript_Object Int32Array;
//        nh_ecmascript_Object Int32ArrayPrototype;
//        nh_ecmascript_Object isFinite;
//        nh_ecmascript_Object isNaN;
//        nh_ecmascript_Object IteratorPrototype;
//        nh_ecmascript_Object JSON;
//        nh_ecmascript_Object JSONParse;
//        nh_ecmascript_Object JSONStringify;
//        nh_ecmascript_Object Map;
//        nh_ecmascript_Object MapIteratorPrototype;
//        nh_ecmascript_Object MapPrototype;
//        nh_ecmascript_Object Math;
//        nh_ecmascript_Object Number;
//        nh_ecmascript_Object NumberPrototype;
        nh_ecmascript_IntrinsicObject Object;
//        nh_ecmascript_Object ObjProto_toString;
//        nh_ecmascript_Object ObjProto_valueOf;
//        nh_ecmascript_Object parseFloat;
//        nh_ecmascript_Object parseInt;
//        nh_ecmascript_Object Promise;
//        nh_ecmascript_Object PromisePrototype;
//        nh_ecmascript_Object PromiseProto_then;
//        nh_ecmascript_Object Promise_all;
//        nh_ecmascript_Object Promise_reject;
//        nh_ecmascript_Object Promise_resolve;
//        nh_ecmascript_Object Proxy;
//        nh_ecmascript_Object RangeError;
//        nh_ecmascript_Object RangeErrorPrototype;
//        nh_ecmascript_Object ReferenceError;
//        nh_ecmascript_Object ReferenceErrorPrototype;
//        nh_ecmascript_Object Reflect;
//        nh_ecmascript_Object RegExp;
//        nh_ecmascript_Object RegExpPrototype;
//        nh_ecmascript_Object RegExpStringIteratorPrototype;
//        nh_ecmascript_Object Set;
//        nh_ecmascript_Object SetIteratorPrototype;
//        nh_ecmascript_Object SetPrototype;
//        nh_ecmascript_Object SharedArrayBuffer;
//        nh_ecmascript_Object SharedArrayBufferPrototype;
//        nh_ecmascript_Object String;
//        nh_ecmascript_Object StringIteratorPrototype;
//        nh_ecmascript_Object StringPrototype;
//        nh_ecmascript_Object Symbol;
//        nh_ecmascript_Object SymbolPrototype;
//        nh_ecmascript_Object SyntaxError;
//        nh_ecmascript_Object SyntaxErrorPrototype;
//        nh_ecmascript_Object ThrowTypeError;
//        nh_ecmascript_Object TypedArray;
//        nh_ecmascript_Object TypedArrayPrototype;
//        nh_ecmascript_Object TypeError;
//        nh_ecmascript_Object TypeErrorPrototype;
//        nh_ecmascript_Object Uint8Array;
//        nh_ecmascript_Object Uint8ArrayPrototype;
//        nh_ecmascript_Object Uint8ClampedArray;
//        nh_ecmascript_Object Uint8ClampedArrayPrototype;
//        nh_ecmascript_Object Uint16Array;
//        nh_ecmascript_Object Uint16ArrayPrototype;
//        nh_ecmascript_Object Uint32Array;
//        nh_ecmascript_Object Uint32ArrayPrototype;
//        nh_ecmascript_Object URIError;
//        nh_ecmascript_Object URIErrorPrototype;
//        nh_ecmascript_Object WeakMap;
//        nh_ecmascript_Object WeakMapPrototype;
//        nh_ecmascript_Object WeakSet;
//        nh_ecmascript_Object WeakSetPrototype;
} nh_ecmascript_Intrinsics;

typedef struct nh_ecmascript_RealmHostDefined {
    char *temporary_p;
} nh_ecmascript_RealmHostDefined;

// https://tc39.es/ecma262/#sec-code-realms
typedef struct nh_ecmascript_Realm {
    nh_ecmascript_Agent *Agent_p;
    nh_ecmascript_Intrinsics Intrinsics;
    nh_ecmascript_Object *GlobalObject_p;
    nh_ecmascript_Environment *GlobalEnvironment_p;
    nh_ecmascript_RealmHostDefined HostDefined;
} nh_ecmascript_Realm;

// FUNCTIONS ==============================================================

nh_ecmascript_Realm *nh_ecmascript_initializeRealm(
    nh_ecmascript_Agent *Agent_p
);

#endif
