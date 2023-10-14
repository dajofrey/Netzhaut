// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "ExecutionContext.h"
#include "Agent.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"

#include <string.h>

// EXECUTION CONTEXT ===============================================================================

nh_ecmascript_ExecutionContext *nh_ecmascript_allocateExecutionContext()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_ExecutionContext *Context_p = nh_core_allocate(sizeof(nh_ecmascript_ExecutionContext)); 
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, Context_p)

    Context_p->Function_p = NULL;
    Context_p->Realm_p = NULL;

NH_ECMASCRIPT_END(Context_p)
}

nh_ecmascript_ExecutionContext *nh_ecmascript_getRunningExecutionContext()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_peekStack(&nh_ecmascript_getCurrentAgent()->ExecutionContextStack))
}

nh_ecmascript_ScriptOrModule *nh_ecmascript_getActiveScriptOrModule()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Agent *Agent_p = nh_ecmascript_getCurrentAgent();

    if (Agent_p->ExecutionContextStack.size == 0) {NH_ECMASCRIPT_END(NULL)}

    for (int i = Agent_p->ExecutionContextStack.size - 1; i >= 0; ++i) {
        nh_ecmascript_ExecutionContext *ExecutionContext_p = Agent_p->ExecutionContextStack.pp[i];
        if (ExecutionContext_p->ScriptOrModule.handle_p != NULL) {
            NH_ECMASCRIPT_END(&ExecutionContext_p->ScriptOrModule)
        }
    }

NH_ECMASCRIPT_END(NULL)
}

nh_ecmascript_Realm *nh_ecmascript_getCurrentRealm()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_ecmascript_getRunningExecutionContext()->Realm_p)
}

nh_ecmascript_Object *nh_ecmascript_getGlobalObject()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_ecmascript_getCurrentRealm()->GlobalObject_p)
}

nh_ecmascript_Reference nh_ecmascript_resolveBinding(
    nh_encoding_UTF8String *Name_p, nh_ecmascript_Environment *Environment_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Environment_p == NULL) {
        Environment_p = nh_ecmascript_getRunningExecutionContext()->LexicalEnvironment_p;
    }

    // TODO If the code matching the syntactic production that is being evaluated is contained in strict mode code, let strict be true; else let strict be false.
    NH_ECMASCRIPT_BOOLEAN strict = NH_FALSE;

NH_ECMASCRIPT_END(nh_ecmascript_getIdentifierReference(Environment_p, Name_p, strict))
}

