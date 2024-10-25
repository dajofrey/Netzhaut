// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Uniform.h"

#include "../Main/Vertices.h"

#include "../../nh-gfx/Common/Macros.h"
#include "../../nh-css/Properties/Color.h"
#include "../../nh-core/Util/Array.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_renderer_vk_createUniform(
    nh_gfx_VulkanDriver *Driver_p, void *data_p, size_t size, nh_gfx_VulkanBuffer *Buffer_p)
{
    VkBufferCreateInfo CreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = size,
        .usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    nh_gfx_VulkanBufferInfo BufferInfo =
    {
        .Info_p           = &CreateInfo,
        .data_p           = data_p,
        .mapMemory        = true,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = false,
    };

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanBuffer(Driver_p, &BufferInfo, Buffer_p))

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_renderer_createVulkanColorUniform(
    nh_gfx_VulkanDriver *Driver_p, nh_css_Fragment *Fragment_p, NH_RENDERER_VK_UNIFORM type)
{
    float data_p[20] = {0.0f};
    data_p[0] = data_p[5] = data_p[10] = data_p[15] = 1.0f;

    float color_p[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    switch (type)
    {
        case NH_RENDERER_VK_UNIFORM_TOP_BORDER    : nh_css_colorToArray(Fragment_p->Box.Values.BorderTopColor, color_p); break;
        case NH_RENDERER_VK_UNIFORM_RIGHT_BORDER  : nh_css_colorToArray(Fragment_p->Box.Values.BorderRightColor, color_p); break;
        case NH_RENDERER_VK_UNIFORM_BOTTOM_BORDER : nh_css_colorToArray(Fragment_p->Box.Values.BorderBottomColor, color_p); break;
        case NH_RENDERER_VK_UNIFORM_LEFT_BORDER   : nh_css_colorToArray(Fragment_p->Box.Values.BorderLeftColor, color_p); break;
        case NH_RENDERER_VK_UNIFORM_BACKGROUND    : nh_css_colorToArray(Fragment_p->Box.Values.BackgroundColor, color_p); break;
        default : return NH_API_ERROR_BAD_STATE;
    }

    data_p[16] = color_p[0];
    data_p[17] = color_p[1];
    data_p[18] = color_p[2];
    data_p[19] = color_p[3];

    nh_gfx_VulkanBuffer *Buffer_p = nh_core_incrementArray(&((nh_renderer_VulkanData*)Fragment_p->data_p)->Uniforms);
    NH_CORE_CHECK_MEM(Buffer_p)

    Buffer_p->type = type;

    NH_CORE_CHECK(nh_renderer_vk_createUniform(Driver_p, data_p, sizeof(float) * 20, Buffer_p))

    return NH_API_SUCCESS;
}

//NH_API_RESULT nh_vk_createImageUniform(
//    nh_gfx_VulkanDriver *Driver_p, nh_html_Node *Node_p, nh_Texture *Texture_p)
//{
//    float data_p[17] = {0.0f};
//    data_p[0] = data_p[5] = data_p[10] = data_p[15] = 1.0f;
//
//    data_p[16] = Texture_p->gamma;
//    
//    nh_gfx_VulkanBuffer *Buffer_p;
//    NH_CHECK(nh_vk_allocateUniform(Node_p, &Buffer_p))
//    Buffer_p->type = NH_VK_UNIFORM_IMAGE;
//
//    NH_CHECK(nh_vk_createUniform(
//        nh_vk_getHost(), Driver_p, data_p, sizeof(float) * 17, Buffer_p
//    ))
//
//return NH_SUCCESS;
//}

//NH_API_RESULT nh_vk_createBackgroundImageUniform(
//    nh_Content *Content_p, nh_gfx_VulkanDriver *Driver_p, nh_html_Node *Node_p, NH_RENDERER_Image *Image_p)
//{
//    float data_p[19] = {0.0f};
//    data_p[0] = data_p[5] = data_p[10] = data_p[15] = 1.0f;
//    data_p[16] = Image_p->Texture_p->gamma;
//
//    nh_html_PixelBox Box;
//
//    switch (Image_p->Background.origin)
//    {
//        case NH_RENDERER_BACKGROUND_ORIGIN_PADDING_BOX : Box = nh_html_getPaddingPixelBox(Node_p, 0); break;
//        case NH_RENDERER_BACKGROUND_ORIGIN_BORDER_BOX  : Box = nh_html_getBorderPixelBox(Node_p, 0); break;
//        case NH_RENDERER_BACKGROUND_ORIGIN_CONTENT_BOX : Box = nh_html_getContentPixelBox(Node_p, 0); break;
//    }
//
//    float repeat_p[2];
//    nh_gfx_getScaledTextureRepeat(Content_p, &Box, Image_p->Texture_p, repeat_p);
//
//    data_p[17] = 1.0f;
//    data_p[18] = 1.0f;
//
//    if (Image_p->Background.Size_p[0].type == NH_RENDERER_BACKGROUND_SIZE_CONTAIN) {
//        data_p[17] = repeat_p[0];
//    }
//    if (Image_p->Background.Size_p[1].type == NH_RENDERER_BACKGROUND_SIZE_CONTAIN) {
//        data_p[18] = repeat_p[1];
//    }
// 
//    nh_gfx_VulkanBuffer *Buffer_p;
//    NH_CHECK(nh_vk_allocateUniform(Node_p, &Buffer_p))
//    Buffer_p->type = NH_VK_UNIFORM_BACKGROUND_IMAGE;
//
//    Image_p->uniform = Node_p->Vulkan.Uniforms.count - 1;
//
//    NH_CHECK(nh_vk_createUniform(
//        nh_vk_getHost(), Driver_p, data_p, sizeof(float) * 19, Buffer_p
//    ))
//
//return NH_SUCCESS;
//}

//NH_API_RESULT nh_vk_createCanvasRenderingContext2DArcUniform(
//    nh_Tab *Tab_p, nh_gfx_VulkanDriver *Driver_p, nh_html_Node *Node_p, NH_JS_CanvasRenderingContext2D *Context_p, 
//    NH_JS_Path2DArc *Arc_p)
//{
//    nh_html_ClipBox ContentBox = nh_renderer_getContentPixelBox(Node_p);
//    nh_renderer_resize(Tab_p, Node_p, &ContentBox);
//
//    int wh_p[2];
//    nh_renderer_getBoxSize(Tab_p, &ContentBox, wh_p);
//
//    float data_p[29] = {0.0f};
//    data_p[0]  = data_p[5]  = data_p[10] = data_p[15] = 1.0f;
//    data_p[16] = data_p[17] = data_p[18] = 0.0f;
//    data_p[19] = data_p[20] = data_p[21] = 1.0f;
//    data_p[22] = (float) wh_p[0];
//    data_p[23] = (float) wh_p[1];
//    data_p[24] = Arc_p->x;
//    data_p[25] = Arc_p->y;
//    data_p[26] = Arc_p->radius;
//    data_p[27] = Arc_p->startAngle;
//    data_p[28] = Arc_p->endAngle;
//
//    nh_gfx_VulkanBuffer *Buffer_p = nh_core_allocate(sizeof(nh_gfx_VulkanBuffer));
//    NH_CHECK_MEM(Buffer_p)
//    NH_CHECK(nh_core_appendToLinkedList(&Context_p->Vulkan.Uniforms, Buffer_p))
//
//    NH_CHECK(nh_vk_createUniform(
//        nh_vk_getHost(), Driver_p, data_p, sizeof(float) * 29, Buffer_p
//    ))
//
//return NH_SUCCESS;
//}

void nh_renderer_destroyVulkanUniformArray(
    nh_gfx_VulkanDriver *Driver_p, nh_renderer_VulkanData *Data_p)
{
    for (int i = 0; i < Data_p->Uniforms.length; ++i) {
        nh_gfx_VulkanBuffer *Buffer_p = &((nh_gfx_VulkanBuffer*)Data_p->Uniforms.p)[i];
        nh_gfx_destroyVulkanBuffer(Driver_p, Buffer_p);
    }

    nh_core_freeArray(&Data_p->Uniforms);
}

//void nh_vk_destroyUniform(
//    nh_gfx_VulkanDriver *Driver_p, nh_html_Node *Node_p, NH_VK_UNIFORM type)
//{
//    for (int i = 0; i < Node_p->Vulkan.Uniforms.count; ++i) {
//        nh_gfx_VulkanBuffer *Buffer_p = nh_core_getFromLinkedList(&Node_p->Vulkan.Uniforms, i);
//        if (Buffer_p->type == type) {nh_gfx_destroyVulkanBuffer(Driver_p, Buffer_p);}
//    }
//
//NH_CORE_SILENT_END()
//}

nh_gfx_VulkanBuffer *nh_renderer_getVulkanUniform(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VK_UNIFORM type)
{
    for (int i = 0; i < Data_p->Uniforms.length; ++i) {
        nh_gfx_VulkanBuffer *Buffer_p = &((nh_gfx_VulkanBuffer*)Data_p->Uniforms.p)[i];
        if (Buffer_p->type == type) {return Buffer_p;}
    }

    return NULL;
}

nh_gfx_VulkanBuffer *nh_renderer_getVulkanUniformFromIndex(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VK_UNIFORM type, unsigned int index)
{
   int count = 0;
    for (int i = 0; i < Data_p->Uniforms.length; ++i) {
        nh_gfx_VulkanBuffer *Uniform_p = &((nh_gfx_VulkanBuffer*)Data_p->Uniforms.p)[i]; 
        if (Uniform_p != NULL && Uniform_p->type == type) {
            if (count == index) {return Uniform_p;}
            else {count++;}
        }
    }

    return NULL;
}

