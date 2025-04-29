// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Surface.h"
#include "Driver.h"
#include "Host.h"
#include "GPU.h"
#include "Helper.h"
#include "Vulkan.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Thread.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-wsi/Window/Window.h"
#include "../../nh-wsi/Platforms/X11/Init.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// FUNCTIONS =======================================================================================

nh_gfx_VulkanSurface nh_gfx_initVulkanSurface()
{
    nh_gfx_VulkanSurface Surface;
    Surface.SurfaceKHR_p = NULL;
    Surface.GPU_p = NULL;

    return Surface;
}

static VkSurfaceKHR *nh_gfx_createVulkanSurfaceKHR(
    nh_gfx_VulkanHost *Host_p, nh_wsi_Window *Window_p)
{
    VkSurfaceKHR *SurfaceKHR_p = (VkSurfaceKHR*)nh_core_allocate(sizeof(VkSurfaceKHR));
    NH_CORE_CHECK_MEM_2(NULL, SurfaceKHR_p)

#ifdef __unix__

    if (Window_p->type == NH_WSI_TYPE_X11)
    {
        VkXlibSurfaceCreateInfoKHR surfaceInfo = 
        { 
            .sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
            .pNext  = VK_NULL_HANDLE,
            .dpy    = NH_WSI_X11.Display_p,
            .window = Window_p->X11.Handle, 
        };
        Host_p->Functions.vkCreateXlibSurfaceKHR(
            Host_p->Instance, &surfaceInfo, VK_NULL_HANDLE, SurfaceKHR_p
        ); 
    }
    else {return NULL;}

#elif defined(_WIN32) || defined (WIN32)

    if (Window_p->type == NH_WSI_TYPE_WIN32)
    {
        VkWin32SurfaceCreateInfoKHR surfaceInfo =
        {
            .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext     = VK_NULL_HANDLE,
            .flags     = 0,
            .hinstance = Window_p->Win32.hinstance,
            .hwnd      = Window_p->Win32.hwnd, 
        };
        Host_p->Functions.vkCreateWin32SurfaceKHR(
            Host_p->Instance, &surfaceInfo, VK_NULL_HANDLE, SurfaceKHR_p
        );
    }
    else {return NULL;}
	
#endif
    
    return SurfaceKHR_p;
}

static void nh_gfx_destroyVulkanSurfaceKHR(
    nh_gfx_VulkanSurface *Surface_p)
{
    NH_VULKAN.Host.Functions.vkDestroySurfaceKHR(NH_VULKAN.Host.Instance, *Surface_p->SurfaceKHR_p, VK_NULL_HANDLE);
    nh_core_free(Surface_p->SurfaceKHR_p);

    Surface_p->SurfaceKHR_p = NULL;

    return;
}

