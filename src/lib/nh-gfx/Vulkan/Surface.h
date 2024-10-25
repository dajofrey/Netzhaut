#ifndef NH_GFX_VULKAN_SURFACE_H
#define NH_GFX_VULKAN_SURFACE_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================
 
#include "Host.h"
#include "GPU.h"

#include "../Common/Includes.h"

// STRUCTS =========================================================================================

typedef struct nh_gfx_VulkanSurface {
    VkSurfaceKHR *SurfaceKHR_p;
    nh_gfx_VulkanGPU *GPU_p;
    VkFramebuffer *Framebuffer_p;        
    VkSwapchainKHR SwapchainKHR;         
    VkExtent2D Extent2D;                 
    VkCommandBuffer *CommandBuffers_p; 
    struct {
        VkImageView ImageView;
        VkImage Image;
        VkDeviceMemory Memory;
    } DepthStencil;
    struct {
        VkImageView *ImageView_p;            
        VkImage *Image_p; 
    } Swapchain;                   
    struct {
        VkFence Fence;                       
        VkSemaphore Semaphore_p[2];         
    } Sync;
    unsigned int imageCount;         
    uint32_t currentImage;               
} nh_gfx_VulkanSurface;

// FUNCTIONS =======================================================================================

nh_gfx_VulkanSurface nh_gfx_initVulkanSurface(
);

NH_API_RESULT nh_gfx_createVulkanSurface(
    nh_gfx_VulkanSurface *Surface_p, nh_api_Window *Window_p, nh_gfx_VulkanGPU *GPU_p
);

NH_API_RESULT nh_gfx_destroyVulkanSurface(
    nh_gfx_VulkanSurface *Surface_p, bool destroySurfaceKHR
);

NH_API_RESULT nh_gfx_resizeVulkanSurface(
    nh_gfx_VulkanSurface *Surface_p, nh_api_Window *Window_p
);

#endif // NH_GFX_VULKAN_SURFACE_H
