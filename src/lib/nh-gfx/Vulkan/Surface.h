#ifndef NH_VK_SURFACE_H
#define NH_VK_SURFACE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Host.h"
#include "GPU.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_vk_Surface {

        VkSurfaceKHR *SurfaceKHR_p;
        nh_vk_GPU *GPU_p;

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

    } nh_vk_Surface;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    nh_vk_Surface nh_vk_initSurface(
    );

    NH_API_RESULT nh_vk_createSurface(
        nh_vk_Surface *Surface_p, nh_api_Window *Window_p, nh_vk_GPU *GPU_p
    );

    NH_API_RESULT nh_vk_destroySurface(
        nh_vk_Surface *Surface_p, bool destroySurfaceKHR
    );

    NH_API_RESULT nh_vk_resize(
        nh_vk_Surface *Surface_p, nh_api_Window *Window_p
    );

/** @} */

#endif
