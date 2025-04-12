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

#include "../../nh-core/System/Thread.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/String.h"
#include "../../nh-core/Util/LinkedList.h"
#include "../../nh-core/Util/RingBuffer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// FUNCTIONS =======================================================================================

/**
 * Related resources:
 * - https://unix.stackexchange.com/questions/202891/how-to-know-whether-wayland-or-x11-is-being-used
 */
static NH_WSI_TYPE_E nh_wsi_getType()
{
    NH_WSI_TYPE_E type = -1;

    #if defined(__unix__)
        type = NH_WSI_TYPE_X11;
    #elif defined(_WIN32) || defined (WIN32)
        type = NH_WSI_TYPE_WIN32;
    #elif defined(__APPLE__)
        type = NH_WSI_TYPE_COCOA;
    #endif

    return type;
}

nh_wsi_Window *nh_wsi_createWindow(
    char *namespace_p, nh_gfx_SurfaceRequirements *Requirements_p)
{
    nh_wsi_Window *Window_p = nh_core_allocate(sizeof(nh_wsi_Window));
    NH_CORE_CHECK_MEM_2(NULL, Window_p)
    memset(Window_p, 0, sizeof(nh_wsi_Window));

    Window_p->type = nh_wsi_getType();

    if (namespace_p && strlen(namespace_p) > 0) {
        strcpy(Window_p->namespace_p, namespace_p);
    } else {
        sprintf(Window_p->namespace_p, "%p", Window_p);
    }

    nh_core_initRingBuffer(&Window_p->Events, 1024, sizeof(nh_api_WSIEvent), NULL);

    if (Window_p->type < 0) {
        nh_core_free(Window_p);
        return NULL;
    }

    switch (Window_p->type)
    {
#if defined(__unix__)
        case NH_WSI_TYPE_X11 : 
            Window_p->X11.Common_p = &NH_WSI_X11;
            NH_CORE_CHECK_2(NULL, nh_wsi_createX11Window(&Window_p->X11, nh_wsi_getWindowConfig(Window_p), Requirements_p))
            break;
#elif defined(__APPLE__)
        case NH_WSI_TYPE_COCOA : 
            NH_CORE_CHECK_2(NULL, nh_wsi_createCocoaWindow(&Window_p->Cocoa, nh_wsi_getWindowConfig(Window_p), Requirements_p))
            break;
#endif
        default : return NULL;
    }

    if (nh_wsi_enableWindowListener(Window_p) != NH_API_SUCCESS) {
        nh_core_free(Window_p);
        return NULL;
    }

    nh_wsi_configureWindow(Window_p);

    return Window_p;
}

NH_API_RESULT nh_wsi_destroyWindow(
    nh_wsi_Window *Window_p)
{
    nh_wsi_disableWindowListener(Window_p);

    switch (Window_p->type)
    {
#if defined(__unix__)
        case NH_WSI_TYPE_X11   : NH_CORE_CHECK(nh_wsi_destroyX11Window(&Window_p->X11)) break;
#elif defined(__APPLE__)
        case NH_WSI_TYPE_COCOA : NH_CORE_CHECK(nh_wsi_destroyCocoaWindow(&Window_p->Cocoa)) break;
#endif
        default                : return NH_API_ERROR_BAD_STATE;
    }

    nh_core_freeRingBuffer(&Window_p->Events);
    nh_core_free(Window_p);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setEventListener(
    nh_wsi_Window *Window_p, nh_api_windowCallback_f callback_f)
{
    Window_p->callback_f = callback_f;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_moveWindow(
    nh_wsi_Window *Window_p)
{
    switch (Window_p->type)
    {
#if defined(__unix__)
        case NH_WSI_TYPE_X11   : return nh_wsi_moveX11Window(&Window_p->X11);
#elif defined(__APPLE__)
        case NH_WSI_TYPE_COCOA : return nh_wsi_moveCocoaWindow(&Window_p->Cocoa);
#endif
        default                : return NH_API_ERROR_BAD_STATE;
    }
}
