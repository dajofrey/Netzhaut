#ifndef NH_VK_SURFACE_H
#define NH_VK_SURFACE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Host.h"
#include "Driver.h"

#include "../../../API/Header/Vulkan.h"
#include "../../../API/Header/Netzhaut.h"

typedef struct Nh_Window Nh_Window;

#endif

/** @addtogroup VulkanStructs Structs
 *  \ingroup Vulkan
 *  @{
 */

    typedef struct Nh_Vk_Surface {

        VkFramebuffer *Framebuffer_p;        
        VkSurfaceKHR SurfaceKHR;             
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

        unsigned int numberOfImages;         
        uint32_t currentImage;               

    } Nh_Vk_Surface;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createSurface(
        Nh_Window *Window_p
    );
    
    NH_RESULT Nh_Vk_destroySurface(
        Nh_Window *Window_p
    );

    NH_RESULT Nh_Vk_resize(
        Nh_Window *Window_p
    );

/** @} */

#endif
