// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Agent.h"
#include "Script.h"
#include "ExecutionContext.h"

#include "../Common/IndexMap.h"
#include "../Common/Log.h"

#include "../../nh-core/System/System.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/System/Thread.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// AGENT ===========================================================================================

void *nh_ecmascript_initAgent(
    void *args_p)
{
//    nh_ecmascript_AgentArgs *Args_p = args_p;
//
    nh_ecmascript_Agent *Agent_p = nh_core_allocate(sizeof(nh_ecmascript_Agent));
    NH_CORE_CHECK_MEM_2(NULL, Agent_p)
//
//    Agent_p->ExecutionContextStack = nh_core_initStack(8);
//    Agent_p->littleEndian = NH_SYSTEM.littleEndian;
//    Agent_p->KeptAlive = nh_core_initList(8);
//
//    NH_CORE_CHECK(NULL, nh_ecmascript_initializeHostDefinedRealm(&Agent_p->ExecutionContextStack))
//
//    nh_ecmascript_ExecutionContext *ExecutionContext_p = nh_core_peekStack(&Agent_p->ExecutionContextStack);
//    ExecutionContext_p->Realm_p->HostDefined.temporary_p = nh_core_allocateBytes(Args_p->p);

    return Agent_p;
}

NH_SIGNAL nh_ecmascript_runAgent(
    void *args_p)
{
//    NH_CORE_CHECK_NULL(NH_SIGNAL_ERROR, args_p)
//    nh_ecmascript_Agent *Agent_p = args_p;
//
//    nh_ecmascript_ExecutionContext *ExecutionContext_p = nh_core_peekStack(&Agent_p->ExecutionContextStack);
//
//    nh_ecmascript_Script *Script_p = nh_ecmascript_parseScript(ExecutionContext_p->Realm_p->HostDefined.temporary_p, ExecutionContext_p->Realm_p, NH_UNICODE_ENCODING_UTF8);
//    NH_CORE_CHECK_MEM(NH_SIGNAL_DONE, Script_p)
//
//    nh_ecmascript_evaluateScript(Script_p);

    return NH_SIGNAL_FINISH;
}

nh_ecmascript_Agent *nh_ecmascript_getCurrentAgent()
{
    return nh_core_getThread()->CurrentWorkload_p->args_p;
}

