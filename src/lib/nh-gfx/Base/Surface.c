// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Surface.h"
#include "Viewport.h"

#if defined(_WIN32) || defined (WIN32) || defined(__unix__)
    #include "../Vulkan/Vulkan.h"
    #include "../Vulkan/Render.h"
#endif

#include "../OpenGL/Render.h"
#include "../Common/Macros.h"
#include "../Common/Config.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Thread.h"

#include "../../nh-wsi/Window/Window.h"

#include <string.h>
#include <limits.h>

// FUNCTIONS =======================================================================================

//static NH_API_RESULT nh_resize(
//    nh_Window *Window_p)
//{
//    nh_syncResizeBegin(Window_p);
//
//    switch (Window_p->API) 
//    { 
//        case NH_API_VULKAN : NH_CORE_CHECK(nh_gfx_resizeVulkanSurface(Window_p)) break;
//    }
//
//    for (int i = 0; i < Window_p->Tabs.count; ++i) {
//        NH_CORE_CHECK(nh_resizeTab(nh_core_getTab(Window_p, i)))
//    }
//
//    nh_syncResizeEnd(Window_p);
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}

typedef struct nh_gfx_SurfaceArgs {
    nh_wsi_Window *Window_p;
    NH_GFX_API_E api;
} nh_gfx_SurfaceArgs;

static void *nh_gfx_initSurface(
    nh_core_Workload *Workload_p)
{
    static char *name_p = "Graphics Surface";
    static char *path_p = "nh-gfx/Main/Surface.c";
    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_GFX;

    nh_gfx_SurfaceArgs *Args_p = Workload_p->args_p;

    NH_CORE_CHECK_NULL_2(NULL, Args_p)
    NH_CORE_CHECK_NULL_2(NULL, Args_p->Window_p)

    nh_gfx_Surface *Surface_p = (nh_gfx_Surface*)nh_core_allocate(sizeof(nh_gfx_Surface));
    NH_CORE_CHECK_NULL_2(NULL, Surface_p)

    Surface_p->args_p = NULL;
    Surface_p->signal = NH_SIGNAL_UPDATE;

    Surface_p->renderRequests = 0;
    Surface_p->api      = Args_p->api;
    Surface_p->Window_p = Args_p->Window_p;

    Surface_p->Viewports = nh_core_initList(8);

    Surface_p->Internal.atomic       = false;
    Surface_p->Internal.skipRender   = false;
    Surface_p->Internal.getNextImage = true;

    Surface_p->Sync.halt         = false;
    Surface_p->Sync.ready        = false;
    Surface_p->Sync.resize       = false;
    Surface_p->Sync.rendering    = false;
    Surface_p->Sync.atomicRender = false;

    Surface_p->Settings.BackgroundColor.r = 0.0f;
    Surface_p->Settings.BackgroundColor.g = 0.0f;
    Surface_p->Settings.BackgroundColor.b = 0.0f;
    Surface_p->Settings.BackgroundColor.a = 1.0f;

#if defined(__unix__)
    Surface_p->Vulkan = nh_gfx_initVulkanSurface();
#endif
    Surface_p->OpenGL = nh_gfx_initOpenGLSurface();

    switch (Surface_p->api)
    {
        case NH_GFX_API_VULKAN : 
#if defined(_WIN32) || defined (WIN32) || defined(__unix__)
            if (NH_VULKAN.GPUs.size <= 0) {return NULL;}
            NH_CORE_CHECK_2(NULL, 
                nh_gfx_createVulkanSurface(&Surface_p->Vulkan, (nh_api_Window*)Surface_p->Window_p, NH_VULKAN.GPUs.pp[0]))
            break;
#else
            return NULL;
#endif
        case NH_GFX_API_OPENGL : 
            NH_CORE_CHECK_2(NULL, nh_gfx_createOpenGLSurface(&Surface_p->OpenGL, (nh_api_Window*)Surface_p->Window_p)) 
            break;

        case NH_GFX_API_METAL :
#if defined(__APPLE__)
//            NH_CORE_CHECK_2(NULL, nh_gfx_createMetalSurface(&Surface_p->Metal, (nh_api_Window*)Surface_p->Window_p))
            break;
#else
            return NULL;
#endif
        default : return NULL;
    }

    Surface_p->Sync.ready = true;

    return Surface_p;
}

