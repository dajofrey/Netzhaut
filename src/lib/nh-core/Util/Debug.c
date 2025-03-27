// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Debug.h"
#include "../Common/Config.h"
#include "../Loader/Loader.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

void nh_core_log(
    char *node_p, char *option_p, char *message_p)
{
    if (nh_core_getConfig().dump_on == true) {
        puts(message_p);
    }

    if (nh_core_getConfig().monitor_on == true) {
        typedef void (*nh_monitor_log_f)(char *node_p, char *options_p, char *message_p);
        static nh_monitor_log_f log_f = NULL;
        if (log_f == NULL) {
            log_f = nh_core_loadSymbol(NH_MODULE_MONITOR, 0, "nh_monitor_log");
        }
        log_f(node_p, option_p, message_p);
    }

    return;
}

void nh_core_debug(
    char *message_p)
{
    nh_core_log("nh-core:debug", NULL, message_p);
}
