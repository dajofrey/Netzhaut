// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Buffers.h"
#include "../Header/Macros.h"
#include "../Header/Vulkan.h"

#include "../../../CSS/Header/Vertices.h"
#include "../../../API/Header/Vulkan.h"
#include "../../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_Vk_allocateBuffer(
    Nh_HTML_Node *Node_p, NH_Vk_Buffer **Data_pp
);

// BACKGROUND ======================================================================================

NH_RESULT Nh_Vk_createBackgroundVertices(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    float vertices_p[1000];
    int count = Nh_CSS_getBackgroundVertices(Tab_p, Node_p, vertices_p, 10);

    NH_Vk_Buffer *Buffer_p;
    NH_CHECK(Nh_Vk_allocateBuffer(Node_p, &Buffer_p))
    Buffer_p->type = NH_VK_BUFFER_BACKGROUND;
    Buffer_p->size = sizeof(float) * count;

    VkBufferCreateInfo CreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = Buffer_p->size,
        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    NH_Vk_BufferInfo BufferInfo =
    {
        .Info_p           = &CreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_TRUE 
    };

    NH_CHECK(Nh_Vk_createBuffer(Nh_Vk_getHost(), Driver_p, &BufferInfo, Buffer_p))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_updateBackgroundVertices(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    float vertices_p[1000];
    int count = Nh_CSS_getBackgroundVertices(Tab_p, Node_p, vertices_p, 10);

    if ((sizeof(float) * count) != Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BACKGROUND)->size) {
        printf("realloc buffer!!!\n");
        exit(0);
    }

    NH_CHECK(Nh_Vk_mapDeviceMemory(
        Driver_p, &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BACKGROUND)->DeviceMemory, vertices_p, 
        Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_BACKGROUND)->size, 0
    ))

NH_END(NH_SUCCESS)
}

// BACKGROUND IMAGE ================================================================================

