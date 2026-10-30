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

static NH_WSI_TYPE_E nh_wsi_getType()
{
    NH_WSI_TYPE_E type = -1;

#if defined(NH_PLATFORM_UNIX)
    type = NH_WSI_TYPE_X11;
#elif defined(_WIN32) || defined(WIN32)
    type = NH_WSI_TYPE_WIN32;
#elif defined(NH_PLATFORM_IOS)
    type = NH_WSI_TYPE_IOS;
#elif defined(NH_PLATFORM_ANDROID)
    type = NH_WSI_TYPE_ANDROID;
#elif defined(NH_PLATFORM_MACOS)
    type = NH_WSI_TYPE_COCOA;
#endif

    return type;
}

nh_wsi_Window *nh_wsi_createWindow(
    char *namespace_p, nh_gfx_SurfaceRequirements *Requirements_p)
{
    nh_wsi_Window *Window_p = (nh_wsi_Window*)nh_core_allocate(sizeof(nh_wsi_Window));
    NH_CORE_CHECK_MEM_2(NULL, Window_p)
    memset(Window_p, 0, sizeof(nh_wsi_Window));

    Window_p->callback_f = NULL;
    Window_p->type = nh_wsi_getType();
    Window_p->scale = 1.0f;

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
#if defined(NH_PLATFORM_UNIX)
        case NH_WSI_TYPE_X11 :
            NH_CORE_CHECK_2(NULL, nh_wsi_createX11Window(&Window_p->X11, nh_wsi_getWindowConfig(Window_p), Requirements_p))
            break;
#elif defined(NH_PLATFORM_MACOS)
        case NH_WSI_TYPE_COCOA :
            NH_CORE_CHECK_2(NULL, nh_wsi_createCocoaWindow(Window_p, nh_wsi_getWindowConfig(Window_p), Requirements_p))
            break;
#elif defined(NH_PLATFORM_IOS)
        case NH_WSI_TYPE_IOS :
            NH_CORE_CHECK_2(NULL, nh_wsi_createIOSWindow(Window_p, nh_wsi_getWindowConfig(Window_p), Requirements_p))
            break;
#elif defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID :
            NH_CORE_CHECK_2(NULL, nh_wsi_createAndroidWindow(Window_p, nh_wsi_getWindowConfig(Window_p), Requirements_p))
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
#if defined(NH_PLATFORM_UNIX)
        case NH_WSI_TYPE_X11   : NH_CORE_CHECK(nh_wsi_destroyX11Window(&Window_p->X11)) break;
#elif defined(NH_PLATFORM_MACOS)
        case NH_WSI_TYPE_COCOA : NH_CORE_CHECK(nh_wsi_destroyCocoaWindow(&Window_p->Cocoa)) break;
#elif defined(NH_PLATFORM_IOS)
        case NH_WSI_TYPE_IOS   : NH_CORE_CHECK(nh_wsi_destroyIOSWindow(&Window_p->IOS)) break;
#elif defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : NH_CORE_CHECK(nh_wsi_destroyAndroidWindow(&Window_p->Android)) break;
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
#if defined(NH_PLATFORM_UNIX)
        case NH_WSI_TYPE_X11   : return nh_wsi_moveX11Window(&Window_p->X11);
#elif defined(NH_PLATFORM_MACOS)
        case NH_WSI_TYPE_COCOA : return nh_wsi_moveCocoaWindow(&Window_p->Cocoa);
#elif defined(NH_PLATFORM_IOS)
        case NH_WSI_TYPE_IOS   : return nh_wsi_moveIOSWindow(&Window_p->IOS);
#elif defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_moveAndroidWindow(&Window_p->Android);
#endif
        default                : return NH_API_ERROR_BAD_STATE;
    }
}

NH_API_RESULT nh_wsi_getWindowSize(
    nh_wsi_Window *Window_p, int *x_p, int *y_p)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_UNIX)
        case NH_WSI_TYPE_X11   : return nh_wsi_getX11WindowSize(&Window_p->X11, x_p, y_p);
#elif defined(NH_PLATFORM_MACOS)
        case NH_WSI_TYPE_COCOA : return nh_wsi_getCocoaWindowSize(&Window_p->Cocoa, x_p, y_p);
