#ifndef NH_API_NH_ECMASCRIPT_H
#define NH_API_NH_ECMASCRIPT_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-core.h"

// STRUCTS =========================================================================================

typedef struct nh_api_HostGlobals {
    void **pp;
    unsigned int size;
} nh_api_HostGlobals;

typedef struct nh_api_Script nh_api_Script;
typedef struct nh_api_Realm nh_api_Realm;
typedef struct nh_api_Runtime nh_api_Runtime;
typedef struct nh_api_Agent nh_api_Agent;

// FUNCTIONS =======================================================================================

nh_api_Runtime *nh_api_startRuntime(
);

nh_api_Agent *nh_api_createAgent(
    nh_api_Runtime *Runtime_p
);

nh_api_Realm *nh_api_initializeRealm(
    nh_api_Agent *Agent_p 
);

nh_api_Realm *nh_api_installHostGlobals(
    nh_api_Realm *Realm_p, nh_api_HostGlobals HostGlobals 
);

nh_api_Script *nh_api_parseScript(
    char *sourceText_p, nh_api_Realm *Realm_p, int encoding
);

NH_API_RESULT nh_api_evaluateScript(
    nh_api_Script *Script_p
);

#endif // NH_API_NH_ECMASCRIPT_H
