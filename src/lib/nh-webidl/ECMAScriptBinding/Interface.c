// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Interface.h"

#include "../../nh-core/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE OBJECT ===================================================================================

// https://heycam.github.io/webidl/#new
static NH_API_RESULT nh_webidl_createInterfaceObject(
    nh_webidl_Interface *Interface_p, nh_ecmascript_Realm *Realm_p)
{
    nh_ecmascript_Object *Object_p = &Realm_p->Intrinsics.Function.Prototype;
    return NH_API_SUCCESS;
}

// https://heycam.github.io/webidl/#new
NH_API_RESULT nh_webidl_createInterfaceObjects(
    nh_ecmascript_Realm *Realm_p)
{
    for (int spec = 0; spec < NH_WEBIDL_RUNTIME.Specifications.length; ++spec) 
    {
        nh_webidl_Specification *Specification_p = &((nh_webidl_Specification*)NH_WEBIDL_RUNTIME.Specifications.p)[spec];

        for (int frag = 0; frag < Specification_p->Fragments.length; ++frag) 
        {
            nh_webidl_Fragment *Fragment_p = &((nh_webidl_Fragment*)Specification_p->Fragments.p)[frag];

            for (int intf = 0; intf < Fragment_p->Interfaces.length; ++intf) {
                NH_CORE_CHECK_2(nh_webidl_createInterfaceObject(&((nh_webidl_Interface*)Fragment_p->Interfaces.p)[intf], Realm_p))
            }
        }
    }

    return NH_API_SUCCESS;
}

