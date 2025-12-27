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

#include "../Intrinsics/Parser.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// FUNCTIONS ======================================================================================

static NH_API_RESULT nh_ecmascript_allocateIntrinsicObjects(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    for (int i = 0; i < Templates_p->size; ++i) {
        nh_ecmascript_IntrinsicTemplate *Template_p = Templates_p->pp[i];
//        nh_ecmascript_Object *Object_p = nh_ecmascript_allocateObject();
//        Object_p->type = Template_p->type;
//        Object_p->isCallable = Template_p->isCallable;
//        Object_p->isConstructor = Template_p->isConstructor;
//        Object_p->isExotic = Template_p->isExotic;
//        Object_p->Prototype_p = NULL;
//
//        Realm_p->Intrinsics_p
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_ecmascript_linkIntrinsicPrototypes(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    for (int i = 0; i < Templates_p->size; ++i) {
        nh_ecmascript_IntrinsicTemplate *Template_p = Templates_p->pp[i];
//        nh_ecmascript_Object *Object_p = Realm_p->Intrinsics_p[Template_p->name_p];
//        if (Template_p->prototype_p != NULL) {
//            Object_p->Prototype_p = Realm_p->Intrinsics_p[Template_p->prototype_p];
//        }
    }
    return NH_API_SUCCESS;
}

// corresponds to https://tc39.es/ecma262/#sec-createintrinsics
static NH_API_RESULT nh_ecmascript_createIntrinsics(
    nh_core_List *Templates_p, nh_ecmascript_Realm *Realm_p)
{
    NH_CORE_CHECK(nh_ecmascript_allocateIntrinsicObjects(Templates_p, Realm_p))
    NH_CORE_CHECK(nh_ecmascript_linkIntrinsicPrototypes(Templates_p, Realm_p))

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

    NH_CORE_CHECK_2(NULL, nh_ecmascript_createIntrinsics(&Agent_p->Cluster_p->Runtime_p->IntrinsicTemplates, Realm_p))

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

//    nh_ecmascript_Object *GlobalObject_p = nh_ecmascript_ordinaryObjectCreate(&Realm_p->Intrinsics.Object.Prototype, NULL, -1);
//    NH_CORE_CHECK_MEM_2(NULL, GlobalObject_p)
//
//    Realm_p->GlobalObject_p = GlobalObject_p;
//    Realm_p->GlobalEnvironment_p = nh_ecmascript_newGlobalEnvironment(GlobalObject_p, GlobalObject_p);
//    NH_CORE_CHECK_MEM_2(NULL, Realm_p->GlobalEnvironment_p)
//
//    nh_ecmascript_Object *Global_p = nh_ecmascript_setDefaultGlobalBindings(Realm_p);

    return Realm_p;
}
