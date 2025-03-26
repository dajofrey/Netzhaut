#ifndef NH_MONITOR_COMMON_CONFIG_H
#define NH_MONITOR_COMMON_CONFIG_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// STUFF ===========================================================================================

#include <stdbool.h>

typedef struct nh_monitor_Config {
    int client_port;
    int server_port;
    bool block;
} nh_monitor_Config;

nh_monitor_Config nh_monitor_getConfig(
);

#endif
