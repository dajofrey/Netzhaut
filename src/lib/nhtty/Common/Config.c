// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Config.h"
#include "IndexMap.h"
#include "Macros.h"

#include "../TTY/TTY.h"

#include "../../nhcore/Config/Config.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"

#include <string.h>
#include <stdlib.h>

// NAMES ===========================================================================================

const NH_BYTE *NH_TTY_SETTING_NAMES_PP[] = {
    "sidebar.state",
    "shell.maxScroll",
    "windows",
    "tabs",
    "menu.program",
    "menu.split",
    "menu.append",
    "menu.window",
    "menu.tab",
    "menu.close",
    "menu.debug",
};

size_t NH_TTY_SETTING_NAMES_PP_COUNT = 
    sizeof(NH_TTY_SETTING_NAMES_PP) / sizeof(NH_TTY_SETTING_NAMES_PP[0]);

const NH_BYTE *nh_tty_getSettingName(
    NH_TTY_SETTING_E setting)
{
NH_TTY_BEGIN()
NH_TTY_END(NH_TTY_SETTING_NAMES_PP[setting])
}

// FUNCTIONS =======================================================================================

static NH_TTY_RESULT nh_tty_getSetting(
    nh_tty_Config *Config_p, NH_BYTE *namespace_p, NH_BYTE *setting_p)
{
NH_TTY_BEGIN()

    unsigned int *index_p = nh_core_getFromHashMap(&NH_TTY_INDEXMAP.SettingNames, setting_p);
    if (index_p == NULL) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}

    nh_List *Setting_p = nh_core_getGlobalConfigSetting(namespace_p, NH_MODULE_TTY, setting_p);
    NH_TTY_CHECK_NULL(Setting_p)

    switch (*index_p) {
        case 0 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->Sidebar.state = atoi(Setting_p->pp[0]);
            break;
        case 1 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->Shell.maxScroll = atoi(Setting_p->pp[0]);
            break;
        case 2 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->windows = atoi(Setting_p->pp[0]);
            break;
        case 3 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->tabs = atoi(Setting_p->pp[0]);
            break;
        case 4 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->Menu.program = atoi(Setting_p->pp[0]);
            break;
        case 5 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->Menu.split = atoi(Setting_p->pp[0]);
            break;
        case 6 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->Menu.append = atoi(Setting_p->pp[0]);
            break;
        case 7 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->Menu.window = atoi(Setting_p->pp[0]);
            break;
        case 8 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->Menu.tab = atoi(Setting_p->pp[0]);
            break;
        case 9 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->Menu.close = atoi(Setting_p->pp[0]);
            break;
        case 10 :
            if (Setting_p->size != 1) {NH_TTY_END(NH_TTY_ERROR_BAD_STATE)}
            Config_p->Menu.debug = atoi(Setting_p->pp[0]);
            break;
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

nh_tty_Config nh_tty_getConfig()
{
NH_TTY_BEGIN()

    nh_tty_Config Config;
    memset(&Config, 0, sizeof(nh_tty_Config));

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();
    NH_TTY_CHECK_NULL_2(Config, TTY_p)

    for (int i = 0; i < NH_TTY_SETTING_NAMES_PP_COUNT; ++i) {
        NH_TTY_CHECK_2(Config, nh_tty_getSetting(&Config, TTY_p->namespace_p, NH_TTY_SETTING_NAMES_PP[i]))
    }

NH_TTY_END(Config)
}

