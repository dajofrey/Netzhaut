#ifndef NH_CORE_COMMON_CONFIG_H
#define NH_CORE_COMMON_CONFIG_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include <stdbool.h>

// STRUCTS =========================================================================================

typedef struct nh_core_Config {
    bool loaderUnload;
    bool monitor_on;
    bool dump_on;
} nh_core_Config;

// FUNCTIONS =======================================================================================

nh_core_Config nh_core_getConfig(
);

#endif // NH_CORE_COMMON_CONFIG_H
