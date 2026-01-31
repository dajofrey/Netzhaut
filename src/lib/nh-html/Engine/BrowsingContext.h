#ifndef NH_HTML_BROWSING_CONTEXTS_H
#define NH_HTML_BROWSING_CONTEXTS_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

// ENUMS ===========================================================================================

typedef enum NH_HTML_CROSS_ORIGIN_ISOLATION_MODE {
    NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_NONE,
    NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_LOGICAL,
    NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_CONCRETE,
} NH_HTML_CROSS_ORIGIN_ISOLATION_MODE;

typedef enum NH_HTML_AGENT_CLUSTER_KEY {
    NH_HTML_AGENT_CLUSTER_KEY_SITE,
    NH_HTML_AGENT_CLUSTER_KEY_TUPLE_ORIGIN,
} NH_HTML_AGENT_CLUSTER_KEY;

// STRUCTS =========================================================================================

typedef struct nh_html_BrowsingContext {
    nh_ecmascript_Object *WindowProxy_p;
    struct nh_html_BrowsingContext *OpenerBrowsingContext_p;
    bool disowned;
    bool isClosing;
    nh_core_List SessionHistory;
    int virtualBrowsingContextGroupID;
    nh_url *InitialURL_p;
    nh_html_Origin *OpenerOriginAtCreation_p;
    nh_html_Origin *CreatorOrigin_p;
    nh_url *CreatorURL_p;
    nh_url *CreatorBaseURL_p;
} nh_html_BrowsingContext;

typedef struct nh_html_OpaqueOrigin {
    NH_HTML_SITE type;
    nh_html_OpaqueOrigin OpaqueOrigin;
    nh_html_SchemeAndHost SchemeAndHost;
} nh_html_Site;

typedef struct nh_html_SchemeAndHost {
    nh_ASCIIString Scheme;
    nh_url_Host Host;
} nh_html_SchemeAndHost;

typedef struct nh_html_Site {
    void *opaqueOrigin_p;
    nh_html_SchemeAndHost SchemeAndHost;
} nh_html_Site;

typedef struct nh_html_TupleOrigin {
    nh_ASCIIString Scheme;
    nh_url_Host Host;
    NH_URL_PORT port;
    nh_ASCIIString Domain;
} nh_html_TupleOrigin;

typedef struct nh_html_Origin {
    void *opaqueOrigin_p;
    nh_html_TupleOrigin TupleOrigin;
} nh_html_Origin;

typedef struct nh_html_AgentClusterKey {
    NH_HTML_AGENT_CLUSTER_KEY type;
    nh_html_Site Site;
    nh_html_TupleOrigin TupleOrigin;
} nh_html_AgentClusterKey;

typedef struct nh_html_AgentClusterMapEntry {
    nh_html_AgentClusterKey Key;
    nh_ecmascript_AgentCluster *AgentCluster_p;
} nh_html_AgentClusterMapEntry;

typedef struct nh_html_HistoricalAgentClusterKeyMapEntry {
    nh_html_Origin Origin;
    nh_html_AgentClusterKey Key;
} nh_html_HistoricalAgentClusterKeyMapEntry;

typedef struct nh_html_BrowsingContextGroup {
    nh_core_List BrowsingContexts;
    nh_core_List AgentClusterMap;
    nh_core_List HistoricalAgentClusterKeyMap;
    NH_HTML_CROSS_ORIGIN_ISOLATION_MODE crossOriginIsolationMode;
} nh_html_BrowsingContextGroup;

#endif
