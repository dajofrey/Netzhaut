// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Window.h"
#include "../Header/Tab.h"
#include "../Header/Memory.h"
#include "../Header/Sync.h"
#include "../Header/Macros.h"
#include "../Header/String.h"
#include "../Header/Time.h"
#include "../Header/Config.h"

#include "../../IO/Header/Input.h"
#include "../../IO/Header/Window.h"

#include "../../Graphics/Header/Window.h"
#include "../../Graphics/Vulkan/Header/Vulkan.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <string.h>
#include <limits.h>

// DECLARE =========================================================================================

typedef struct Nh_WindowArgs {
    int size_p[2];
    Nh_GPU GPU;
    NH_IO_TYPE type;
} Nh_WindowArgs;

static NH_RESULT Nh_startWindow(
    Nh_WindowArgs *Args_p
);
static NH_RESULT Nh_initWindow(
    Nh_Window *Window_p, Nh_GPU GPU, NH_IO_TYPE type
);

static NH_RESULT Nh_prepareRendering(
    Nh_Window *Window_p
);
static NH_RESULT Nh_render(
    Nh_Window *Window_p
);
static NH_RESULT Nh_resize(
    Nh_Window *Window_p
);

// THREAD ==========================================================================================

Nh_Thread *Nh_startWindowThread(
    int pxSize_p[2])
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_WindowArgs *Args_p = Nh_allocate(sizeof(Nh_WindowArgs));
    NH_CHECK_NULL(NULL, Args_p)

#include NH_DEFAULT_CHECK

    if (Nh_getSystem()->available_p[NH_API_VULKAN]) {
        Args_p->GPU.API     = NH_API_VULKAN;
        Args_p->GPU.Pointer = Nh_Vk_getGPU(0);
    }
    else {NH_END(NULL)}

    Args_p->size_p[0] = pxSize_p[0];
    Args_p->size_p[1] = pxSize_p[1];
    Args_p->type      = Nh_IO_getType();

    if (Args_p->type == -1) {NH_END(NULL)}

    Nh_Thread *Thread_p = Nh_startThread(
        NH_THREAD_WINDOW, (NH_RESULT(*)(void *))Nh_startWindow, (void*)Args_p
    );

NH_END(Thread_p)
}

static NH_RESULT Nh_startWindow(
    Nh_WindowArgs *Args_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Args_p)

    NH_CHECK_NULL(Nh_getThread())
    NH_CHECK_TYPE(Nh_getThread()->type, NH_THREAD_WINDOW)

    Nh_Window *Window_p = Nh_getThread()->Data_p;

    NH_CHECK(Nh_initWindow(Window_p, Args_p->GPU, Args_p->type))
    NH_CHECK(Nh_IO_createWindow("Empty Window", Args_p->size_p[0], Args_p->size_p[1], &Window_p->WSI))
    NH_CHECK(Nh_Gfx_createWindow(Window_p))

    if (!Nh_isVirtualThread()) {Nh_runWindow();}

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_runWindow()
{
NH_BEGIN()

    Nh_Thread *Thread_p = Nh_getThread();

    NH_CHECK_NULL(Thread_p)
    NH_CHECK_TYPE(Thread_p->type, NH_THREAD_WINDOW)

    Nh_Window *Window_p = Thread_p->Data_p;
    Window_p->Sync.ready = true;
    
    do {
        if (Thread_p->Flags.run)
        {
            NH_START_TAT()
            if (Window_p->Sync.halt) {Nh_syncHaltWindow(Window_p);}
            if (Window_p->Tabs.count > 0) 
            {
                if (Window_p->Internal.getNextImage) {NH_CHECK(Nh_prepareRendering(Window_p))}
                if (!Window_p->Internal.skipRender)  {NH_CHECK(Nh_render(Window_p))}
                else                                 {Window_p->Internal.skipRender = false;}
                NH_RUN_DELAY()
            } 
            else {NH_IDLE_DELAY()}
            NH_STOP_TAT()
            NH_CHECK(Nh_runVirtualThreads())

        } else {NH_IDLE_DELAY()}

    } while (!Window_p->Flags.close && !Nh_getConfig()->Flags.Internal.shutdown && !Thread_p->Info.isVirtual);

    if (Window_p->Flags.close || Nh_getConfig()->Flags.Internal.shutdown) {
        NH_CHECK(Nh_closeWindow(Window_p))
    }

NH_END(NH_SUCCESS)
}

// INIT ============================================================================================

static NH_RESULT Nh_initWindow(
    Nh_Window *Window_p, Nh_GPU GPU, NH_IO_TYPE type)
{
NH_BEGIN()

    NH_INIT_LIST(Window_p->Tabs) 

    Window_p->GPU = GPU;
    Window_p->WSI.type = type;

    Window_p->Internal.resizeFactors_p[0] = 1.0f;
    Window_p->Internal.resizeFactors_p[1] = 1.0f;
    Window_p->Internal.atomic             = false;
    Window_p->Internal.skipRender         = false;
    Window_p->Internal.getNextImage       = true;
    Window_p->Internal.ownerId            = Nh_getThread()->id;

    Window_p->Sync.halt         = false;
    Window_p->Sync.ready        = false;
    Window_p->Sync.resize       = false;
    Window_p->Sync.rendering    = false;
    Window_p->Sync.atomicRender = false;

    Window_p->Flags.getInput = true;
    Window_p->Flags.close    = false;

    Window_p->Info.inheritBackground = false;
    Window_p->Info.originalPxSize_p[0] = -1;
    Window_p->Info.originalPxSize_p[1] = -1;
    Window_p->Info.pxSize_p[0] = -1;
    Window_p->Info.pxSize_p[1] = -1;
    Window_p->Info.backgroundColor_p[0] = 1.0f;
    Window_p->Info.backgroundColor_p[1] = 1.0f;
    Window_p->Info.backgroundColor_p[2] = 1.0f;
    Window_p->Info.backgroundColor_p[3] = 1.0f;

    NH_CHECK(Nh_IO_initInput(&Window_p->Input, Nh_getConfig()->Settings.inputBufferSize))

NH_END(NH_SUCCESS)
}

