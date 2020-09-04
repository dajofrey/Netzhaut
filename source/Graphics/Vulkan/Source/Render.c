// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Render.h"
#include "../Header/Surface.h"
#include "../Header/Driver.h"
#include "../Header/GPU.h"
#include "../Header/Utils.h"
#include "../Header/Macros.h"
#include "../Header/Vulkan.h"

#include "../../../Core/Header/Window.h"
#include "../../../Core/Header/Tab.h"
#include "../../../Core/Header/Memory.h"
#include "../../../Core/Header/Sync.h"
#include "../../../Core/Header/List.h"
#include "../../../Core/Header/Config.h"

#include "../../../CSS/Header/Box.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_Vk_clearWindow(
    Nh_Window *Window_p, Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p
);

// PREPARE =========================================================================================

NH_RESULT Nh_Vk_prepareRendering(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Nh_Vk_GPU *GPU_p         = Window_p->GPU.Pointer;
    Nh_Vk_Surface *Surface_p = &Window_p->Vulkan.Surface;
    Nh_Vk_Driver *Driver_p   = &GPU_p->Driver;

    VkResult result = Driver_p->Func.vkAcquireNextImageKHR(
        Driver_p->Device, Surface_p->SwapchainKHR, UINT64_MAX, Surface_p->Sync.Semaphore_p[0], 
        VK_NULL_HANDLE, &Surface_p->currentImage
    );

    NH_CHECK_VULKAN(result)

NH_END(NH_SUCCESS)
}

// RENDER ==========================================================================================

NH_RESULT Nh_Vk_render(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Nh_Vk_GPU *GPU_p = Window_p->GPU.Pointer;

    Nh_Vk_Surface *Surface_p = &Window_p->Vulkan.Surface;
    VkSemaphore *Semaphores_pp[2] = {&Surface_p->Sync.Semaphore_p[0], &Surface_p->Sync.Semaphore_p[1]};

    int tabCount = Window_p->Tabs.count;
    bool render_p[tabCount];
    int count = 1;
    
    VkCommandBuffer Commands_p[tabCount];
    VkCommandBuffer SortedCommands_p[tabCount + 1];   

    // find out which commandbuffers to render
    for (int i = 0; i < tabCount; ++i) {
        render_p[i] = Nh_getTab(Window_p, i)->Sync.render;
    }

    // insert clear commandbuffer
    NH_CHECK(Nh_Vk_clearWindow(Window_p, &GPU_p->Driver, Surface_p))
    SortedCommands_p[0] = Surface_p->CommandBuffers_p[Surface_p->currentImage];

    // get commandbuffers
    for (int i = 0; i < tabCount; ++i) 
    {
        if (render_p[i])
        {
            Nh_Tab *Tab_p = Nh_getTab(Window_p, i);;
            Tab_p->Sync.submittedBuffers_p[Surface_p->currentImage]
                = Tab_p->Sync.newestBuffers_p[Surface_p->currentImage];

            Nh_syncUseCommandBuffer(Tab_p);

            Commands_p[i] = Tab_p->Vulkan.CommandBuffers_p[
                Tab_p->Sync.submittedBuffers_p[
                    Surface_p->currentImage
                ]
            ];
        }
    }

    // sort commandbuffers based on tab priority (from low to high priority)
    int sortedIndices_p[tabCount], max = 0;

    for (int i = 0; i < tabCount; ++i) {
        Nh_Tab *Tab_p = Nh_getTab(Window_p, i);
        if (render_p[i] && Tab_p->Info.priority > max) {max = Tab_p->Info.priority;}
    }
    
    for (int i = 0; i < tabCount; ++i) {
        if (!render_p[i]) {continue;}
        int diff = max, newMax = -1;
        for (int j = 0; j < tabCount; ++j) {
            if (!render_p[j]) {continue;}
            Nh_Tab *Tab_p = Nh_getTab(Window_p, j);
            if ((Tab_p->Info.priority - max) == 0) {sortedIndices_p[i] = j;}
            else if ((max - Tab_p->Info.priority) <= diff) {diff = max - Tab_p->Info.priority; newMax = Tab_p->Info.priority;}
        }
        max = newMax;
    }

    bool onTop_p[tabCount];
    for (int i = 0; i < tabCount; ++i) {
        if (render_p[i]) {onTop_p[i] = Nh_getTab(Window_p, i)->Flags.alwaysOnTop;}
    }

    for (int i = 0; i < tabCount; ++i) {
        if (render_p[i] && !onTop_p[i]){SortedCommands_p[count++] = Commands_p[sortedIndices_p[i]];}
    }
    for (int i = 0; i < tabCount; ++i) {
        if (render_p[i] && onTop_p[i]){SortedCommands_p[count++] = Commands_p[sortedIndices_p[i]];}
    }

    // don't render if there's nothing to do
    Window_p->Internal.getNextImage = count >= 1;
    if (!Window_p->Internal.getNextImage) {NH_END(NH_SUCCESS)}

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
        .commandBufferCount   = count,
        .pCommandBuffers      = SortedCommands_p,
    };

    NH_CHECK_VULKAN(GPU_p->Driver.Func.vkQueueSubmit(GPU_p->Driver.GraphicsQueue, 1, &submitInfo, Surface_p->Sync.Fence))
    GPU_p->Driver.Func.vkWaitForFences(GPU_p->Driver.Device, 1, &Surface_p->Sync.Fence, VK_TRUE, UINT64_MAX);
    GPU_p->Driver.Func.vkResetFences(GPU_p->Driver.Device, 1, &Surface_p->Sync.Fence); 

    // present graphics
    VkPresentInfoKHR presentInfo =
    {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .swapchainCount     = 1,
        .pSwapchains        = &Surface_p->SwapchainKHR,
        .pWaitSemaphores    = VK_NULL_HANDLE,
        .waitSemaphoreCount = 0,
        .pImageIndices      = &Surface_p->currentImage
    };
    
    NH_CHECK_VULKAN(GPU_p->Driver.Func.vkQueuePresentKHR(GPU_p->Driver.GraphicsQueue, &presentInfo))

