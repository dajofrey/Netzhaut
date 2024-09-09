#ifndef NH_WSI_CONFIG_H
#define NH_WSI_CONFIG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#include <stddef.h>

#endif

/** @addtogroup lib_nh-wsi_enums
 *  @{
 */

    typedef enum NH_WSI_WINDOW_DECORATION_E {
        NH_WSI_WINDOW_DECORATION_NONE,
        NH_WSI_WINDOW_DECORATION_ALL,
        NH_WSI_WINDOW_DECORATION_BORDER,
        NH_WSI_WINDOW_DECORATION_TITLE_BAR,
        NH_WSI_WINDOW_DECORATION_MENU,
        NH_WSI_WINDOW_DECORATION_MINIMIZE,
        NH_WSI_WINDOW_DECORATION_MAXIMIZE,
        NH_WSI_WINDOW_DECORATION_E_COUNT,
    } NH_WSI_WINDOW_DECORATION_E;

    typedef enum NH_WSI_WINDOW_ACTION_E {
        NH_WSI_WINDOW_ACTION_ALL,
        NH_WSI_WINDOW_ACTION_RESIZE,
        NH_WSI_WINDOW_ACTION_MOVE,
        NH_WSI_WINDOW_ACTION_MINIMIZE,
        NH_WSI_WINDOW_ACTION_MAXIMIZE,
        NH_WSI_WINDOW_ACTION_CLOSE,
        NH_WSI_WINDOW_ACTION_E_COUNT,
    } NH_WSI_WINDOW_ACTION_E;

    typedef enum NH_WSI_WINDOW_TYPE_E {
        NH_WSI_WINDOW_TYPE_NORMAL,
        NH_WSI_WINDOW_TYPE_DESKTOP,
        NH_WSI_WINDOW_TYPE_DOCK,
        NH_WSI_WINDOW_TYPE_TOOLBAR,
        NH_WSI_WINDOW_TYPE_UTILITY,
        NH_WSI_WINDOW_TYPE_SPLASH,
        NH_WSI_WINDOW_TYPE_DIALOG,
        NH_WSI_WINDOW_TYPE_E_COUNT,
    } NH_WSI_WINDOW_TYPE_E;

    typedef enum NH_WSI_WINDOW_STATE_E {
        NH_WSI_WINDOW_STATE_ABOVE,     /**<Always on top of other windows.*/
        NH_WSI_WINDOW_STATE_MAXIMIZED, /**<Maximised.*/
        NH_WSI_WINDOW_STATE_E_COUNT,
    } NH_WSI_WINDOW_STATE_E;

    typedef enum NH_WSI_SETTING_E {
        NH_WSI_SETTING_WINDOW_TITLE,
        NH_WSI_SETTING_WINDOW_WIDTH,
        NH_WSI_SETTING_WINDOW_HEIGHT,
        NH_WSI_SETTING_WINDOW_X,
        NH_WSI_SETTING_WINDOW_Y,
        NH_WSI_SETTING_WINDOW_DECORATED,
        NH_WSI_SETTING_WINDOW_RESIZABLE,
        NH_WSI_SETTING_WINDOW_ACTION,
        NH_WSI_SETTING_WINDOW_STATE,
        NH_WSI_SETTING_WINDOW_TYPE,
    } NH_WSI_SETTING_E;

/** @} */

/** @addtogroup lib_nh-wsi_structs
 *  @{
 */

    typedef struct nh_wsi_WindowConfig {
        NH_BYTE namespace_p[255];
        NH_BYTE title_p[255];
        nh_PixelSize Size;
        nh_PixelPosition Position;
        double delay;
        NH_BOOL decorated;
        NH_BOOL resizable;
        NH_BOOL decoration_p[NH_WSI_WINDOW_DECORATION_E_COUNT];
        NH_BOOL action_p[NH_WSI_WINDOW_ACTION_E_COUNT];
        NH_BOOL state_p[NH_WSI_WINDOW_STATE_E_COUNT];
        NH_WSI_WINDOW_TYPE_E type;
    } nh_wsi_WindowConfig;
 
/** @} */

/** @addtogroup lib_nh-wsi_vars
 *  @{
 */

    extern const NH_BYTE *NH_WSI_SETTING_NAMES_PP[];
    extern size_t NH_WSI_SETTING_NAMES_PP_COUNT;

/** @} */

/** @addtogroup lib_nh-wsi_functions
 *  @{
 */

    const NH_BYTE *nh_wsi_getSettingName(
        NH_WSI_SETTING_E setting
    );

    nh_wsi_WindowConfig nh_wsi_getWindowConfig(
        void *Window_p
    );
 
/** @} */

#endif