static NH_API_RESULT nh_gfx_chooseVulkanSurfaceFormat(
    VkSurfaceFormatKHR availableFormats[], VkSurfaceFormatKHR *format_p) 
{
    if (availableFormats[0].format == VK_FORMAT_UNDEFINED) { 
        VkSurfaceFormatKHR surfaceFormat = {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
        *format_p = surfaceFormat;
    }

    for(int i = 0; i < sizeof(*availableFormats) / sizeof(availableFormats[0]); ++i) {
        if (availableFormats[i].format     == VK_FORMAT_B8G8R8A8_UNORM 
        &&  availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            *format_p = availableFormats[i];
	}
    }

    *format_p = availableFormats[0];
    
    return NH_API_SUCCESS;;
}

static VkPresentModeKHR nh_gfx_chooseVulkanPresentMode(
    const VkPresentModeKHR *presentModes_p, int count) 
{
    for (int i = 0; i < count; ++i) {
        if (presentModes_p[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {return presentModes_p[i];}
    }
    
    return -1;;
}

static NH_API_RESULT nh_gfx_createVulkanSwapchain(
    nh_gfx_VulkanDriver *Driver_p, VkSurfaceKHR *SurfaceKHR_p, nh_gfx_VulkanSurface *Surface_p, 
    uint32_t graphicsQueueFamily)
{
    VkPhysicalDevice *Device_p = &Driver_p->PhysicalDevice;

    // choose swap surface format
    uint32_t formatCount;
    NH_VULKAN.Host.Functions.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Device_p, *SurfaceKHR_p, &formatCount, VK_NULL_HANDLE
    ); 

    VkSurfaceFormatKHR *SurfaceFormats_p = (VkSurfaceFormatKHR*)nh_core_allocate(sizeof(VkSurfaceFormatKHR) * formatCount);
    NH_CORE_CHECK_MEM(SurfaceFormats_p)

    NH_VULKAN.Host.Functions.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Device_p, *SurfaceKHR_p, &formatCount, SurfaceFormats_p
    ); 
    VkSurfaceFormatKHR SurfaceFormat;
    nh_gfx_chooseVulkanSurfaceFormat(SurfaceFormats_p, &SurfaceFormat);

    // choose present mode
    uint32_t presentModeCount;
    NH_VULKAN.Host.Functions.vkGetPhysicalDeviceSurfacePresentModesKHR(
        *Device_p, *SurfaceKHR_p, &presentModeCount, VK_NULL_HANDLE
    ); 

    VkPresentModeKHR *presentModes_p = (VkPresentModeKHR*)nh_core_allocate(sizeof(VkPresentModeKHR) * presentModeCount);
    NH_CORE_CHECK_MEM(presentModes_p)

    NH_VULKAN.Host.Functions.vkGetPhysicalDeviceSurfacePresentModesKHR(
        *Device_p, *SurfaceKHR_p, &presentModeCount, presentModes_p
    ); 
    VkPresentModeKHR presentMode = nh_gfx_chooseVulkanPresentMode(presentModes_p, presentModeCount);
    if (presentMode == -1) {return NH_API_ERROR_BAD_STATE;}

    // choose swapchain extent
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    NH_VULKAN.Host.Functions.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        *Device_p, *SurfaceKHR_p, &surfaceCapabilities
    );
    VkExtent2D swapExtent = surfaceCapabilities.currentExtent;

    // create swapchain
    VkSwapchainCreateInfoKHR swapchainInfo = 
    {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface               = *SurfaceKHR_p,
        .minImageCount         = Surface_p->imageCount,
        .imageFormat           = SurfaceFormat.format,
        .imageColorSpace       = SurfaceFormat.colorSpace,
        .imageExtent           = swapExtent,
        .imageArrayLayers      = 1,
        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT 
                               | VK_IMAGE_USAGE_TRANSFER_SRC_BIT 
                               | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .preTransform          = surfaceCapabilities.currentTransform,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        .presentMode           = presentMode,
        .clipped               = VK_TRUE,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices   = &graphicsQueueFamily
    };

    Driver_p->Functions.vkCreateSwapchainKHR(Driver_p->Device, &swapchainInfo, VK_NULL_HANDLE, &Surface_p->SwapchainKHR);
    Surface_p->Extent2D = swapExtent;

    VkSwapchainKHR *VkSwapchainKHR_p = &Surface_p->SwapchainKHR;

    // create swapchain images
    int imageCount = 0;
    Driver_p->Functions.vkGetSwapchainImagesKHR(Driver_p->Device, *VkSwapchainKHR_p, &imageCount, VK_NULL_HANDLE); 
   
    if (imageCount != Surface_p->imageCount) {return NH_API_ERROR_BAD_STATE;;}

    VkImage *Images_p = (VkImage*)nh_core_allocate(sizeof(VkImage) * imageCount);
    NH_CORE_CHECK_MEM(Images_p)

    Driver_p->Functions.vkGetSwapchainImagesKHR(Driver_p->Device, *VkSwapchainKHR_p, &imageCount, Images_p); 

    // create swapchain image views
    VkImageViewCreateInfo ImageViewInfo = 
    {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType                        = VK_IMAGE_VIEW_TYPE_2D,
        .format                          = SurfaceFormat.format,
        .components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY,
        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1
    };

    for (int i = 0; i < imageCount; ++i)
    {
        VkImage *tmp_p = &Surface_p->Swapchain.Image_p[i];
        *tmp_p = Images_p[i];
        ImageViewInfo.image = *tmp_p;
        Driver_p->Functions.vkCreateImageView(Driver_p->Device, &ImageViewInfo, VK_NULL_HANDLE, &Surface_p->Swapchain.ImageView_p[i]);
    }

    nh_core_free(SurfaceFormats_p);
    nh_core_free(presentModes_p);
    nh_core_free(Images_p);

    return NH_API_SUCCESS;      
}

