// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "IndexMap.h"
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

NH_TERMINAL_RESULT nh_terminal_initialize()
{
NH_TERMINAL_BEGIN()

    nh_terminal_createIndexMap();

    for (int i = 0; i < NH_VULKAN.GPUs.size; ++i) 
    {
        nh_vk_GPU *GPU_p = NH_VULKAN.GPUs.pp[0];

        GPU_p->Term.Pipelines_p = nh_core_allocate(sizeof(nh_vk_Pipeline) * _NH_TERMINAL_VULKAN_PIPELINE_COUNT);
        NH_TERMINAL_CHECK_MEM(GPU_p->Term.Pipelines_p)
 
        NH_TERMINAL_CHECK(nh_terminal_createVulkanPipelines(&GPU_p->Driver, GPU_p->Term.Pipelines_p))
    }

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

