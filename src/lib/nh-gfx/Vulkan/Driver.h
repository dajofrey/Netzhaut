#ifndef NH_VULKAN_DRIVER_H
#define NH_VULKAN_DRIVER_H

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

typedef struct nh_gfx_VulkanDriverInfo {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
    uint32_t vendorId;
    uint32_t deviceId;
} nh_gfx_VulkanDriverInfo;

typedef struct nh_gfx_VulkanDriver {
    nh_gfx_VulkanDriverInfo Info;
    VkPhysicalDevice PhysicalDevice;
    VkDevice Device;
    struct VolkDeviceTable Functions;
    VkCommandPool *ComputeCommandPools_p;
    VkCommandPool *GraphicsCommandPools_p;
    VkDescriptorPool *DescriptorPool_p;
    VkRenderPass *RenderPass_p;
    VkQueue GraphicsQueue;
    VkQueue ComputeQueue;
    unsigned int descriptorPoolCount;
    unsigned int renderPassCount;
} nh_gfx_VulkanDriver;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_createVulkanDriver(
    nh_gfx_VulkanHost *Host_p, nh_gfx_VulkanDriver *Driver_p, char *name_p
);

void nh_gfx_destroyVulkanDriver(
    nh_gfx_VulkanDriver *Driver_p
);

#endif // NH_VULKAN_DRIVER_H