void nh_gfx_freeSurface(
    void *surface_p)
{
    nh_gfx_Surface *Surface_p = surface_p;

    for (int i = 0; i < Surface_p->Viewports.size; ++i) {
        nh_gfx_destroyViewport(Surface_p, Surface_p->Viewports.pp[i]);
    }
    nh_core_freeList(&Surface_p->Viewports, false);
 
    switch (Surface_p->api)
    {
        case NH_GFX_API_VULKAN : 
#if defined(_WIN32) || defined (WIN32) || defined(__unix__)
            nh_gfx_destroyVulkanSurface(&Surface_p->Vulkan, true);
#endif
            break;
        case NH_GFX_API_OPENGL : 
            nh_gfx_destroyOpenGLSurface(&Surface_p->OpenGL, (nh_api_Window*)Surface_p->Window_p);
            break;
    }

    nh_core_free(Surface_p);

    return;
}

static NH_API_RESULT nh_gfx_getSortedViewports(
    nh_gfx_Surface *Surface_p, nh_core_List *SortedViewports_p)
{
    nh_core_List Viewports = nh_core_initList(16);
    
    // find out which viewports to render
    for (int i = 0; i < Surface_p->Viewports.size; ++i) {
        if (((nh_gfx_Viewport*)Surface_p->Viewports.pp[i])->Sync.render) {
            nh_core_appendToList(&Viewports, Surface_p->Viewports.pp[i]);
        }
    }

    int *sortedIndices_p = (int*)nh_core_allocate(sizeof(int) * Viewports.size);
    NH_CORE_CHECK_MEM(sortedIndices_p)

    // sort viewports based on priority (from low to high priority)
    int max = 0;
    for (int i = 0; i < Viewports.size; ++i) {
        nh_gfx_Viewport *Viewport_p = Viewports.pp[i];
        if (Viewport_p->Settings.priority > max) {max = Viewport_p->Settings.priority;}
    }
    
    for (int i = 0; i < Viewports.size; ++i) 
    {
        int diff = max, newMax = -1;
        for (int j = 0; j < Viewports.size; ++j) 
        {
            nh_gfx_Viewport *Viewport_p = Viewports.pp[j];
            if ((Viewport_p->Settings.priority - max) == 0) {sortedIndices_p[i] = j;}
            else if ((max - Viewport_p->Settings.priority) <= diff) {
                diff = max - Viewport_p->Settings.priority; 
                newMax = Viewport_p->Settings.priority;
            }
        }
        max = newMax;
    }

    for (int i = 0; i < Viewports.size; ++i) {
        nh_core_appendToList(SortedViewports_p, Viewports.pp[sortedIndices_p[i]]);
    }

    nh_core_free(sortedIndices_p);
    nh_core_freeList(&Viewports, false);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_prepareRendering(
    nh_gfx_Surface *Surface_p)
{
    NH_API_RESULT result = NH_API_ERROR_BAD_STATE;

    switch (Surface_p->api)
    {
        case NH_GFX_API_VULKAN : 
#if defined(_WIN32) || defined (WIN32) || defined(__unix__)
            result = nh_gfx_prepareVulkanRendering(&Surface_p->Vulkan); 
            break;
#else
            return NH_API_ERROR_BAD_STATE;
#endif
        case NH_GFX_API_OPENGL :
            result = NH_API_SUCCESS;
            break;
    }

    if (result == NH_API_VULKAN_ERROR_OUT_OF_DATE_KHR) {
        result = NH_API_ERROR_RESIZE_NEEDED;
    }

    return result;
}

static NH_API_RESULT nh_gfx_render(
    nh_gfx_Surface *Surface_p)
{
    nh_core_List SortedViewports = nh_core_initList(16);
    NH_CORE_CHECK(nh_gfx_getSortedViewports(Surface_p, &SortedViewports))

    switch (Surface_p->api)
    {
        case NH_GFX_API_VULKAN : 
#if defined(_WIN32) || defined (WIN32) || defined(__unix__)
            NH_CORE_CHECK(nh_gfx_renderVulkan(Surface_p, &SortedViewports))
            break;
#else
            return NH_API_ERROR_BAD_STATE;
#endif
        case NH_GFX_API_OPENGL : 
            NH_CORE_CHECK(nh_gfx_renderOpenGL(Surface_p, &SortedViewports))
            break;

        case NH_GFX_API_METAL :
#if defined(__APPLE__)
//            NH_CORE_CHECK(nh_gfx_renderMetal(Surface_p, &SortedViewports))
            break;
#else
            return NH_API_ERROR_BAD_STATE;
#endif
        default : return NH_API_ERROR_BAD_STATE;
    }

    nh_core_freeList(&SortedViewports, false);

    return NH_API_SUCCESS;
}

static NH_SIGNAL nh_gfx_runSurface(
    void *args_p)
{
    NH_CORE_CHECK_NULL_2(NH_SIGNAL_ERROR, args_p)
    nh_gfx_Surface *Surface_p = args_p;
    bool idle = true;

    switch (Surface_p->signal)
    {
        case NH_SIGNAL_UPDATE : 
        {
            if (Surface_p->Viewports.size > 0) 
            {
                if (Surface_p->Internal.getNextImage)
                {
                    NH_API_RESULT prepareResult = nh_gfx_prepareRendering(Surface_p);

                    if (prepareResult == NH_API_ERROR_RESIZE_NEEDED) {
                        // TODO handle resize
                    }
                    else if (prepareResult != NH_API_SUCCESS) {
                        return NH_SIGNAL_ERROR;
                    }
                    else {
                        Surface_p->Internal.getNextImage = false;
                    }
                }
                if (Surface_p->renderRequests)
                {
                    nh_gfx_render(Surface_p);
                    Surface_p->Internal.getNextImage = true;
                    Surface_p->renderRequests--;
                    idle = false;
                }
            } 

            break;
        }
    }

    return idle ? NH_SIGNAL_IDLE : NH_SIGNAL_OK;
}

nh_gfx_Surface *nh_gfx_createSurface(
    nh_wsi_Window *Window_p)
{
    if (Window_p->surface_p) {
        return NULL;
    }

    nh_gfx_SurfaceArgs Args;
    Args.Window_p = Window_p;
    Args.api = nh_gfx_getConfig().api;

    nh_gfx_Surface *Surface_p = nh_core_activateWorkload(nh_gfx_initSurface, nh_gfx_runSurface, nh_gfx_freeSurface, NULL, &Args, true);

    Window_p->surface_p = Surface_p;

    return Surface_p;
}

//static NH_API_RESULT nh_core_sendWindowSignal(
//    nh_Window *Window_p, void *args_p, NH_SIGNAL signal)
//{
//    NH_SIGNAL previous = Window_p->signal;
//    Window_p->signal = signal;
//    Window_p->args_p = args_p;
//
//    if (nh_core_getThreadFromArgs(Window_p) == nh_core_getThread()) {
//        NH_CORE_CHECK(nh_core_runWorkload(nh_core_getWorkloadFromArgs(Window_p)))
//    }
//    else {
//        while (Window_p->signal != NH_SIGNAL_DONE) {
//            if (Window_p->signal == NH_SIGNAL_ERROR) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
//        }
//    }
//
//    Window_p->signal = previous;
//    Window_p->args_p = NULL;
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}

