// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Config.h"
#include "Macros.h"

#include "../Config/Config.h"

#include <string.h>
#include <stdlib.h>

// DATA ============================================================================================

static nh_core_Config NH_CORE_CONFIG;

// FUNCTIONS =======================================================================================

const char *NH_CORE_SETTING_NAMES_PP[] = {
    "loader.unload",
};

static NH_API_RESULT nh_core_getConfigSetting(
    nh_core_Config *Config_p, NH_CORE_SETTING_E setting)
{
NH_CORE_BEGIN()

    nh_List *Setting_p = nh_core_getGlobalConfigSetting(
        NULL, NH_MODULE_CORE, NH_CORE_SETTING_NAMES_PP[setting]);

    NH_CORE_CHECK_NULL(Setting_p)

    switch (setting) {
        case NH_CORE_SETTING_LOADER_UNLOAD :
            if (Setting_p->size != 1) {NH_CORE_END(NH_API_ERROR_BAD_STATE)}
            Config_p->loaderUnload = atoi(Setting_p->pp[0]) == 1;
            break;
    }

NH_CORE_END(NH_API_SUCCESS)
}

nh_core_Config nh_core_getConfig()
{
NH_CORE_BEGIN()

    nh_core_Config Config;
    memset(&Config, 0, sizeof(nh_core_Config));

    for (int i = 0; i < NH_CORE_SETTING_E_COUNT; ++i) {
        nh_core_getConfigSetting(&Config, i);
    }

NH_CORE_END(Config)
}

