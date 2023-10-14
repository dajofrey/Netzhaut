// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Object.h"
#include "Runtime.h"
#include "Type.h"
#include "Definitions.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DECLARE =========================================================================================

static nh_webidl_Object *nh_webidl_createObjectFromInterfaceWithParent(
    nh_webidl_Interface *Interface_p, nh_webidl_Object *Parent_p
);

typedef int (*init_f)(nh_webidl_Object *Object_p);

// CREATE OBJECT ===================================================================================

// https://heycam.github.io/webidl/#internally-create-a-new-object-implementing-the-interface
static NH_ECMAScript_Object *nh_webidl_createObjectImplementingInterfaceInternally(
    nh_webidl_Interface *Interface_p, nh_ecmascript_Realm *Realm_p, void *newTarget_p)
{
NH_WEBIDL_BEGIN()
NH_WEBIDL_END(NULL)
}

// https://heycam.github.io/webidl/#new
NH_ECMAScript_Object *nh_webidl_createObjectImplementingInterface(
    nh_webidl_Interface *Interface_p, nh_ecmascript_Realm *Realm_p)
{
NH_WEBIDL_BEGIN()
NH_WEBIDL_END(NULL)
}

