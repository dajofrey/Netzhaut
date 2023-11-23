// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Config.h"
#include "Parser.h"

#include "../Util/List.h"

#include "../Common/Data/default.conf.inc"
#include "../Common/Macros.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

static nh_RawConfig NH_GLOBAL_CONFIG;

// GET CONFIG ======================================================================================

nh_RawConfig *nh_core_getGlobalConfig()
{
NH_CORE_BEGIN()
NH_CORE_END(&NH_GLOBAL_CONFIG)
}

nh_List *nh_core_getGlobalConfigSetting(
    NH_BYTE *namespace_p, int _module, const NH_BYTE *name_p)
{
NH_CORE_BEGIN()

    nh_RawConfigSetting *DefaultSetting_p = NULL;

    for (int i = 0; i < NH_GLOBAL_CONFIG.Settings.size; ++i) {
        nh_RawConfigSetting *Setting_p = NH_GLOBAL_CONFIG.Settings.pp[i];

        // Get default setting.
        if (!Setting_p->Default_p && Setting_p->module == _module && !strcmp(Setting_p->name_p, name_p)) {
            DefaultSetting_p = Setting_p;
        }

        // If there was no namespace specified, we are only searching for the default setting.
        if (namespace_p == NULL || strlen(namespace_p) == 0) {
            if (DefaultSetting_p) {
                break;
            } else {
                continue;
            }
        }

        if (strlen(Setting_p->namespace_p) > 0 && !strcmp(namespace_p, Setting_p->namespace_p)) {
            if (Setting_p->Default_p && Setting_p->Default_p->module == _module && !strcmp(Setting_p->Default_p->name_p, name_p)) {
                // We found custom setting!
                NH_CORE_END(&Setting_p->Values)
            }
        }
    }

    // The default config setting could not be found and the setting is therefore not valid.
    if (!DefaultSetting_p) {NH_CORE_END(NULL)}

NH_CORE_END(&DefaultSetting_p->Values)
}

// OVERWRITE =======================================================================================

static nh_RawConfig nh_core_initRawConfig()
{
NH_CORE_BEGIN()

    nh_RawConfig Config;
    memset(&Config, 0, sizeof(nh_RawConfig));
    Config.Settings = nh_core_initList(32);

NH_CORE_END(Config)
}

static NH_BOOL nh_matchConfigValues(
    nh_List *Values1_p, nh_List *Values2_p)
{
NH_CORE_BEGIN()

    if (Values1_p->size != Values2_p->size) {NH_CORE_END(NH_FALSE)}

    for (int i = 0; i < Values1_p->size; ++i) {
        if (strcmp(Values1_p->pp[i], Values2_p->pp[i])) {
            NH_CORE_END(NH_FALSE)
        }
    }

NH_CORE_END(NH_TRUE)
}

static nh_List nh_core_copyConfigValues(
    nh_List *Values_p)
{
NH_CORE_BEGIN()

    nh_List Values = nh_core_initList(Values_p->size);

    for (int i = 0; i < Values_p->size; ++i) {
        NH_BYTE *p = malloc(sizeof(NH_BYTE)*(strlen(Values_p->pp[i])+1));
        strcpy(p, Values_p->pp[i]);
        nh_core_appendToList(&Values, p);
    }

NH_CORE_END(Values)
}