// RENDER ==========================================================================================

static NH_RESULT Nh_prepareRendering(
    Nh_Window *Window_p)
{
NH_BEGIN()

    NH_RESULT result = Nh_Gfx_prepareRendering(Window_p);

    if (result == NH_ERROR_RESIZE_NEEDED) {
        NH_CHECK(Nh_resize(Window_p))
        result = Nh_prepareRendering(Window_p);
    }

NH_END(result)
}

static NH_RESULT Nh_render(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Nh_syncRenderBegin(Window_p);

    NH_RESULT result = Nh_Gfx_render(Window_p);

    if (result == NH_ERROR_RESIZE_NEEDED) {
        result = Nh_resize(Window_p);
    }

    Nh_syncRenderEnd(Window_p);

NH_END(NH_SUCCESS)
}

// RESIZE ==========================================================================================

static NH_RESULT Nh_resize(
    Nh_Window *Window_p)
{
NH_BEGIN();

printf("resize!!!!!!!!!!!\n");
exit(0);

//    Nh_syncResizeBegin(Window_p);
//
//    NH_CHECK(Nh_Gfx_resize(Window_p))
//
//    Window_p->Internal.resizeFactors_p[0] = 
//       ((float)Window_p->Info.pxSize_p[0]) / ((float)Window_p->Info.originalPxSize_p[0]);
//    Window_p->Internal.resizeFactors_p[1] = 
//       (((float)Window_p->Info.pxSize_p[1]) / ((float)Window_p->Info.originalPxSize_p[1]));
//
//    for (int i = 0; i < Window_p->Tabs.count; ++i) 
//    {
//        Nh_Tab *Tab_p = Nh_getTab(Window_p, i);
//        Tab_p->Info.pxSize_p[0] *= Window_p->Internal.resizeFactors_p[0];
//        Tab_p->Info.pxSize_p[1] *= Window_p->Internal.resizeFactors_p[1];
//
//        NH_CHECK(Nh_HTML_destroyDocument(&Tab_p->Document, &Window_p->GPU, NH_TRUE))
//        NH_CHECK(Nh_HTML_computeDocument(Tab_p))
//    }
//
//    Nh_syncResizeEnd(Window_p);

NH_END(NH_SUCCESS);
}

// CONFIGURE =======================================================================================

void Nh_setWindowBackgroundColor(
    Nh_Window *Window_p, float color_p[4])
{
NH_BEGIN()

    memcpy(Window_p->Info.backgroundColor_p, color_p, sizeof(float) * 4);
    Nh_IO_setWindowBackgroundColor(&Window_p->WSI, color_p);

    // TODO set body of html if background-color is not set

NH_SILENT_END()
}

NH_RESULT Nh_removeTab(
    Nh_Window *Window_p, void *Tab_p)
{
NH_BEGIN()

    for (int i = 0; i < Window_p->Tabs.count; ++i) {
        if (Nh_getTab(Window_p, i) == Tab_p) {
            Nh_destroyListItem(&Window_p->Tabs, i, false);
            break;
        }
    }

NH_END(NH_SUCCESS)
}

// CLOSE ===========================================================================================

NH_RESULT Nh_closeWindow(
    Nh_Window *Window_p)
{
NH_BEGIN()
  
    if (Nh_getThread()->Data_p != Window_p) {NH_END(NH_ERROR_BAD_STATE)}

    for (int i = 0; i < Window_p->Tabs.count; ++i) 
    {
        Nh_Tab *Tab_p = Nh_getTab(Window_p, i);
        Tab_p->Flags.close = true;

        Nh_Thread *Thread_p = Nh_getThreadFromData(Tab_p);

        if (NH_THREAD_SWITCH_POSSIBLE(Thread_p)) 
        {
            int context = Nh_switchThread(Thread_p);
            NH_CHECK(Nh_closeTab(Tab_p))
            Nh_switchThreadFromContext(context);
        }
        else {while (Thread_p->Info.active) {NH_IDLE_DELAY()}} // wait for other thread to close tab
    }

    NH_CHECK(Nh_Gfx_destroyWindow(Window_p))
    NH_CHECK(Nh_IO_destroyWindow(&Window_p->WSI))
    Nh_IO_freeInput(&Window_p->Input);

    Nh_destroyList(&Window_p->Tabs, false);

    NH_CHECK(Nh_deactivateThread(NULL))

NH_END(NH_SUCCESS)
}

// GET =============================================================================================

Nh_Tab *Nh_getTabFromIndex(
    Nh_Window *Window_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(&Window_p->Tabs, index))
}

