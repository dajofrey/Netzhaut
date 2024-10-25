// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Config.h"

#include "../Window/Window.h"

#include "../../nh-core/Config/Config.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdlib.h>

// NAMES ===========================================================================================

const char *NH_WSI_SETTING_NAMES_PP[] = {
    "window.title",
    "window.width",
    "window.height",
    "window.x",
    "window.y",
    "window.decorated",
    "window.resizable",
    "window.action",
    "window.state",
    "window.type",
};

size_t NH_WSI_SETTING_NAMES_PP_COUNT = 
    sizeof(NH_WSI_SETTING_NAMES_PP) / sizeof(NH_WSI_SETTING_NAMES_PP[0]);

const char *nh_wsi_getSettingName(
    NH_WSI_SETTING_E setting)
{
    return NH_WSI_SETTING_NAMES_PP[setting];
}

// CONFIG ==========================================================================================

static NH_API_RESULT nh_wsi_getWindowSetting(
    nh_wsi_WindowConfig *Config_p, char *namespace_p, int index)
{
    nh_core_List *Setting_p = nh_core_getGlobalConfigSetting(strlen(namespace_p) == 0 ? NULL : namespace_p, NH_MODULE_WSI, NH_WSI_SETTING_NAMES_PP[index]);
    NH_CORE_CHECK_NULL(Setting_p)

    switch (index) {
        case 0 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            strcpy(Config_p->title_p, Setting_p->pp[0]);
            break;
        case 1 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->Size.width = atoi(Setting_p->pp[0]);
            break;
        case 2 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->Size.height = atoi(Setting_p->pp[0]);
            break;
        case 3 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->Position.x = atoi(Setting_p->pp[0]);
            break;
        case 4 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->Position.y = atoi(Setting_p->pp[0]);
            break;
        case 5 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->decorated = atoi(Setting_p->pp[0]) == 1;
            break;
        case 6 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->resizable = atoi(Setting_p->pp[0]) == 1;
            break;
        case 7 :
            memset(&Config_p->action_p, 0, sizeof(bool)*NH_WSI_WINDOW_ACTION_E_COUNT);
            for (int i = 0; i < Setting_p->size; ++i) {
                if (!strcmp("all", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_ALL] = true;
                }
                if (!strcmp("resize", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_RESIZE] = true;
                }
                if (!strcmp("move", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_MOVE] = true;
                }
                if (!strcmp("minimize", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_MINIMIZE] = true;
                }
                if (!strcmp("maximize", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_MAXIMIZE] = true;
                }
                if (!strcmp("close", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_CLOSE] = true;
                }
            }
            break;
        case 8 :
            memset(&Config_p->state_p, 0, sizeof(bool)*NH_WSI_WINDOW_STATE_E_COUNT);
            for (int i = 0; i < Setting_p->size; ++i) {
                if (!strcmp("above", Setting_p->pp[i])) {
                    Config_p->state_p[NH_WSI_WINDOW_STATE_ABOVE] = true;
                }
                if (!strcmp("maximized", Setting_p->pp[i])) {
                    Config_p->state_p[NH_WSI_WINDOW_STATE_MAXIMIZED] = true;
                }
            }
            break;
        case 9 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            if (!strcmp("normal", Setting_p->pp[0])) {
                Config_p->type = NH_WSI_WINDOW_TYPE_NORMAL;
            }
            if (!strcmp("desktop", Setting_p->pp[0])) {
                Config_p->type = NH_WSI_WINDOW_TYPE_DESKTOP;
            }
            if (!strcmp("dock", Setting_p->pp[0])) {
                Config_p->type = NH_WSI_WINDOW_TYPE_DOCK;
            }
            if (!strcmp("toolbar", Setting_p->pp[0])) {
                Config_p->type = NH_WSI_WINDOW_TYPE_TOOLBAR;
            }
            break;
    }

    return NH_API_SUCCESS;
}

nh_wsi_WindowConfig nh_wsi_getWindowConfig(
    void *Window_p)
{
    nh_wsi_WindowConfig Config;
    memset(&Config, 0, sizeof(nh_wsi_WindowConfig));

    for (int i = 0; i < NH_WSI_SETTING_NAMES_PP_COUNT; ++i) {
        nh_wsi_getWindowSetting(&Config, ((nh_wsi_Window*)Window_p)->namespace_p, i);
    }

    return Config;
}

