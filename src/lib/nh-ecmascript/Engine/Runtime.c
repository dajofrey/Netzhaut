// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Runtime.h"
#include "Agent.h"
#include "ExecutionContext.h"
#include "../Parser/Script.h"
#include "../Intrinsics/Parser.h"

#include "../Common/IndexMap.h"
#include "../Common/Log.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/System/Thread.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

static void *nh_ecmascript_initRuntime(
    nh_core_Workload *Workload_p)
{
    static char *name_p = "ECMAScript Runtime Workload";
    static char *path_p = "nh-ecmascript/Engine/Runtime.c";

    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_ECMASCRIPT;

    nh_ecmascript_Runtime *Runtime_p = (nh_ecmascript_Runtime*)nh_core_allocate(sizeof(nh_ecmascript_Runtime));
    Runtime_p->AgentClusters = nh_core_initList(32);
    Runtime_p->IntrinsicTemplates = nh_ecmascript_parseIntrinsicTemplates();
    if (Runtime_p->IntrinsicTemplates.size == 0) {return NULL;}

    return Runtime_p;
}

static NH_SIGNAL nh_ecmascript_runRuntime(
    void *args_p)
{
    nh_ecmascript_Runtime *Runtime_p = args_p;

    return NH_SIGNAL_IDLE;
}

static bool nh_ecmascript_findAgent(
    nh_ecmascript_Runtime *Runtime_p, nh_core_WorkloadCommand *Command_p)
{
    bool ok = false;
    for (int i = 0; i < Runtime_p->AgentClusters.size && !ok; ++i) {
        nh_ecmascript_AgentCluster *AgentCluster_p = Runtime_p->AgentClusters.pp[i];
        for (int j = 0; j < AgentCluster_p->Agents.size && !ok; ++j) {
            nh_ecmascript_Agent *Agent_p = AgentCluster_p->Agents.pp[j];
            if (Agent_p->inUse == false) {
                Agent_p->inUse = true;
                Agent_p->Cluster_p = AgentCluster_p;
                Command_p->result_p = Agent_p;
                ok = true;
            }
        }
    }
    return ok;
}

static NH_SIGNAL nh_ecmascript_runRuntimeCommand(
    void *args_p, nh_core_WorkloadCommand *Command_p)
{
    nh_ecmascript_Runtime *Runtime_p = args_p;

    switch (Command_p->type) {
        case 0 :
        {
            if (!nh_ecmascript_findAgent(Runtime_p, Command_p)) {
                // create agent cluster + agent
                nh_ecmascript_AgentCluster *AgentCluster_p = (nh_ecmascript_AgentCluster*)nh_core_allocate(sizeof(nh_ecmascript_AgentCluster));
                nh_core_appendToList(&Runtime_p->AgentClusters, AgentCluster_p);
                nh_ecmascript_startAgentCluster(Runtime_p, AgentCluster_p);
                // try again
                nh_ecmascript_findAgent(Runtime_p, Command_p);
            }
        }
        default :
            return NH_SIGNAL_ERROR;
    }

    return NH_SIGNAL_OK;
}
 
static void nh_ecmascript_freeRuntime(
    void *args_p)
{
    nh_ecmascript_Runtime *Runtime_p = args_p;

    // TODO

    nh_core_free(Runtime_p);
}

static void nh_ecmascript_enqueueRuntimeCommand(
    nh_api_Runtime *Runtime_p, int type, void *p, size_t size)
{
    nh_core_executeWorkloadCommand(Runtime_p, type, p, size);
}

nh_api_Runtime *nh_ecmascript_startRuntime(
    void *args_p)
{
    return nh_core_activateWorkload(
        nh_ecmascript_initRuntime,
        nh_ecmascript_runRuntime,
        nh_ecmascript_freeRuntime,
        nh_ecmascript_runRuntimeCommand,
        args_p,
        false
    );
}

nh_api_Agent *nh_ecmascript_createAgent(
    nh_api_Runtime *Runtime_p)
{
    return nh_core_executeWorkloadCommand(Runtime_p, 0, NULL, 0);
}
