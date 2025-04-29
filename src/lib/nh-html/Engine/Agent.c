// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "BrowsingContext.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

static nh_html_Agent nh_html_createAgent(
    bool canBlock)
{
    nh_html_Signifier Signifier = nh_html_createSignifier();
    nh_ecmascript_CandidateExecution CandidateExecution = nh_ecmascript_initCandidateExecution();
    nh_ecmascript_Agent Agent;
    Agent.canBlock = canBlock;
    Agent.Signifier = Signifier;
    Agent.CandidateExecution = CandidateExecution;
    Agent.isLockFree1 = true;
    Agent.isLockFree2 = true;
    Agent.littleEndian = true;
    Agent.EventLoop = nh_html_createEventLoop();
    return Agent;
}

NH_API_RESULT nh_html_obtainSimilarOriginWindowAgent(
    nh_html_Origin Origin, nh_html_BrowsingContextGroup *Group_p, bool requestsOAC,
    nh_ecmascript_Agent **Agent_pp)
{
    nh_html_Site Site = nh_html_obtainSite(Origin);
    nh_html_Site Key = Site;
    if (Group.crossOriginIsolationMode == "none") {
        Key = Origin;
    } else if (Group.historicalAgentClusterKeyMap[Origin] exists) {
        Key = Group.historicalAgentClusterKeyMap[Origin];
    } else {
        if (requestsOAC) {
            Key = Origin;
            Group.historicalAgentClusterKeyMap[Origin] = Key;
        }
    }
    if (Group.agentClusterKeymap[key] doesnt exist) {
        nh_ecmascript_AgentCluster AgentCluster_p = nh_ecmascript_createAgentCluster();
        AgentCluster_p->crossOriginIsolationMode = Group_p->crossOriginIsolationMode;
        if (nh_html_isOrigin(Key)) {
            AgentCluster_p->isOriginKeyed = true;
        }
        nh_html_Agent *Agent_p = nh_html_createAgent(false);
        nh_core_addToList(&AgentCluster_p->Agents, Agent_p);
        Group_p->AgentClusterMap[key] = AgentCluster;
    }
    *Agent_pp = Group.AgentClusterMap[key].SimilarOriginWindowAgent;
    return NH_API_SUCCESS;
}
