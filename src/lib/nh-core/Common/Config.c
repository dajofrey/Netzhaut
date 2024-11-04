// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Config.h"

#include "../Config/Config.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// DATA ============================================================================================

static nh_core_Config NH_CORE_CONFIG;

const char *NH_CORE_SETTING_NAMES_PP[] = {
    "loader.unload",
    "debug.toConsole",
    "debug.level",
    "logger.on",
    "logger.ipc",
    "logger.port",
};

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_core_getSetting(
    nh_core_Config *Config_p, NH_CORE_SETTING_E setting)
{
    nh_core_List *Setting_p = nh_core_getGlobalConfigSetting(
        NULL, NH_MODULE_CORE, NH_CORE_SETTING_NAMES_PP[setting]);

    NH_CORE_CHECK_NULL(Setting_p)

    switch (setting) {
        case NH_CORE_SETTING_LOADER_UNLOAD :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->loaderUnload = atoi(Setting_p->pp[0]) == 1;
            break;
        case NH_CORE_SETTING_DEBUG_TO_CONSOLE :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->debugToConsole = atoi(Setting_p->pp[0]) == 1;
            break;
        case NH_CORE_SETTING_LOGGER_ON :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->loggerOn = atoi(Setting_p->pp[0]) == 1;
            break;
        case NH_CORE_SETTING_LOGGER_IPC :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->loggerIPC = atoi(Setting_p->pp[0]) == 1;
            break;
        case NH_CORE_SETTING_LOGGER_PORT :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->loggerPort = atoi(Setting_p->pp[0]);
            break;
    }

    return NH_API_SUCCESS;
}

static nh_core_Config nh_core_getStaticConfig() 
{ 
    nh_core_Config Config; 
    memset(&Config, 0, sizeof(nh_core_Config)); 

    for (int i = 0; i < NH_CORE_SETTING_E_COUNT; ++i) { 
        nh_core_getSetting(&Config, i); 
    } 

    return Config; 
} 

static int nh_core_consoleInRawMode() {
    struct termios term;
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        return -1;
    }
    if (!(term.c_lflag & ICANON) && !(term.c_lflag & ECHO) &&
        !(term.c_iflag & (IXON | ICRNL | INLCR | IGNCR)) &&
        !(term.c_oflag & OPOST)) {
        return 1;
    } else {
        return 0;
    }
}

nh_core_Config nh_core_getConfig()
{
    nh_core_Config Config = nh_core_getStaticConfig();

    if (nh_core_consoleInRawMode() == 1) {
        Config.debugToConsole = false;
    }

    return Config;
}
