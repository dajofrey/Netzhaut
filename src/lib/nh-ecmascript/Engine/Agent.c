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

// STRUCTS =========================================================================================

typedef struct nh_ecmascript_AgentClusterArgs {
    void *Runtime_p;
    nh_ecmascript_AgentCluster *AgentCluster_p;
} nh_ecmascript_AgentClusterArgs;

// FUNCTIONS =======================================================================================

static nh_ecmascript_Agent *nh_ecmascript_createAgent()
{
    nh_ecmascript_Agent *Agent_p = (nh_ecmascript_Agent*)nh_core_allocate(sizeof(nh_ecmascript_Agent));

    Agent_p->Jobs = nh_core_initQueue(32);
    Agent_p->HostCommands = nh_core_initQueue(32);
    Agent_p->ExecutionContextStack = nh_core_initStack(8);
    Agent_p->littleEndian = NH_SYSTEM.littleEndian;
    Agent_p->KeptAlive = nh_core_initList(8);

//    NH_CORE_CHECK(NULL, nh_ecmascript_initializeHostDefinedRealm(&Agent_p->ExecutionContextStack))
//
//    nh_ecmascript_ExecutionContext *ExecutionContext_p = nh_core_peekStack(&Agent_p->ExecutionContextStack);
//    ExecutionContext_p->Realm_p->HostDefined.temporary_p = nh_core_allocateBytes(Args_p->p);

    return Agent_p;
}

static void *nh_ecmascript_initAgentCluster(
    nh_core_Workload *Workload_p)
{
    nh_ecmascript_AgentClusterArgs *Args_p = Workload_p->args_p;
 
    static char *name_p = "ECMAScript Agent Cluster Workload";
    static char *path_p = "nh-ecmascript/Engine/Agent.c";

    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_ECMASCRIPT;

    nh_ecmascript_AgentCluster *AgentCluster_p = Args_p->AgentCluster_p;

    AgentCluster_p->Agents = nh_core_initList(8);
    AgentCluster_p->Runtime_p = Args_p->Runtime_p;

    nh_ecmascript_Agent *Agent_p = nh_ecmascript_createAgent();
    nh_core_appendToList(&AgentCluster_p->Agents, Agent_p);

    return AgentCluster_p;
}

//static NH_SIGNAL nh_ecmascript_runAgentClusterWorkloadCommand(
//    void *p, nh_core_WorkloadCommand *Command_p)
//{
//    nh_ecmascript_Agent *Agent_p = p;
//
//    typedef enum NH_ECMASCRIPT_AGENT_COMMAND_E {
//        NH_ECMASCRIPT_AGENT_COMMAND_GLOBAL_DECLARATION_INSTANTIATION,
//        NH_ECMASCRIPT_AGENT_COMMAND_PARSE_SCRIPT,
//        NH_ECMASCRIPT_AGENT_COMMAND_SCRIPT_EVALUATION,
//    } NH_ECMASCRIPT_AGENT_COMMAND_E;
//
//    switch (Command_p->type) {
//        case NH_ECMASCRIPT_AGENT_COMMAND_GLOBAL_DECLARATION_INSTANTIATION :
//            // TODO
//            break;
//        case NH_ECMASCRIPT_AGENT_COMMAND_PARSE_SCRIPT :
//            // TODO
//            break;
//        case NH_ECMASCRIPT_AGENT_COMMAND_SCRIPT_EVALUATION :
//            // TODO
//            break;
//        default :
//            return NH_SIGNAL_ERROR;
//    }
//
//    return NH_SIGNAL_OK;
//}

static NH_API_RESULT nh_ecmascript_runAgent(
    nh_ecmascript_Agent *Agent_p)
{
    if (nh_core_peekQueue(&Agent_p->HostCommands)) {

    }

    if (nh_core_peekQueue(&Agent_p->Jobs)) {

    }

    return NH_API_SUCCESS;
}

static NH_SIGNAL nh_ecmascript_runAgentCluster(
    void *args_p)
{
    nh_ecmascript_AgentCluster *AgentCluster_p = args_p;

    for (int i = 0; i < AgentCluster_p->Agents.size; ++i) {
        nh_ecmascript_runAgent(AgentCluster_p->Agents.pp[i]);
    }

    return NH_SIGNAL_IDLE;
}

static void nh_ecmascript_freeAgentCluster(
    void *p)
{
}

NH_API_RESULT nh_ecmascript_startAgentCluster(
    void *Runtime_p, nh_ecmascript_AgentCluster *AgentCluster_p)
{
    nh_ecmascript_AgentClusterArgs Args;

    Args.Runtime_p = Runtime_p;
    Args.AgentCluster_p = AgentCluster_p;

    nh_core_activateWorkload(
        nh_ecmascript_initAgentCluster,
        nh_ecmascript_runAgentCluster,
        nh_ecmascript_freeAgentCluster,
        NULL,
        &Args,
        false
    );

    return NH_API_SUCCESS;
}
