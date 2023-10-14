// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Window.h"
#include "WindowSettings.h"
#include "Listener.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Thread.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/String.h"
#include "../../nhcore/Util/LinkedList.h"
#include "../../nhcore/Util/RingBuffer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// CREATE ==========================================================================================

/**
 * Related resources:
 * - https://unix.stackexchange.com/questions/202891/how-to-know-whether-wayland-or-x11-is-being-used
 */
static NH_WSI_TYPE_E nh_wsi_getType()
{
NH_WSI_BEGIN()

    NH_WSI_TYPE_E type = -1;

    #ifdef __unix__
        type = NH_WSI_TYPE_X11;
    #elif defined(_WIN32) || defined (WIN32)
        type = NH_WSI_TYPE_WIN32;
    #endif

NH_WSI_END(type)
}

nh_wsi_Window *nh_wsi_createWindow(
    NH_BYTE namespace_p[255], nh_gfx_SurfaceRequirements *Requirements_p)
{
NH_WSI_BEGIN();

    nh_wsi_Window *Window_p = nh_core_allocate(sizeof(nh_wsi_Window));
    NH_WSI_CHECK_MEM_2(NULL, Window_p)
    memset(Window_p, 0, sizeof(nh_wsi_Window));

    Window_p->type = nh_wsi_getType();

    if (namespace_p && strlen(namespace_p) > 0) {
        strcpy(Window_p->namespace_p, namespace_p);
    } else {
        sprintf(Window_p->namespace_p, "%p", Window_p);
    }

    nh_core_initRingBuffer(&Window_p->Events, 1024, sizeof(nh_wsi_Event), NULL);

    if (Window_p->type < 0) {
        nh_core_free(Window_p);
        NH_WSI_END(NULL)
    }

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11 : 
            Window_p->X11.Common_p = &NH_WSI_X11;
            NH_WSI_CHECK_2(NULL, nh_x11_createWindow(&Window_p->X11, nh_wsi_getWindowConfig(Window_p), Requirements_p))
            break;
        default : NH_WSI_END(NULL)
    }

    if (nh_wsi_enableWindowListener(Window_p) != NH_WSI_SUCCESS) {
        nh_core_free(Window_p);
        NH_WSI_END(NULL)
    }

    nh_wsi_configureWindow(Window_p);

NH_WSI_END(Window_p);
}

// DESTROY =========================================================================================

NH_WSI_RESULT_E nh_wsi_destroyWindow(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    nh_wsi_disableWindowListener(Window_p);

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11 : NH_WSI_CHECK(nh_x11_destroyWindow(&Window_p->X11)) break;
        default              : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

    nh_core_freeRingBuffer(&Window_p->Events);
    nh_core_free(Window_p);

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

// SET =============================================================================================

NH_WSI_RESULT_E nh_wsi_setEventListener(
    nh_wsi_Window *Window_p, nh_wsi_callback_f callback_f)
{
NH_WSI_BEGIN()

    Window_p->callback_f = callback_f;

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

NH_WSI_RESULT_E nh_wsi_moveWindow(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    switch (Window_p->type)
    {
        case NH_WSI_TYPE_X11 : NH_WSI_CHECK(nh_x11_moveWindow(&Window_p->X11)) break;
        default              : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}


