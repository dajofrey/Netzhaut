// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-wsi.h"
#include "../nh-core/Loader/Loader.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_api_Window *nh_api_createWindow(
    char *namespace_p, nh_api_SurfaceRequirements *Requirements_p)
{
    typedef nh_api_Window *(*nh_wsi_createWindow_f)(char *namespace_p, nh_api_SurfaceRequirements *Requirements_p);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_createWindow_f createWindow_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_createWindow");
    return createWindow_f ? createWindow_f(namespace_p, Requirements_p) : NULL;
}

#if defined(__ANDROID__)
NH_API_RESULT nh_api_setAndroidApp(
    struct android_app *app_p)
{
    typedef NH_API_RESULT (*nh_wsi_setAndroidApp_f)(struct android_app *app_p);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setAndroidApp_f setAndroidApp_f = !Loader_p ? NULL :
        Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setAndroidApp");
    return setAndroidApp_f ? setAndroidApp_f(app_p) : NH_API_ERROR_BAD_STATE;
}
#endif

NH_API_RESULT nh_api_setWindowEventListener(
    nh_api_Window *Window_p, nh_api_windowCallback_f callback_f)
{
    typedef NH_API_RESULT (*nh_wsi_setEventListener_f)(nh_api_Window *Window_p, nh_api_windowCallback_f callback_f);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setEventListener_f setEventListener_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEventListener");
    return setEventListener_f ? setEventListener_f(Window_p, callback_f) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_moveWindow(
    nh_api_Window *Window_p)
{
    typedef NH_API_RESULT (*nh_wsi_moveWindow_f)(nh_api_Window *Window_p);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_moveWindow_f moveWindow_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_moveWindow");
    return moveWindow_f ? moveWindow_f(Window_p) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_showKeyboard(
    nh_api_Window *Window_p)
{
    typedef NH_API_RESULT (*nh_wsi_showKeyboard_f)(nh_api_Window *Window_p);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_showKeyboard_f showKeyboard_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_showKeyboard");
    return showKeyboard_f ? showKeyboard_f(Window_p) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_hideKeyboard(
    nh_api_Window *Window_p)
{
    typedef NH_API_RESULT (*nh_wsi_hideKeyboard_f)(nh_api_Window *Window_p);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_hideKeyboard_f hideKeyboard_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_hideKeyboard");
    return hideKeyboard_f ? hideKeyboard_f(Window_p) : NH_API_ERROR_BAD_STATE;
}

// EPAPER FUNCTIONS ===============================================================

NH_API_RESULT nh_api_setEPaperRawDrawingEnabled(
    nh_api_Window *Window_p, bool enabled)
{
    typedef NH_API_RESULT (*nh_wsi_setEPaperRawDrawingEnabled_f)(nh_api_Window *Window_p, bool enabled);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setEPaperRawDrawingEnabled_f setEPaperRawDrawingEnabled_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEPaperRawDrawingEnabled");
    return setEPaperRawDrawingEnabled_f ? setEPaperRawDrawingEnabled_f(Window_p, enabled) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_setEPaperRawDrawingRenderEnabled(
    nh_api_Window *Window_p, bool enabled)
{
    typedef NH_API_RESULT (*nh_wsi_setEPaperRawDrawingRenderEnabled_f)(nh_api_Window *Window_p, bool enabled);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setEPaperRawDrawingRenderEnabled_f setEPaperRawDrawingRenderEnabled_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEPaperRawDrawingRenderEnabled");
    return setEPaperRawDrawingRenderEnabled_f ? setEPaperRawDrawingRenderEnabled_f(Window_p, enabled) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_setEPaperRawDrawingLimitRect(
    nh_api_Window *Window_p, int left, int top, int right, int bottom,
    const nh_api_Rect *excludeRects_p, int excludeCount)
{
    typedef NH_API_RESULT (*nh_wsi_setEPaperRawDrawingLimitRect_f)(nh_api_Window *Window_p, int left, int top, int right, int bottom, const nh_api_Rect *excludeRects_p, int excludeCount);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setEPaperRawDrawingLimitRect_f setEPaperRawDrawingLimitRect_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEPaperRawDrawingLimitRect");
    return setEPaperRawDrawingLimitRect_f ? setEPaperRawDrawingLimitRect_f(Window_p, left, top, right, bottom, excludeRects_p, excludeCount) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_setEPaperRawDrawingStrokeWidth(
    nh_api_Window *Window_p, float width)
{
    typedef NH_API_RESULT (*nh_wsi_setEPaperRawDrawingStrokeWidth_f)(nh_api_Window *Window_p, float width);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setEPaperRawDrawingStrokeWidth_f setEPaperRawDrawingStrokeWidth_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEPaperRawDrawingStrokeWidth");
    return setEPaperRawDrawingStrokeWidth_f ? setEPaperRawDrawingStrokeWidth_f(Window_p, width) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_setEPaperRawDrawingStrokeStyle(
    nh_api_Window *Window_p, int style)
{
    typedef NH_API_RESULT (*nh_wsi_setEPaperRawDrawingStrokeStyle_f)(nh_api_Window *Window_p, int style);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setEPaperRawDrawingStrokeStyle_f setEPaperRawDrawingStrokeStyle_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEPaperRawDrawingStrokeStyle");
    return setEPaperRawDrawingStrokeStyle_f ? setEPaperRawDrawingStrokeStyle_f(Window_p, style) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_setEPaperRawDrawingEraserEnabled(
    nh_api_Window *Window_p, bool enabled)
{
    typedef NH_API_RESULT (*nh_wsi_setEPaperRawDrawingEraserEnabled_f)(nh_api_Window *Window_p, bool enabled);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setEPaperRawDrawingEraserEnabled_f setEPaperRawDrawingEraserEnabled_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEPaperRawDrawingEraserEnabled");
    return setEPaperRawDrawingEraserEnabled_f ? setEPaperRawDrawingEraserEnabled_f(Window_p, enabled) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_setEPaperUpdateMode(
    nh_api_Window *Window_p, int mode)
{
    typedef NH_API_RESULT (*nh_wsi_setEPaperUpdateMode_f)(nh_api_Window *Window_p, int mode);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_setEPaperUpdateMode_f setEPaperUpdateMode_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_setEPaperUpdateMode");
    return setEPaperUpdateMode_f ? setEPaperUpdateMode_f(Window_p, mode) : NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_api_requestEPaperRefreshRect(
    nh_api_Window *Window_p, int left, int top, int right, int bottom, int mode)
{
    typedef NH_API_RESULT (*nh_wsi_requestEPaperRefreshRect_f)(nh_api_Window *Window_p, int left, int top, int right, int bottom, int mode);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_wsi_requestEPaperRefreshRect_f requestEPaperRefreshRect_f = !Loader_p || !Window_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WSI, 0, "nh_wsi_requestEPaperRefreshRect");
    return requestEPaperRefreshRect_f ? requestEPaperRefreshRect_f(Window_p, left, top, right, bottom, mode) : NH_API_ERROR_BAD_STATE;
}
