// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "GPU.h"
#include "Host.h"
#include "Driver.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_vk_prepareDetectedGPUs(
    nh_core_List *GPUs_p, nh_gfx_VulkanHost *Host_p)
{
    int detectedCount = 0;
    Host_p->Functions.vkEnumeratePhysicalDevices(Host_p->Instance, &detectedCount, VK_NULL_HANDLE);
    if (detectedCount <= 0) {return NH_API_SUCCESS;}

    VkPhysicalDevice *Physical_p = nh_core_allocate(sizeof(VkPhysicalDevice) * detectedCount);
    NH_CORE_CHECK_MEM(Physical_p)

    Host_p->Functions.vkEnumeratePhysicalDevices(Host_p->Instance, &detectedCount, Physical_p);

    for (int i = 0; i < detectedCount; i++) 
    {
        nh_gfx_VulkanGPU *GPU_p = nh_core_allocate(sizeof(nh_gfx_VulkanGPU));
        NH_CORE_CHECK_MEM(GPU_p)

        GPU_p->Textures = nh_core_initList(128);
        GPU_p->Renderer.Pipelines_p = NULL;
        GPU_p->Term.Pipelines_p = NULL;

        VkPhysicalDeviceProperties Properties;
        Host_p->Functions.vkGetPhysicalDeviceProperties(Physical_p[i], &Properties);

        GPU_p->name_p = nh_core_allocate(strlen(Properties.deviceName) + 1);
        NH_CORE_CHECK_MEM(GPU_p->name_p)

        strcpy(GPU_p->name_p, Properties.deviceName);

        nh_core_appendToList(GPUs_p, GPU_p);
    }

    nh_core_free(Physical_p);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_initVulkanGPUs(
    nh_core_List *GPUs_p, nh_gfx_VulkanHost *Host_p)
{
    NH_CORE_CHECK(nh_vk_prepareDetectedGPUs(GPUs_p, Host_p))

    for (int i = 0; i < GPUs_p->size; i++) 
    {
        nh_gfx_VulkanGPU *GPU_p = GPUs_p->pp[i];
        NH_CORE_CHECK(nh_gfx_createVulkanDriver(Host_p, &GPU_p->Driver, GPU_p->name_p))
    }

    return NH_API_SUCCESS;
}

void nh_gfx_freeVulkanGPUs(
    nh_core_List *GPUs_p)
{
    for (int i = 0; i < GPUs_p->size; i++) 
    {
        nh_gfx_VulkanGPU *GPU_p = GPUs_p->pp[i];
        nh_core_free(GPU_p->name_p);
        nh_gfx_destroyVulkanDriver(&GPU_p->Driver);
    }

    nh_core_freeList(GPUs_p, true);

    return;
}