static NH_API_RESULT nh_gfx_createVulkanDepthStencil(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanSurface *Surface_p) 
{
    // prepare
    VkPhysicalDevice *Device_p = &Driver_p->PhysicalDevice;
    VkExtent2D *Extent_p       = &Surface_p->Extent2D;

    VkFormat format = VK_FORMAT_D32_SFLOAT_S8_UINT; // TODO check if available
    VkImageCreateInfo ImageInfo = 
    {
        .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType     = VK_IMAGE_TYPE_2D,
        .format        = format,
        .mipLevels     = 1,
        .arrayLayers   = 1,
        .samples       = VK_SAMPLE_COUNT_1_BIT,
        .tiling        = VK_IMAGE_TILING_OPTIMAL,
        .usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    VkExtent3D Extent;
    Extent.width  = Extent_p->width;
    Extent.height = Extent_p->height;
    Extent.depth  = 1;
    ImageInfo.extent = Extent;

    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkCreateImage(Driver_p->Device, &ImageInfo, VK_NULL_HANDLE, &Surface_p->DepthStencil.Image))

    nh_gfx_VulkanDeviceMemoryInfo MemoryInfo;
    MemoryInfo.deviceMemoryType = NH_VK_DEVICE_MEMORY_IMAGE;
    MemoryInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    MemoryInfo.bindSource_p     = &Surface_p->DepthStencil.Image;

    NH_CORE_CHECK(nh_gfx_createVulkanDeviceMemory(Driver_p, &MemoryInfo, &Surface_p->DepthStencil.Memory))
    NH_CORE_CHECK(nh_gfx_bindVulkanDeviceMemory(Driver_p, &Surface_p->DepthStencil.Memory, &MemoryInfo))

    // create image view
    VkImageViewCreateInfo ImageViewInfo = 
    {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image                           = Surface_p->DepthStencil.Image,
        .viewType                        = VK_IMAGE_VIEW_TYPE_2D,
        .format                          = format,
        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT 
                                         | VK_IMAGE_ASPECT_STENCIL_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1
    };
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkCreateImageView(Driver_p->Device, &ImageViewInfo, VK_NULL_HANDLE, &Surface_p->DepthStencil.ImageView))

    // submit pipeline barrier 
    VkCommandBufferAllocateInfo CommandBufferInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool        = Driver_p->GraphicsCommandPools_p[0], // TODO
        .commandBufferCount = 1
    };
    
    VkCommandBuffer CommandBuffer;    
    Driver_p->Functions.vkAllocateCommandBuffers(Driver_p->Device, &CommandBufferInfo, &CommandBuffer); 

    VkCommandBufferBeginInfo CommandBufferBeginInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    Driver_p->Functions.vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo);
    VkImageMemoryBarrier barrier = 
    {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout                       = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout                       = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        .srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED,
        .image                           = Surface_p->DepthStencil.Image,
        .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT 
                                         | VK_IMAGE_ASPECT_STENCIL_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1,
        .srcAccessMask                   = 0,
        .dstAccessMask                   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT 
                                         | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
    };
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    Driver_p->Functions.vkCmdPipelineBarrier(
        CommandBuffer, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier
    );
    Driver_p->Functions.vkEndCommandBuffer(CommandBuffer);

    VkSubmitInfo submitInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers    = &CommandBuffer
    };
    
    Driver_p->Functions.vkQueueSubmit(Driver_p->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    Driver_p->Functions.vkQueueWaitIdle(Driver_p->GraphicsQueue);

    Driver_p->Functions.vkFreeCommandBuffers(Driver_p->Device, Driver_p->GraphicsCommandPools_p[0], 1, &CommandBuffer);

    return NH_API_SUCCESS;;
}

