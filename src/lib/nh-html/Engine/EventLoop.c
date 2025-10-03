// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "EventLoop.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_html_startEventLoop(
    nh_ecmascript_Agent *Agent_p)
{
    nh_core_activateWorkload(
        nh_html_initEventLoop,
        nh_html_runEventLoop,
        nh_html_freeEventLoop,
        nh_html_enqueueTask,
        Agent_p,
        false
    );
}