//NH_API_RESULT nh_setWindowBackgroundColor(
//    nh_Window *Window_p, nh_css_Color Color)
//{
//    NH_CORE_CHECK_NULL(Window_p)
//
//NH_DIAGNOSTIC_END(nh_core_sendWindowSignal(Window_p, &Color, NH_SIGNAL_SET_BACKGROUND_COLOR))
//}
//
//NH_API_RESULT nh_setWindowTitle(
//    nh_Window *Window_p, char *title_p)
//{
//    NH_CORE_CHECK_NULL(Window_p)
//    NH_CORE_CHECK_NULL(title_p)
//
//NH_DIAGNOSTIC_END(nh_core_sendWindowSignal(Window_p, title_p, NH_SIGNAL_SET_TITLE))
//}

//nh_Window *nh_openWindow()
//{
//return nh_core_activateWorkload(nh_core_initSurface, nh_core_runSurface, NULL, false);
//}
//
//static NH_API_RESULT nh_core_waitForWindowClosure(
//    nh_Window *Window_p)
//{
//    if (nh_core_getThreadFromArgs(Window_p) == nh_core_getThread()) {
//        NH_DIAGNOSTIC_END(nh_core_runWorkload(nh_core_getWorkloadFromArgs(Window_p)))
//    }
//    else {
//        while (nh_core_getWorkloadFromArgs(Window_p)->signal != NH_SIGNAL_INACTIVE) {}
//    }
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//NH_API_RESULT nh_closeWindow(
//    nh_Window *Window_p)
//{
//    NH_CORE_CHECK_NULL(Window_p)
//    Window_p->signal = NH_SIGNAL_CLOSE;
//    NH_CORE_CHECK(nh_core_waitForWindowClosure(Window_p))
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}