NH_CORE_RESULT nh_core_overwriteGlobalConfig(
    NH_BYTE *data_p, int length)
{
NH_CORE_BEGIN()

    nh_RawConfig Config = nh_core_initRawConfig();
    nh_core_parseRawConfig(&Config, data_p, length, nh_core_getGlobalConfig());

    for (int i = 0; i < Config.Settings.size; ++i) {
        nh_RawConfigSetting *NewSetting_p = Config.Settings.pp[i];

        NH_BOOL found = NH_FALSE;

        // Overwrite default setting if no namespace.
        for (int j = 0; j < NH_GLOBAL_CONFIG.Settings.size && strlen(NewSetting_p->namespace_p) == 0; ++j) {
            nh_RawConfigSetting *Setting_p = NH_GLOBAL_CONFIG.Settings.pp[j];
            if (Setting_p == NewSetting_p->Default_p) {

                found = NH_TRUE;

                // If the setting values match, nothing needs to be overwritten.
                if (nh_matchConfigValues(&Setting_p->Values, &NewSetting_p->Values)) {
                    break;
                } else {
                    nh_core_freeList(&Setting_p->Values, NH_TRUE);
                    Setting_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
                    Setting_p->mark = NH_TRUE;
                }
            }
        }

        // Overwrite custom setting if namespace.
        for (int j = 0; j < NH_GLOBAL_CONFIG.Settings.size && strlen(NewSetting_p->namespace_p) > 0; ++j) {
            nh_RawConfigSetting *Setting_p = NH_GLOBAL_CONFIG.Settings.pp[j];
            if (strlen(Setting_p->namespace_p) == 0) {continue;}
            if (!strcmp(Setting_p->namespace_p, NewSetting_p->namespace_p) && Setting_p->Default_p == NewSetting_p->Default_p) {

                found = NH_TRUE;

                if (nh_matchConfigValues(&Setting_p->Values, &NewSetting_p->Values)) {
                    break;
                } else {
                    nh_core_freeList(&Setting_p->Values, NH_TRUE);
                    Setting_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
                    Setting_p->mark = NH_TRUE;
                }
            }
        }

        if (!found) {
            if (!NewSetting_p->Default_p || strlen(NewSetting_p->namespace_p) <= 0 || NewSetting_p->module < 0) {
                continue;
            }

            // If no matching global config setting was found, we have to add it if it's valid.
 
            nh_RawConfigSetting *Allocated_p = malloc(sizeof(nh_RawConfigSetting));
            NH_CORE_CHECK_MEM(Allocated_p)

            memset(Allocated_p, 0, sizeof(nh_RawConfigSetting));

            Allocated_p->module = NewSetting_p->module;
            Allocated_p->Default_p = NewSetting_p->Default_p;
            Allocated_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
            strcpy(Allocated_p->namespace_p, NewSetting_p->namespace_p);

            NH_CORE_CHECK(nh_core_appendToList(&NH_GLOBAL_CONFIG.Settings, Allocated_p))
        }
    }

    nh_core_freeRawConfig(&Config);

NH_CORE_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_core_overwriteGlobalConfigSetting(
    NH_BYTE *namespace_p, NH_MODULE_E module, const NH_BYTE *setting_p, NH_BYTE *value_p)
{
NH_CORE_BEGIN()

    NH_BYTE config_p[255];

    if (namespace_p) {
        sprintf(config_p, "%s.%s.%s:%s;", namespace_p, NH_MODULE_NAMES_PP[module], setting_p, value_p);
    } else {
        sprintf(config_p, "%s.%s:%s;", NH_MODULE_NAMES_PP[module], setting_p, value_p);
    }

    NH_CORE_CHECK(nh_core_overwriteGlobalConfig(config_p, strlen(config_p)))

NH_CORE_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_core_overwriteGlobalConfigSettingInt(
    NH_BYTE *namespace_p, int _module, const NH_BYTE *setting_p, int value)
{
NH_CORE_BEGIN()

    NH_BYTE config_p[255];

    if (namespace_p) {
        if (_module >= 0) {
            sprintf(config_p, "%s.%s.%s:%d;", namespace_p, NH_MODULE_NAMES_PP[_module], setting_p, value);
        } else {
            sprintf(config_p, "%s.%s:%d;", namespace_p, setting_p, value);
        }
    } else {
        if (_module >= 0) {
            sprintf(config_p, "%s.%s:%d;", NH_MODULE_NAMES_PP[_module], setting_p, value);
        } else {
            sprintf(config_p, "%s:%d;", setting_p, value);
        }
    }

    NH_CORE_CHECK(nh_core_overwriteGlobalConfig(config_p, strlen(config_p)))

NH_CORE_END(NH_CORE_SUCCESS)
}

// INIT/FREE =======================================================================================

NH_CORE_RESULT nh_core_initGlobalConfig()
{
NH_CORE_BEGIN()

    NH_GLOBAL_CONFIG = nh_core_initRawConfig();
    nh_core_parseRawConfig(&NH_GLOBAL_CONFIG, default_conf_inc, default_conf_inc_len, NULL);

NH_CORE_END(NH_CORE_SUCCESS)
}

void nh_core_freeGlobalConfig()
{
NH_CORE_BEGIN()
 
   nh_core_freeRawConfig(&NH_GLOBAL_CONFIG);
 
NH_CORE_SILENT_END()
}

