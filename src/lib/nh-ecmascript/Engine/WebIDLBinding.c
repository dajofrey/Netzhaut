// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Realm.h"

#include NH_DEFAULT_CHECK
#include NH_FLOW
#include NH_JS_UTILS

#include <string.h>

// WEB BINDING ==================================================================================

// https://heycam.github.io/webidl/#interface-object
nh_ecmascript_Object *nh_ecmascript_createInterfaceObject(
    nh_webidl_Interface *Interface_p, nh_ecmascript_Realm *Realm_p)
{
    // step 1
    // TODO

    // step 2
    nh_ecmascript_Object *ConstructorProto_p = &Realm_p->Intrinsics.FunctionPrototype;

    return;
}


