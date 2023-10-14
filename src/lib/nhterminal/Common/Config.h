#ifndef NH_TERMINAL_CONFIG_H
#define NH_TERMINAL_CONFIG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#include <stddef.h>

#endif

/** @addtogroup lib_nhterminal_enums
 *  @{
 */

    typedef enum NH_TERMINAL_SETTING_E {
        NH_TERMINAL_SETTING_FONT_SIZE,
        NH_TERMINAL_SETTING_BLINK_FREQUENCY,
        NH_TERMINAL_SETTING_FOREGROUND,
        NH_TERMINAL_SETTING_BACKGROUND,
        NH_TERMINAL_SETTING_ACCENT,
    } NH_TERMINAL_SETTING_E;

/** @} */

/** @addtogroup lib_nhterminal_structs
 *  @{
 */

    typedef struct nh_terminal_Config {
        int fontSize;
        float blinkFrequency;
        nh_Color Foreground;
        nh_Color Background;
        nh_Color Accents_p[8];
        int accents;
    } nh_terminal_Config;

/** @} */

/** @addtogroup lib_nhterminal_vars
 *  @{
 */

    extern const NH_BYTE *NH_TERMINAL_SETTING_NAMES_PP[];
    extern size_t NH_TERMINAL_SETTING_NAMES_PP_COUNT;

/** @} */

/** @addtogroup lib_nhterminal_functions
 *  @{
 */

    const NH_BYTE *nh_terminal_getSettingName(
        NH_TERMINAL_SETTING_E setting
    );

    nh_terminal_Config nh_terminal_getConfig(
    );

/** @} */

#endif
