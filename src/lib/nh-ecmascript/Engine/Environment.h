#ifndef NH_ECMASCRIPT_ENGINE_ENVIRONMENT_H
#define NH_ECMASCRIPT_ENGINE_ENVIRONMENT_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Object.h"
#include "../Parser/CST.h"
#include "../../nh-encoding/Encodings/UTF8.h"

// ENUMS ===========================================================================================

typedef enum {
    NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE,
    NH_ECMASCRIPT_ENVIRONMENT_OBJECT,
    NH_ECMASCRIPT_ENVIRONMENT_GLOBAL,
    NH_ECMASCRIPT_ENVIRONMENT_FUNCTION
} NH_ECMASCRIPT_ENVIRONMENT_E;

// STRUCTS =========================================================================================

typedef struct nh_ecmascript_Binding {
    bool _mutable;
    bool deletable;
    bool strict;
    bool initialized;
    nh_encoding_UTF8String *Name_p;
    nh_ecmascript_Value Value;
} nh_ecmascript_Binding;

typedef struct nh_ecmascript_Environment {
    NH_ECMASCRIPT_ENVIRONMENT_E type;
    struct nh_ecmascript_Environment *outer; 

    union {
        struct {
            nh_core_List Bindings; 
        } declarative;
        struct {
            nh_core_List Bindings; // Functional is declarative + extras
            nh_ecmascript_Value thisValue;
            nh_ecmascript_Object *functionObject;
            nh_ecmascript_Object *newTarget;
            // Section 8.1.1.3: [[ThisBindingStatus]] can be simplified to a check
            // if thisValue is empty/undefined.
        } function;
        struct {
            nh_ecmascript_Object *Object_p;
            bool withEnvironment;
        } object;
        struct {
            nh_ecmascript_Object *objectRecord;
            nh_core_HashMap declarativeRecord;
            nh_ecmascript_Value globalThisValue;
        } global;
    } records;
} nh_ecmascript_Environment;

// FUNCTIONS =======================================================================================

//nh_ecmascript_Environment *nh_ecmascript_newDeclarativeEnvironment(
//    nh_ecmascript_Environment *Outer_p
//);
//
//nh_ecmascript_Environment *nh_ecmascript_newObjectEnvironment(
//    nh_ecmascript_Object *Object_p, nh_ecmascript_Environment *Outer_p
//);

nh_ecmascript_Environment *nh_ecmascript_newGlobalEnvironment(
    nh_ecmascript_Object *GlobalObject_p,
    nh_ecmascript_Object *ThisValue_p
);

//bool nh_ecmascript_canDeclareGlobalVar(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *String_p
//);
//
//nh_ecmascript_Completion nh_ecmascript_createGlobalVarBinding(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN deletable
//);
//
//bool nh_ecmascript_canDeclareGlobalFunction(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *String_p
//);
//
//nh_ecmascript_Completion nh_ecmascript_createGlobalFunctionBinding(
//    nh_encoding_UTF8String *Name_p, nh_ecmascript_Any V, NH_ECMASCRIPT_BOOLEAN configurable
//);
//
//bool nh_ecmascript_hasVarDeclaration(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p
//);
//
//bool nh_ecmascript_hasLexicalDeclaration(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p
//);
//
//bool nh_ecmascript_hasRestrictedGlobalProperty(
//    nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p
//);
//
//bool nh_ecmascript_hasBinding(
//    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p
//);
//
//nh_ecmascript_Completion nh_ecmascript_createMutableBinding(
//    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, bool deletable
//);
//
//nh_ecmascript_Completion nh_ecmascript_createImmutableBinding(
//    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, bool strict
//);
//
//nh_ecmascript_Completion nh_ecmascript_initializeBinding(
//    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value
//);
//
//nh_ecmascript_Completion nh_ecmascript_setMutableBinding(
//    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value, bool _throw
//);

#endif // NH_ECMASCRIPT_ENGINE_ENVIRONMENT_H
