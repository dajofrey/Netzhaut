// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Buffer.h"

#include "../Main/Vertices.h"

#include "../../nh-gfx/Common/Macros.h"
#include "../../nh-core/Util/Array.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_renderer_createVulkanBackgroundVertices(
    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_css_Fragment *Fragment_p)
{
    int count = nh_renderer_getBackgroundVertices(Viewport_p, Fragment_p, NULL, 10);

    float *vertices_p = nh_core_allocate(sizeof(float) * count);
    NH_CORE_CHECK_MEM(vertices_p)

    nh_renderer_getBackgroundVertices(Viewport_p, Fragment_p, vertices_p, 10);

    nh_gfx_VulkanBuffer *Buffer_p = nh_core_incrementArray(&((nh_renderer_VulkanData*)Fragment_p->data_p)->Buffers);
    NH_CORE_CHECK_MEM(Buffer_p)

    Buffer_p->type = NH_RENDERER_VULKAN_BUFFER_BACKGROUND;
    Buffer_p->size = sizeof(float) * count;

    VkBufferCreateInfo CreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = Buffer_p->size,
        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    nh_gfx_VulkanBufferInfo BufferInfo =
    {
        .Info_p           = &CreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = true,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = true 
    };

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanBuffer(Driver_p, &BufferInfo, Buffer_p))
    nh_core_free(vertices_p);

    return NH_API_SUCCESS;
}

//NH_API_RESULT nh_vk_updateBackgroundVertices(
//    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_renderer_FormattingNode *Fragment_p)
//{
//    float vertices_p[1000];
//    int count = nh_renderer_getBackgroundVertices(Viewport_p, Fragment_p, vertices_p, 10);
//
//    if ((sizeof(float) * count) != nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_BACKGROUND)->size) {
//        printf("realloc buffer!!!\n");
//        exit(0);
//    }
//
//    NH_CHECK(nh_gfx_mapVulkanDeviceMemory(
//        Driver_p, &nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_BACKGROUND)->DeviceMemory, vertices_p, 
//        nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_BACKGROUND)->size, 0
//    ))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//NH_API_RESULT nh_vk_createBackgroundImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_renderer_FormattingNode *Fragment_p, NH_RENDERER_Image *Image_p)
//{
//    float vertices_p[30];
//    NH_CHECK(nh_renderer_getBackgroundImageVertices(Viewport_p, Fragment_p, vertices_p, Image_p, 0.001f))
//
//    nh_gfx_VulkanBuffer *Buffer_p;
//    NH_CHECK(nh_vk_allocateBuffer(Fragment_p, &Buffer_p))
//    Buffer_p->type = NH_VK_BUFFER_BACKGROUND_IMAGE;
//    Buffer_p->size = sizeof(float) * 30;
//
//    Image_p->buffer = Fragment_p->Vulkan.Buffers.count - 1;
//
//    VkBufferCreateInfo CreateInfo = 
//    {
//        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
//        .pNext       = VK_NULL_HANDLE,
//        .size        = Buffer_p->size,
//        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
//    };
//    nh_gfx_VulkanBufferInfo BufferInfo =
//    {
//        .Info_p           = &CreateInfo,
//        .data_p           = vertices_p,
//        .mapMemory        = true,
//        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//        .createInfo       = true 
//    };
//
//    NH_CHECK(nh_gfx_createVulkanBuffer(nh_vk_getHost(), Driver_p, &BufferInfo, Buffer_p))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//NH_API_RESULT nh_vk_updateBackgroundImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_renderer_FormattingNode *Fragment_p, NH_RENDERER_Image *Image_p)
//{
//    float vertices_p[30];
//    NH_CHECK(nh_renderer_getBackgroundImageVertices(Viewport_p, Fragment_p, vertices_p, Image_p, 0.001f))
//
//    NH_CHECK(nh_gfx_mapVulkanDeviceMemory(
//        Driver_p, &nh_vk_getBufferFromIndex(Fragment_p, Image_p->buffer)->DeviceMemory, vertices_p, 
//        nh_vk_getBufferFromIndex(Fragment_p, Image_p->buffer)->size, 0
//    ))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}

NH_API_RESULT nh_renderer_createVulkanBorderVertices(
    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_css_Fragment *Fragment_p, 
    NH_RENDERER_VULKAN_BUFFER type)
{
    float vertices_p[1000];
    int count = 0, cornerTriangleCount = 10;

    switch (type)
    {
        case NH_RENDERER_VULKAN_BUFFER_TOP_BORDER_VERTICES    : count = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, "top", cornerTriangleCount); break;
        case NH_RENDERER_VULKAN_BUFFER_RIGHT_BORDER_VERTICES  : count = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, "right", cornerTriangleCount); break;
        case NH_RENDERER_VULKAN_BUFFER_BOTTOM_BORDER_VERTICES : count = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, "bottom", cornerTriangleCount); break;
        case NH_RENDERER_VULKAN_BUFFER_LEFT_BORDER_VERTICES   : count = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, "left", cornerTriangleCount); break;
    }

    nh_gfx_VulkanBuffer *Buffer_p = nh_core_incrementArray(&((nh_renderer_VulkanData*)Fragment_p->data_p)->Buffers);
    NH_CORE_CHECK_MEM(Buffer_p)

    Buffer_p->type = type;
    Buffer_p->size = sizeof(float) * count;

    VkBufferCreateInfo CreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = Buffer_p->size,
        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    nh_gfx_VulkanBufferInfo BufferInfo =
    {
        .Info_p           = &CreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = true,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = true
    };

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanBuffer(Driver_p, &BufferInfo, Buffer_p))

    return NH_API_SUCCESS;
}

