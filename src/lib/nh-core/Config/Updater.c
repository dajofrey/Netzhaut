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
#include "../Util/File.h"
#include "../System/Memory.h"
#include "../System/Thread.h"

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// DATA ============================================================================================

#define NH_MAX_CONFIG_PATH 255

typedef struct nh_ConfigUpdaterFile {
    char path_p[NH_MAX_CONFIG_PATH];
    time_t lastModification;
} nh_ConfigUpdaterFile;

typedef struct nh_ConfigUpdater {
    nh_core_List Files;
} nh_ConfigUpdater;

static nh_ConfigUpdater NH_CONFIG_UPDATER;

// FUNCTIONS =======================================================================================

static void *nh_core_initConfigWorkload(
    nh_core_Workload *Workload_p)
{
    static char *name_p = "Config Workload";
    static char *path_p = "nh-core/Config/Updater.c";

    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_CORE;

    // Normalize global config.
    nh_core_RawConfig *Config_p = nh_core_getGlobalConfig();
    for (int i = 0; i < Config_p->Settings.size; ++i) {
        ((nh_core_RawConfigSetting*)Config_p->Settings.pp[i])->mark = false;
    }

    NH_CONFIG_UPDATER.Files = nh_core_initList(4);

    return &NH_CONFIG_UPDATER;
}

static void nh_core_freeConfigWorkload(
    void *p)
{
    nh_core_freeList(&NH_CONFIG_UPDATER.Files, true);
    return;
}

static NH_SIGNAL nh_core_runConfigWorkload(
    void *args_p) 
{
    NH_SIGNAL signal = NH_SIGNAL_IDLE;

    for (int i = 0; i < NH_CONFIG_UPDATER.Files.size; ++i) {
        nh_ConfigUpdaterFile *File_p = NH_CONFIG_UPDATER.Files.pp[i];
//        struct stat statbuf;
//        if (stat(File_p->path_p, &statbuf) == -1) {
//            continue;
//        }
//        if (File_p->lastModification != statbuf.st_mtime) {
//            long length = 0;
//            char *data_p = nh_core_getFileData(File_p->path_p, &length);
//            nh_overwriteGlobalConfig(data_p, length);
//            nh_core_free(data_p);
//            File_p->lastModification = statbuf.st_mtime;
//        }
    }

    nh_core_RawConfig *Config_p = nh_core_getGlobalConfig();
    for (int i = 0; i < Config_p->Settings.size; ++i) {
//        nh_core_RawConfigSetting *Setting_p = Config_p->Settings.pp[i];
//        if (!Setting_p->mark) {continue;}
//        nh_core_Workload *Workloads_p = nh_core_getWorkloads();
//        for (int j = 0; j < NH_MAX_WORKLOADS; ++j) {
//            if (Workloads_p[j].Thread_p && Workloads_p[j].module == Setting_p->module) {
//                nh_core_executeWorkloadCommand(Workloads_p[j].args_p, NH_CORE_CONFIG_COMMAND, Setting_p, 0);
//            }
//        }
//
//        Setting_p->mark = false;
    }

    return signal;
}

typedef enum NH_CONFIG_UPDATER_COMMAND_E {
    NH_CONFIG_UPDATER_COMMAND_REGISTER_CONFIG,
    NH_CONFIG_UPDATER_COMMAND_LOAD_CONFIG,
} NH_CONFIG_UPDATER_COMMAND_E;

/**
 * Here, most commands that came in through the API are handled.
 */
static NH_SIGNAL nh_core_runConfigWorkloadCommand(
    void *p, nh_core_WorkloadCommand *Command_p)
{
    switch (Command_p->type)
    {
        case NH_CONFIG_UPDATER_COMMAND_REGISTER_CONFIG :
        {
            if (strlen(Command_p->p) > NH_MAX_CONFIG_PATH) {
                return NH_SIGNAL_OK;
            }
            nh_ConfigUpdaterFile *File_p = (nh_ConfigUpdaterFile*)nh_core_allocate(sizeof(nh_ConfigUpdaterFile));
            if (!File_p) {
                return NH_SIGNAL_OK;
            }
            strcpy(File_p->path_p, Command_p->p); 
            struct stat statbuf;
            if (stat(Command_p->p, &statbuf) == -1) {
                return NH_SIGNAL_OK;
            }
            File_p->lastModification = statbuf.st_mtime;
            nh_core_appendToList(&NH_CONFIG_UPDATER.Files, File_p);
            long length = 0;
            char *data_p = nh_core_getFileData(File_p->path_p, &length);
            nh_core_updateConfig(data_p, length, 0);
            nh_core_free(data_p);
            break;
        }
        case NH_CONFIG_UPDATER_COMMAND_LOAD_CONFIG :
        {
            nh_core_RawConfig Config = nh_core_initRawConfig();
            nh_core_parseRawConfig(&Config, Command_p->p, Command_p->length, NULL);
            if (Config.Settings.size <= 0) {break;}
            for (int i = 0; i < Config.Settings.size; ++i) {
                nh_core_RawConfigSetting *Setting_p = Config.Settings.pp[i];
                nh_core_overwriteGlobalConfigSetting(
                    NULL, Setting_p->module, Setting_p->name_p, Setting_p->Values.pp[0]);
            }
            break;
        }
    }

    return NH_SIGNAL_OK;
}

NH_API_RESULT nh_core_startConfigWorkload()
{
    NH_CORE_CHECK_NULL(nh_core_activateWorkload(
        nh_core_initConfigWorkload, nh_core_runConfigWorkload, nh_core_freeConfigWorkload,
        nh_core_runConfigWorkloadCommand, NULL, false))

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_registerConfig(
    char *absolutePath_p, int length)
{
    nh_core_executeWorkloadCommand(&NH_CONFIG_UPDATER, 
        NH_CONFIG_UPDATER_COMMAND_REGISTER_CONFIG, absolutePath_p, length);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_loadConfig(
    char *data_p, int length)
{
    nh_core_executeWorkloadCommand(&NH_CONFIG_UPDATER, 
        NH_CONFIG_UPDATER_COMMAND_LOAD_CONFIG, data_p, length);

    return NH_API_SUCCESS;
}
