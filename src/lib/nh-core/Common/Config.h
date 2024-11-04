#ifndef NH_CORE_COMMON_CONFIG_H
#define NH_CORE_COMMON_CONFIG_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"
#include "Includes.h"

typedef enum NH_CORE_SETTING_E {
    NH_CORE_SETTING_LOADER_UNLOAD,
    NH_CORE_SETTING_DEBUG_TO_CONSOLE,
    NH_CORE_SETTING_DEBUG_LEVEL,
    NH_CORE_SETTING_LOGGER_ON,
    NH_CORE_SETTING_LOGGER_IPC,
    NH_CORE_SETTING_LOGGER_PORT,
    NH_CORE_SETTING_E_COUNT,
} NH_CORE_SETTING_E;

typedef struct nh_core_Config {
    bool loaderUnload;
    bool debugToConsole;
    int debugLevel;
    bool loggerOn;
    bool loggerIPC;
    bool loggerPort;
} nh_core_Config;

nh_core_Config nh_core_getConfig(
);

#endif // NH_CORE_COMMON_CONFIG_H
