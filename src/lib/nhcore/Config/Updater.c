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

#include "../Common/Macros.h"

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// DATA ============================================================================================

#define NH_MAX_CONFIG_PATH 255

typedef struct nh_ConfigUpdaterFile {
    NH_BYTE path_p[NH_MAX_CONFIG_PATH];
    time_t lastModification;
} nh_ConfigUpdaterFile;

typedef struct nh_ConfigUpdater {
    nh_List Files;
} nh_ConfigUpdater;

static nh_ConfigUpdater NH_CONFIG_UPDATER;

// INIT/FREE =======================================================================================
// The next functions comprise the in/exit points of nhterminal.

static void *nh_core_initConfigUpdater(
    nh_core_Workload *Workload_p)
{
NH_CORE_BEGIN()

    static NH_BYTE *name_p = "Config Updater";
    static NH_BYTE *path_p = "nhcore/Config/Updater.c";

    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_CORE;

    // Normalize global config.
    nh_RawConfig *Config_p = nh_core_getGlobalConfig();
    for (int i = 0; i < Config_p->Settings.size; ++i) {
        ((nh_RawConfigSetting*)Config_p->Settings.pp[i])->mark = NH_FALSE;
    }

    NH_CONFIG_UPDATER.Files = nh_core_initList(4);

NH_CORE_END(&NH_CONFIG_UPDATER)
}

static void nh_core_freeConfigUpdater(
    void *p)
{
NH_CORE_BEGIN()

    nh_core_freeList(&NH_CONFIG_UPDATER.Files, NH_TRUE);

NH_CORE_SILENT_END()
}

// RUN LOOP ========================================================================================
// The next functions comprise the top-level of the nhterminal run loop.

static NH_SIGNAL nh_core_runConfigUpdater(
    void *args_p) 
{
NH_CORE_BEGIN()

    NH_SIGNAL signal = NH_SIGNAL_IDLE;

    for (int i = 0; i < NH_CONFIG_UPDATER.Files.size; ++i) {
        nh_ConfigUpdaterFile *File_p = NH_CONFIG_UPDATER.Files.pp[i];
//        struct stat statbuf;
//        if (stat(File_p->path_p, &statbuf) == -1) {
//            continue;
//        }
//        if (File_p->lastModification != statbuf.st_mtime) {
//            long length = 0;
//            NH_BYTE *data_p = nh_core_getFileData(File_p->path_p, &length);
//            nh_overwriteGlobalConfig(data_p, length);
//            nh_core_free(data_p);
//            File_p->lastModification = statbuf.st_mtime;
//        }
    }

    nh_RawConfig *Config_p = nh_core_getGlobalConfig();
    for (int i = 0; i < Config_p->Settings.size; ++i) {
//        nh_RawConfigSetting *Setting_p = Config_p->Settings.pp[i];
//        if (!Setting_p->mark) {continue;}
//        nh_core_Workload *Workloads_p = nh_core_getWorkloads();
//        for (int j = 0; j < NH_MAX_WORKLOADS; ++j) {
//            if (Workloads_p[j].Thread_p && Workloads_p[j].module == Setting_p->module) {
//                nh_core_executeWorkloadCommand(Workloads_p[j].args_p, NH_CORE_CONFIG_COMMAND, Setting_p, 0);
//            }
//        }
//
//        Setting_p->mark = NH_FALSE;
    }

NH_CORE_END(signal)
}

// COMMANDS ========================================================================================
// The next functions are executed by nh_terminal_cmd_* functions.

typedef enum NH_CONFIG_UPDATER_COMMAND_E {
    NH_CONFIG_UPDATER_COMMAND_REGISTER_CONFIG,
    NH_CONFIG_UPDATER_COMMAND_LOAD_CONFIG,
} NH_CONFIG_UPDATER_COMMAND_E;

/**
 * Here, most commands that came in through the API are handled.
 */
static NH_SIGNAL nh_core_runConfigUpdaterCommand(
    void *p, nh_core_WorkloadCommand *Command_p)
{
NH_CORE_BEGIN()

    switch (Command_p->type)
    {
        case NH_CONFIG_UPDATER_COMMAND_REGISTER_CONFIG :
        {
            if (strlen(Command_p->p) > NH_MAX_CONFIG_PATH) {
                NH_CORE_END(NH_SIGNAL_OK)
            }
            nh_ConfigUpdaterFile *File_p = nh_core_allocate(sizeof(nh_ConfigUpdaterFile));
            if (!File_p) {
                NH_CORE_END(NH_SIGNAL_OK)
            }
            strcpy(File_p->path_p, Command_p->p); 
            struct stat statbuf;
            if (stat(Command_p->p, &statbuf) == -1) {
                NH_CORE_END(NH_SIGNAL_OK)
            }
            File_p->lastModification = statbuf.st_mtime;
            nh_core_appendToList(&NH_CONFIG_UPDATER.Files, File_p);
            long length = 0;
            NH_BYTE *data_p = nh_core_getFileData(File_p->path_p, &length);
            nh_core_overwriteGlobalConfig(data_p, length);
            nh_core_free(data_p);
            break;
        }
        case NH_CONFIG_UPDATER_COMMAND_LOAD_CONFIG :
            break;
    }

NH_CORE_END(NH_SIGNAL_OK)
}

// API =============================================================================================
// The next functions are called by lib/netzhaut/nhterminal.h functions.

NH_CORE_RESULT nh_core_startConfigUpdater()
{
NH_CORE_BEGIN()

    NH_CORE_CHECK_NULL(nh_core_activateWorkload(
        nh_core_initConfigUpdater, nh_core_runConfigUpdater, nh_core_freeConfigUpdater,
        nh_core_runConfigUpdaterCommand, NULL, NH_FALSE))

NH_CORE_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_core_registerConfig(
    NH_BYTE *absolutePath_p, int length)
{
NH_CORE_BEGIN()

    nh_core_executeWorkloadCommand(&NH_CONFIG_UPDATER, 
        NH_CONFIG_UPDATER_COMMAND_REGISTER_CONFIG, absolutePath_p, length);

NH_CORE_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_core_loadConfig(
    NH_BYTE *data_p, int length)
{
NH_CORE_BEGIN()

    nh_core_executeWorkloadCommand(&NH_CONFIG_UPDATER, 
        NH_CONFIG_UPDATER_COMMAND_LOAD_CONFIG, data_p, length);

NH_CORE_END(NH_CORE_SUCCESS)
}

