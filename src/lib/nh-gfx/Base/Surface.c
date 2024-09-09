// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Surface.h"
#include "Viewport.h"

#include "../Vulkan/Vulkan.h"
#include "../Vulkan/Render.h"
#include "../OpenGL/Render.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Thread.h"

#include "../../nh-wsi/Window/Window.h"

#include <string.h>
#include <limits.h>

// REQUIREMENTS ====================================================================================

static nh_gfx_SurfaceRequirements Requirements;

nh_gfx_SurfaceRequirements *nh_gfx_getSurfaceRequirements()
{
NH_GFX_BEGIN()
NH_GFX_END(&Requirements)
}

NH_GFX_RESULT nh_gfx_createSurfaceRequirements()
{
NH_GFX_BEGIN()

    NH_GFX_CHECK(nh_opengl_createSurfaceRequirements(&Requirements.OpenGL))

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

NH_GFX_RESULT nh_gfx_freeSurfaceRequirements()
{
NH_GFX_BEGIN()

    NH_GFX_CHECK(nh_opengl_freeSurfaceRequirements(&Requirements.OpenGL))

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

//// RESIZE ==========================================================================================
//
//static NH_CORE_RESULT nh_resize(
//    nh_Window *Window_p)
//{
//NH_GFX_BEGIN()
//
//    nh_syncResizeBegin(Window_p);
//
//    switch (Window_p->API) 
//    { 
//        case NH_API_VULKAN : NH_GFX_CHECK(nh_vk_resize(Window_p)) break;
//    }
//
//    for (int i = 0; i < Window_p->Tabs.count; ++i) {
//        NH_GFX_CHECK(nh_resizeTab(nh_core_getTab(Window_p, i)))
//    }
//
//    nh_syncResizeEnd(Window_p);
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}

// INIT ============================================================================================

typedef struct nh_gfx_SurfaceArgs {
    nh_wsi_Window *Window_p;
    NH_GFX_API_E api;
} nh_gfx_SurfaceArgs;

static void *nh_gfx_initSurface(
    nh_core_Workload *Workload_p)
{
NH_GFX_BEGIN()

    static NH_BYTE *name_p = "Graphics Surface";
    static NH_BYTE *path_p = "nh-gfx/Main/Surface.c";
    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_GFX;

    nh_gfx_SurfaceArgs *Args_p = Workload_p->args_p;

    NH_GFX_CHECK_NULL_2(NULL, Args_p)
    NH_GFX_CHECK_NULL_2(NULL, Args_p->Window_p)

    nh_gfx_Surface *Surface_p = nh_core_allocate(sizeof(nh_gfx_Surface));
    NH_GFX_CHECK_NULL_2(NULL, Surface_p)

    Surface_p->args_p = NULL;
    Surface_p->signal = NH_SIGNAL_UPDATE;

    Surface_p->renderRequests = 0;
    Surface_p->api      = Args_p->api;
    Surface_p->Window_p = Args_p->Window_p;

    Surface_p->Viewports = nh_core_initList(8);

    Surface_p->Internal.atomic       = NH_FALSE;
    Surface_p->Internal.skipRender   = NH_FALSE;
    Surface_p->Internal.getNextImage = NH_TRUE;

    Surface_p->Sync.halt         = NH_FALSE;
    Surface_p->Sync.ready        = NH_FALSE;
    Surface_p->Sync.resize       = NH_FALSE;
    Surface_p->Sync.rendering    = NH_FALSE;
    Surface_p->Sync.atomicRender = NH_FALSE;

    Surface_p->Settings.BackgroundColor.r = 0.0f;
    Surface_p->Settings.BackgroundColor.g = 0.0f;
    Surface_p->Settings.BackgroundColor.b = 0.0f;
    Surface_p->Settings.BackgroundColor.a = 1.0f;

    Surface_p->Vulkan = nh_vk_initSurface();
    Surface_p->OpenGL = nh_opengl_initSurface();

    switch (Surface_p->api)
    {
        case NH_GFX_API_VULKAN : 
            if (NH_VULKAN.GPUs.size <= 0) {NH_GFX_END(NULL)}
            NH_GFX_CHECK_2(NULL, 
                nh_vk_createSurface(&Surface_p->Vulkan, Surface_p->Window_p, NH_VULKAN.GPUs.pp[0]))
            break;
        case NH_GFX_API_OPENGL : 
            NH_GFX_CHECK_2(NULL, nh_opengl_createSurface(&Surface_p->OpenGL, Surface_p->Window_p)) 
            break;
        default : NH_GFX_END(NULL)
    }

    Surface_p->Sync.ready = NH_TRUE;

NH_GFX_END(Surface_p)
}

void nh_gfx_freeSurface(
    void *surface_p)
{
NH_GFX_BEGIN()

    nh_gfx_Surface *Surface_p = surface_p;

    for (int i = 0; i < Surface_p->Viewports.size; ++i) {
        nh_gfx_destroyViewport(Surface_p, Surface_p->Viewports.pp[i]);
    }
    nh_core_freeList(&Surface_p->Viewports, NH_FALSE);
 
    switch (Surface_p->api)
    {
        case NH_GFX_API_VULKAN : 
            nh_vk_destroySurface(&Surface_p->Vulkan, NH_TRUE);
            break;
        case NH_GFX_API_OPENGL : 
            nh_opengl_destroySurface(&Surface_p->OpenGL, Surface_p->Window_p);
            break;
    }

    nh_core_free(Surface_p);

NH_GFX_SILENT_END()
}

// RUN =============================================================================================

static NH_GFX_RESULT nh_gfx_getSortedViewports(
    nh_gfx_Surface *Surface_p, nh_List *SortedViewports_p)
{
NH_GFX_BEGIN()

    nh_List Viewports = nh_core_initList(16);
    
    // find out which viewports to render
    for (int i = 0; i < Surface_p->Viewports.size; ++i) {
        if (((nh_gfx_Viewport*)Surface_p->Viewports.pp[i])->Sync.render) {
            nh_core_appendToList(&Viewports, Surface_p->Viewports.pp[i]);
        }
    }

    int *sortedIndices_p = nh_core_allocate(sizeof(int) * Viewports.size);
    NH_GFX_CHECK_MEM(sortedIndices_p)

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
    nh_core_freeList(&Viewports, NH_FALSE);

NH_GFX_END(NH_GFX_SUCCESS)
}

static NH_GFX_RESULT nh_gfx_prepareRendering(
    nh_gfx_Surface *Surface_p)
{
NH_GFX_BEGIN()

    NH_GFX_RESULT result = NH_GFX_ERROR_BAD_STATE;

    switch (Surface_p->api)
    {
        case NH_GFX_API_VULKAN : result = nh_vk_prepareRendering(&Surface_p->Vulkan); break;
        case NH_GFX_API_OPENGL : result = NH_GFX_SUCCESS; break;
    }

    if (result == NH_GFX_VULKAN_ERROR_OUT_OF_DATE_KHR) {
        result = NH_GFX_ERROR_RESIZE_NEEDED;
    }

NH_GFX_END(result)
}

static NH_GFX_RESULT nh_gfx_render(
    nh_gfx_Surface *Surface_p)
{
NH_GFX_BEGIN()

    nh_List SortedViewports = nh_core_initList(16);
    NH_GFX_CHECK(nh_gfx_getSortedViewports(Surface_p, &SortedViewports))

    switch (Surface_p->api)
    {
        case NH_GFX_API_VULKAN : 

            NH_GFX_CHECK(nh_vk_render(Surface_p, &SortedViewports))
            break;

        case NH_GFX_API_OPENGL : 

            NH_GFX_CHECK(nh_opengl_render(Surface_p, &SortedViewports))
            break;

        default : NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_BAD_STATE)
    }

    nh_core_freeList(&SortedViewports, NH_FALSE);

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

static NH_SIGNAL nh_gfx_runSurface(
    void *args_p)
{
NH_GFX_BEGIN()

    NH_GFX_CHECK_NULL_2(NH_SIGNAL_ERROR, args_p)
    nh_gfx_Surface *Surface_p = args_p;
    NH_BOOL idle = NH_TRUE;

    switch (Surface_p->signal)
    {
        case NH_SIGNAL_UPDATE : 
        {
            if (Surface_p->Viewports.size > 0) 
            {
                if (Surface_p->Internal.getNextImage)
                {
                    NH_GFX_RESULT prepareResult = nh_gfx_prepareRendering(Surface_p);

                    if (prepareResult == NH_GFX_ERROR_RESIZE_NEEDED) {
                        // TODO handle resize
                    }
                    else if (prepareResult != NH_GFX_SUCCESS) {
                        NH_GFX_END(NH_SIGNAL_ERROR)
                    }
                    else {
                        Surface_p->Internal.getNextImage = NH_FALSE;
                    }
                }
                if (Surface_p->renderRequests)
                {
                    nh_gfx_render(Surface_p);
                    Surface_p->Internal.getNextImage = NH_TRUE;
                    Surface_p->renderRequests--;
                    idle = NH_FALSE;
                }
            } 

            break;
        }
    }

NH_GFX_END(idle ? NH_SIGNAL_IDLE : NH_SIGNAL_OK)
}

// CREATE ==========================================================================================

nh_gfx_Surface *nh_gfx_createSurface(
    nh_wsi_Window *Window_p, NH_GFX_API_E api)
{
NH_GFX_BEGIN()

    if (Window_p->surface_p) {
        NH_GFX_DIAGNOSTIC_END(NULL)
    }

    nh_gfx_SurfaceArgs Args;
    Args.Window_p = Window_p;
    Args.api = api;

    nh_gfx_Surface *Surface_p = nh_core_activateWorkload(nh_gfx_initSurface, nh_gfx_runSurface, nh_gfx_freeSurface, NULL, &Args, NH_TRUE);

    Window_p->surface_p = Surface_p;

NH_GFX_END(Surface_p)
}

//// CONFIGURE SIGNALS ===============================================================================
//
//static NH_CORE_RESULT nh_core_sendWindowSignal(
//    nh_Window *Window_p, void *args_p, NH_SIGNAL signal)
//{
//NH_GFX_BEGIN()
//
//    NH_SIGNAL previous = Window_p->signal;
//    Window_p->signal = signal;
//    Window_p->args_p = args_p;
//
//    if (nh_core_getThreadFromArgs(Window_p) == nh_core_getThread()) {
//        NH_GFX_CHECK(nh_core_runWorkload(nh_core_getWorkloadFromArgs(Window_p)))
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
//
//NH_CORE_RESULT nh_setWindowBackgroundColor(
//    nh_Window *Window_p, nh_Color Color)
//{
//NH_GFX_BEGIN()
//
//    NH_GFX_CHECK_NULL(Window_p)
//
//NH_DIAGNOSTIC_END(nh_core_sendWindowSignal(Window_p, &Color, NH_SIGNAL_SET_BACKGROUND_COLOR))
//}
//
//NH_CORE_RESULT nh_setWindowTitle(
//    nh_Window *Window_p, char *title_p)
//{
//NH_GFX_BEGIN()
//
//    NH_GFX_CHECK_NULL(Window_p)
//    NH_GFX_CHECK_NULL(title_p)
//
//NH_DIAGNOSTIC_END(nh_core_sendWindowSignal(Window_p, title_p, NH_SIGNAL_SET_TITLE))
//}
//
//// OPEN/CLOSE ======================================================================================
//
//nh_Window *nh_openWindow()
//{
//NH_GFX_BEGIN()
//NH_GFX_END(nh_core_activateWorkload(nh_core_initSurface, nh_core_runSurface, NULL, NH_FALSE))
//}
//
//static NH_CORE_RESULT nh_core_waitForWindowClosure(
//    nh_Window *Window_p)
//{
//NH_GFX_BEGIN()
//
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
//NH_CORE_RESULT nh_closeWindow(
//    nh_Window *Window_p)
//{
//NH_GFX_BEGIN()
//
//    NH_GFX_CHECK_NULL(Window_p)
//    Window_p->signal = NH_SIGNAL_CLOSE;
//    NH_GFX_CHECK(nh_core_waitForWindowClosure(Window_p))
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}

