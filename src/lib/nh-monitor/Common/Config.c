// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Config.h"

#include "../../nh-core/Config/Config.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_monitor_getSetting(
    nh_monitor_Config *Config_p, char *namespace_p, int option, const char *option_p)
{
    nh_core_List *Setting_p = nh_core_getGlobalConfigSetting(NULL, NH_MODULE_MONITOR, option_p);
    NH_CORE_CHECK_NULL(Setting_p)

    switch (option) {
        case 0 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->client_port = atoi(Setting_p->pp[0]);
            break;
        case 1 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->server_port = atoi(Setting_p->pp[0]);
            break;
        case 2 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->block = atoi(Setting_p->pp[0]);
            break;
        case 3 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->reset = atoi(Setting_p->pp[0]);
            break;
    }

    return NH_API_SUCCESS;
}

nh_monitor_Config nh_monitor_getConfig()
{
    nh_monitor_Config Config;
    memset(&Config, 0, sizeof(nh_monitor_Config));

    static const char *optionNames_pp[] = {
        "client_port",
        "server_port",
        "block",
        "reset",
    };
    static size_t options = sizeof(optionNames_pp) / sizeof(optionNames_pp[0]);

    for (int i = 0; i < options; ++i) {
        nh_monitor_getSetting(&Config, NULL, i, optionNames_pp[i]);
    }

    return Config;
}
