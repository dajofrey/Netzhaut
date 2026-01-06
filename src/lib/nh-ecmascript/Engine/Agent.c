// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Agent.h"
#include "ExecutionContext.h"
#include "Realm.h"

#include "../Parser/Script.h"
#include "../Interpreter/Evaluation.h"

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

typedef struct nh_ecmascript_ParseScriptArgs {
    nh_ecmascript_Realm *Realm_p;
    char *src_p;
} nh_ecmascript_ParseScriptArgs;

typedef struct nh_ecmascript_InstallHostGlobalsArgs {
    void *Globals_p;
    nh_ecmascript_Realm *Realm_p;
} nh_ecmascript_InstallHostGlobalsArgs;

// FUNCTIONS =======================================================================================

static nh_ecmascript_Agent *nh_ecmascript_initAgent()
{
    nh_ecmascript_Agent *Agent_p = (nh_ecmascript_Agent*)nh_core_allocate(sizeof(nh_ecmascript_Agent));

    Agent_p->Jobs = nh_core_initQueue(32);
    Agent_p->HostCommands = nh_core_initQueue(32);
    Agent_p->ExecutionContextStack = nh_core_initStack(8);
    Agent_p->littleEndian = NH_SYSTEM.littleEndian;
    Agent_p->KeptAlive = nh_core_initList(8);
    Agent_p->inUse = false;

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

    nh_ecmascript_Agent *Agent_p = nh_ecmascript_initAgent();
    nh_core_appendToList(&AgentCluster_p->Agents, Agent_p);

    return AgentCluster_p;
}

static NH_SIGNAL nh_ecmascript_runJob(
    nh_ecmascript_Agent *Agent_p, nh_ecmascript_Job *Job_p)
{
    // TODO

    return NH_SIGNAL_OK;
}

static NH_API_RESULT nh_ecmascript_runHostCommand(
    nh_ecmascript_Agent *Agent_p, nh_ecmascript_HostCommand *HostCommand_p)
{
    switch (HostCommand_p->type) {
        case 0 :
            HostCommand_p->result_p = nh_ecmascript_initializeRealm(Agent_p);
            HostCommand_p->done = true;
            break;
        case 1 :
        {
            nh_ecmascript_InstallHostGlobalsArgs *Args_p = HostCommand_p->args_p; 
//            nh_ecmascript_installHostGlobals(Args_p->Realm_p, Args_p->Globals_p);
            HostCommand_p->result_p = NULL;
            HostCommand_p->done = true;
            break;
        }
        case 2 :
        {
            nh_ecmascript_ParseScriptArgs *Args_p = HostCommand_p->args_p; 
            nh_ecmascript_Script *Script_p = nh_ecmascript_parseScript(Args_p->src_p, Args_p->Realm_p, 0);
            HostCommand_p->result_p = Script_p;
            HostCommand_p->done = true;
            break;
        }
        case 3 :
            nh_ecmascript_evaluateScript(HostCommand_p->args_p);
            HostCommand_p->done = true;
            break;
        default :
            return NH_API_ERROR_BAD_STATE;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_ecmascript_runAgent(
    nh_ecmascript_Agent *Agent_p)
{
    if (nh_core_peekQueue(&Agent_p->HostCommands)) {
        nh_ecmascript_runHostCommand(Agent_p, nh_core_dequeue(&Agent_p->HostCommands));
    }

    if (nh_core_peekQueue(&Agent_p->Jobs)) {
        nh_ecmascript_runJob(Agent_p, nh_core_dequeue(&Agent_p->Jobs));
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

    return NH_SIGNAL_OK;
}

static void nh_ecmascript_freeAgentCluster(
    void *p)
{
    // TODO
}

static NH_SIGNAL nh_ecmascript_enqueueHostCommandOrJob(
    void *args_p, nh_core_WorkloadCommand *Command_p)
{
    nh_ecmascript_AgentCluster *Cluster_p = args_p;
    nh_ecmascript_Job *Job_p = Command_p->p;

    switch (Command_p->type) {
        case 0 : nh_core_enqueue(&Job_p->Agent_p->HostCommands, Command_p->p); break;
        case 1 : nh_core_enqueue(&Job_p->Agent_p->Jobs, Command_p->p); break;
    }

    return NH_SIGNAL_OK;
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
        nh_ecmascript_enqueueHostCommandOrJob,
        &Args,
        false
    );

    return NH_API_SUCCESS;
}

static void *nh_ecmascript_enqueueHostCommand(
    nh_ecmascript_Agent *Agent_p, int type, void *args_p, size_t size)
{
    nh_ecmascript_HostCommand *HostCommand_p = (nh_ecmascript_HostCommand*)nh_core_allocate(sizeof(nh_ecmascript_HostCommand));
    memset(HostCommand_p, 0, sizeof(nh_ecmascript_HostCommand));
    HostCommand_p->type = type;
    HostCommand_p->Agent_p = Agent_p;
    HostCommand_p->args_p = args_p;
    HostCommand_p->size = size;
    nh_core_executeWorkloadCommand(Agent_p->Cluster_p, 0, HostCommand_p, 0); 
    nh_core_Workload *Workload_p = nh_core_getWorkload(Agent_p->Cluster_p);
    while (HostCommand_p->done == false) {
        nh_core_runOrSleep(Workload_p);
    }
    void *result_p = HostCommand_p->result_p;
    nh_core_free(HostCommand_p);
    return result_p;
}

//static void nh_ecmascript_enqueueJob(
//    nh_ecmascript_Agent *Agent_p, NH_ECMASCRIPT_JOB_E type, void *p, size_t size)
//{
//    nh_ecmascript_Job *Job_p = (nh_ecmascript_Job*)nh_core_allocate(sizeof(nh_ecmascript_Job));
//    Job_p->type = type;
//    Job_p->Agent_p = Agent_p;
//    nh_core_executeWorkloadCommand(Agent_p->Cluster_p, 1, Job_p, sizeof(Job_p)); 
//}

nh_api_Realm *nh_ecmascript_enqueueInitializeRealm(
    nh_api_Agent *Agent_p)
{
    return nh_ecmascript_enqueueHostCommand(Agent_p, 0, NULL, 0);
}

NH_API_RESULT nh_ecmascript_enqueueInstallHostGlobals(
    nh_ecmascript_Realm *Realm_p, void *Globals_p)
{
    nh_ecmascript_InstallHostGlobalsArgs Args;
    Args.Realm_p = Realm_p;
    Args.Globals_p = Globals_p;
    return nh_ecmascript_enqueueHostCommand(Realm_p->Agent_p, 1, NULL, 0);
}

nh_api_Script *nh_ecmascript_enqueueParseScript(
    char *src_p, nh_ecmascript_Realm *Realm_p, int encoding)
{
    nh_ecmascript_ParseScriptArgs Args;
    Args.src_p = src_p;
    Args.Realm_p = Realm_p;
    return nh_ecmascript_enqueueHostCommand(Realm_p->Agent_p, 2, &Args, sizeof(nh_ecmascript_ParseScriptArgs));
}

NH_API_RESULT nh_ecmascript_enqueueEvaluateScript(
    nh_ecmascript_Script *Script_p)
{
    nh_ecmascript_enqueueHostCommand(Script_p->Realm_p->Agent_p, 3, Script_p, 0);
    return NH_API_SUCCESS;
}
