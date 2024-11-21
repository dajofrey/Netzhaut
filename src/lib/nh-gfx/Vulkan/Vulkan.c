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

#include "../../nh-core/Util/List.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DATA ============================================================================================

nh_gfx_Vulkan NH_VULKAN;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_initVulkan()
{
    NH_VULKAN.GPUs = nh_core_initList(8);

    if (nh_gfx_createVulkanHost(&NH_VULKAN.Host, true) == NH_API_SUCCESS) {
        NH_CORE_CHECK(nh_gfx_initVulkanGPUs(&NH_VULKAN.GPUs, &NH_VULKAN.Host))
        if (NH_VULKAN.GPUs.size <= 0) {return NH_API_VULKAN_ERROR_NO_DEVICE_SUITABLE;}
    } 
    else {return NH_API_VULKAN_ERROR_HOST_CREATION_FAILED;}

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_terminateVulkan()
{
    nh_gfx_freeVulkanGPUs(&NH_VULKAN.GPUs);
    nh_gfx_destroyVulkanHost(&NH_VULKAN.Host);

    return NH_API_SUCCESS;
}

