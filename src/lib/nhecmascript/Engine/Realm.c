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

#include "../Intrinsics/GlobalObject.h"
#include "../Intrinsics/Object.h"
#include "../Intrinsics/Function.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"

#include <string.h>

// INTRINSICS ======================================================================================

// corresponds to https://tc39.es/ecma262/#sec-createintrinsics
static NH_ECMASCRIPT_RESULT nh_ecmascript_createIntrinsics(
    nh_ecmascript_Realm *Realm_p)
{
NH_ECMASCRIPT_BEGIN()

    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicObject(&Realm_p->Intrinsics.Object, Realm_p))
//    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIntrinsicFunction(&Realm_p->Intrinsics.Function, Realm_p))

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

static void nh_ecmascript_freeIntrinsics(
    nh_ecmascript_Intrinsics Intrinsics)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_SILENT_END()
}

// REALM ===========================================================================================

// corresponds to https://www.262.ecma-international.org/11.0/index.html#sec-createrealm
static nh_ecmascript_Realm *nh_ecmascript_createRealm()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Realm *Realm_p = nh_core_allocate(sizeof(nh_ecmascript_Realm));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, Realm_p)

    NH_ECMASCRIPT_CHECK_2(NULL, nh_ecmascript_createIntrinsics(Realm_p))

    Realm_p->GlobalObject_p = NULL;
    Realm_p->GlobalEnvironment_p = NULL;

NH_ECMASCRIPT_END(Realm_p)
}

// corresponds to https://www.262.ecma-international.org/11.0/index.html#sec-setrealmglobalobject
static nh_ecmascript_Realm *nh_ecmascript_setRealmGlobalObject(
    nh_ecmascript_Realm *Realm_p, nh_ecmascript_Object *GlobalObject_p, nh_ecmascript_Object *ThisValue_p)
{
NH_ECMASCRIPT_BEGIN()

    if (GlobalObject_p == NULL) {
        nh_ecmascript_Intrinsics *Intrinsics_p = &Realm_p->Intrinsics;
        GlobalObject_p = nh_ecmascript_ordinaryObjectCreate(&Intrinsics_p->Object.Prototype, NULL, -1);
        NH_ECMASCRIPT_CHECK_MEM_2(NULL, GlobalObject_p)
    }

    if (ThisValue_p == NULL) {ThisValue_p = GlobalObject_p;}

    Realm_p->GlobalObject_p = GlobalObject_p;
    Realm_p->GlobalEnvironment_p = nh_ecmascript_newGlobalEnvironment(GlobalObject_p, ThisValue_p);
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, Realm_p->GlobalEnvironment_p)

NH_ECMASCRIPT_END(Realm_p)
}

// corresponds to https://www.262.ecma-international.org/11.0/index.html#sec-initializehostdefinedrealm
NH_ECMASCRIPT_RESULT nh_ecmascript_initializeHostDefinedRealm(
    nh_ecmascript_Agent *Agent_p, nh_ecmascript_Object *GlobalObject_p, nh_ecmascript_Object *ThisValue_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Realm *Realm_p = nh_ecmascript_createRealm();
    NH_ECMASCRIPT_CHECK_MEM(Realm_p)

    nh_ecmascript_ExecutionContext *NewContext_p = nh_ecmascript_allocateExecutionContext();
    NH_ECMASCRIPT_CHECK_MEM(NewContext_p)

    NewContext_p->Function_p = NULL;
    NewContext_p->Realm_p    = Realm_p;
    NewContext_p->ScriptOrModule.isScript = -1;
    NewContext_p->ScriptOrModule.handle_p = NULL;

    nh_pushStack(&Agent_p->ExecutionContextStack, NewContext_p);

    NH_ECMASCRIPT_CHECK_MEM(nh_ecmascript_setRealmGlobalObject(Realm_p, GlobalObject_p, ThisValue_p))

    nh_ecmascript_Object *Global_p = nh_ecmascript_setDefaultGlobalBindings(Realm_p);

// TODO Create any host-defined global object properties on globalObj.

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

