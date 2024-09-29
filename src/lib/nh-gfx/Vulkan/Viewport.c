// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Viewport.h"

#include "../Base/Viewport.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Thread.h"

#include <string.h>

// RESOURCES =======================================================================================

NH_API_RESULT nh_vk_createViewport(
    nh_gfx_Viewport *Viewport_p)
{
NH_GFX_BEGIN()

    nh_vk_Driver *Driver_p = &Viewport_p->Surface_p->Vulkan.GPU_p->Driver;

    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = Driver_p->GraphicsCommandPools_p[nh_core_getThreadIndex()],  
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 6,
    };

    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkAllocateCommandBuffers(
        Driver_p->Device, &AllocateInfo, Viewport_p->Vulkan.CommandBuffers_p
    ))

NH_GFX_DIAGNOSTIC_END(NH_API_SUCCESS)
}

void nh_vk_destroyViewport(
    nh_gfx_Viewport *Viewport_p)
{
NH_GFX_BEGIN()

    nh_vk_Driver *Driver_p = &Viewport_p->Surface_p->Vulkan.GPU_p->Driver;
  
    Driver_p->Functions.vkFreeCommandBuffers(
        Driver_p->Device, 
        Driver_p->GraphicsCommandPools_p[nh_core_getThreadIndex()], 
        6,
        Viewport_p->Vulkan.CommandBuffers_p
    );

NH_GFX_SILENT_END()
}

// RECORD ==========================================================================================

static VkRect2D nh_vk_getDefaultScissor(
    nh_api_PixelSize Size)
{
NH_GFX_BEGIN()

    VkOffset2D Offset = {
        .x = 0,
        .y = 0,
    };
    VkExtent2D Extent = {
        .width  = Size.width,
        .height = Size.height,
    };
    VkRect2D Scissor = 
    { 
        .offset = Offset,
        .extent = Extent, 
    };

NH_GFX_END(Scissor)
}

NH_API_RESULT nh_vk_beginRecording(
    nh_gfx_Viewport *Viewport_p)
{
NH_GFX_BEGIN()

    nh_vk_Driver *Driver_p = &Viewport_p->Surface_p->Vulkan.GPU_p->Driver;

    for (int i = 0; i < Viewport_p->Vulkan.images; ++i)
    {
        VkCommandBuffer *CommandBuffer_p = Viewport_p->Vulkan.CommandBuffers_pp[i];
        Driver_p->Functions.vkResetCommandBuffer(*CommandBuffer_p, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

        VkCommandBufferBeginInfo CommandBufferInfo = 
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
        };

        NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkBeginCommandBuffer(*CommandBuffer_p, &CommandBufferInfo))

        VkClearColorValue clearColorValue = {{
            Viewport_p->Settings.ClearColor.r,
            Viewport_p->Settings.ClearColor.g,
            Viewport_p->Settings.ClearColor.b,
            Viewport_p->Settings.ClearColor.a
        }};
        VkClearDepthStencilValue depthStencilValue =
        {
            .depth   = 1.0f,
            .stencil = 0
        };
        VkClearValue clearValues[2];
        clearValues[0].color        = clearColorValue;
        clearValues[1].depthStencil = depthStencilValue;

        VkRenderPassBeginInfo RenderPassInfo =
        {
            .sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass               = Driver_p->RenderPass_p[0],
            .framebuffer              = Viewport_p->Surface_p->Vulkan.Framebuffer_p[i],
            .renderArea.offset.x      = 0,
            .renderArea.offset.y      = 0,
            .renderArea.extent.width  = Viewport_p->Surface_p->Vulkan.Extent2D.width,
            .renderArea.extent.height = Viewport_p->Surface_p->Vulkan.Extent2D.height,
            .clearValueCount          = 2,
            .pClearValues             = clearValues
        };

        Driver_p->Functions.vkCmdBeginRenderPass(*CommandBuffer_p, &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport Viewport = 
        { 
            .x        = Viewport_p->Settings.Position.x, 
            .y        = Viewport_p->Settings.Position.y, 
            .width    = Viewport_p->Settings.Size.width, 
            .height   = Viewport_p->Settings.Size.height, 
            .minDepth = 0.0f, 
            .maxDepth = 1.0f 
        };

        VkRect2D Scissor = nh_vk_getDefaultScissor(Viewport_p->Settings.Size); 
        Scissor.offset.x = Viewport_p->Settings.Position.x;
        Scissor.offset.y = Viewport_p->Settings.Position.y;

        Driver_p->Functions.vkCmdSetViewport(*CommandBuffer_p, 0, 1, &Viewport);
        Driver_p->Functions.vkCmdSetScissor(*CommandBuffer_p, 0, 1, &Scissor);

        VkClearAttachment ClearAttachment = {
            .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
            .colorAttachment = 0,
            .clearValue      = clearValues[0],
        };

        VkClearRect ClearRect = { 
            .rect           = Scissor,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        };
        Driver_p->Functions.vkCmdClearAttachments(*CommandBuffer_p, 1, &ClearAttachment, 1, &ClearRect);
    }

NH_GFX_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_vk_endRecording(
    nh_gfx_Viewport *Viewport_p)
{
NH_GFX_BEGIN()

    for (int i = 0; i < Viewport_p->Vulkan.images; ++i) {
        Viewport_p->Surface_p->Vulkan.GPU_p->Driver.Functions.vkCmdEndRenderPass(*Viewport_p->Vulkan.CommandBuffers_pp[i]);
        Viewport_p->Surface_p->Vulkan.GPU_p->Driver.Functions.vkEndCommandBuffer(*Viewport_p->Vulkan.CommandBuffers_pp[i]);
    }

NH_GFX_DIAGNOSTIC_END(NH_API_SUCCESS)
}

