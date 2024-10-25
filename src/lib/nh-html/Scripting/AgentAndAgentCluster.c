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

https://html.spec.whatwg.org/multipage/webappapis.html#creating-a-new-javascript-realm
nh_ecmascript_ExecutionContext *nh_html_createJavaScriptRealm(
    nh_ecmascript_Agent *Agent_p, nh_ecmascript_Object *GlobalObject_p, nh_ecmascript_Object *ThisValue_p)
{
    nh_ecmascript_initializeHostDefinedRealm(Agent_p, GlobalObject_p, ThisValue_p);
    return;
}

