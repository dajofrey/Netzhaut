#ifndef NH_GFX_VULKAN_VULKAN_H
#define NH_GFX_VULKAN_VULKAN_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Host.h"
#include "../Common/Includes.h"

// STRUCTS =========================================================================================

typedef struct nh_vk_Vulkan {
    nh_gfx_VulkanHost Host;
    nh_core_List GPUs;
} nh_vk_Vulkan;

// DATA ============================================================================================

extern nh_vk_Vulkan NH_VULKAN;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_vk_initVulkan(
);

NH_API_RESULT nh_vk_terminateVulkan(
);

#endif // NH_GFX_VULKAN_VULKAN_H 