//NH_API_RESULT nh_vk_updateBorderVertices(
//    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_renderer_FormattingNode *Fragment_p, NH_VK_BUFFER type)
//{
//    int triangleCount = 0, cornerTriangleCount = 10;
//    float vertices_p[1000];
//
//    switch (type)
//    {
//        case NH_VK_BUFFER_TOP_BORDER_VERTICES    : triangleCount = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, 0, cornerTriangleCount); break;
//        case NH_VK_BUFFER_RIGHT_BORDER_VERTICES  : triangleCount = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, 1, cornerTriangleCount); break;
//        case NH_VK_BUFFER_BOTTOM_BORDER_VERTICES : triangleCount = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, 2, cornerTriangleCount); break;
//        case NH_VK_BUFFER_LEFT_BORDER_VERTICES   : triangleCount = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, 3, cornerTriangleCount); break;
//    }
//
//    NH_CHECK(nh_gfx_mapVulkanDeviceMemory(
//        Driver_p, &nh_vk_getBuffer(Fragment_p, type)->DeviceMemory, vertices_p, 
//        nh_vk_getBuffer(Fragment_p, type)->size, 0
//    ))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//NH_API_RESULT nh_vk_createImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_renderer_FormattingNode *Fragment_p, float subtractFromZ)
//{
//    float vertices_p[30];
//    NH_CHECK(nh_html_getImageVertices(Viewport_p, Fragment_p, vertices_p, subtractFromZ))
//
//    nh_gfx_VulkanBuffer *Buffer_p;
//    NH_CHECK(nh_vk_allocateBuffer(Fragment_p, &Buffer_p))
//    Buffer_p->type = NH_VK_BUFFER_TEXTURE;
//    Buffer_p->size = sizeof(float) * 30;
//
//    VkBufferCreateInfo CreateInfo = 
//    {
//        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
//        .pNext       = VK_NULL_HANDLE,
//        .size        = Buffer_p->size,
//        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
//    };
//    nh_gfx_VulkanBufferInfo BufferInfo =
//    {
//        .Info_p           = &CreateInfo,
//        .data_p           = vertices_p,
//        .mapMemory        = true,
//        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//        .createInfo       = true 
//    };
//
//    NH_CHECK(nh_gfx_createVulkanBuffer(nh_vk_getHost(), Driver_p, &BufferInfo, Buffer_p))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//NH_API_RESULT nh_vk_updateImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_gfx_VulkanDriver *Driver_p, nh_renderer_FormattingNode *Fragment_p, float subtractFromZ)
//{
//    float vertices_p[30];
//    NH_CHECK(nh_html_getImageVertices(Viewport_p, Fragment_p, vertices_p, subtractFromZ))
//
//    NH_CHECK(nh_gfx_mapVulkanDeviceMemory(
//        Driver_p, &nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_TEXTURE)->DeviceMemory, vertices_p, 
//        nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_TEXTURE)->size, 0
//    ))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}

void nh_renderer_destroyVulkanBufferArray(
    nh_gfx_VulkanDriver *Driver_p, nh_renderer_VulkanData *Data_p)
{
    for (int i = 0; i < Data_p->Buffers.length; ++i) {
        nh_gfx_VulkanBuffer *Buffer_p = &((nh_gfx_VulkanBuffer*)Data_p->Buffers.p)[i];
        nh_gfx_destroyVulkanBuffer(Driver_p, Buffer_p);
    }

    nh_core_freeArray(&Data_p->Buffers);
}

//void nh_gfx_destroyVulkanBufferFromType(
//    nh_gfx_VulkanDriver *Driver_p, nh_renderer_FormattingNode *Fragment_p, NH_VK_BUFFER type)
//{
//    for (int i = 0; i < Fragment_p->Vulkan.Buffers.count; ++i) {
//        nh_gfx_VulkanBuffer *Buffer_p = nh_core_getFromLinkedList(&Fragment_p->Vulkan.Buffers, i);
//        if (Buffer_p->type == type) {nh_gfx_destroyVulkanBuffer(Driver_p, Buffer_p);}
//    }
//
//NH_CORE_SILENT_END()
//}

nh_gfx_VulkanBuffer *nh_renderer_getVulkanBuffer(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VULKAN_BUFFER type)
{
    for (int i = 0; i < Data_p->Buffers.length; ++i) {
        nh_gfx_VulkanBuffer *Buffer_p = &((nh_gfx_VulkanBuffer*)Data_p->Buffers.p)[i]; 
        if (Buffer_p != NULL && Buffer_p->type == type) {return Buffer_p;}
    }

    return NULL;
}

nh_gfx_VulkanBuffer *nh_renderer_getVulkanBufferFromIndex(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VULKAN_BUFFER type, unsigned int index)
{
    int count = 0;
    for (int i = 0; i < Data_p->Buffers.length; ++i) {
        nh_gfx_VulkanBuffer *Buffer_p = &((nh_gfx_VulkanBuffer*)Data_p->Buffers.p)[i]; 
        if (Buffer_p != NULL && Buffer_p->type == type) {
            if (count == index) {return Buffer_p;}
            else {count++;}
        }
    }

    return NULL;
}
