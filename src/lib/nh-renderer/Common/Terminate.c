// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminate.h"
#include "Macros.h"

#include "../Vulkan/Pipeline.h"

#include "../../nh-gfx/Vulkan/Vulkan.h"
#include "../../nh-gfx/Vulkan/GPU.h"
#include "../../nh-core/System/Memory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_renderer_terminate()
{
NH_RENDERER_BEGIN()

    for (int i = 0; i < NH_VULKAN.GPUs.size; ++i) 
    {
        nh_vk_GPU *GPU_p = NH_VULKAN.GPUs.pp[i];
        nh_renderer_vk_destroyPipelines(&GPU_p->Driver, GPU_p->Renderer.Pipelines_p);
        nh_core_free(GPU_p->Renderer.Pipelines_p);
    }

NH_RENDERER_DIAGNOSTIC_END(NH_API_SUCCESS)
}

