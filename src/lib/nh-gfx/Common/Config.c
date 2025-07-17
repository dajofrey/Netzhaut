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

// DATA ============================================================================================

static const char *NAMES_PP[] = {
    "api",
    "viewport.width",
    "viewport.height",
    "viewport.x",
    "viewport.y",
};

static size_t NAMES_PP_COUNT = sizeof(NAMES_PP) / sizeof(NAMES_PP[0]);

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_gfx_getSetting(
    nh_gfx_Config *Config_p, char *namespace_p, int index)
{
    nh_core_List *Setting_p = nh_core_getGlobalConfigSetting(NULL, NH_MODULE_GFX, NAMES_PP[index]);
    NH_CORE_CHECK_NULL(Setting_p)

    switch (index) {
        case 0 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            if (!strcmp("vulkan", Setting_p->pp[0])) {
                Config_p->api = NH_GFX_API_VULKAN;
                break;
            }
            if (!strcmp("opengl", Setting_p->pp[0])) {
                Config_p->api = NH_GFX_API_OPENGL;
                break;
            }
            if (!strcmp("metal", Setting_p->pp[0])) {
                Config_p->api = NH_GFX_API_METAL;
                break;
            }
            if (!strcmp("directx", Setting_p->pp[0])) {
                Config_p->api = NH_GFX_API_DIRECTX;
                break;
            }
            return NH_API_ERROR_BAD_STATE;
        case 1 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->ViewportSize.width = atoi(Setting_p->pp[0]);
            break;
        case 2 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->ViewportSize.height = atoi(Setting_p->pp[0]);
            break;
        case 3 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->ViewportPosition.x = atoi(Setting_p->pp[0]);
            break;
        case 4 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->ViewportPosition.y = atoi(Setting_p->pp[0]);
            break;
    }

    return NH_API_SUCCESS;
}

nh_gfx_Config nh_gfx_getConfig()
{
    nh_gfx_Config Config;
    memset(&Config, 0, sizeof(nh_gfx_Config));

    for (int i = 0; i < NAMES_PP_COUNT; ++i) {
        nh_gfx_getSetting(&Config, NULL, i);
    }

    return Config;
}
