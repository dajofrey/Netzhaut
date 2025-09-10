// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-ecmascript.h"
#include "nh-core.h"

#include "../nh-core/Loader/Loader.h" 
#include "../nh-ecmascript/Engine/Script.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_api_Runtime *nh_api_startRuntime()
{
    typedef nh_api_Runtime *(*nh_ecmascript_startRuntime_f)(); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_ecmascript_startRuntime_f startRuntime_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_ECMASCRIPT, 0, "nh_ecmascript_startRuntime");
    return startRuntime_f ? startRuntime_f() : NULL;
}

nh_api_Agent *nh_api_createAgent(
    nh_api_Runtime *Runtime_p)
{
    typedef nh_api_Agent *(*nh_ecmascript_createAgent_f)(nh_api_Runtime *Runtime_p); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_ecmascript_createAgent_f createAgent_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_ECMASCRIPT, 0, "nh_ecmascript_createAgent");
    return createAgent_f ? createAgent_f(Runtime_p) : NULL;
}

nh_api_Realm *nh_api_initializeHostDefinedRealm(
    nh_api_Agent *Agent_p)
{
    typedef nh_api_Realm *(*nh_ecmascript_initializeHostDefinedRealm_f)(nh_api_Agent *Agent_p); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_ecmascript_initializeHostDefinedRealm_f initializeHostDefinedRealm_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_ECMASCRIPT, 0, "nh_ecmascript_initializeHostDefinedRealm");
    return initializeHostDefinedRealm_f ? initializeHostDefinedRealm_f(Agent_p) : NULL;
}

nh_api_Script *nh_api_parseScript(
    char *sourceText_p, nh_api_Realm *Realm_p, int encoding)
{
    typedef nh_api_Script *(*nh_ecmascript_parseScript_f)(char *sourceText_p, nh_api_Realm *Realm_p, int encoding); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_ecmascript_parseScript_f parseScript_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_ECMASCRIPT, 0, "nh_ecmascript_parseScript");
    return parseScript_f ? parseScript_f(sourceText_p, Realm_p, encoding) : NULL;
}

NH_API_RESULT nh_api_evaluateScript(
    nh_api_Script *Script_p)
{
    typedef NH_API_RESULT (*nh_ecmascript_evaluateScript_f)(nh_api_Script *Script_p); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_ecmascript_evaluateScript_f evaluateScript_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_ECMASCRIPT, 0, "nh_ecmascript_evaluateScript");
    return evaluateScript_f ? evaluateScript_f(Script_p) : NH_API_ERROR_BAD_STATE;
}
