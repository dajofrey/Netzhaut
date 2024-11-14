// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Debug.h"

#include "../Common/Log.h"
#include "../Common/Config.h"

#include <stdio.h>

// FUNCTIONS =======================================================================================

void nh_core_debug(
    char *message_p)
{
    nh_core_logDebugMessage(message_p);
    if (nh_core_getConfig().debugToConsole) {
        puts(message_p);
    }
}
