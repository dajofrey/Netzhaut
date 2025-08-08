#ifndef NH_CORE_CONFIG_CONFIG_H
#define NH_CORE_CONFIG_CONFIG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Loader/Loader.h"
#include "../Util/List.h"

#endif

/** @addtogroup lib_nh-core_defines
 *  @{
 */

    #define NH_CORE_CONFIG_COMMAND 999

/** @} */

/** @addtogroup lib_nh-core_structs
 *  @{
 */

    typedef struct nh_core_RawConfigSetting {
        int module;
        char *name_p;
        char namespace_p[255];
        struct nh_core_RawConfigSetting *Default_p;
        nh_core_List Values;
        bool mark;
        unsigned int priority;
    } nh_core_RawConfigSetting;

    typedef struct nh_core_RawConfig {
        nh_core_List Settings;
    } nh_core_RawConfig;

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    nh_core_RawConfig nh_core_initRawConfig(
    );

    NH_API_RESULT nh_core_initGlobalConfig(
    );

    void nh_core_freeGlobalConfig(
    );

    nh_core_RawConfig *nh_core_getGlobalConfig(
    );

    nh_core_List *nh_core_getGlobalConfigSetting(
        char *namespace_p, int _module, const char *name_p
    );

    NH_API_RESULT nh_core_updateConfig(
        char *data_p, int length, unsigned int priority
    );
    
    NH_API_RESULT nh_core_overwriteGlobalConfigSetting(
        char *namespace_p, NH_MODULE_E module, const char *setting_p, char *value_p
    );

    NH_API_RESULT nh_core_overwriteGlobalConfigSettingInt(
        char *namespace_p, int _module, const char *setting_p, int value 
    );

/** @} */

#endif // NH_CORE_CONFIG_CONFIG_H
