// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "EventTarget.h"

#include "../Common/Macros.h"

#include "../../nhcore/Util/List.h"
#include "../../nhcore/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define EVENT_LISTENERS EventTarget_p->internal_p

// INITIALIZE ======================================================================================

NH_DOM_RESULT nh_dom_initializeEventTarget(
    nh_webidl_Object *EventTarget_p)
{
NH_DOM_BEGIN()

    EventTarget_p->internal_p = nh_core_allocate(sizeof(nh_List));
    NH_DOM_CHECK_MEM(EventTarget_p->internal_p)

    *((nh_List*)EVENT_LISTENERS) = nh_core_initList(16);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

