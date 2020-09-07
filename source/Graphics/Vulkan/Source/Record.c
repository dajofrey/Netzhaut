// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Record.h"
#include "../Header/Driver.h"
#include "../Header/Surface.h"
#include "../Header/Buffers.h"
#include "../Header/Descriptors.h"
#include "../Header/GPU.h"
#include "../Header/Utils.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/Sync.h"
#include "../../../Core/Header/Callbacks.h"
#include "../../../Core/Header/Memory.h"

#include "../../../CSS/Header/Helper.h"
#include "../../../CSS/Header/Box.h"

#include "../../../JavaScript/DOM/Header/Path2D.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include <string.h>

// DECLARE =========================================================================================

static inline void Nh_Vk_recordExternal(
    VkCommandBuffer *CommandBuffer_p, VkViewport Viewport, Nh_Vk_Driver *Driver_p, Nh_Tab *Tab_p, 
    NH_Vk_Pipeline *Pipelines_p, Nh_HTML_Node *Node_p
);
static inline void Nh_Vk_recordInternal(
    VkCommandBuffer *CommandBuffer_p, VkViewport *Viewport_p, VkRect2D *Scissor_p, Nh_Vk_Driver *Driver_p, 
    Nh_Tab *Tab_p, NH_Vk_Pipeline *Pipelines_p, Nh_HTML_Node *Node_p
);

static void Nh_Vk_recordBackground(
    Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p, 
    VkCommandBuffer *CommandBuffer_p
);
static void Nh_Vk_recordBorder(
    Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p
);
static void Nh_Vk_recordText(
    Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p
);
static void Nh_Vk_recordImage(
    Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p
);
static void Nh_Vk_recordBackgroundImage(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p, 
    VkCommandBuffer *CommandBuffer_p, VkViewport *Viewport_p, VkRect2D *Scissor_p
);
static void Nh_Vk_recordCanvasRenderingContext2D(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p, 
    VkCommandBuffer *CommandBuffer_p, NH_JS_CanvasRenderingContext2D *Context_p
);

// RECORD ==========================================================================================

NH_RESULT Nh_Vk_record(
    Nh_Vk_Driver *Driver_p, Nh_Vk_Surface *Surface_p, Nh_Tab *Tab_p, NH_Vk_Pipeline *Pipelines_p, 
    int bufferIndex, int imageIndex)
{
NH_BEGIN()

    VkCommandBuffer *CommandBuffer_p = &Tab_p->Vulkan.CommandBuffers_p[bufferIndex];
    Driver_p->Func.vkResetCommandBuffer(*CommandBuffer_p, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

    VkCommandBufferBeginInfo CommandBufferInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };

    NH_CHECK_VULKAN(Driver_p->Func.vkBeginCommandBuffer(*CommandBuffer_p, &CommandBufferInfo))

    VkClearColorValue clearColorValue =
    {
        {1.0f, 1.0f, 1.0f, 1.0f}
    };
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
        .framebuffer              = Surface_p->Framebuffer_p[imageIndex],
        .renderArea.offset.x      = 0,
        .renderArea.offset.y      = 0,
        .renderArea.extent.width  = Surface_p->Extent2D.width,
        .renderArea.extent.height = Surface_p->Extent2D.height,
        .clearValueCount          = 2,
        .pClearValues             = clearValues
    };

    Driver_p->Func.vkCmdBeginRenderPass(*CommandBuffer_p, &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport Viewport = 
    { 
        .x        = Tab_p->Info.relativeOffset_p[0] * Surface_p->Extent2D.width, 
        .y        = Tab_p->Info.relativeOffset_p[1] * Surface_p->Extent2D.height, 
        .width    = Tab_p->Info.pxSize_p[0], 
        .height   = Tab_p->Info.pxSize_p[1], 
        .minDepth = 0.0f, 
        .maxDepth = 1.0f 
    };

    VkOffset2D Offset =
    {
        .x = 0,
        .y = 0
    };
    VkRect2D Scissor = 
    { 
        .offset = Offset,
        .extent = Surface_p->Extent2D
    };

    if (Tab_p->Flags.crop) 
    {
        int wh_p[2] = {0}, xy_p[2] = {0};

        Nh_CSS_Box Box = Nh_CSS_getCropBox(Tab_p);

        Nh_CSS_getBoxSize(Tab_p, &Box, wh_p);
        Nh_CSS_getBoxOffsetRelToWindow(Tab_p, &Box, xy_p);

        Offset.x = xy_p[0];
        Offset.y = xy_p[1];

        VkExtent2D Extent = {
            .width  = wh_p[0],
            .height = wh_p[1]
        };

        Scissor.offset = Offset;
        Scissor.extent = Extent;
    }

    Driver_p->Func.vkCmdSetViewport(*CommandBuffer_p, 0, 1, &Viewport);
    Driver_p->Func.vkCmdSetScissor(*CommandBuffer_p, 0, 1, &Scissor);

    bool *inFlow_p = Nh_allocate(sizeof(bool) * Tab_p->Document.Tree.Flat.Formatted.count);
    NH_CHECK_MEM(inFlow_p);
    memset(inFlow_p, true, sizeof(bool) * Tab_p->Document.Tree.Flat.Formatted.count);

    for (int i = 0; i < Tab_p->Document.Tree.Flat.Formatted.count; ++i)
    {
        Nh_HTML_Node *Node_p = Nh_getListItem(&Tab_p->Document.Tree.Flat.Formatted, i);
        if (!Nh_CSS_respectFlow(Node_p, NH_TRUE)) {inFlow_p[i] = false; continue;}

        Nh_Vk_recordExternal(CommandBuffer_p, Viewport, Driver_p, Tab_p, Pipelines_p, Node_p);
        Nh_Vk_recordInternal(CommandBuffer_p, &Viewport, &Scissor, Driver_p, Tab_p, Pipelines_p, Node_p);
    }

    for (int i = 0; i < Tab_p->Document.Tree.Flat.Formatted.count; ++i)
    {
        Nh_HTML_Node *Node_p = Nh_getListItem(&Tab_p->Document.Tree.Flat.Formatted, i);

        if (inFlow_p[i] == false)
        {
            Nh_Vk_recordExternal(CommandBuffer_p, Viewport, Driver_p, Tab_p, Pipelines_p, Node_p);
            Nh_Vk_recordInternal(CommandBuffer_p, &Viewport, &Scissor, Driver_p, Tab_p, Pipelines_p, Node_p);
        }
    }

    Driver_p->Func.vkCmdEndRenderPass(*CommandBuffer_p);
    Driver_p->Func.vkEndCommandBuffer(*CommandBuffer_p);

    Nh_free(inFlow_p);

NH_END(NH_SUCCESS)
}

