// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Surface.h"
#include "../Header/Driver.h"
#include "../Header/Host.h"
#include "../Header/GPU.h"
#include "../Header/Utils.h"
#include "../Header/Macros.h"
#include "../Header/Vulkan.h"

#include "../../../Core/Header/Memory.h"
#include "../../../Core/Header/Window.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// DECLARATION ====================================================================================

static NH_RESULT Nh_Vk_createSwapchain(
    Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p, uint32_t graphicsQueueFamily
);
static NH_RESULT Nh_Vk_createDepthStencil(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p
);
static NH_RESULT Nh_Vk_getSurface(
    Nh_Vk_Host *Host_p, Nh_Window *Window_p, Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p, 
    uint32_t qraphicsQueueFamily
);
static NH_RESULT Nh_Vk_createFramebuffer(
    Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p
);

static NH_RESULT Nh_Vk_chooseSurfaceFormat(
    VkSurfaceFormatKHR availableFormats[], VkSurfaceFormatKHR *format_p
);
static NH_RESULT Nh_Vk_choosePresentMode(
    const VkPresentModeKHR *availablePresentModes_p, VkPresentModeKHR *presentMode_p
);

// CREATE ==========================================================================================

NH_RESULT Nh_Vk_createSurface(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Nh_Vk_Host *Host_p       = Nh_Vk_getHost();
    Nh_Vk_Driver *Driver_p   = &((Nh_Vk_GPU*)Window_p->GPU.Pointer)->Driver;
    Nh_Vk_Surface *Surface_p = &Window_p->Vulkan.Surface; 

    // surface and swapchain
    NH_CHECK(Nh_Vk_getSurface(Host_p, Window_p, Driver_p, Surface_p, 0))

    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    Host_p->Func.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        Driver_p->PhysicalDevice, Surface_p->SurfaceKHR, &SurfaceCapabilities
    );
    
    Surface_p->numberOfImages        = SurfaceCapabilities.minImageCount;
    Surface_p->Framebuffer_p         = Nh_allocate(sizeof(VkFramebuffer) * Surface_p->numberOfImages);
    Surface_p->CommandBuffers_p      = Nh_allocate(sizeof(VkCommandBuffer) * Surface_p->numberOfImages);
    Surface_p->Swapchain.Image_p     = Nh_allocate(sizeof(VkImage) * Surface_p->numberOfImages);
    Surface_p->Swapchain.ImageView_p = Nh_allocate(sizeof(VkImageView) * Surface_p->numberOfImages);
    NH_CHECK_MEM(Surface_p->Framebuffer_p, Surface_p->CommandBuffers_p, Surface_p->Swapchain.Image_p, Surface_p->Swapchain.ImageView_p)

    NH_CHECK(Nh_Vk_createSwapchain(Driver_p, Surface_p, 0))
    
    // depth/stencil and framebuffer
    NH_CHECK(Nh_Vk_createDepthStencil(Host_p, Driver_p, Surface_p))
    NH_CHECK(Nh_Vk_createFramebuffer(Driver_p, Surface_p))

    // synchronisation resources
    VkFenceCreateInfo FenceInfo = {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateFence(Driver_p->Device, &FenceInfo, VK_NULL_HANDLE, &Surface_p->Sync.Fence))
    
    VkSemaphoreCreateInfo SemaphoreInfo = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateSemaphore(Driver_p->Device, &SemaphoreInfo, VK_NULL_HANDLE, &Surface_p->Sync.Semaphore_p[0]))
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateSemaphore(Driver_p->Device, &SemaphoreInfo, VK_NULL_HANDLE, &Surface_p->Sync.Semaphore_p[1]))

    // commandbuffers
    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = Driver_p->GraphicsCommandPools_p[Nh_getKernelThread()->id], // TODO create commandpools based on hardware threads 
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = Surface_p->numberOfImages
    };

    NH_CHECK_VULKAN(Driver_p->Func.vkAllocateCommandBuffers(Driver_p->Device, &AllocateInfo, Surface_p->CommandBuffers_p))

    Window_p->Info.pxSize_p[0] = Surface_p->Extent2D.width;
    Window_p->Info.pxSize_p[1] = Surface_p->Extent2D.height;

    if (Window_p->Info.originalPxSize_p[0] == -1 && Window_p->Info.originalPxSize_p[1] == -1) {
        Window_p->Info.originalPxSize_p[0] = Surface_p->Extent2D.width;
        Window_p->Info.originalPxSize_p[1] = Surface_p->Extent2D.height;
    }

