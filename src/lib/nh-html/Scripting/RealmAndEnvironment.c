// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "RealmsAndEnvironments.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DATA ============================================================================================

// https://html.spec.whatwg.org/multipage/webappapis.html#creating-a-new-javascript-realm
nh_ecmascript_ExecutionContext *nh_html_createJavaScriptRealm(
    nh_ecmascript_Agent *Agent_p, nh_ecmascript_Object *GlobalObject_p, nh_ecmascript_Object *ThisValue_p)
{
    nh_ecmascript_initializeHostDefinedRealm(Agent_p, GlobalObject_p, ThisValue_p);

    nh_ecmascript_ExecutionContext *ExecutionContext_p = Agent_p->ExecutionContextStack.pp[0];
    nh_core_popStack(&Agent_p->ExecutionContextStack, false);

    nh_ecmascript_Realm *Realm_p = ExecutionContext_p->Realm_p;

    nh_webidl_createInterfaceObjects(Realm_p);

    return ExecutionContext_p;
}

