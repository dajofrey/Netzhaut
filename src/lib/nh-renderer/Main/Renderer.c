// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Renderer.h"

#include "../Vulkan/Data.h"
#include "../Vulkan/Record.h"

#include "../Common/Log.h"

#include "../../nh-gfx/Base/Viewport.h"
#include "../../nh-gfx/Common/Macros.h"
#include "../../nh-css/Main/Canvas.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/System/Thread.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/Array.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
// INIT ============================================================================================

void *nh_renderer_initRenderer(
    nh_core_Workload *Workload_p)
{
    static char *name_p = "Renderer";
    static char *path_p = "nh-renderer/Main/Renderer.c";
    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_RENDERER;

    nh_css_LayoutEngine *LayoutEngine_p = Workload_p->args_p;
    NH_CORE_CHECK_NULL_2(NULL, LayoutEngine_p)

    nh_renderer_Renderer *Renderer_p = nh_core_allocate(sizeof(nh_renderer_Renderer));
    NH_CORE_CHECK_MEM_2(NULL, Renderer_p)

    Renderer_p->LayoutEngine_p = LayoutEngine_p;
    Renderer_p->RenderTargets = nh_core_initArray(sizeof(nh_renderer_RenderTarget), 8);

    return Renderer_p;
}

// RUN =============================================================================================

static NH_API_RESULT nh_renderer_createRenderData(
    nh_gfx_Viewport *Viewport_p, nh_css_Canvas *Canvas_p)
{
    switch (Viewport_p->Surface_p->api)
    {
        case NH_GFX_API_VULKAN :
            nh_renderer_createFragmentTreeVulkanData(&Canvas_p->FragmentTree, Viewport_p);
            break;
        default :
            return NH_API_ERROR_BAD_STATE;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_renderer_renderCanvas(
    nh_gfx_Viewport *Viewport_p, nh_css_Canvas *Canvas_p)
{
    nh_gfx_beginRecording(Viewport_p);

    switch (Viewport_p->Surface_p->api)
    {
        case NH_GFX_API_VULKAN :
            nh_renderer_recordVulkanFragmentTree(&Canvas_p->FragmentTree, Viewport_p);
            break;
        default :
            return NH_API_ERROR_BAD_STATE;
    }

    nh_gfx_endRecording(Viewport_p, false);

    return NH_API_SUCCESS;
}

static nh_css_Canvas *nh_renderer_getCanvas(
    nh_css_Layout *Layout_p, nh_renderer_RenderTarget *RenderTarget_p)
{
    for (int i = 0; i < Layout_p->Canvases.size; ++i) {
        nh_css_Canvas *Canvas_p = Layout_p->Canvases.pp[i];
        if (Canvas_p->Type.Size.width == RenderTarget_p->Viewport_p->Settings.Size.width
        &&  Canvas_p->Type.Size.height == RenderTarget_p->Viewport_p->Settings.Size.height) {
            return Canvas_p;
        }
    }

    return NULL;
}

static NH_SIGNAL nh_renderer_updateRenders(
    nh_renderer_Renderer *Renderer_p) 
{
    if (!Renderer_p->LayoutEngine_p) {return NH_SIGNAL_ERROR;}
    if (!Renderer_p->LayoutEngine_p->Layout_p) {return NH_SIGNAL_IDLE;}

    bool render = false;

    for (int i = 0; i < Renderer_p->RenderTargets.length; ++i) {
        nh_renderer_RenderTarget *RenderTarget_p = 
            ((nh_renderer_RenderTarget*)Renderer_p->RenderTargets.p)+i;
//        if (!RenderTarget_p->render) {continue;}
        nh_css_Canvas *Canvas_p = 
            nh_renderer_getCanvas(Renderer_p->LayoutEngine_p->Layout_p, RenderTarget_p);
        if (!Canvas_p) {continue;}
        NH_CORE_CHECK_2(NH_SIGNAL_ERROR, nh_renderer_createRenderData(RenderTarget_p->Viewport_p, Canvas_p))
        NH_CORE_CHECK_2(NH_SIGNAL_ERROR, nh_renderer_renderCanvas(RenderTarget_p->Viewport_p, Canvas_p))
        RenderTarget_p->render = false;
        render = true;
    }

    return render ? NH_SIGNAL_OK : NH_SIGNAL_IDLE;
}

static NH_SIGNAL nh_renderer_runRenderer(
    void *args_p) 
{
    nh_renderer_Renderer *Renderer_p = args_p;
    NH_CORE_CHECK_NULL_2(NH_SIGNAL_ERROR, Renderer_p)

    return nh_renderer_updateRenders(Renderer_p);
}

// CREATE ==========================================================================================

nh_renderer_Renderer *nh_renderer_createRenderer(
    nh_css_LayoutEngine *LayoutEngine_p) 
{
    nh_renderer_Renderer *Renderer_p = 
        nh_core_activateWorkload(nh_renderer_initRenderer, nh_renderer_runRenderer, NULL, NULL, LayoutEngine_p, true);

    return Renderer_p;
}

NH_API_RESULT nh_renderer_addViewport(
    nh_renderer_Renderer *Renderer_p, nh_gfx_Viewport *Viewport_p)
{
    NH_CORE_CHECK_NULL(Renderer_p)
    NH_CORE_CHECK_NULL(Viewport_p)

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, 
        nh_gfx_claimViewport(Viewport_p, NH_GFX_VIEWPORT_OWNER_CSS, Renderer_p))

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, 
        nh_css_addCanvasType(Renderer_p->LayoutEngine_p, nh_css_createCanvasType(Viewport_p->Settings.Size)))

    nh_renderer_RenderTarget *RenderTarget_p = nh_core_incrementArray(&Renderer_p->RenderTargets);
    NH_CORE_CHECK_MEM(RenderTarget_p)

    RenderTarget_p->Viewport_p = Viewport_p;
    RenderTarget_p->render = true;
 
    return NH_API_SUCCESS;
}
