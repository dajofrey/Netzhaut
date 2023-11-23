#ifndef NH_CORE_CONFIG_PARSER_H
#define NH_CORE_CONFIG_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Config.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_parseRawConfig(
        nh_RawConfig *Config_p, NH_BYTE *data_p, int length, nh_RawConfig *GlobalConfig_p
    );

    NH_CORE_RESULT nh_core_freeRawConfig(
        nh_RawConfig *Config_p
    );

/** @} */

#endif // NH_DOCS_CORE_CONFIG_PARSER_H