static inline void Nh_Vk_recordExternal(
    VkCommandBuffer *CommandBuffer_p, VkViewport Viewport, Nh_Vk_Driver *Driver_p, Nh_Tab *Tab_p, 
    NH_Vk_Pipeline *Pipelines_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_VulkanCallback Callback = Nh_getVulkanCallback(Tab_p, Node_p->Computed.Attributes.id_p);
    
    if (Callback != NULL) 
    {
//        Nh_Vk_recordBackground(Node_p, Pipelines_p, Driver_p, CommandBuffer_p);
//
//        Nh_CSS_Box ContentBox;
//        Nh_CSS_getContentBox(Tab_p, Node_p, &ContentBox, NULL);
//
//        int offset_p[2], size_p[2];
//        Nh_CSS_getBoxOffsetRelToWindow(Tab_p, &ContentBox, offset_p);
//        Nh_CSS_getBoxSize(Tab_p, &ContentBox, offset_p);
//
//        VkViewport Viewport2 = 
//        { 
//            .x        = offset_p[0], 
//            .y        = offset_p[1], 
//            .width    = size_p[0], 
//            .height   = size_p[1], 
//            .minDepth = 0.0f, 
//            .maxDepth = 1.0f 
//        };
//        Driver_p->Func.vkCmdSetViewport(*CommandBuffer_p, 0, 1, &Viewport2);
//
//        Callback(Driver_p->Func, CommandBuffer_p);
//        
//        Driver_p->Func.vkCmdSetViewport(*CommandBuffer_p, 0, 1, &Viewport);
    }

NH_SILENT_END()
}

static inline void Nh_Vk_recordInternal(
    VkCommandBuffer *CommandBuffer_p, VkViewport *Viewport_p, VkRect2D *Scissor_p, Nh_Vk_Driver *Driver_p, 
    Nh_Tab *Tab_p, NH_Vk_Pipeline *Pipelines_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_VulkanCallback Callback = Nh_getVulkanCallback(Tab_p, Node_p->Computed.Attributes.id_p);
    
    if (Callback == NULL) 
    {
        Nh_Vk_recordImage(Node_p, Pipelines_p, Driver_p, CommandBuffer_p);
        Nh_Vk_recordBackground(Node_p, Pipelines_p, Driver_p, CommandBuffer_p);
        Nh_Vk_recordBackgroundImage(Tab_p, Node_p, Pipelines_p, Driver_p, CommandBuffer_p, Viewport_p, Scissor_p);
        Nh_Vk_recordText(Node_p, Pipelines_p, Driver_p, CommandBuffer_p);
        Nh_Vk_recordBorder(Node_p, Pipelines_p, Driver_p, CommandBuffer_p);

//        for (int i = 0; i < Node_p->Injections.count; ++i) 
//        {
//            NH_JS_Injection *Injection_p = Nh_getListItem(&Node_p->Injections, i);
//            switch (Injection_p->type)
//            {
//                case NH_JS_INJECTION_CANVAS_RENDERING_CONTEXT_2D :
//                    Nh_Vk_recordCanvasRenderingContext2D(
//                        Tab_p, Node_p, Pipelines_p, Driver_p, CommandBuffer_p, Injection_p->data_p
//                    );
//                    break;
//            }
//        }
    }

NH_SILENT_END()
}

