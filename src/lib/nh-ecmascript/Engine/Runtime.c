// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Runtime.h"
#include "Agent.h"
#include "Script.h"
#include "ExecutionContext.h"

#include "../Common/IndexMap.h"
#include "../Common/Log.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/System/Thread.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// STRUCTS =========================================================================================

typedef struct nh_ecmascript_Runtime {
    nh_core_List AgentClusters;
} nh_ecmascript_Runtime;

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

    return Runtime_p;
}

static NH_SIGNAL nh_ecmascript_runRuntime(
    void *args_p)
{
    nh_ecmascript_Runtime *Runtime_p = args_p;

    return NH_SIGNAL_IDLE;
}

static NH_SIGNAL nh_ecmascript_runRuntimeCommand(
    void *args_p, nh_core_WorkloadCommand *Command_p)
{
    nh_ecmascript_Runtime *Runtime_p = args_p;

    switch (Command_p->type) {
        case NH_ECMASCRIPT_RUNTIME_COMMAND_START_AGENT_CLUSTER :
        {
            nh_ecmascript_AgentCluster *AgentCluster_p = (nh_ecmascript_AgentCluster*)nh_core_allocate(sizeof(nh_ecmascript_AgentCluster));
            nh_core_appendToList(&Runtime_p->AgentClusters, AgentCluster_p);
            nh_ecmascript_startAgentCluster(Runtime_p, AgentCluster_p);
        }
        case NH_ECMASCRIPT_RUNTIME_COMMAND_GLOBAL_DECLARATION_INSTANTIATION :
            // TODO
            break;
        case NH_ECMASCRIPT_RUNTIME_COMMAND_PARSE_SCRIPT :
            // TODO
            break;
        case NH_ECMASCRIPT_RUNTIME_COMMAND_SCRIPT_EVALUATION :
            // TODO
            break;
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

void nh_ecmascript_startRuntime(
    void *args_p)
{
    nh_core_activateWorkload(
        nh_ecmascript_initRuntime,
        nh_ecmascript_runRuntime,
        nh_ecmascript_freeRuntime,
        nh_ecmascript_runRuntimeCommand,
        args_p,
        false
    );
}

void nh_ecmascript_enqueueRuntimeCommand(
    )
{
//    nh_core_executeWorkloadCommand(Workloads_p[j].args_p, NH_CORE_CONFIG_COMMAND, Setting_p, 0);
}