NH_RESULT Nh_Vk_createBackgroundImageVertices(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p)
{
NH_BEGIN()

    float vertices_p[30];
    NH_CHECK(Nh_CSS_getBackgroundImageVertices(Tab_p, Node_p, vertices_p, Image_p, 0.001f))

    NH_Vk_Buffer *Buffer_p;
    NH_CHECK(Nh_Vk_allocateBuffer(Node_p, &Buffer_p))
    Buffer_p->type = NH_VK_BUFFER_BACKGROUND_IMAGE;
    Buffer_p->size = sizeof(float) * 30;

    Image_p->buffer = Node_p->Vulkan.Buffers.count - 1;

    VkBufferCreateInfo CreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = Buffer_p->size,
        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    NH_Vk_BufferInfo BufferInfo =
    {
        .Info_p           = &CreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_TRUE 
    };

    NH_CHECK(Nh_Vk_createBuffer(Nh_Vk_getHost(), Driver_p, &BufferInfo, Buffer_p))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_updateBackgroundImageVertices(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p)
{
NH_BEGIN()

    float vertices_p[30];
    NH_CHECK(Nh_CSS_getBackgroundImageVertices(Tab_p, Node_p, vertices_p, Image_p, 0.001f))

    NH_CHECK(Nh_Vk_mapDeviceMemory(
        Driver_p, &Nh_Vk_getBufferFromIndex(Node_p, Image_p->buffer)->DeviceMemory, vertices_p, 
        Nh_Vk_getBufferFromIndex(Node_p, Image_p->buffer)->size, 0
    ))

NH_END(NH_SUCCESS)
}

// BORDER ==========================================================================================

NH_RESULT Nh_Vk_createBorderVertices(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_BUFFER type)
{
NH_BEGIN()

    float vertices_p[1000];
    int count = 0, cornerTriangleCount = 10;

    switch (type)
    {
        case NH_VK_BUFFER_TOP_BORDER_VERTICES    : count = Nh_CSS_getBorderVertices(Tab_p, Node_p, vertices_p, NH_CSS_BORDER_TOP, cornerTriangleCount); break;
        case NH_VK_BUFFER_RIGHT_BORDER_VERTICES  : count = Nh_CSS_getBorderVertices(Tab_p, Node_p, vertices_p, NH_CSS_BORDER_RIGHT, cornerTriangleCount); break;
        case NH_VK_BUFFER_BOTTOM_BORDER_VERTICES : count = Nh_CSS_getBorderVertices(Tab_p, Node_p, vertices_p, NH_CSS_BORDER_BOTTOM, cornerTriangleCount); break;
        case NH_VK_BUFFER_LEFT_BORDER_VERTICES   : count = Nh_CSS_getBorderVertices(Tab_p, Node_p, vertices_p, NH_CSS_BORDER_LEFT, cornerTriangleCount); break;
    }

    NH_Vk_Buffer *Buffer_p;
    NH_CHECK(Nh_Vk_allocateBuffer(Node_p, &Buffer_p))
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
    NH_Vk_BufferInfo BufferInfo =
    {
        .Info_p           = &CreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_TRUE
    };

    NH_CHECK(Nh_Vk_createBuffer(Nh_Vk_getHost(), Driver_p, &BufferInfo, Buffer_p))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_updateBorderVertices(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_BUFFER type)
{
NH_BEGIN()

    int triangleCount = 0, cornerTriangleCount = 10;
    float vertices_p[1000];

    switch (type)
    {
        case NH_VK_BUFFER_TOP_BORDER_VERTICES    : triangleCount = Nh_CSS_getBorderVertices(Tab_p, Node_p, vertices_p, 0, cornerTriangleCount); break;
        case NH_VK_BUFFER_RIGHT_BORDER_VERTICES  : triangleCount = Nh_CSS_getBorderVertices(Tab_p, Node_p, vertices_p, 1, cornerTriangleCount); break;
        case NH_VK_BUFFER_BOTTOM_BORDER_VERTICES : triangleCount = Nh_CSS_getBorderVertices(Tab_p, Node_p, vertices_p, 2, cornerTriangleCount); break;
        case NH_VK_BUFFER_LEFT_BORDER_VERTICES   : triangleCount = Nh_CSS_getBorderVertices(Tab_p, Node_p, vertices_p, 3, cornerTriangleCount); break;
    }

    NH_CHECK(Nh_Vk_mapDeviceMemory(
        Driver_p, &Nh_Vk_getBuffer(Node_p, type)->DeviceMemory, vertices_p, 
        Nh_Vk_getBuffer(Node_p, type)->size, 0
    ))

NH_END(NH_SUCCESS)
}

// TEXTURE =========================================================================================

NH_RESULT Nh_Vk_createImageVertices(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, float subtractFromZ)
{
NH_BEGIN()

    float vertices_p[30];
    NH_CHECK(Nh_CSS_getImageVertices(Tab_p, Node_p, vertices_p, subtractFromZ))

    NH_Vk_Buffer *Buffer_p;
    NH_CHECK(Nh_Vk_allocateBuffer(Node_p, &Buffer_p))
    Buffer_p->type = NH_VK_BUFFER_TEXTURE;
    Buffer_p->size = sizeof(float) * 30;

    VkBufferCreateInfo CreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = Buffer_p->size,
        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    NH_Vk_BufferInfo BufferInfo =
    {
        .Info_p           = &CreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_TRUE 
    };

    NH_CHECK(Nh_Vk_createBuffer(Nh_Vk_getHost(), Driver_p, &BufferInfo, Buffer_p))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_updateImageVertices(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, float subtractFromZ)
{
NH_BEGIN()

    float vertices_p[30];
    NH_CHECK(Nh_CSS_getImageVertices(Tab_p, Node_p, vertices_p, subtractFromZ))

    NH_CHECK(Nh_Vk_mapDeviceMemory(
        Driver_p, &Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXTURE)->DeviceMemory, vertices_p, 
        Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXTURE)->size, 0
    ))

NH_END(NH_SUCCESS)
}

// LIST ============================================================================================

void Nh_Vk_destroyBufferList(
    Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Vulkan.Buffers.count; ++i) {
        NH_Vk_Buffer *Buffer_p = Nh_getListItem(&Node_p->Vulkan.Buffers, i);
        Nh_Vk_destroyBuffer(Driver_p, Buffer_p);
    }

    Nh_destroyList(&Node_p->Vulkan.Buffers, true);

NH_SILENT_END()
}

void Nh_Vk_destroyBufferFromType(
    Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_BUFFER type)
{
NH_BEGIN()
    
    for (int i = 0; i < Node_p->Vulkan.Buffers.count; ++i) {
        NH_Vk_Buffer *Buffer_p = Nh_getListItem(&Node_p->Vulkan.Buffers, i);
        if (Buffer_p->type == type) {Nh_Vk_destroyBuffer(Driver_p, Buffer_p);}
    }

NH_SILENT_END()
}

NH_Vk_Buffer *Nh_Vk_getBuffer(
    Nh_HTML_Node *Node_p, NH_VK_BUFFER type)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Vulkan.Buffers.count; ++i) {
        NH_Vk_Buffer *Buffer_p = Nh_getListItem(&Node_p->Vulkan.Buffers, i);
        if (Buffer_p != NULL && Buffer_p->type == type) {NH_END(Buffer_p)}
    }

NH_END(NULL)
}

NH_Vk_Buffer *Nh_Vk_getBufferFromIndex(
    Nh_HTML_Node *Node_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(&Node_p->Vulkan.Buffers, index))
}

// HELPER ==========================================================================================

static NH_RESULT Nh_Vk_allocateBuffer(
    Nh_HTML_Node *Node_p, NH_Vk_Buffer **Data_pp)
{
NH_BEGIN()

    *Data_pp = Nh_allocate(sizeof(NH_Vk_Buffer));
    NH_CHECK_MEM(*Data_pp)
    Nh_addListItem(&Node_p->Vulkan.Buffers, *Data_pp);

NH_END(NH_SUCCESS)
}

