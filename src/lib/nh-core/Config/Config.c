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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

static nh_core_RawConfig NH_GLOBAL_CONFIG;

// FUNCTIONS =======================================================================================

nh_core_RawConfig *nh_core_getGlobalConfig()
{
    return &NH_GLOBAL_CONFIG;
}

nh_core_List *nh_core_getGlobalConfigSetting(
    char *namespace_p, int _module, const char *name_p)
{
    nh_core_RawConfigSetting *DefaultSetting_p = NULL;

    for (int i = 0; i < NH_GLOBAL_CONFIG.Settings.size; ++i) {
        nh_core_RawConfigSetting *Setting_p = NH_GLOBAL_CONFIG.Settings.pp[i];

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
                return &Setting_p->Values;
            }
        }
    }

    // The default config setting could not be found and the setting is therefore not valid.
    if (!DefaultSetting_p) {return NULL;}

    return &DefaultSetting_p->Values;
}

nh_core_RawConfig nh_core_initRawConfig()
{
    nh_core_RawConfig Config;
    memset(&Config, 0, sizeof(nh_core_RawConfig));
    Config.Settings = nh_core_initList(32);
    return Config;
}

static bool nh_core_matchConfigValues(
    nh_core_List *Values1_p, nh_core_List *Values2_p)
{
    if (Values1_p->size != Values2_p->size) {return false;}

    for (int i = 0; i < Values1_p->size; ++i) {
        if (strcmp(Values1_p->pp[i], Values2_p->pp[i])) {
            return false;
        }
    }

    return true;
}

static nh_core_List nh_core_copyConfigValues(
    nh_core_List *Values_p)
{
    nh_core_List Values = nh_core_initList(Values_p->size);

    for (int i = 0; i < Values_p->size; ++i) {
        char *p = malloc(sizeof(char)*(strlen(Values_p->pp[i])+1));
        strcpy(p, Values_p->pp[i]);
        nh_core_appendToList(&Values, p);
    }

    return Values;
}

NH_API_RESULT nh_core_appendConfig(
    char *data_p, int length, bool overwrite)
{
    nh_core_RawConfig Config = nh_core_initRawConfig();
    nh_core_parseRawConfig(&Config, data_p, length, nh_core_getGlobalConfig());

    for (int i = 0; i < Config.Settings.size; ++i) {
        nh_core_RawConfigSetting *NewSetting_p = Config.Settings.pp[i];

        bool found = false;

        // Overwrite default setting if no namespace.
        for (int j = 0; j < NH_GLOBAL_CONFIG.Settings.size && strlen(NewSetting_p->namespace_p) == 0; ++j) {
            nh_core_RawConfigSetting *Setting_p = NH_GLOBAL_CONFIG.Settings.pp[j];
            if (Setting_p == NewSetting_p->Default_p) {

                found = true;
                if (!overwrite) {break;}

                // If the setting values match, nothing needs to be overwritten.
                if (nh_core_matchConfigValues(&Setting_p->Values, &NewSetting_p->Values)) {
                    break;
                } else {
                    nh_core_freeList(&Setting_p->Values, true);
                    Setting_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
                    Setting_p->mark = true;
                    break;
                }
            }
        }

        // Overwrite custom setting if namespace.
        for (int j = 0; j < NH_GLOBAL_CONFIG.Settings.size && strlen(NewSetting_p->namespace_p) > 0; ++j) {
            nh_core_RawConfigSetting *Setting_p = NH_GLOBAL_CONFIG.Settings.pp[j];
            if (strlen(Setting_p->namespace_p) == 0) {continue;}
            if (!strcmp(Setting_p->namespace_p, NewSetting_p->namespace_p) && Setting_p->Default_p == NewSetting_p->Default_p) {

                found = true;
                if (!overwrite) {break;}

                if (nh_core_matchConfigValues(&Setting_p->Values, &NewSetting_p->Values)) {
                    break;
                } else {
                    nh_core_freeList(&Setting_p->Values, true);
                    Setting_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
                    Setting_p->mark = true;
                    break;
                }
            }
        }

        if (!found) {

            // If no matching global config setting was found, we have to add it if it's valid.
 
            nh_core_RawConfigSetting *Allocated_p = malloc(sizeof(nh_core_RawConfigSetting));
            NH_CORE_CHECK_MEM(Allocated_p)

            memset(Allocated_p, 0, sizeof(nh_core_RawConfigSetting));

            Allocated_p->name_p = malloc(sizeof(char)*(strlen(NewSetting_p->name_p)+1));
            strcpy(Allocated_p->name_p, NewSetting_p->name_p);
            Allocated_p->module = NewSetting_p->module;
            Allocated_p->Default_p = NewSetting_p->Default_p;
            Allocated_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
            strcpy(Allocated_p->namespace_p, NewSetting_p->namespace_p);

            NH_CORE_CHECK(nh_core_appendToList(&NH_GLOBAL_CONFIG.Settings, Allocated_p))
        }
    }

    nh_core_freeRawConfig(&Config);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_overwriteGlobalConfigSetting(
    char *namespace_p, NH_MODULE_E module, const char *setting_p, char *value_p)
{
    char config_p[255];

    if (namespace_p) {
        sprintf(config_p, "%s_%s.%s:%s;", namespace_p, NH_MODULE_NAMES_PP[module], setting_p, value_p);
    } else {
        sprintf(config_p, "%s.%s:%s;", NH_MODULE_NAMES_PP[module], setting_p, value_p);
    }

    NH_CORE_CHECK(nh_core_appendConfig(config_p, strlen(config_p), true))

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_overwriteGlobalConfigSettingInt(
    char *namespace_p, int _module, const char *setting_p, int value)
{
    char config_p[255];

    if (namespace_p) {
        if (_module >= 0) {
            sprintf(config_p, "%s_%s.%s:%d;", namespace_p, NH_MODULE_NAMES_PP[_module], setting_p, value);
        } else {
            sprintf(config_p, "%s_%s:%d;", namespace_p, setting_p, value);
        }
    } else {
        if (_module >= 0) {
            sprintf(config_p, "%s.%s:%d;", NH_MODULE_NAMES_PP[_module], setting_p, value);
        } else {
            sprintf(config_p, "%s:%d;", setting_p, value);
        }
    }

    NH_CORE_CHECK(nh_core_appendConfig(config_p, strlen(config_p), true))

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_initGlobalConfig()
{
    NH_GLOBAL_CONFIG = nh_core_initRawConfig();
    nh_core_parseRawConfig(&NH_GLOBAL_CONFIG, default_conf_inc, default_conf_inc_len, NULL);
    return NH_API_SUCCESS;
}

void nh_core_freeGlobalConfig()
{
    nh_core_freeRawConfig(&NH_GLOBAL_CONFIG);
    return;
}
