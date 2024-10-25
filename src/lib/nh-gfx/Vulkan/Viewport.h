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

typedef struct nh_vk_Viewport {
    VkCommandBuffer CommandBuffers_p[6];
    VkCommandBuffer *CommandBuffers_pp[3];
    int images;
} nh_vk_Viewport;

// FUNCTIONS =======================================================================================

typedef struct nh_gfx_Viewport nh_gfx_Viewport;

NH_API_RESULT nh_vk_createViewport(
    nh_gfx_Viewport *Viewport_p
);

void nh_vk_destroyViewport(
    nh_gfx_Viewport *Viewport_p
);

NH_API_RESULT nh_vk_beginRecording(
    nh_gfx_Viewport *Viewport_p
);

NH_API_RESULT nh_vk_endRecording(
    nh_gfx_Viewport *Viewport_p
);

#endif // NH_GFX_VULKAN_VIEWPORT_H
