// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#ifdef __unix__
    #define _GNU_SOURCE
#endif

#include "Vulkan.h"
#include "Host.h"
#include "GPU.h"

#include "../Common/Macros.h"

#include "../../nhcore/Util/List.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DATA ============================================================================================

nh_vk_Vulkan NH_VULKAN;

// VULKAN ==========================================================================================

NH_GFX_RESULT nh_vk_initVulkan()
{
NH_GFX_BEGIN()

    NH_VULKAN.GPUs = nh_core_initList(8);

    if (nh_vk_createHost(&NH_VULKAN.Host, NH_TRUE) == NH_GFX_SUCCESS) {
        NH_GFX_CHECK(nh_vk_initGPUs(&NH_VULKAN.GPUs, &NH_VULKAN.Host))
        if (NH_VULKAN.GPUs.size <= 0) {NH_GFX_DIAGNOSTIC_END(NH_GFX_VULKAN_ERROR_NO_DEVICE_SUITABLE)}
    } 
    else {NH_GFX_DIAGNOSTIC_END(NH_GFX_VULKAN_ERROR_HOST_CREATION_FAILED)}

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

NH_GFX_RESULT nh_vk_terminateVulkan()
{
NH_GFX_BEGIN()

    nh_vk_freeGPUs(&NH_VULKAN.GPUs);
    nh_vk_destroyHost(&NH_VULKAN.Host);

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

