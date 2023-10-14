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

#include "../Terminal/Terminal.h"

#include "../../nhcore/Config/Config.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"

#include <string.h>
#include <stdlib.h>

// NAMES ===========================================================================================

const NH_BYTE *NH_TERMINAL_SETTING_NAMES_PP[] = {
    "font.size",
    "blink.frequency",
    "color.foreground",
    "color.background",
    "color.accent",
};

size_t NH_TERMINAL_SETTING_NAMES_PP_COUNT = 
    sizeof(NH_TERMINAL_SETTING_NAMES_PP) / sizeof(NH_TERMINAL_SETTING_NAMES_PP[0]);

const NH_BYTE *nh_terminal_getSettingName(
    NH_TERMINAL_SETTING_E setting)
{
NH_TERMINAL_BEGIN()
NH_TERMINAL_END(NH_TERMINAL_SETTING_NAMES_PP[setting])
}

// FUNCTIONS =======================================================================================

static NH_TERMINAL_RESULT nh_terminal_getSetting(
    nh_terminal_Config *Config_p, NH_BYTE namespace_p[255], const NH_BYTE *setting_p)
{
NH_TERMINAL_BEGIN()

    unsigned int *index_p = nh_core_getFromHashMap(&NH_TERMINAL_INDEXMAP.SettingNames, (NH_BYTE*)setting_p);
    if (index_p == NULL) {NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)}

    nh_List *Setting_p = nh_core_getGlobalConfigSetting(namespace_p, NH_MODULE_TERMINAL, setting_p);
    NH_TERMINAL_CHECK_NULL(Setting_p)

    switch (*index_p) {
        case 0 :
            if (Setting_p->size != 1) {NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)}
            Config_p->fontSize = atoi(Setting_p->pp[0]);
            break;
        case 1 :
            if (Setting_p->size != 1) {NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)}
            Config_p->blinkFrequency = atof(Setting_p->pp[0]);
            break;
        case 2 :
            if (Setting_p->size != 4) {NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)}
            Config_p->Foreground.r = ((float)atoi(Setting_p->pp[0]))/255.0f;
            Config_p->Foreground.g = ((float)atoi(Setting_p->pp[1]))/255.0f;
            Config_p->Foreground.b = ((float)atoi(Setting_p->pp[2]))/255.0f;
            Config_p->Foreground.a = ((float)atoi(Setting_p->pp[3]))/255.0f;
            break;
        case 3 :
            if (Setting_p->size != 4) {NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)}
            Config_p->Background.r = ((float)atoi(Setting_p->pp[0]))/255.0f;
            Config_p->Background.g = ((float)atoi(Setting_p->pp[1]))/255.0f;
            Config_p->Background.b = ((float)atoi(Setting_p->pp[2]))/255.0f;
            Config_p->Background.a = ((float)atoi(Setting_p->pp[3]))/255.0f;
            break;
        case 4 :
            Config_p->accents = 0;
            if (Setting_p->size < 4 || (Setting_p->size % 4) != 0) {NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)}
            for (int i = 0, j = 0; j < Setting_p->size; i++, j += 4) {
                Config_p->Accents_p[i].r = ((float)atof(Setting_p->pp[j+0]))/255.0f;
                Config_p->Accents_p[i].g = ((float)atof(Setting_p->pp[j+1]))/255.0f;
                Config_p->Accents_p[i].b = ((float)atof(Setting_p->pp[j+2]))/255.0f;
                Config_p->Accents_p[i].a = ((float)atof(Setting_p->pp[j+3]))/255.0f;
                Config_p->accents++;
            }
            break;
    }

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

nh_terminal_Config nh_terminal_getConfig()
{
NH_TERMINAL_BEGIN()

    nh_terminal_Config Config;
    memset(&Config, 0, sizeof(nh_terminal_Config));

    nh_terminal_Terminal *Terminal_p = nh_core_getWorkloadArg();
    NH_TERMINAL_CHECK_NULL_2(Config, Terminal_p)

    for (int i = 0; i < NH_TERMINAL_SETTING_NAMES_PP_COUNT; ++i) {
        NH_TERMINAL_CHECK_2(Config, nh_terminal_getSetting(&Config, Terminal_p->namespace_p, NH_TERMINAL_SETTING_NAMES_PP[i]))
    }

NH_TERMINAL_END(Config)
}

