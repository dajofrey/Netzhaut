// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Realm.h"
#include "ExecutionContext.h"
#include "BuiltinFunctionObject.h"
#include "OrdinaryObject.h"
#include "GlobalObject.h"

#include "../Intrinsics/Object.h"
#include "../Intrinsics/Function.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>

// FUNCTIONS ======================================================================================

// corresponds to https://tc39.es/ecma262/#sec-createintrinsics
static NH_API_RESULT nh_ecmascript_createIntrinsics(
    nh_ecmascript_Realm *Realm_p)
{
    NH_CORE_CHECK(nh_ecmascript_createIntrinsicObject(&Realm_p->Intrinsics.Object, Realm_p))
//    NH_CORE_CHECK(nh_ecmascript_createIntrinsicFunction(&Realm_p->Intrinsics.Function, Realm_p))

    return NH_API_SUCCESS;
}

static void nh_ecmascript_freeIntrinsics(
    nh_ecmascript_Intrinsics Intrinsics)
{
    return;
}

// corresponds to https://www.262.ecma-international.org/11.0/index.html#sec-initializehostdefinedrealm
// with no modifications to the default global object
nh_ecmascript_Realm *nh_ecmascript_initializeRealm(
    nh_ecmascript_Agent *Agent_p)
{
    nh_ecmascript_Realm *Realm_p = (nh_ecmascript_Realm*)nh_core_allocate(sizeof(nh_ecmascript_Realm));
    NH_CORE_CHECK_MEM_2(NULL, Realm_p)

    NH_CORE_CHECK_2(NULL, nh_ecmascript_createIntrinsics(Realm_p))

    Realm_p->Agent_p = Agent_p; // aka agent signifier in the spec
    Realm_p->GlobalObject_p = NULL;
    Realm_p->GlobalEnvironment_p = NULL;

    nh_ecmascript_ExecutionContext *NewContext_p = nh_ecmascript_allocateExecutionContext();
    NH_CORE_CHECK_MEM_2(NULL, NewContext_p)

    NewContext_p->Function_p = NULL;
    NewContext_p->Realm_p    = Realm_p;
    NewContext_p->ScriptOrModule.isScript = -1;
    NewContext_p->ScriptOrModule.handle_p = NULL;

    nh_core_pushStack(&Agent_p->ExecutionContextStack, NewContext_p);

    nh_ecmascript_Object *GlobalObject_p = nh_ecmascript_ordinaryObjectCreate(&Realm_p->Intrinsics.Object.Prototype, NULL, -1);
    NH_CORE_CHECK_MEM_2(NULL, GlobalObject_p)

    Realm_p->GlobalObject_p = GlobalObject_p;
    Realm_p->GlobalEnvironment_p = nh_ecmascript_newGlobalEnvironment(GlobalObject_p, GlobalObject_p);
    NH_CORE_CHECK_MEM_2(NULL, Realm_p->GlobalEnvironment_p)

    nh_ecmascript_Object *Global_p = nh_ecmascript_setDefaultGlobalBindings(Realm_p);

    return Realm_p;
}
