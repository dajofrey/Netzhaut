#ifndef NH_ECMASCRIPT_EXECUTION_CONTEXT_H
#define NH_ECMASCRIPT_EXECUTION_CONTEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Parser/Script.h"
#include "Realm.h"
#include "Environment.h"
#include "Reference.h"

#endif

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

//    typedef struct nh_ecmascript_CodeEvaluation {
//        bool suspend;
//    } nh_ecmascript_CodeEvaluation;

    typedef struct nh_ecmascript_ScriptOrModule {
        bool isScript;
        void *handle_p;
    } nh_ecmascript_ScriptOrModule;

// https://www.262.ecma-international.org/11.0/index.html#sec-execution-contexts
typedef struct nh_ecmascript_ExecutionContext {
    // 1. Spec components
    nh_ecmascript_Object      *Function_p;           // The function being executed (or NULL for global)
    nh_ecmascript_Realm       *Realm_p;
    nh_ecmascript_ScriptOrModule ScriptOrModule;
    
    nh_ecmascript_Environment *LexicalEnvironment_p; // For 'let/const'
    nh_ecmascript_Environment *VariableEnvironment_p;// For 'var'
    nh_ecmascript_Environment *PrivateEnvironment_p; // For class #private fields
    
    // 2. The "This" Cache
    nh_ecmascript_Value        ThisBinding;          // Fast access to 'this'

    // 3. Interpreter State
    uint8_t                   *ip;                   // Instruction Pointer (if using bytecode)
    // OR: nh_ecmascript_ParseNode *currentNode;     // If using an AST interpreter

    // 4. Stack Linkage
    struct nh_ecmascript_ExecutionContext *Previous_p; // The context to return to after 'pop'
} nh_ecmascript_ExecutionContext;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    nh_ecmascript_ExecutionContext *nh_ecmascript_allocateExecutionContext(
    );

    nh_ecmascript_ExecutionContext *nh_ecmascript_getRunningExecutionContext(
    );

    nh_ecmascript_Realm *nh_ecmascript_getCurrentRealm(
    );

    nh_ecmascript_Object *nh_ecmascript_getGlobalObject(
    );

    nh_ecmascript_ScriptOrModule *nh_ecmascript_getActiveScriptOrModule(
    );

    nh_ecmascript_Reference nh_ecmascript_resolveBinding(
        nh_encoding_UTF8String *Name_p, nh_ecmascript_Environment *Environment_p
    );

/** @} */

#endif
