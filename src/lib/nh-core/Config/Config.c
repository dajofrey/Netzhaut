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
#include "../Util/HashMap.h"
#include "../System/Memory.h"

#include "../Common/Data/default.conf.inc"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

static nh_core_RawConfig NH_GLOBAL_CONFIG;
static nh_core_HashMap MARK;

// FUNCTIONS =======================================================================================

nh_core_RawConfig *nh_core_getGlobalConfig()
{
    return &NH_GLOBAL_CONFIG;
}

bool nh_core_popGlobalConfigMark(
    char *namespace_p)
{
    void *p = nh_core_getFromHashMap(&MARK, namespace_p);
    if (p) {
        nh_core_removeFromHashMap(&MARK, namespace_p);
    }
    return p != NULL;
}

static void nh_core_markNamespace(
    char *namespace_p)
{
    nh_core_addToHashMap(&MARK, namespace_p, 1);
}

nh_core_List *nh_core_getGlobalConfigSetting(
    char *namespace_p, const char *name_p)
{
    nh_core_RawConfigSetting *DefaultSetting_p = NULL;

    for (int i = 0; i < NH_GLOBAL_CONFIG.Settings.size; ++i) {
        nh_core_RawConfigSetting *Setting_p = NH_GLOBAL_CONFIG.Settings.pp[i];

        // Get default setting.
        if (!Setting_p->Default_p && !strcmp(Setting_p->name_p, name_p)) {
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
            if (!strcmp(Setting_p->name_p, name_p)) {
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

NH_API_RESULT nh_core_updateConfig(
    char *data_p, int length, unsigned int priority)
{
    nh_core_RawConfig NewConfig = nh_core_initRawConfig();
    nh_core_parseRawConfig(&NewConfig, data_p, length);

    for (int i = 0; i < NewConfig.Settings.size; ++i) {
        nh_core_RawConfigSetting *NewSetting_p = NewConfig.Settings.pp[i];

        // search default
        nh_core_RawConfigSetting *Default_p = NULL;
        for (int j = 0; j < NH_GLOBAL_CONFIG.Settings.size; ++j) {
            nh_core_RawConfigSetting *PotentialDefault_p = NH_GLOBAL_CONFIG.Settings.pp[j];
            if (PotentialDefault_p->Default_p == NULL && !strcmp(PotentialDefault_p->name_p, NewSetting_p->name_p)) {
                Default_p = PotentialDefault_p;
                break;
            }
        }

        // create new default if not found
        if (!Default_p) {
            Default_p = nh_core_allocate(sizeof(nh_core_RawConfigSetting));
            NH_CORE_CHECK_MEM(Default_p)

            memset(Default_p, 0, sizeof(nh_core_RawConfigSetting));

            Default_p->name_p = malloc(sizeof(char)*(strlen(NewSetting_p->name_p)+1));
            strcpy(Default_p->name_p, NewSetting_p->name_p);

            Default_p->Default_p = NULL;
            Default_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
            Default_p->priority = priority;

            NH_CORE_CHECK(nh_core_appendToList(&NH_GLOBAL_CONFIG.Settings, Default_p))
            continue;
        }

        // update default if no namespace
        if (strlen(NewSetting_p->namespace_p) == 0) {
            if (Default_p->priority > priority) {
                // don't change setting if global priority is higher
                continue;
            }
            // If the setting values match, nothing needs to be overwritten.
            if (nh_core_matchConfigValues(&Default_p->Values, &NewSetting_p->Values)) {
                continue;
            } else {
                nh_core_freeList(&Default_p->Values, true);
                Default_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
                Default_p->mark = true;
                Default_p->priority = priority;
                continue;
            }
        }

        // update existing namespace setting
        bool found = false;
        if (strlen(NewSetting_p->namespace_p) > 0) {
            for (int j = 0; j < NH_GLOBAL_CONFIG.Settings.size; ++j) {
                nh_core_RawConfigSetting *Setting_p = NH_GLOBAL_CONFIG.Settings.pp[j];
                if (strlen(Setting_p->namespace_p) == 0) {continue;}
                if (!strcmp(Setting_p->namespace_p, NewSetting_p->namespace_p) && !strcmp(Setting_p->name_p, NewSetting_p->name_p)) {
                    found = true;
                    if (nh_core_matchConfigValues(&Setting_p->Values, &NewSetting_p->Values)) {
                        break;
                    } else {
                        nh_core_freeList(&Setting_p->Values, true);
                        Setting_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
                        Setting_p->mark = true;
                        nh_core_markNamespace(Setting_p->namespace_p);
                        break;
                    }
                }
            }
        }

        // create new namespace setting if not found
        if (!found) {
            nh_core_RawConfigSetting *NamespaceSetting_p = nh_core_allocate(sizeof(nh_core_RawConfigSetting));
            NH_CORE_CHECK_MEM(NamespaceSetting_p)

            memset(NamespaceSetting_p, 0, sizeof(nh_core_RawConfigSetting));

            NamespaceSetting_p->name_p = malloc(sizeof(char)*(strlen(NewSetting_p->name_p)+1));
            strcpy(NamespaceSetting_p->name_p, NewSetting_p->name_p);

            NamespaceSetting_p->Default_p = Default_p;
            NamespaceSetting_p->Values = nh_core_copyConfigValues(&NewSetting_p->Values);
            NamespaceSetting_p->priority = priority;

            strcpy(NamespaceSetting_p->namespace_p, NewSetting_p->namespace_p);
 
            NH_CORE_CHECK(nh_core_appendToList(&NH_GLOBAL_CONFIG.Settings, NamespaceSetting_p))

            nh_core_markNamespace(NamespaceSetting_p->namespace_p);
        }
    }

    nh_core_freeRawConfig(&NewConfig);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_overwriteGlobalConfigSetting(
    char *namespace_p, const char *setting_p, char *value_p)
{
    char config_p[255];

    if (namespace_p) {
        sprintf(config_p, "%s_%s:%s;", namespace_p, setting_p, value_p);
    } else {
        sprintf(config_p, "%s:%s;", setting_p, value_p);
    }

    NH_CORE_CHECK(nh_core_updateConfig(config_p, strlen(config_p), 1))

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_overwriteGlobalConfigSettingInt(
    char *namespace_p, const char *setting_p, int value)
{
    char config_p[255];

    if (namespace_p) {
        sprintf(config_p, "%s_%s:%d;", namespace_p, setting_p, value);
    } else {
        sprintf(config_p, "%s:%d;", setting_p, value);
    }

    NH_CORE_CHECK(nh_core_updateConfig(config_p, strlen(config_p), 1))

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_initGlobalConfig()
{
    NH_GLOBAL_CONFIG = nh_core_initRawConfig();
    MARK = nh_core_createHashMap();
    nh_core_parseRawConfig(&NH_GLOBAL_CONFIG, default_conf_inc, default_conf_inc_len);
    return NH_API_SUCCESS;
}

void nh_core_freeGlobalConfig()
{
    nh_core_freeRawConfig(&NH_GLOBAL_CONFIG);
    nh_core_freeHashMap(MARK);
    return;
}