#elif defined(NH_PLATFORM_IOS)
        case NH_WSI_TYPE_IOS   : return nh_wsi_getIOSWindowSize(&Window_p->IOS, x_p, y_p);
#elif defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_getAndroidWindowSize(Window_p, x_p, y_p);
#endif
        default                : return NH_API_ERROR_BAD_STATE;
    }
}

NH_API_RESULT nh_wsi_showKeyboard(
    nh_wsi_Window *Window_p)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_IOS)
        case NH_WSI_TYPE_IOS   : return nh_wsi_showIOSKeyboard(&Window_p->IOS);
#elif defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_showAndroidKeyboard(&Window_p->Android);
#endif
        default                : return NH_API_ERROR_BAD_STATE;
    }
}

NH_API_RESULT nh_wsi_hideKeyboard(
    nh_wsi_Window *Window_p)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_IOS)
        case NH_WSI_TYPE_IOS     : return nh_wsi_hideIOSKeyboard(&Window_p->IOS);
#elif defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_hideAndroidKeyboard(&Window_p->Android);
#endif
        default                  : return NH_API_ERROR_BAD_STATE;
    }
}

NH_API_RESULT nh_wsi_setEPaperRawDrawingEnabled(
    nh_wsi_Window *Window_p, bool enabled)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_setAndroidRawDrawingEnabled(&Window_p->Android.Onyx, enabled);
#endif
        default : return NH_API_ERROR_BAD_STATE;
    }
}

NH_API_RESULT nh_wsi_setEPaperRawDrawingRenderEnabled(
    nh_wsi_Window *Window_p, bool enabled)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_setAndroidRawDrawingRenderEnabled(&Window_p->Android.Onyx, enabled);
#endif
        default : return NH_API_ERROR_BAD_STATE;
    }
}

NH_API_RESULT nh_wsi_setEPaperRawDrawingLimitRect(
    nh_wsi_Window *Window_p, int left, int top, int right, int bottom,
    const nh_api_Rect *excludeRects_p, int excludeCount)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_setAndroidRawDrawingLimitRect(
            &Window_p->Android.Onyx, left, top, right, bottom, excludeRects_p, excludeCount);
#endif
        default : return NH_API_ERROR_BAD_STATE;
    }
}

NH_API_RESULT nh_wsi_setEPaperRawDrawingEraserEnabled(
    nh_wsi_Window *Window_p, bool enabled)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_setAndroidRawDrawingEraserEnabled(&Window_p->Android.Onyx, enabled);
#endif
        default : return NH_API_ERROR_BAD_STATE;
    }
}

NH_API_RESULT nh_wsi_setEPaperRawDrawingStrokeWidth(
    nh_wsi_Window *Window_p, float width)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_ANDROID) 
        case NH_WSI_TYPE_ANDROID : return nh_wsi_setAndroidRawDrawingStrokeWidth(&Window_p->Android.Onyx, width);
#endif  
        default : return NH_API_ERROR_BAD_STATE;
    }

}

NH_API_RESULT nh_wsi_setEPaperRawDrawingStrokeStyle(
    nh_wsi_Window *Window_p, int style)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_ANDROID) 
        case NH_WSI_TYPE_ANDROID : return nh_wsi_setAndroidRawDrawingStrokeStyle(&Window_p->Android.Onyx, style);
#endif  
        default : return NH_API_ERROR_BAD_STATE;
    }

}

NH_API_RESULT nh_wsi_setEPaperUpdateMode(
    nh_wsi_Window *Window_p, int mode)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_setAndroidEPaperUpdateMode(&Window_p->Android.Onyx, mode);
#endif
        default : return NH_API_ERROR_BAD_STATE;
    }
}

NH_API_RESULT nh_wsi_requestEPaperRefreshRect(
    nh_wsi_Window *Window_p, int left, int top, int right, int bottom, int mode)
{
    switch (Window_p->type)
    {
#if defined(NH_PLATFORM_ANDROID)
        case NH_WSI_TYPE_ANDROID : return nh_wsi_requestAndroidEPaperRefreshRect(
            &Window_p->Android.Onyx, left, top, right, bottom, mode);
#endif
        default : return NH_API_ERROR_BAD_STATE;
    }
}
