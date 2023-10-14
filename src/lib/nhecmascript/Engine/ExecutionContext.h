#ifndef NH_ECMASCRIPT_EXECUTION_CONTEXT_H
#define NH_ECMASCRIPT_EXECUTION_CONTEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Script.h"
#include "Realm.h"
#include "Environment.h"
#include "Reference.h"

#endif

/** @addtogroup lib_nhecmascript_structs
 *  @{
 */

//    typedef struct nh_ecmascript_CodeEvaluation {
//        NH_BOOL suspend;
//    } nh_ecmascript_CodeEvaluation;

    typedef struct nh_ecmascript_ScriptOrModule {
        NH_BOOL isScript;
        void *handle_p;
    } nh_ecmascript_ScriptOrModule;

    // https://www.262.ecma-international.org/11.0/index.html#sec-execution-contexts
    typedef struct nh_ecmascript_ExecutionContext {
//        nh_ecmascript_CodeEvalution CodeEvalution;
        nh_ecmascript_Object *Function_p;
        nh_ecmascript_Realm *Realm_p;
        nh_ecmascript_ScriptOrModule ScriptOrModule;
        nh_ecmascript_Environment *LexicalEnvironment_p;
        nh_ecmascript_Environment *VariableEnvironment_p;
    } nh_ecmascript_ExecutionContext;

/** @} */

/** @addtogroup lib_nhecmascript_functions
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
