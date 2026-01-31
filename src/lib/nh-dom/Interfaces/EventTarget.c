// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "EventTarget.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define EVENT_LISTENERS EventTarget_p->internal_p

// INITIALIZE ======================================================================================

NH_API_RESULT nh_dom_initializeEventTarget(
    nh_ecmascript_Object *EventTarget_p)
{
    EventTarget_p->internal_p = nh_core_allocate(sizeof(nh_core_List));
    NH_CORE_CHECK_MEM(EventTarget_p->internal_p)

    *((nh_core_List*)EVENT_LISTENERS) = nh_core_initList(16);

    return NH_API_SUCCESS;
}
