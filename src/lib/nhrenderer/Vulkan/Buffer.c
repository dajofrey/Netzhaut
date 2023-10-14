// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Buffer.h"

#include "../Main/Vertices.h"
#include "../Common/Macros.h"

#include "../../nhgfx/Common/Macros.h"
#include "../../nhcore/Util/Array.h"
#include "../../nhcore/System/Memory.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// BACKGROUND ======================================================================================

NH_RENDERER_RESULT nh_renderer_vk_createBackgroundVertices(
    nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_css_Fragment *Fragment_p)
{
NH_RENDERER_BEGIN()

    int count = nh_renderer_getBackgroundVertices(Viewport_p, Fragment_p, NULL, 10);

    float *vertices_p = nh_core_allocate(sizeof(float) * count);
    NH_RENDERER_CHECK_MEM(vertices_p)

    nh_renderer_getBackgroundVertices(Viewport_p, Fragment_p, vertices_p, 10);

    nh_vk_Buffer *Buffer_p = nh_core_incrementArray(&((nh_renderer_vk_Data*)Fragment_p->data_p)->Buffers);
    NH_RENDERER_CHECK_MEM(Buffer_p)

    Buffer_p->type = NH_RENDERER_VK_BUFFER_BACKGROUND;
    Buffer_p->size = sizeof(float) * count;

    VkBufferCreateInfo CreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = Buffer_p->size,
        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    nh_vk_BufferInfo BufferInfo =
    {
        .Info_p           = &CreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_TRUE 
    };

    NH_GFX_CHECK_2(NH_RENDERER_ERROR_BAD_STATE, nh_vk_createBuffer(Driver_p, &BufferInfo, Buffer_p))
    nh_core_free(vertices_p);

NH_RENDERER_DIAGNOSTIC_END(NH_RENDERER_SUCCESS)
}

//NH_CORE_RESULT nh_vk_updateBackgroundVertices(
//    nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_renderer_FormattingNode *Fragment_p)
//{
//NH_RENDERER_BEGIN()
//
//    float vertices_p[1000];
//    int count = nh_renderer_getBackgroundVertices(Viewport_p, Fragment_p, vertices_p, 10);
//
//    if ((sizeof(float) * count) != nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_BACKGROUND)->size) {
//        printf("realloc buffer!!!\n");
//        exit(0);
//    }
//
//    NH_CHECK(nh_vk_mapDeviceMemory(
//        Driver_p, &nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_BACKGROUND)->DeviceMemory, vertices_p, 
//        nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_BACKGROUND)->size, 0
//    ))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//// BACKGROUND IMAGE ================================================================================
//
//NH_CORE_RESULT nh_vk_createBackgroundImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_renderer_FormattingNode *Fragment_p, NH_RENDERER_Image *Image_p)
//{
//NH_RENDERER_BEGIN()
//
//    float vertices_p[30];
//    NH_CHECK(nh_renderer_getBackgroundImageVertices(Viewport_p, Fragment_p, vertices_p, Image_p, 0.001f))
//
//    nh_vk_Buffer *Buffer_p;
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
//    nh_vk_BufferInfo BufferInfo =
//    {
//        .Info_p           = &CreateInfo,
//        .data_p           = vertices_p,
//        .mapMemory        = NH_TRUE,
//        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//        .createInfo       = NH_TRUE 
//    };
//
//    NH_CHECK(nh_vk_createBuffer(nh_vk_getHost(), Driver_p, &BufferInfo, Buffer_p))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//NH_CORE_RESULT nh_vk_updateBackgroundImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_renderer_FormattingNode *Fragment_p, NH_RENDERER_Image *Image_p)
//{
//NH_RENDERER_BEGIN()
//
//    float vertices_p[30];
//    NH_CHECK(nh_renderer_getBackgroundImageVertices(Viewport_p, Fragment_p, vertices_p, Image_p, 0.001f))
//
//    NH_CHECK(nh_vk_mapDeviceMemory(
//        Driver_p, &nh_vk_getBufferFromIndex(Fragment_p, Image_p->buffer)->DeviceMemory, vertices_p, 
//        nh_vk_getBufferFromIndex(Fragment_p, Image_p->buffer)->size, 0
//    ))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
// BORDER ==========================================================================================

NH_RENDERER_RESULT nh_renderer_vk_createBorderVertices(
    nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_css_Fragment *Fragment_p, 
    NH_RENDERER_VK_BUFFER type)
{
NH_RENDERER_BEGIN()

    float vertices_p[1000];
    int count = 0, cornerTriangleCount = 10;

    switch (type)
    {
        case NH_RENDERER_VK_BUFFER_TOP_BORDER_VERTICES    : count = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, "top", cornerTriangleCount); break;
        case NH_RENDERER_VK_BUFFER_RIGHT_BORDER_VERTICES  : count = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, "right", cornerTriangleCount); break;
        case NH_RENDERER_VK_BUFFER_BOTTOM_BORDER_VERTICES : count = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, "bottom", cornerTriangleCount); break;
        case NH_RENDERER_VK_BUFFER_LEFT_BORDER_VERTICES   : count = nh_renderer_getBorderVertices(Viewport_p, Fragment_p, vertices_p, "left", cornerTriangleCount); break;
    }

    nh_vk_Buffer *Buffer_p = nh_core_incrementArray(&((nh_renderer_vk_Data*)Fragment_p->data_p)->Buffers);
    NH_RENDERER_CHECK_MEM(Buffer_p)

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
    nh_vk_BufferInfo BufferInfo =
    {
        .Info_p           = &CreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_TRUE
    };

    NH_GFX_CHECK_2(NH_RENDERER_ERROR_BAD_STATE, nh_vk_createBuffer(Driver_p, &BufferInfo, Buffer_p))

