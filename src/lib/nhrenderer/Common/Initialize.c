// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "Macros.h"

#include "../Vulkan/Pipeline.h"

#include "../../nhgfx/Vulkan/GPU.h"
#include "../../nhgfx/Vulkan/Vulkan.h"

#include "../../nhcore/System/Logger.h"
#include "../../nhcore/System/Memory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_RENDERER_RESULT nh_renderer_initialize()
{
NH_RENDERER_BEGIN()

    for (int i = 0; i < NH_VULKAN.GPUs.size; ++i) 
    {
        nh_vk_GPU *GPU_p = NH_VULKAN.GPUs.pp[i];

        GPU_p->Renderer.Pipelines_p = nh_core_allocate(sizeof(nh_vk_Pipeline) * NH_RENDERER_VK_PIPELINE_COUNT);
        NH_RENDERER_CHECK_MEM(GPU_p->Renderer.Pipelines_p)
 
        NH_RENDERER_CHECK(nh_renderer_vk_createPipelines(&GPU_p->Driver, GPU_p->Renderer.Pipelines_p))
    }

NH_RENDERER_DIAGNOSTIC_END(NH_RENDERER_SUCCESS)
}

