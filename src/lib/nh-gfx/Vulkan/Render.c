// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Render.h"
#include "Vulkan.h"
#include "Surface.h"
#include "GPU.h"

#include "../Base/Viewport.h"
#include "../Base/Surface.h"
#include "../Common/Macros.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_prepareVulkanRendering(
    nh_gfx_VulkanSurface *Surface_p)
{
    VkResult result = Surface_p->GPU_p->Driver.Functions.vkAcquireNextImageKHR(
        Surface_p->GPU_p->Driver.Device, Surface_p->SwapchainKHR, UINT64_MAX, Surface_p->Sync.Semaphore_p[0], 
        VK_NULL_HANDLE, &Surface_p->currentImage
    );

    NH_GFX_CHECK_VULKAN(result)

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_clearVulkanSurface(
    nh_gfx_Surface *Surface_p, nh_gfx_VulkanDriver *Driver_p)
{
    VkCommandBuffer *CommandBuffer_p = &Surface_p->Vulkan.CommandBuffers_p[Surface_p->Vulkan.currentImage];

    VkCommandBufferBeginInfo CommandBufferInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkBeginCommandBuffer(*CommandBuffer_p, &CommandBufferInfo))

    VkClearColorValue clearColorValue = {{
        Surface_p->Settings.BackgroundColor.r, 
        Surface_p->Settings.BackgroundColor.g, 
        Surface_p->Settings.BackgroundColor.b, 
        Surface_p->Settings.BackgroundColor.a
    }};

    VkImageSubresourceRange imageSubresourceRange =
    {
        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel   = 0,
        .levelCount     = 1,
        .baseArrayLayer = 0,
        .layerCount     = 1
    };

    VkImageMemoryBarrier imageMemoryBarrier = {0};
    imageMemoryBarrier.sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.oldLayout        = VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier.newLayout        = VK_IMAGE_LAYOUT_GENERAL;
    imageMemoryBarrier.image            = Surface_p->Vulkan.Swapchain.Image_p[Surface_p->Vulkan.currentImage];
    imageMemoryBarrier.subresourceRange = imageSubresourceRange;
    imageMemoryBarrier.srcAccessMask    = VK_ACCESS_SHADER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask    = VK_ACCESS_SHADER_WRITE_BIT;

    Driver_p->Functions.vkCmdPipelineBarrier(
        *CommandBuffer_p, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &imageMemoryBarrier
    );

    Driver_p->Functions.vkCmdClearColorImage(
        *CommandBuffer_p, Surface_p->Vulkan.Swapchain.Image_p[Surface_p->Vulkan.currentImage],
        VK_IMAGE_LAYOUT_GENERAL, &clearColorValue, 1, &imageSubresourceRange
    );

    Driver_p->Functions.vkEndCommandBuffer(*CommandBuffer_p);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_renderVulkan( // TODO multi GPU rendering
    nh_gfx_Surface *Surface_p, nh_core_List *SortedViewports_p)
{
    VkSemaphore *Semaphores_pp[2] = {&Surface_p->Vulkan.Sync.Semaphore_p[0], &Surface_p->Vulkan.Sync.Semaphore_p[1]};

    VkCommandBuffer *Commands_p = (VkCommandBuffer*)nh_core_allocate(sizeof(void*)*(SortedViewports_p->size+1));
    NH_CORE_CHECK_MEM(Commands_p)

    // insert clear commandbuffer
    NH_CORE_CHECK(nh_gfx_clearVulkanSurface(Surface_p, &Surface_p->Vulkan.GPU_p->Driver))
    Commands_p[0] = Surface_p->Vulkan.CommandBuffers_p[Surface_p->Vulkan.currentImage];

    // get viewport commandbuffers
    for (int i = 0, j = 1; i < SortedViewports_p->size; ++i, ++j)
    {
        nh_gfx_Viewport *Viewport_p = SortedViewports_p->pp[i];
        Viewport_p->Sync.submittedBuffers_p[Surface_p->Vulkan.currentImage]
            = Viewport_p->Sync.newestBuffers_p[Surface_p->Vulkan.currentImage];

//        while (Viewport_p->Sync.isRecording) {}

        Commands_p[j] = Viewport_p->Vulkan.CommandBuffers_p[
            Viewport_p->Sync.submittedBuffers_p[
                Surface_p->Vulkan.currentImage
            ]
        ];
    }

    // submit graphics
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo submitInfo = 
    {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount   = 1,
        .signalSemaphoreCount = 0,
        .pWaitSemaphores      = Semaphores_pp[0], 
        .pWaitDstStageMask    = waitStages,
        .pSignalSemaphores    = VK_NULL_HANDLE,
        .commandBufferCount   = SortedViewports_p->size+1,
        .pCommandBuffers      = Commands_p,
    };

    nh_gfx_VulkanGPU *GPU_p = Surface_p->Vulkan.GPU_p;

    NH_GFX_CHECK_VULKAN(GPU_p->Driver.Functions.vkQueueSubmit(GPU_p->Driver.GraphicsQueue, 1, &submitInfo, Surface_p->Vulkan.Sync.Fence))
    GPU_p->Driver.Functions.vkWaitForFences(GPU_p->Driver.Device, 1, &Surface_p->Vulkan.Sync.Fence, VK_TRUE, UINT64_MAX);
    GPU_p->Driver.Functions.vkResetFences(GPU_p->Driver.Device, 1, &Surface_p->Vulkan.Sync.Fence); 

    // present graphics
    VkPresentInfoKHR presentInfo =
    {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .swapchainCount     = 1,
        .pSwapchains        = &Surface_p->Vulkan.SwapchainKHR,
        .pWaitSemaphores    = VK_NULL_HANDLE,
        .waitSemaphoreCount = 0,
        .pImageIndices      = &Surface_p->Vulkan.currentImage
    };
    
    NH_GFX_CHECK_VULKAN(GPU_p->Driver.Functions.vkQueuePresentKHR(GPU_p->Driver.GraphicsQueue, &presentInfo))

    nh_core_free(Commands_p); 

    return NH_API_SUCCESS;
}