NH_END(NH_SUCCESS)
}

// CLEAR ===========================================================================================

static NH_RESULT Nh_Vk_clearWindow(
    Nh_Window *Window_p, Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p)
{
NH_BEGIN()

    VkCommandBuffer *CommandBuffer_p = &Surface_p->CommandBuffers_p[Surface_p->currentImage];

    VkCommandBufferBeginInfo CommandBufferInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    NH_CHECK_VULKAN(Driver_p->Func.vkBeginCommandBuffer(*CommandBuffer_p, &CommandBufferInfo))

    VkClearColorValue clearColorValue = {{
        Window_p->Info.backgroundColor_p[0], 
        Window_p->Info.backgroundColor_p[1], 
        Window_p->Info.backgroundColor_p[2], 
        Window_p->Info.backgroundColor_p[3]
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
    imageMemoryBarrier.image            = Surface_p->Swapchain.Image_p[Surface_p->currentImage];
    imageMemoryBarrier.subresourceRange = imageSubresourceRange;
    imageMemoryBarrier.srcAccessMask    = VK_ACCESS_SHADER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask    = VK_ACCESS_SHADER_WRITE_BIT;

    Driver_p->Func.vkCmdPipelineBarrier(
        *CommandBuffer_p, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &imageMemoryBarrier
    );

    Driver_p->Func.vkCmdClearColorImage(
        *CommandBuffer_p, Surface_p->Swapchain.Image_p[Surface_p->currentImage],
        VK_IMAGE_LAYOUT_GENERAL, &clearColorValue, 1, &imageSubresourceRange
    );

    Driver_p->Func.vkEndCommandBuffer(*CommandBuffer_p);

NH_END(NH_SUCCESS)
}

