// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "ExecutionContext.h"
#include "Agent.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>

// FUNCTIONS ======================================================================================

nh_ecmascript_ExecutionContext *nh_ecmascript_allocateExecutionContext()
{
    nh_ecmascript_ExecutionContext *Context_p = nh_core_allocate(sizeof(nh_ecmascript_ExecutionContext)); 
    if (Context_p == NULL) return NULL;

    // Initialize all pointers to NULL to prevent garbage access
    Context_p->Function_p = NULL;
    Context_p->Realm_p = NULL;
    Context_p->LexicalEnvironment_p = NULL;
    Context_p->VariableEnvironment_p = NULL;
    Context_p->PrivateEnvironment_p = NULL;
    Context_p->ScriptOrModule.handle_p = NULL;
    
    return Context_p;
}

// 9.4.1 GetRunningExecutionContext
nh_ecmascript_ExecutionContext *nh_ecmascript_getRunningExecutionContext()
{
    nh_ecmascript_Agent *Agent_p = nh_ecmascript_getRunningAgent();
    if (Agent_p == NULL || Agent_p->ExecutionContextStack.size == 0) {
        return NULL;
    }
    // The running context is always the top of the stack
    return nh_core_getLastElement(&Agent_p->ExecutionContextStack);
}

// 9.4.2 GetActiveScriptOrModule
nh_ecmascript_ScriptOrModule *nh_ecmascript_getActiveScriptOrModule()
{
    nh_ecmascript_Agent *Agent_p = nh_ecmascript_getRunningAgent();
    
    // Search from top to bottom for the first context that has a Script/Module
    for (int i = Agent_p->ExecutionContextStack.size - 1; i >= 0; --i) {
        nh_ecmascript_ExecutionContext *ctx = nh_core_getElement(&Agent_p->ExecutionContextStack, i);
        if (ctx->ScriptOrModule.handle_p != NULL) {
            return &ctx->ScriptOrModule;
        }
    }
    return NULL;
}

// 9.4.3 ResolveBinding
nh_ecmascript_Reference nh_ecmascript_resolveBinding(
    nh_encoding_UTF8String *Name_p, 
    nh_ecmascript_Environment *Environment_p)
{
    nh_ecmascript_ExecutionContext *runningContext = nh_ecmascript_getRunningExecutionContext();
    
    // If no environment is provided, use the LexicalEnvironment of the running context
    if (Environment_p == NULL) {
        Environment_p = runningContext->LexicalEnvironment_p;
    }

    // You can determine strictness from the ScriptOrModule or the Function object
    bool strict = false;
    if (runningContext->Function_p && runningContext->Function_p->strict) {
        strict = true;
    }

    return nh_ecmascript_getIdentifierReference(Environment_p, Name_p, strict);
}

void nh_ecmascript_pushExecutionContext(nh_ecmascript_ExecutionContext *Context_p)
{
    nh_ecmascript_Agent *Agent_p = nh_ecmascript_getRunningAgent();
    nh_core_addToList(&Agent_p->ExecutionContextStack, Context_p);
}

void nh_ecmascript_popExecutionContext()
{
    nh_ecmascript_Agent *Agent_p = nh_ecmascript_getRunningAgent();
    // In C, you'll need to decide if popping also frees the memory
    // or if the Garbage Collector handles it.
    nh_ecmascript_ExecutionContext *Context_p = nh_core_removeLastElement(&Agent_p->ExecutionContextStack);
    
    // nh_core_free(Context_p); // Only if not GC'd
}

// 9.4.4 ResolveThisBinding
nh_ecmascript_Value nh_ecmascript_resolveThisBinding()
{
    nh_ecmascript_ExecutionContext *context = nh_ecmascript_getRunningExecutionContext();
    
    // The spec says to start the search from the LexicalEnvironment
    return nh_ecmascript_getThisBinding(context->LexicalEnvironment_p, context->Realm_p);
}
