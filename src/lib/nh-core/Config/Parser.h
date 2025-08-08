#ifndef NH_CORE_CONFIG_PARSER_H
#define NH_CORE_CONFIG_PARSER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Config.h"

NH_API_RESULT nh_core_parseRawConfig(
    nh_core_RawConfig *Result_p, char *data_p, int length, nh_core_RawConfig *GlobalConfig_p
);

NH_API_RESULT nh_core_freeRawConfig(
    nh_core_RawConfig *Config_p
);

#endif // NH_CORE_CONFIG_PARSER_H
