// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#ifdef __unix__
#define _GNU_SOURCE
#endif

#include "../Header/Vulkan.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/List.h"
#include "../../../Core/Header/Config.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <string.h>
#include <stdio.h>

// DECLARE =========================================================================================

typedef struct Nh_Vk_Vulkan {
    Nh_Vk_Host Host;
    Nh_List GPUs;
} Nh_Vk_Vulkan;

// DATA ============================================================================================

Nh_Vk_Vulkan Vulkan;

// VULKAN ==========================================================================================

NH_RESULT Nh_Vk_initVulkan()
{
NH_BEGIN()

    NH_INIT_LIST(Vulkan.GPUs)

    if (Nh_Vk_createHost(Nh_getConfig()->Flags.Log.vulkan) == NH_SUCCESS) {
        NH_CHECK(Nh_Vk_initGPUs(&Vulkan.GPUs, &Vulkan.Host))
        if (Vulkan.GPUs.count <= 0) {NH_END(NH_VULKAN_ERROR_NO_DEVICE_SUITABLE)}
    } 
    else {NH_END(NH_VULKAN_ERROR_HOST_CREATION_FAILED)}

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_freeVulkan()
{
NH_BEGIN()

    Nh_Vk_freeGPUs(&Vulkan.GPUs);
    Nh_Vk_destroyHost();

NH_END(NH_SUCCESS)
}

Nh_Vk_Host *Nh_Vk_getHost()
{
NH_BEGIN()
NH_END(&Vulkan.Host)
}

Nh_Vk_GPU *Nh_Vk_getGPU(
    int i)
{
NH_BEGIN()
NH_END(Nh_getListItem(&Vulkan.GPUs, i))
}

int Nh_Vk_getGPUCount()
{
NH_BEGIN()
NH_END(Vulkan.GPUs.count)
}

