#ifndef NH_ECMASCRIPT_ENVIRONMENT_H
#define NH_ECMASCRIPT_ENVIRONMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Parser.h"
#include "ObjectType.h"

#endif

/** @addtogroup lib_nh-ecmascript_enums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_ENVIRONMENT {
        NH_ECMASCRIPT_ENVIRONMENT_DECLARATIVE,
        NH_ECMASCRIPT_ENVIRONMENT_OBJECT,
        NH_ECMASCRIPT_ENVIRONMENT_GLOBAL,
    } NH_ECMASCRIPT_ENVIRONMENT;

/** @} */

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_Environment {
        NH_ECMASCRIPT_ENVIRONMENT type;
        void *Handle_p;
        struct nh_ecmascript_Environment *Outer_p;
    } nh_ecmascript_Environment;

    typedef struct nh_ecmascript_Binding {
        bool _mutable;
        bool deletable;
        bool strict;
        bool initialized;
        nh_encoding_UTF8String *Name_p;
        nh_ecmascript_Any Value;
    } nh_ecmascript_Binding;

    typedef struct nh_ecmascript_DeclarativeEnvironment {
        nh_List Bindings;
    } nh_ecmascript_DeclarativeEnvironment;

    typedef struct nh_ecmascript_ObjectEnvironment {
        nh_ecmascript_Object *BindingObject_p;
        NH_ECMASCRIPT_BOOLEAN withEnvironment;
    } nh_ecmascript_ObjectEnvironment;

    typedef struct nh_ecmascript_GlobalEnvironment {
        nh_ecmascript_Environment *ObjectEnvironment_p;
        nh_ecmascript_Environment *DeclarativeEnvironment_p;
        nh_ecmascript_Object *GlobalThisValue_p;
        nh_List VarNames;
    } nh_ecmascript_GlobalEnvironment;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

// NEW

    nh_ecmascript_Environment *nh_ecmascript_newDeclarativeEnvironment(
        nh_ecmascript_Environment *Outer_p
    );

    nh_ecmascript_Environment *nh_ecmascript_newObjectEnvironment(
        nh_ecmascript_Object *Object_p, nh_ecmascript_Environment *Outer_p
    );
    
    nh_ecmascript_Environment *nh_ecmascript_newGlobalEnvironment(
        nh_ecmascript_Object *GlobalObject_p, nh_ecmascript_Object *ThisValue_p
    );

// MISC

    bool nh_ecmascript_canDeclareGlobalVar(
        nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *String_p
    );

    nh_ecmascript_Completion nh_ecmascript_createGlobalVarBinding(
        nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN deletable
    );

    bool nh_ecmascript_canDeclareGlobalFunction(
        nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *String_p
    );

    nh_ecmascript_Completion nh_ecmascript_createGlobalFunctionBinding(
        nh_encoding_UTF8String *Name_p, nh_ecmascript_Any V, NH_ECMASCRIPT_BOOLEAN configurable
    );

    bool nh_ecmascript_hasVarDeclaration(
        nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p
    );
    
    bool nh_ecmascript_hasLexicalDeclaration(
        nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p
    );
    
    bool nh_ecmascript_hasRestrictedGlobalProperty(
        nh_ecmascript_GlobalEnvironment *GlobalEnvironment_p, nh_encoding_UTF8String *Name_p
    );

// ABSTRACT

    bool nh_ecmascript_hasBinding(
        nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p
    );

    nh_ecmascript_Completion nh_ecmascript_createMutableBinding(
        nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, bool deletable
    );
    
    nh_ecmascript_Completion nh_ecmascript_createImmutableBinding(
        nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, bool strict
    );
    
    nh_ecmascript_Completion nh_ecmascript_initializeBinding(
        nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value
    );

    nh_ecmascript_Completion nh_ecmascript_setMutableBinding(
        nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, nh_ecmascript_Any Value, bool _throw
    );

/** @} */

#endif
