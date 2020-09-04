#ifndef NH_VK_VULKAN_H
#define NH_VK_VULKAN_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Host.h"
#include "GPU.h"

#endif

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_initVulkan(
    );
    
    NH_RESULT Nh_Vk_freeVulkan(
    );

    Nh_Vk_Host *Nh_Vk_getHost(
    );
    
    Nh_Vk_GPU *Nh_Vk_getGPU(
        int i
    );

    int Nh_Vk_getGPUCount(
    );

/** @} */

#endif 
