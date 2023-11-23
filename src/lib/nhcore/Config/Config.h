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

#endif

/** @addtogroup lib_nhcore_defines
 *  @{
 */

    #define NH_CORE_CONFIG_COMMAND 999

/** @} */

/** @addtogroup lib_nhcore_structs
 *  @{
 */

    typedef struct nh_RawConfigSetting {
        int module;
        NH_BYTE *name_p;
        NH_BYTE namespace_p[255];
        struct nh_RawConfigSetting *Default_p;
        nh_List Values;
        NH_BOOL mark;
    } nh_RawConfigSetting;

    typedef struct nh_RawConfig {
        nh_List Settings;
    } nh_RawConfig;

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_initGlobalConfig(
    );

    void nh_core_freeGlobalConfig(
    );

    nh_RawConfig *nh_core_getGlobalConfig(
    );

    nh_List *nh_core_getGlobalConfigSetting(
        NH_BYTE *namespace_p, int _module, const NH_BYTE *name_p
    );

    NH_CORE_RESULT nh_core_overwriteGlobalConfig(
        NH_BYTE *data_p, int length
    );
    
    NH_CORE_RESULT nh_core_overwriteGlobalConfigSetting(
        NH_BYTE *namespace_p, NH_MODULE_E module, const NH_BYTE *setting_p, NH_BYTE *value_p
    );

    NH_CORE_RESULT nh_core_overwriteGlobalConfigSettingInt(
        NH_BYTE *namespace_p, int _module, const NH_BYTE *setting_p, int value 
    );

/** @} */

#endif // NH_CORE_CONFIG_CONFIG_H
