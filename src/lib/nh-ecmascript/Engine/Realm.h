#ifndef NH_ECMASCRIPT_ENGINE_REALM_H
#define NH_ECMASCRIPT_ENGINE_REALM_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Agent.h"
#include "ObjectType.h"
#include "Environment.h"

#include "../Common/Includes.h"

#include "../../nh-core/Util/HashMap.h"
#include "../../nh-core/Util/Stack.h"

// STRUCTS ================================================================================

typedef struct nh_ecmascript_RealmHostDefined {
    char *temporary_p;
} nh_ecmascript_RealmHostDefined;

// https://tc39.es/ecma262/#sec-code-realms
typedef struct nh_ecmascript_Realm {
    nh_ecmascript_Agent *Agent_p;
    nh_core_HashMap Intrinsics;
    nh_ecmascript_Object *GlobalObject_p;
    nh_ecmascript_Environment *GlobalEnvironment_p;
    nh_ecmascript_RealmHostDefined HostDefined;
} nh_ecmascript_Realm;

// FUNCTIONS ==============================================================

nh_ecmascript_Realm *nh_ecmascript_initializeRealm(
    nh_ecmascript_Agent *Agent_p
);

#endif
