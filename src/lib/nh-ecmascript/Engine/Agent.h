#ifndef NH_ECMASCRIPT_ENGINE_AGENT_H
#define NH_ECMASCRIPT_ENGINE_AGENT_H

// INCLUDES ========================================================================================

#include "Runtime.h"

#include "../../nh-core/Util/Stack.h"
#include "../../nh-core/Util/Queue.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/Array.h"

// STRUCTS =========================================================================================

typedef struct nh_ecmascript_AgentCluster {
    nh_ecmascript_Runtime *Runtime_p;
    nh_core_List Agents;
} nh_ecmascript_AgentCluster;

typedef struct nh_ecmascript_Agent {
    nh_Stack ExecutionContextStack;
    nh_core_Queue Jobs;
    nh_core_Queue HostCommands;
    nh_ecmascript_AgentCluster *Cluster_p;
    bool littleEndian;
    bool canBlock;
    int signifier;
    bool isLockFree1;
    bool isLockFree2;
    bool isLockFree8;
//        nh_ecmascript_CandidateExecution CandidateExecution;
    nh_core_List KeptAlive;
    bool inUse;
} nh_ecmascript_Agent;

typedef struct nh_ecmascript_Job {
    nh_ecmascript_Agent *Agent_p;
    int type;
    bool done;
    void *result_p;
    void *args_p;
    size_t size;
} nh_ecmascript_Job;

typedef struct nh_ecmascript_Job nh_ecmascript_HostCommand;
typedef struct nh_ecmascript_Realm nh_ecmascript_Realm;
typedef struct nh_ecmascript_Script nh_ecmascript_Script;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_ecmascript_startAgentCluster(
    void *Runtime_p, nh_ecmascript_AgentCluster *AgentCluster_p 
);

nh_api_Realm *nh_ecmascript_enqueueInitializeRealm(
    nh_api_Agent *Agent_p
);

NH_API_RESULT nh_ecmascript_enqueueInstallHostGlobals(
    nh_ecmascript_Realm *Realm_p, void *Globals_p
);

nh_api_Script *nh_ecmascript_enqueueParseScript(
    char *src_p, nh_ecmascript_Realm *Realm_p, int encoding
);

NH_API_RESULT nh_ecmascript_enqueueEvaluateScript(
    nh_ecmascript_Script *Script_p
);

#endif
