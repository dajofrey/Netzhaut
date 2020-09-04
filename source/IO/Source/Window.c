// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Window.h"
#include "../Header/Macros.h"

#include "../Win/Header/Window.h"
#include "../X11/Header/Window.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __unix__
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/Xutil.h>
#elif defined(_WIN32) || defined (WIN32)
    #include <windows.h>
    #include <windowsx.h>
    #include <winuser.h>
    #include <Shellapi.h>
#endif

// CREATE ==========================================================================================

NH_RESULT Nh_IO_createWindow(
    char *title_p, int width, int height, NH_IO_Window *Window_p)
{
NH_BEGIN();

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11 : NH_CHECK(NH_IO_X11_createWindow(title_p, width, height, &Window_p->X11)) break;
        case NH_IO_TYPE_WIN : NH_CHECK(NH_IO_Win_createWindow(title_p, width, height, &Window_p->Win)) break;
        default             : NH_END(NH_ERROR_BAD_STATE)
    }

NH_END(NH_SUCCESS);
}

// DESTROY =========================================================================================

NH_RESULT Nh_IO_destroyWindow(
    NH_IO_Window *Window_p)
{
NH_BEGIN()

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11 : NH_CHECK(NH_IO_X11_destroyWindow(&Window_p->X11)) break;
        case NH_IO_TYPE_WIN : NH_CHECK(NH_IO_Win_destroyWindow(&Window_p->Win)) break;
        default             : NH_END(NH_ERROR_BAD_STATE)
    }

NH_END(NH_SUCCESS)
}

// SET =============================================================================================

NH_RESULT Nh_IO_setWindowBackgroundColor(
    NH_IO_Window *Window_p, float color_p[4])
{
NH_BEGIN()

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11 : NH_CHECK(NH_IO_X11_setWindowBackgroundColor(&Window_p->X11, color_p)) break;
        case NH_IO_TYPE_WIN : NH_CHECK(NH_IO_Win_setWindowBackgroundColor(&Window_p->Win, color_p)) break;
        default             : NH_END(NH_ERROR_BAD_STATE)
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_IO_setWindowTitle(
    NH_IO_Window *Window_p, char *title_p)
{
NH_BEGIN()

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11 : NH_CHECK(NH_IO_X11_setWindowTitle(&Window_p->X11, title_p)) break;
        case NH_IO_TYPE_WIN : NH_CHECK(NH_IO_Win_setWindowTitle(&Window_p->Win, title_p)) break;
        default             : NH_END(NH_ERROR_BAD_STATE)
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_IO_setMouseCursor(
    NH_IO_Window *Window_p, char *title_p)
{
NH_BEGIN()

    switch (Window_p->type)
    {
        case NH_IO_TYPE_X11 : NH_CHECK(NH_IO_X11_setMouseCursor(&Window_p->X11, title_p)) break;
        case NH_IO_TYPE_WIN : NH_CHECK(NH_IO_Win_setMouseCursor(&Window_p->Win, title_p)) break;
        default             : NH_END(NH_ERROR_BAD_STATE)
    }

NH_END(NH_SUCCESS)
}

// GET =============================================================================================

NH_IO_TYPE Nh_IO_getType()
{
NH_BEGIN()

    NH_IO_TYPE type = -1;

    #ifdef __unix__
        type = NH_IO_TYPE_X11;
    #elif defined(_WIN32) || defined (WIN32)
        type = NH_IO_TYPE_WIN;
    #endif

NH_END(type)
}