NH_END(NH_SUCCESS)
}

// RESIZE ==========================================================================================

NH_RESULT Nh_Vk_resize(
    Nh_Window *Window_p)
{
NH_BEGIN()

    NH_CHECK(Nh_Vk_destroySurface(Window_p))
    NH_CHECK(Nh_Vk_createSurface(Window_p))

NH_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

NH_RESULT Nh_Vk_destroySurface(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Nh_Vk_Host *Host_p       = Nh_Vk_getHost();
    Nh_Vk_Driver *Driver_p   = &((Nh_Vk_GPU*)Window_p->GPU.Pointer)->Driver;
    Nh_Vk_Surface *Surface_p = &Window_p->Vulkan.Surface; 

    Driver_p->Func.vkDestroyFence(Driver_p->Device, Surface_p->Sync.Fence, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroySemaphore(Driver_p->Device, Surface_p->Sync.Semaphore_p[0], VK_NULL_HANDLE);
    Driver_p->Func.vkDestroySemaphore(Driver_p->Device, Surface_p->Sync.Semaphore_p[1], VK_NULL_HANDLE);

    for (int i = 0; i < Surface_p->numberOfImages; i++) {
        Driver_p->Func.vkDestroyFramebuffer(Driver_p->Device, Surface_p->Framebuffer_p[i], VK_NULL_HANDLE);
        Driver_p->Func.vkDestroyImageView(Driver_p->Device, Surface_p->Swapchain.ImageView_p[i], VK_NULL_HANDLE);
    }

    Driver_p->Func.vkFreeMemory(Driver_p->Device, Surface_p->DepthStencil.Memory, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyImageView(Driver_p->Device, Surface_p->DepthStencil.ImageView, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyImage(Driver_p->Device, Surface_p->DepthStencil.Image, VK_NULL_HANDLE);

    Driver_p->Func.vkDestroySwapchainKHR(Driver_p->Device, Surface_p->SwapchainKHR, VK_NULL_HANDLE); // TODO
    Host_p->Func.vkDestroySurfaceKHR(Host_p->Instance, Surface_p->SurfaceKHR, VK_NULL_HANDLE);

    Nh_free(Surface_p->Framebuffer_p);
    Nh_free(Surface_p->CommandBuffers_p);
    Nh_free(Surface_p->Swapchain.Image_p);
    Nh_free(Surface_p->Swapchain.ImageView_p);

NH_END(NH_SUCCESS)
}

// HELPER ==========================================================================================

static NH_RESULT Nh_Vk_getSurface(
    Nh_Vk_Host *Host_p, Nh_Window *Window_p, Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p, 
    uint32_t graphicsQueueFamily)
{
NH_BEGIN()

#ifdef __unix__

    if (Window_p->WSI.type == NH_IO_TYPE_X11)
    {
        VkXlibSurfaceCreateInfoKHR surfaceInfo = 
        { 
            .sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
            .pNext  = VK_NULL_HANDLE,
            .dpy    = Window_p->WSI.X11.Display_p,
            .window = Window_p->WSI.X11.XWindow, 
        };
        Host_p->Func.vkCreateXlibSurfaceKHR(
            Host_p->Instance, &surfaceInfo, VK_NULL_HANDLE, &Surface_p->SurfaceKHR
        ); 
    }
    else {NH_END(NH_ERROR_BAD_STATE)}

#elif defined(_WIN32) || defined (WIN32)

    if (Window_p->WSI.type == NH_IO_TYPE_WIN)
    {
        VkWin32SurfaceCreateInfoKHR surfaceInfo =
        {
            .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext     = VK_NULL_HANDLE,
            .flags     = 0,
            .hinstance = Window_p->WSI.Win.hinstance,
            .hwnd      = Window_p->WSI.Win.hwnd, 
        };
        Host_p->Func.vkCreateWin32SurfaceKHR(
            Host_p->Instance, &surfaceInfo, VK_NULL_HANDLE, &Surface_p->SurfaceKHR
        );
    }
    else {NH_END(NH_ERROR_BAD_STATE)}
	
#endif
    
    VkBool32 surfaceSupported = VK_FALSE;
    Host_p->Func.vkGetPhysicalDeviceSurfaceSupportKHR(
        Driver_p->PhysicalDevice, graphicsQueueFamily, Surface_p->SurfaceKHR, &surfaceSupported
    ); 
    
    if (!surfaceSupported) {NH_END(NH_ERROR_BAD_STATE);}
	
NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_createSwapchain(
    Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p, uint32_t graphicsQueueFamily)
{
NH_BEGIN()
      
    Nh_Vk_Host *Host_p = Nh_Vk_getHost(); 
    VkPhysicalDevice *Device_p = &Driver_p->PhysicalDevice;
    VkSurfaceKHR *SurfaceKHR_p = &Surface_p->SurfaceKHR;

    // choose swap surface format
    uint32_t formatCount;
    Host_p->Func.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Device_p, *SurfaceKHR_p, &formatCount, VK_NULL_HANDLE
    ); 

    VkSurfaceFormatKHR *surfaceFormats = Nh_allocate(sizeof(VkSurfaceFormatKHR) * formatCount);
    NH_CHECK_MEM(surfaceFormats)

    Host_p->Func.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Device_p, *SurfaceKHR_p, &formatCount, surfaceFormats
    ); 
    VkSurfaceFormatKHR surfaceFormat;
    Nh_Vk_chooseSurfaceFormat(surfaceFormats, &surfaceFormat);

    // choose present mode
    uint32_t presentModeCount;
    Host_p->Func.vkGetPhysicalDeviceSurfacePresentModesKHR(
        *Device_p, *SurfaceKHR_p, &presentModeCount, VK_NULL_HANDLE
    ); 

    VkPresentModeKHR *presentModes = Nh_allocate(sizeof(VkPresentModeKHR) * presentModeCount);
    NH_CHECK_MEM(presentModes)

    Host_p->Func.vkGetPhysicalDeviceSurfacePresentModesKHR(
        *Device_p, *SurfaceKHR_p, &presentModeCount, presentModes
    ); 
    VkPresentModeKHR presentMode; 
    Nh_Vk_choosePresentMode(presentModes, &presentMode);

    // choose swapchain extent
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    Host_p->Func.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        *Device_p, *SurfaceKHR_p, &surfaceCapabilities
    );
    VkExtent2D swapExtent = surfaceCapabilities.currentExtent;

    // create swapchain
    VkSwapchainCreateInfoKHR swapchainInfo = 
    {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface               = *SurfaceKHR_p,
        .minImageCount         = Surface_p->numberOfImages,
        .imageFormat           = surfaceFormat.format,
        .imageColorSpace       = surfaceFormat.colorSpace,
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
    
    Driver_p->Func.vkCreateSwapchainKHR(Driver_p->Device, &swapchainInfo, VK_NULL_HANDLE, &Surface_p->SwapchainKHR);
    Surface_p->Extent2D = swapExtent;

    VkSwapchainKHR *VkSwapchainKHR_p = &Surface_p->SwapchainKHR;

    // create swapchain images
    int numberOfImages = 0;
    Driver_p->Func.vkGetSwapchainImagesKHR(Driver_p->Device, *VkSwapchainKHR_p, &numberOfImages, VK_NULL_HANDLE); 
   
    if (numberOfImages != Surface_p->numberOfImages) {NH_END(NH_ERROR_BAD_STATE);}

    VkImage *Images_p = Nh_allocate(sizeof(VkImage) * numberOfImages);
    NH_CHECK_MEM(Images_p)

    Driver_p->Func.vkGetSwapchainImagesKHR(Driver_p->Device, *VkSwapchainKHR_p, &numberOfImages, Images_p); 

    // create swapchain image views
    VkImageViewCreateInfo ImageViewInfo = 
    {
        .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType                        = VK_IMAGE_VIEW_TYPE_2D,
        .format                          = surfaceFormat.format,
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

    for (int i = 0; i < numberOfImages; ++i)
    {
        VkImage *tmp_p = &Surface_p->Swapchain.Image_p[i];
        *tmp_p = Images_p[i];
        ImageViewInfo.image = *tmp_p;
        Driver_p->Func.vkCreateImageView(Driver_p->Device, &ImageViewInfo, VK_NULL_HANDLE, &Surface_p->Swapchain.ImageView_p[i]);
    }

    Nh_free(surfaceFormats);
    Nh_free(presentModes);
    Nh_free(Images_p);

NH_END(NH_SUCCESS)      
}

static NH_RESULT Nh_Vk_createDepthStencil(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p) 
{
NH_BEGIN();

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

    NH_CHECK_VULKAN(Driver_p->Func.vkCreateImage(Driver_p->Device, &ImageInfo, VK_NULL_HANDLE, &Surface_p->DepthStencil.Image))

    NH_Vk_DeviceMemoryInfo MemoryInfo =
    {
        .deviceMemoryType = NH_VK_DEVICE_MEMORY_IMAGE,
        .memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        .bindSource_p     = &Surface_p->DepthStencil.Image
    };

    NH_CHECK(Nh_Vk_createDeviceMemory(Host_p, Driver_p, &MemoryInfo, &Surface_p->DepthStencil.Memory))
    NH_CHECK(Nh_Vk_bindDeviceMemory(Driver_p, &Surface_p->DepthStencil.Memory, &MemoryInfo))

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
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateImageView(Driver_p->Device, &ImageViewInfo, VK_NULL_HANDLE, &Surface_p->DepthStencil.ImageView))

    // submit pipeline barrier 
    VkCommandBufferAllocateInfo CommandBufferInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool        = Driver_p->GraphicsCommandPools_p[0], // TODO
        .commandBufferCount = 1
    };
    
    VkCommandBuffer CommandBuffer;    
    Driver_p->Func.vkAllocateCommandBuffers(Driver_p->Device, &CommandBufferInfo, &CommandBuffer); 

    VkCommandBufferBeginInfo CommandBufferBeginInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    Driver_p->Func.vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo);
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
    Driver_p->Func.vkCmdPipelineBarrier(
        CommandBuffer, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier
    );
    Driver_p->Func.vkEndCommandBuffer(CommandBuffer);

    VkSubmitInfo submitInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers    = &CommandBuffer
    };
    
    Driver_p->Func.vkQueueSubmit(Driver_p->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    Driver_p->Func.vkQueueWaitIdle(Driver_p->GraphicsQueue);

    Driver_p->Func.vkFreeCommandBuffers(Driver_p->Device, Driver_p->GraphicsCommandPools_p[0], 1, &CommandBuffer);

NH_END(NH_SUCCESS);
}

static NH_RESULT Nh_Vk_createFramebuffer(
    Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p)
{  
NH_BEGIN();
   
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

    for (size_t i = 0; i < Surface_p->numberOfImages; i++) 
    {
        VkImageView *Swapchain_p = &Surface_p->Swapchain.ImageView_p[i]; 
        VkImageView *DepthStencil_p = &Surface_p->DepthStencil.ImageView;

        VkImageView Attachments_p[2] = {*Swapchain_p, *DepthStencil_p};
        FramebufferInfo.pAttachments = Attachments_p;

        NH_CHECK_VULKAN(Driver_p->Func.vkCreateFramebuffer(Driver_p->Device, &FramebufferInfo, VK_NULL_HANDLE, &Surface_p->Framebuffer_p[i]))
    }

NH_END(NH_SUCCESS);
}

static NH_RESULT Nh_Vk_chooseSurfaceFormat(
    VkSurfaceFormatKHR availableFormats[], VkSurfaceFormatKHR *format_p) 
{
NH_BEGIN();

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
    
NH_END(NH_SUCCESS);
}

static NH_RESULT Nh_Vk_choosePresentMode(
    const VkPresentModeKHR *availablePresentModes_p, VkPresentModeKHR *presentMode_p) 
{
NH_BEGIN();

    // VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR; 
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    // VkPresentModeKHR bestMode = VK_PRESENT_MODE_MAILBOX_KHR;
    // VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR; 
    *presentMode_p = bestMode;
    
NH_END(NH_SUCCESS);
}