static NH_API_RESULT nh_gfx_createVulkanFramebuffer(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanSurface *Surface_p)
{  
    VkFramebufferCreateInfo FramebufferInfo = 
    {
        .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass      = Driver_p->RenderPass_p[0],
        .flags           = 0,
        .attachmentCount = 2,
        .width           = Surface_p->Extent2D.width,
        .height          = Surface_p->Extent2D.height,
        .layers          = 1
    };

    for (size_t i = 0; i < Surface_p->imageCount; i++) 
    {
        VkImageView *Swapchain_p = &Surface_p->Swapchain.ImageView_p[i]; 
        VkImageView *DepthStencil_p = &Surface_p->DepthStencil.ImageView;

        VkImageView Attachments_p[2] = {*Swapchain_p, *DepthStencil_p};
        FramebufferInfo.pAttachments = Attachments_p;

        NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkCreateFramebuffer(Driver_p->Device, &FramebufferInfo, VK_NULL_HANDLE, &Surface_p->Framebuffer_p[i]))
    }

    return NH_API_SUCCESS;;
}

NH_API_RESULT nh_gfx_createVulkanSurface(
    nh_gfx_VulkanSurface *Surface_p, nh_api_Window *Window_p, nh_gfx_VulkanGPU *GPU_p)
{
    NH_CORE_CHECK_NULL(Surface_p)
    NH_CORE_CHECK_NULL(GPU_p)

    Surface_p->GPU_p = GPU_p;

    if (!Surface_p->SurfaceKHR_p) {
        Surface_p->SurfaceKHR_p = nh_gfx_createVulkanSurfaceKHR(&NH_VULKAN.Host, (nh_wsi_Window*)Window_p);
        NH_CORE_CHECK_NULL(Surface_p->SurfaceKHR_p)
    }

    VkBool32 surfaceSupported = VK_FALSE;
    NH_VULKAN.Host.Functions.vkGetPhysicalDeviceSurfaceSupportKHR(
        GPU_p->Driver.PhysicalDevice, 0, *Surface_p->SurfaceKHR_p, &surfaceSupported
    ); 
    
    if (!surfaceSupported) {return NH_API_ERROR_BAD_STATE;;}

    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    NH_VULKAN.Host.Functions.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        GPU_p->Driver.PhysicalDevice, *Surface_p->SurfaceKHR_p, &SurfaceCapabilities
    );
    
    Surface_p->imageCount = SurfaceCapabilities.minImageCount;

    Surface_p->Framebuffer_p = (VkFramebuffer*)nh_core_allocate(sizeof(VkFramebuffer) * Surface_p->imageCount);
    NH_CORE_CHECK_MEM(Surface_p->Framebuffer_p)

    Surface_p->CommandBuffers_p = (VkCommandBuffer*)nh_core_allocate(sizeof(VkCommandBuffer) * Surface_p->imageCount);
    NH_CORE_CHECK_MEM(Surface_p->CommandBuffers_p)

    Surface_p->Swapchain.Image_p = (VkImage*)nh_core_allocate(sizeof(VkImage) * Surface_p->imageCount);
    NH_CORE_CHECK_MEM(Surface_p->Swapchain.Image_p) 

    Surface_p->Swapchain.ImageView_p = (VkImageView*)nh_core_allocate(sizeof(VkImageView) * Surface_p->imageCount);
    NH_CORE_CHECK_MEM(Surface_p->Swapchain.ImageView_p)

    NH_CORE_CHECK(nh_gfx_createVulkanSwapchain(&GPU_p->Driver, Surface_p->SurfaceKHR_p, Surface_p, 0))
    
    // depth/stencil and framebuffer
    NH_CORE_CHECK(nh_gfx_createVulkanDepthStencil(&GPU_p->Driver, Surface_p))
    NH_CORE_CHECK(nh_gfx_createVulkanFramebuffer(&GPU_p->Driver, Surface_p))

    // synchronisation resources
    VkFenceCreateInfo FenceInfo = {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    NH_GFX_CHECK_VULKAN(GPU_p->Driver.Functions.vkCreateFence(GPU_p->Driver.Device, &FenceInfo, VK_NULL_HANDLE, &Surface_p->Sync.Fence))
    
    VkSemaphoreCreateInfo SemaphoreInfo = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    NH_GFX_CHECK_VULKAN(GPU_p->Driver.Functions.vkCreateSemaphore(GPU_p->Driver.Device, &SemaphoreInfo, VK_NULL_HANDLE, &Surface_p->Sync.Semaphore_p[0]))
    NH_GFX_CHECK_VULKAN(GPU_p->Driver.Functions.vkCreateSemaphore(GPU_p->Driver.Device, &SemaphoreInfo, VK_NULL_HANDLE, &Surface_p->Sync.Semaphore_p[1]))

    // commandbuffers
    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = GPU_p->Driver.GraphicsCommandPools_p[nh_core_getThreadIndex()],
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = Surface_p->imageCount
    };

    NH_GFX_CHECK_VULKAN(GPU_p->Driver.Functions.vkAllocateCommandBuffers(GPU_p->Driver.Device, &AllocateInfo, Surface_p->CommandBuffers_p))

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_destroyVulkanSurface(
    nh_gfx_VulkanSurface *Surface_p, bool destroySurfaceKHR)
{
    nh_gfx_VulkanDriver *Driver_p = &Surface_p->GPU_p->Driver;

    Driver_p->Functions.vkDestroyFence(Driver_p->Device, Surface_p->Sync.Fence, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroySemaphore(Driver_p->Device, Surface_p->Sync.Semaphore_p[0], VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroySemaphore(Driver_p->Device, Surface_p->Sync.Semaphore_p[1], VK_NULL_HANDLE);

    for (int i = 0; i < Surface_p->imageCount; i++) {
        Driver_p->Functions.vkDestroyFramebuffer(Driver_p->Device, Surface_p->Framebuffer_p[i], VK_NULL_HANDLE);
        Driver_p->Functions.vkDestroyImageView(Driver_p->Device, Surface_p->Swapchain.ImageView_p[i], VK_NULL_HANDLE);
    }

    Driver_p->Functions.vkFreeMemory(Driver_p->Device, Surface_p->DepthStencil.Memory, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyImageView(Driver_p->Device, Surface_p->DepthStencil.ImageView, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyImage(Driver_p->Device, Surface_p->DepthStencil.Image, VK_NULL_HANDLE);

    Driver_p->Functions.vkDestroySwapchainKHR(Driver_p->Device, Surface_p->SwapchainKHR, VK_NULL_HANDLE); // TODO

    nh_core_free(Surface_p->Framebuffer_p);
    nh_core_free(Surface_p->CommandBuffers_p);
    nh_core_free(Surface_p->Swapchain.Image_p);
    nh_core_free(Surface_p->Swapchain.ImageView_p);

    if (destroySurfaceKHR) {
        nh_gfx_destroyVulkanSurfaceKHR(Surface_p);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_resizeVulkanSurface(
    nh_gfx_VulkanSurface *Surface_p, nh_api_Window *Window_p)
{
    NH_CORE_CHECK(nh_gfx_destroyVulkanSurface(Surface_p, false))
    NH_CORE_CHECK(nh_gfx_createVulkanSurface(Surface_p, Window_p, Surface_p->GPU_p))

    return NH_API_SUCCESS;
}

