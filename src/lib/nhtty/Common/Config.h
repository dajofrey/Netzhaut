#ifndef NH_TTY_CONFIG_H
#define NH_TTY_CONFIG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#include <stddef.h>

#endif

/** @addtogroup lib_nhtty_enums
 *  @{
 */

    typedef enum NH_TTY_SIDEBAR_STATE_E {
        NH_TTY_SIDEBAR_STATE_OFF,
        NH_TTY_SIDEBAR_STATE_LEFT,
        NH_TTY_SIDEBAR_STATE_RIGHT,
    } NH_TTY_SIDEBAR_STATE_E;

    typedef enum NH_TTY_SETTING_E {
        NH_TTY_SETTING_SIDEBAR_TYPE,
        NH_TTY_SETTING_SHELL_MAX_SCROLL,
    } NH_TTY_SETTING_E;

/** @} */

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_SidebarConfig {
        NH_TTY_SIDEBAR_STATE_E state;
    } nh_tty_SidebarConfig;

    typedef struct nh_tty_ShellConfig {
        NH_BOOL maxScroll;
    } nh_tty_ShellConfig;

    typedef struct nh_tty_Config {
        NH_BYTE *name_p;
        nh_tty_SidebarConfig Sidebar;
        nh_tty_ShellConfig Shell;
    } nh_tty_Config;

/** @} */

/** @addtogroup lib_nhtty_vars
 *  @{
 */

    extern const NH_BYTE *NH_TTY_SETTING_NAMES_PP[];
    extern size_t NH_TTY_SETTING_NAMES_PP_COUNT;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_Config nh_tty_getConfig(
    );

    const NH_BYTE *nh_tty_getSettingName(
        NH_TTY_SETTING_E setting
    );

/** @} */

#endif
