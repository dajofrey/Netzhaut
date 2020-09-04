// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Tab.h"
#include "../Header/GPU.h"
#include "../Header/Utils.h"
#include "../Header/Macros.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

// TAB =============================================================================================

NH_RESULT Nh_Vk_createTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Nh_Vk_GPU *GPU_p = Tab_p->Window_p->GPU.Pointer;

    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = GPU_p->Driver.GraphicsCommandPools_p[Nh_getKernelThread()->id],  
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 4
    };

    NH_CHECK_VULKAN(GPU_p->Driver.Func.vkAllocateCommandBuffers(
        GPU_p->Driver.Device, &AllocateInfo, Tab_p->Vulkan.CommandBuffers_p
    ))

NH_END(NH_SUCCESS)
}

void Nh_Vk_destroyTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Nh_Vk_GPU *GPU_p = Tab_p->Window_p->GPU.Pointer;
  
    GPU_p->Driver.Func.vkFreeCommandBuffers(
        GPU_p->Driver.Device, 
        GPU_p->Driver.GraphicsCommandPools_p[Nh_getKernelThread()->id], 
        4,
        Tab_p->Vulkan.CommandBuffers_p
    );

NH_SILENT_END()
}

