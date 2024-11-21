#ifndef NH_GFX_VULKAN_VIEWPORT_H
#define NH_GFX_VULKAN_VIEWPORT_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

// STRUCTS =========================================================================================

typedef struct nh_gfx_VulkanViewport {
    VkCommandBuffer CommandBuffers_p[6];
    VkCommandBuffer *CommandBuffers_pp[3];
    int images;
} nh_gfx_VulkanViewport;

// FUNCTIONS =======================================================================================

typedef struct nh_gfx_Viewport nh_gfx_Viewport;

NH_API_RESULT nh_gfx_createVulkanViewport(
    nh_gfx_Viewport *Viewport_p
);

void nh_gfx_destroyVulkanViewport(
    nh_gfx_Viewport *Viewport_p
);

NH_API_RESULT nh_gfx_beginVulkanRecording(
    nh_gfx_Viewport *Viewport_p
);

NH_API_RESULT nh_gfx_endVulkanRecording(
    nh_gfx_Viewport *Viewport_p
);

#endif // NH_GFX_VULKAN_VIEWPORT_H