// TEXTURE =========================================================================================

static void Nh_Vk_recordImage(
    Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p)
{
NH_BEGIN()

    if (NH_HTML_SHOW_IMAGE(Node_p))
    {
        VkDeviceSize size_p[1] = {0};

        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_IMAGE].Pipeline
        );
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_IMAGE].PipelineLayout, 0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_IMAGE)->DescriptorSet,
            0, VK_NULL_HANDLE
        );

        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXTURE)->Buffer, 
            size_p
        );
        
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXTURE)->size / 3, 1, 0, 0);
    }

NH_SILENT_END()
}

// BACKGROUND ======================================================================================

static void Nh_Vk_recordBackground(
    Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p)
{
NH_BEGIN()

    if (!NH_HTML_SHOW_IMAGE(Node_p))
    {
        VkDeviceSize size_p[1] = {0};

        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
        );
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_BACKGROUND)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
        
        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BACKGROUND)->Buffer, 
            size_p
        );
        
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BACKGROUND)->size / 3, 1, 0, 0);
    }

NH_SILENT_END()
}

// BACKGROUND IMAGE ================================================================================

static void Nh_Vk_recordBackgroundImage(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p, 
    VkCommandBuffer *CommandBuffer_p, VkViewport *Viewport_p, VkRect2D *Scissor_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Computed.Properties.Background.Images.count; ++i)
    {
        NH_CSS_Image *Image_p = Nh_CSS_getImage(&Node_p->Computed.Properties.Background.Images, i);

        Nh_CSS_Box Box = Nh_CSS_getBackgroundImageBox(Tab_p, Node_p, Image_p, 0.0f);

        VkOffset2D Offset =
        {
            .x = (int)NH_CSS_DENORMALIZE_X_PIXEL(Box.TopLeft.x, Tab_p),
            .y = (int)NH_CSS_DENORMALIZE_Y_PIXEL(Box.TopLeft.y, Tab_p),
        };
        VkExtent2D Extent = 
        {
            .width  = (int)NH_CSS_DENORMALIZE_X_PIXEL(Box.BottomRight.x, Tab_p) - (int)NH_CSS_DENORMALIZE_X_PIXEL(Box.TopLeft.x, Tab_p),
            .height = (int)NH_CSS_DENORMALIZE_Y_PIXEL(Box.BottomRight.y, Tab_p) - (int)NH_CSS_DENORMALIZE_Y_PIXEL(Box.TopLeft.y, Tab_p),
        };
        VkRect2D Scissor = 
        { 
            .offset = Offset,
            .extent = Extent,
        };

        Driver_p->Func.vkCmdSetScissor(*CommandBuffer_p, 0, 1, &Scissor);

        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_BACKGROUND_IMAGE].Pipeline
        );
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_BACKGROUND_IMAGE].PipelineLayout, 0, 1, 
            &Nh_Vk_getDescriptorFromIndex(Node_p, Image_p->descriptor)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
       
        VkDeviceSize size_p[1] = {0};

        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBufferFromIndex(Node_p, Image_p->buffer)->Buffer, 
            size_p
        );
        
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBufferFromIndex(Node_p, Image_p->buffer)->size / 3, 1, 0, 0);
        Driver_p->Func.vkCmdSetScissor(*CommandBuffer_p, 0, 1, Scissor_p);
    }

NH_SILENT_END()
}

// TEXT ============================================================================================

static void Nh_Vk_recordText(
    Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p)
{
NH_BEGIN()

    if (Nh_HTML_isTextNode(Node_p))
    {
        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_TEXT_SDF].Pipeline
        );
        VkDeviceSize size_p[1] = {0};

        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1,
            &(Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXT_SDF)->Buffer), 
            size_p
        );
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_TEXT_SDF].PipelineLayout, 
            0, 1, 
            &(Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_TEXT_SDF)->DescriptorSet),
            0, VK_NULL_HANDLE
        );

        Driver_p->Func.vkCmdBindIndexBuffer(
            *CommandBuffer_p, 
            Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXT_SDF_INDEX)->Buffer, 
            0, VK_INDEX_TYPE_UINT32
        );

        Driver_p->Func.vkCmdDrawIndexed(
            *CommandBuffer_p, strlen(Node_p->text_p) * 6, 1, 0, 0, 0
        );
    }

