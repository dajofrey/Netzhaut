// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "WindowSettings.h"
#include "Window.h"

#include "../Common/Macros.h"
#include "../Platforms/X11/WindowSettings.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Config/Config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// SET =============================================================================================

//static NH_WSI_RESULT_E nh_wsi_setWindowBackgroundColor(
//    nh_wsi_Window *Window_p, nh_Color Color)
//{
//NH_WSI_BEGIN()
//
//    switch (Window_p->type)
//    {
//        case NH_WSI_TYPE_X11 : NH_WSI_DIAGNOSTIC_END(nh_x11_setWindowBackgroundColor(&Window_p->X11, Color))
//    }
//
//NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
//}

static NH_WSI_RESULT_E nh_wsi_setWindowDecorated(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    nh_wsi_WindowConfig Config = nh_wsi_getWindowConfig(Window_p);

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11 : 
            NH_WSI_DIAGNOSTIC_END(nh_x11_setWindowDecorated(&Window_p->X11, Config.decorated))
            break;
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

static NH_WSI_RESULT_E nh_wsi_setWindowState(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    nh_wsi_WindowConfig Config = nh_wsi_getWindowConfig(Window_p);

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11 : NH_WSI_DIAGNOSTIC_END(nh_x11_setWindowState(&Window_p->X11, Config.state_p))
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

static NH_WSI_RESULT_E nh_wsi_setWindowType(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    nh_wsi_WindowConfig Config = nh_wsi_getWindowConfig(Window_p);

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11 : NH_WSI_DIAGNOSTIC_END(nh_x11_setWindowType(&Window_p->X11, Config.type))
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

static NH_WSI_RESULT_E nh_wsi_setWindowTitle(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    nh_wsi_WindowConfig Config = nh_wsi_getWindowConfig(Window_p);

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11 : NH_WSI_DIAGNOSTIC_END(nh_x11_setWindowTitle(&Window_p->X11, Config.title_p))
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

NH_WSI_RESULT_E nh_wsi_setMouseCursor(
    nh_wsi_Window *Window_p, NH_WSI_CURSOR_E type)
{
NH_WSI_BEGIN()

    if (Window_p->cursor == type) {NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)}
    Window_p->cursor = type;

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11 : NH_WSI_CHECK(nh_x11_setMouseCursor(&Window_p->X11, type)) break;
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

// CONFIGURE =======================================================================================

NH_WSI_RESULT_E nh_wsi_configureWindow(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    NH_WSI_CHECK(nh_wsi_setWindowTitle(Window_p))
    NH_WSI_CHECK(nh_wsi_setWindowDecorated(Window_p))

NH_WSI_END(NH_WSI_SUCCESS)
}

NH_WSI_RESULT_E nh_wsi_toggleWindowSize(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    // Update config.
    nh_wsi_WindowConfig Config = nh_wsi_getWindowConfig(Window_p);

    nh_core_overwriteGlobalConfigSetting(Window_p->namespace_p, NH_MODULE_WSI, nh_wsi_getSettingName(NH_WSI_SETTING_WINDOW_STATE), Config.state_p[NH_WSI_WINDOW_STATE_MAXIMIZED] ? "none" : "maximized");

    // Update window.
    NH_WSI_CHECK(nh_wsi_setWindowState(Window_p))

NH_WSI_END(NH_WSI_SUCCESS)
}