NH_RENDERER_DIAGNOSTIC_END(NH_RENDERER_SUCCESS)
}

//NH_CORE_RESULT nh_vk_updateBorderVertices(
//    nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_renderer_FormattingNode *Fragment_p, NH_VK_BUFFER type)
//{
//NH_RENDERER_BEGIN()
//
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
//    NH_CHECK(nh_vk_mapDeviceMemory(
//        Driver_p, &nh_vk_getBuffer(Fragment_p, type)->DeviceMemory, vertices_p, 
//        nh_vk_getBuffer(Fragment_p, type)->size, 0
//    ))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//// TEXTURE =========================================================================================
//
//NH_CORE_RESULT nh_vk_createImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_renderer_FormattingNode *Fragment_p, float subtractFromZ)
//{
//NH_RENDERER_BEGIN()
//
//    float vertices_p[30];
//    NH_CHECK(nh_html_getImageVertices(Viewport_p, Fragment_p, vertices_p, subtractFromZ))
//
//    nh_vk_Buffer *Buffer_p;
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
//    nh_vk_BufferInfo BufferInfo =
//    {
//        .Info_p           = &CreateInfo,
//        .data_p           = vertices_p,
//        .mapMemory        = NH_TRUE,
//        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//        .createInfo       = NH_TRUE 
//    };
//
//    NH_CHECK(nh_vk_createBuffer(nh_vk_getHost(), Driver_p, &BufferInfo, Buffer_p))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//NH_CORE_RESULT nh_vk_updateImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_vk_Driver *Driver_p, nh_renderer_FormattingNode *Fragment_p, float subtractFromZ)
//{
//NH_RENDERER_BEGIN()
//
//    float vertices_p[30];
//    NH_CHECK(nh_html_getImageVertices(Viewport_p, Fragment_p, vertices_p, subtractFromZ))
//
//    NH_CHECK(nh_vk_mapDeviceMemory(
//        Driver_p, &nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_TEXTURE)->DeviceMemory, vertices_p, 
//        nh_vk_getBuffer(Fragment_p, NH_VK_BUFFER_TEXTURE)->size, 0
//    ))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}

// LIST ============================================================================================

void nh_renderer_vk_destroyBufferArray(
    nh_vk_Driver *Driver_p, nh_renderer_vk_Data *Data_p)
{
NH_RENDERER_BEGIN()

    for (int i = 0; i < Data_p->Buffers.length; ++i) {
        nh_vk_Buffer *Buffer_p = &((nh_vk_Buffer*)Data_p->Buffers.p)[i];
        nh_vk_destroyBuffer(Driver_p, Buffer_p);
    }

    nh_core_freeArray(&Data_p->Buffers);

NH_RENDERER_SILENT_END()
}

//void nh_vk_destroyBufferFromType(
//    nh_vk_Driver *Driver_p, nh_renderer_FormattingNode *Fragment_p, NH_VK_BUFFER type)
//{
//NH_RENDERER_BEGIN()
//    
//    for (int i = 0; i < Fragment_p->Vulkan.Buffers.count; ++i) {
//        nh_vk_Buffer *Buffer_p = nh_core_getFromLinkedList(&Fragment_p->Vulkan.Buffers, i);
//        if (Buffer_p->type == type) {nh_vk_destroyBuffer(Driver_p, Buffer_p);}
//    }
//
//NH_CORE_SILENT_END()
//}

nh_vk_Buffer *nh_renderer_vk_getBuffer(
    nh_renderer_vk_Data *Data_p, NH_RENDERER_VK_BUFFER type)
{
NH_RENDERER_BEGIN()

    for (int i = 0; i < Data_p->Buffers.length; ++i) {
        nh_vk_Buffer *Buffer_p = &((nh_vk_Buffer*)Data_p->Buffers.p)[i]; 
        if (Buffer_p != NULL && Buffer_p->type == type) {NH_RENDERER_END(Buffer_p)}
    }

NH_RENDERER_END(NULL)
}

nh_vk_Buffer *nh_renderer_vk_getBufferFromIndex(
    nh_renderer_vk_Data *Data_p, NH_RENDERER_VK_BUFFER type, unsigned int index)
{
NH_RENDERER_BEGIN()

    int count = 0;
    for (int i = 0; i < Data_p->Buffers.length; ++i) {
        nh_vk_Buffer *Buffer_p = &((nh_vk_Buffer*)Data_p->Buffers.p)[i]; 
        if (Buffer_p != NULL && Buffer_p->type == type) {
            if (count == index) {NH_RENDERER_END(Buffer_p)}
            else {count++;}
        }
    }

NH_RENDERER_END(NULL)
}