NH_SILENT_END()
}

// BORDER ==========================================================================================

static void Nh_Vk_recordBorder(
    Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p,
    VkCommandBuffer *CommandBuffer_p)
{
NH_BEGIN()

    if (NH_CSS_SHOW_TOP_BORDER(Node_p))
    {
        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
        );
        VkDeviceSize size_p[1] = {0};
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 
            0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_TOP_BORDER)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
        
        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TOP_BORDER_VERTICES)->Buffer, 
            size_p
        );
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TOP_BORDER_VERTICES)->size / 3, 1, 0, 0);
    }
    if (NH_CSS_SHOW_RIGHT_BORDER(Node_p))
    {
         Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
        );
        VkDeviceSize size_p[1] = {0};
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 
            0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_RIGHT_BORDER)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
        
        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_RIGHT_BORDER_VERTICES)->Buffer, 
            size_p
        );
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_RIGHT_BORDER_VERTICES)->size / 3, 1, 0, 0);
   
    }
    if (NH_CSS_SHOW_BOTTOM_BORDER(Node_p))
    {
        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
        );
        VkDeviceSize size_p[1] = {0};
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 
            0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_BOTTOM_BORDER)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
        
        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BOTTOM_BORDER_VERTICES)->Buffer, 
            size_p
        );
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BOTTOM_BORDER_VERTICES)->size / 3, 1, 0, 0);

    }
    if (NH_CSS_SHOW_LEFT_BORDER(Node_p))
    {
        Driver_p->Func.vkCmdBindPipeline(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].Pipeline
        );
        VkDeviceSize size_p[1] = {0};
        
        Driver_p->Func.vkCmdBindDescriptorSets(
            *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            Pipelines_p[NH_VK_PIPELINE_COLOR].PipelineLayout, 
            0, 1, 
            &Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_LEFT_BORDER)->DescriptorSet,
            0, VK_NULL_HANDLE
        );
        
        Driver_p->Func.vkCmdBindVertexBuffers(
            *CommandBuffer_p, 0, 1, 
            &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_LEFT_BORDER_VERTICES)->Buffer, 
            size_p
        );
        Driver_p->Func.vkCmdDraw(*CommandBuffer_p, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_LEFT_BORDER_VERTICES)->size / 3, 1, 0, 0);
    }

NH_SILENT_END()
}

// CANVAS RENDERING CONTEXT 2D =====================================================================

static void Nh_Vk_recordCanvasRenderingContext2D(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_Vk_Pipeline *Pipelines_p, Nh_Vk_Driver *Driver_p, 
    VkCommandBuffer *CommandBuffer_p, NH_JS_CanvasRenderingContext2D *Context_p)
{
NH_BEGIN()

    int count = 0;

    for (int i = 0; i < Context_p->Paths.count; ++i)
    {
        NH_JS_Path2D *Path_p = Nh_getListItem(&Context_p->Paths, i);
 
        for (int j = 0; j < Path_p->Commands.count; ++j)
        {
            NH_JS_Path2DCommand *Command_p = Nh_getListItem(&Path_p->Commands, j);
            switch (Command_p->type)
            {
                case NH_JS_PATH_2D_COMMAND_ARC :

                    Driver_p->Func.vkCmdBindPipeline(
                        *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
                        Pipelines_p[NH_VK_PIPELINE_CANVAS_RENDERING_CONTEXT_2D_ARC].Pipeline
                    );
                    
                    Driver_p->Func.vkCmdBindDescriptorSets(
                        *CommandBuffer_p, VK_PIPELINE_BIND_POINT_GRAPHICS, 
                        Pipelines_p[NH_VK_PIPELINE_CANVAS_RENDERING_CONTEXT_2D_ARC].PipelineLayout, 
                        0, 1, 
                        &((NH_Vk_DescriptorSet*)Nh_getListItem(&Context_p->Vulkan.Descriptors, count))->DescriptorSet,
                        0, VK_NULL_HANDLE
                    );

                    VkDeviceSize size_p[1] = {0};
                    Driver_p->Func.vkCmdBindVertexBuffers(
                        *CommandBuffer_p, 0, 1, 
                        &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXTURE)->Buffer, 
                        size_p
                    );
                    Driver_p->Func.vkCmdDraw(*CommandBuffer_p, 6, 1, 0, 0);
           
                    break;
            }
            count++;
        }
    }

NH_SILENT_END()
}

