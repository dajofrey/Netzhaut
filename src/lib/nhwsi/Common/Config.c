// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Config.h"
#include "Macros.h"

#include "../Window/Window.h"

#include "../../nhcore/Config/Config.h"
#include "../../nhcore/System/Memory.h"

#include <string.h>
#include <stdlib.h>

// NAMES ===========================================================================================

const NH_BYTE *NH_WSI_SETTING_NAMES_PP[] = {
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

const NH_BYTE *nh_wsi_getSettingName(
    NH_WSI_SETTING_E setting)
{
NH_WSI_BEGIN()
NH_WSI_END(NH_WSI_SETTING_NAMES_PP[setting])
}

// CONFIG ==========================================================================================

static NH_WSI_RESULT_E nh_wsi_getWindowSetting(
    nh_wsi_WindowConfig *Config_p, NH_BYTE *namespace_p, int index)
{
NH_WSI_BEGIN()

    nh_List *Setting_p = nh_core_getGlobalConfigSetting(strlen(namespace_p) == 0 ? NULL : namespace_p, NH_MODULE_WSI, NH_WSI_SETTING_NAMES_PP[index]);
    NH_WSI_CHECK_NULL(Setting_p)

    switch (index) {
        case 0 :
            if (Setting_p->size != 1) {NH_WSI_END(NH_WSI_ERROR_BAD_STATE)}
            strcpy(Config_p->title_p, Setting_p->pp[0]);
            break;
        case 1 :
            if (Setting_p->size != 1) {NH_WSI_END(NH_WSI_ERROR_BAD_STATE)}
            Config_p->Size.width = atoi(Setting_p->pp[0]);
            break;
        case 2 :
            if (Setting_p->size != 1) {NH_WSI_END(NH_WSI_ERROR_BAD_STATE)}
            Config_p->Size.height = atoi(Setting_p->pp[0]);
            break;
        case 3 :
            if (Setting_p->size != 1) {NH_WSI_END(NH_WSI_ERROR_BAD_STATE)}
            Config_p->Position.x = atoi(Setting_p->pp[0]);
            break;
        case 4 :
            if (Setting_p->size != 1) {NH_WSI_END(NH_WSI_ERROR_BAD_STATE)}
            Config_p->Position.y = atoi(Setting_p->pp[0]);
            break;
        case 5 :
            if (Setting_p->size != 1) {NH_WSI_END(NH_WSI_ERROR_BAD_STATE)}
            Config_p->decorated = atoi(Setting_p->pp[0]) == 1;
            break;
        case 6 :
            if (Setting_p->size != 1) {NH_WSI_END(NH_WSI_ERROR_BAD_STATE)}
            Config_p->resizable = atoi(Setting_p->pp[0]) == 1;
            break;
        case 7 :
            memset(&Config_p->action_p, 0, sizeof(NH_BOOL)*NH_WSI_WINDOW_ACTION_E_COUNT);
            for (int i = 0; i < Setting_p->size; ++i) {
                if (!strcmp("all", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_ALL] = NH_TRUE;
                }
                if (!strcmp("resize", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_RESIZE] = NH_TRUE;
                }
                if (!strcmp("move", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_MOVE] = NH_TRUE;
                }
                if (!strcmp("minimize", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_MINIMIZE] = NH_TRUE;
                }
                if (!strcmp("maximize", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_MAXIMIZE] = NH_TRUE;
                }
                if (!strcmp("close", Setting_p->pp[i])) {
                    Config_p->action_p[NH_WSI_WINDOW_ACTION_CLOSE] = NH_TRUE;
                }
            }
            break;
        case 8 :
            memset(&Config_p->state_p, 0, sizeof(NH_BOOL)*NH_WSI_WINDOW_STATE_E_COUNT);
            for (int i = 0; i < Setting_p->size; ++i) {
                if (!strcmp("above", Setting_p->pp[i])) {
                    Config_p->state_p[NH_WSI_WINDOW_STATE_ABOVE] = NH_TRUE;
                }
                if (!strcmp("maximized", Setting_p->pp[i])) {
                    Config_p->state_p[NH_WSI_WINDOW_STATE_MAXIMIZED] = NH_TRUE;
                }
            }
            break;
        case 9 :
            if (Setting_p->size != 1) {NH_WSI_END(NH_WSI_ERROR_BAD_STATE)}
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

NH_WSI_END(NH_WSI_SUCCESS)
}

nh_wsi_WindowConfig nh_wsi_getWindowConfig(
    void *Window_p)
{
NH_WSI_BEGIN()

    nh_wsi_WindowConfig Config;
    memset(&Config, 0, sizeof(nh_wsi_WindowConfig));

    for (int i = 0; i < NH_WSI_SETTING_NAMES_PP_COUNT; ++i) {
        nh_wsi_getWindowSetting(&Config, ((nh_wsi_Window*)Window_p)->namespace_p, i);
    }

NH_WSI_END(Config)
}

