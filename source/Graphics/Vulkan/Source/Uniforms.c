// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Uniforms.h"
#include "../Header/Driver.h"
#include "../Header/Surface.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/Tab.h"
#include "../../../Core/Header/Memory.h"

#include "../../../API/Header/Vulkan.h"
#include "../../../CSS/Header/Box.h"
#include "../../../HTML/Header/Document.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_Vk_createUniform(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, void *data_p, size_t size, NH_Vk_Buffer *Buffer_p
);
static NH_RESULT Nh_Vk_allocateUniform(
    Nh_HTML_Node *Node_p, NH_Vk_Buffer **Data_pp
);

// COLOR ===========================================================================================

NH_RESULT Nh_Vk_createColorUniform(
    Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_UNIFORM type)
{
NH_BEGIN()

    float data_p[20] = {0.0f};
    data_p[0] = data_p[5] = data_p[10] = data_p[15] = 1.0f;

    float color_p[4];

    switch (type)
    {
        case NH_VK_UNIFORM_TOP_BORDER    : memcpy(color_p, Node_p->Computed.Properties.Border.Color.top_p, sizeof(float) * 4); break;
        case NH_VK_UNIFORM_RIGHT_BORDER  : memcpy(color_p, Node_p->Computed.Properties.Border.Color.right_p, sizeof(float) * 4); break;
        case NH_VK_UNIFORM_BOTTOM_BORDER : memcpy(color_p, Node_p->Computed.Properties.Border.Color.bottom_p, sizeof(float) * 4); break;
        case NH_VK_UNIFORM_LEFT_BORDER   : memcpy(color_p, Node_p->Computed.Properties.Border.Color.left_p, sizeof(float) * 4); break;
        case NH_VK_UNIFORM_BACKGROUND    : memcpy(color_p, Node_p->Computed.Properties.Background.color_p, sizeof(float) * 4); break;
        default : NH_END(NH_ERROR_PARAMETERS)
    }

    for (int i = 16, j = 0; j < 4; ++i, ++j) {data_p[i] = color_p[j];} 

    NH_Vk_Buffer *Buffer_p;
    NH_CHECK(Nh_Vk_allocateUniform(Node_p, &Buffer_p))
    Buffer_p->type = type;

    NH_CHECK(Nh_Vk_createUniform(
        Nh_Vk_getHost(), Driver_p, data_p, sizeof(float) * 20, Buffer_p
    ))

NH_END(NH_SUCCESS)
}

// IMAGE ===========================================================================================

NH_RESULT Nh_Vk_createImageUniform(
    Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    float data_p[17] = {0.0f};
    data_p[0] = data_p[5] = data_p[10] = data_p[15] = 1.0f;

    data_p[16] = Node_p->Computed.Attributes.Texture_p->gamma;
    
    NH_Vk_Buffer *Buffer_p;
    NH_CHECK(Nh_Vk_allocateUniform(Node_p, &Buffer_p))
    Buffer_p->type = NH_VK_UNIFORM_IMAGE;

    NH_CHECK(Nh_Vk_createUniform(
        Nh_Vk_getHost(), Driver_p, data_p, sizeof(float) * 17, Buffer_p
    ))

NH_END(NH_SUCCESS)
}

// BACKGROUND IMAGE ================================================================================

NH_RESULT Nh_Vk_createBackgroundImageUniform(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p)
{
NH_BEGIN()

    float data_p[19] = {0.0f};
    data_p[0] = data_p[5] = data_p[10] = data_p[15] = 1.0f;
    data_p[16] = Image_p->Texture_p->gamma;

    Nh_CSS_Box Box;

    switch (Image_p->Background.origin)
    {
        case NH_CSS_BACKGROUND_ORIGIN_PADDING_BOX : Box = Nh_CSS_getPaddingBox(Node_p); break;
        case NH_CSS_BACKGROUND_ORIGIN_BORDER_BOX  : Box = Nh_CSS_getBorderBox(Node_p); break;
        case NH_CSS_BACKGROUND_ORIGIN_CONTENT_BOX : Box = Nh_CSS_getContentBox(Node_p); break;
    }

    Nh_CSS_resize(Tab_p, Node_p, &Box);

    float repeat_p[2];
    Nh_Gfx_getScaledTextureRepeat(Tab_p->Window_p, &Box, Image_p->Texture_p, repeat_p);

    data_p[17] = 1.0f;
    data_p[18] = 1.0f;

    if (Image_p->Background.Size_p[0].type == NH_CSS_BACKGROUND_SIZE_CONTAIN) {
        data_p[17] = repeat_p[0];
    }
    if (Image_p->Background.Size_p[1].type == NH_CSS_BACKGROUND_SIZE_CONTAIN) {
        data_p[18] = repeat_p[1];
    }
 
    NH_Vk_Buffer *Buffer_p;
    NH_CHECK(Nh_Vk_allocateUniform(Node_p, &Buffer_p))
    Buffer_p->type = NH_VK_UNIFORM_BACKGROUND_IMAGE;

    Image_p->uniform = Node_p->Vulkan.Uniforms.count - 1;

    NH_CHECK(Nh_Vk_createUniform(
        Nh_Vk_getHost(), Driver_p, data_p, sizeof(float) * 19, Buffer_p
    ))

NH_END(NH_SUCCESS)
}

// CANVAS RENDERING CONTEXT 2D ARC =================================================================

NH_RESULT Nh_Vk_createCanvasRenderingContext2DArcUniform(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_JS_CanvasRenderingContext2D *Context_p, 
    NH_JS_Path2DArc *Arc_p)
{
NH_BEGIN()

    Nh_CSS_Box ContentBox = Nh_CSS_getContentBox(Node_p);
    Nh_CSS_resize(Tab_p, Node_p, &ContentBox);

    int wh_p[2];
    Nh_CSS_getBoxSize(Tab_p, &ContentBox, wh_p);

    float data_p[29] = {0.0f};
    data_p[0]  = data_p[5]  = data_p[10] = data_p[15] = 1.0f;
    data_p[16] = data_p[17] = data_p[18] = 0.0f;
    data_p[19] = data_p[20] = data_p[21] = 1.0f;
    data_p[22] = (float) wh_p[0];
    data_p[23] = (float) wh_p[1];
    data_p[24] = Arc_p->x;
    data_p[25] = Arc_p->y;
    data_p[26] = Arc_p->radius;
    data_p[27] = Arc_p->startAngle;
    data_p[28] = Arc_p->endAngle;

    NH_Vk_Buffer *Buffer_p = Nh_allocate(sizeof(NH_Vk_Buffer));
    NH_CHECK_MEM(Buffer_p)
    NH_CHECK(Nh_addListItem(&Context_p->Vulkan.Uniforms, Buffer_p))

    NH_CHECK(Nh_Vk_createUniform(
        Nh_Vk_getHost(), Driver_p, data_p, sizeof(float) * 29, Buffer_p
    ))

NH_END(NH_SUCCESS)
}

// LIST ============================================================================================

void Nh_Vk_destroyUniformList(
    Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Vulkan.Uniforms.count; ++i) {
        NH_Vk_Buffer *Buffer_p = Nh_getListItem(&Node_p->Vulkan.Uniforms, i);
        Nh_Vk_destroyBuffer(Driver_p, Buffer_p);
    }

    Nh_destroyList(&Node_p->Vulkan.Uniforms, true);

NH_SILENT_END()
}

void Nh_Vk_destroyUniform(
    Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_UNIFORM type)
{
NH_BEGIN()
    
    for (int i = 0; i < Node_p->Vulkan.Uniforms.count; ++i) {
        NH_Vk_Buffer *Buffer_p = Nh_getListItem(&Node_p->Vulkan.Uniforms, i);
        if (Buffer_p->type == type) {Nh_Vk_destroyBuffer(Driver_p, Buffer_p);}
    }

NH_SILENT_END()
}

NH_Vk_Buffer *Nh_Vk_getUniform(
    Nh_HTML_Node *Node_p, NH_VK_UNIFORM type)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Vulkan.Uniforms.count; ++i) {
        NH_Vk_Buffer *Buffer_p = Nh_getListItem(&Node_p->Vulkan.Uniforms, i);
        if (Buffer_p->type == type) {NH_END(Buffer_p)}
    }

NH_END(NULL)
}

NH_Vk_Buffer *Nh_Vk_getUniformFromIndex(
    Nh_HTML_Node *Node_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(&Node_p->Vulkan.Uniforms, index))
}

// HELPER ==========================================================================================

static NH_RESULT Nh_Vk_createUniform(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, void *data_p, size_t size, NH_Vk_Buffer *Buffer_p)
{
NH_BEGIN()

    VkBufferCreateInfo VkInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = size,
        .usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    NH_Vk_BufferInfo bufferInfo =
    {
        .Info_p           = &VkInfo,
        .data_p           = data_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_FALSE,
    };

    NH_CHECK(Nh_Vk_createBuffer(Host_p, Driver_p, &bufferInfo, Buffer_p))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_allocateUniform(
    Nh_HTML_Node *Node_p, NH_Vk_Buffer **Data_pp)
{
NH_BEGIN()

    *Data_pp = Nh_allocate(sizeof(NH_Vk_Buffer));
    NH_CHECK_MEM(*Data_pp)
    Nh_addListItem(&Node_p->Vulkan.Uniforms, *Data_pp);

NH_END(NH_SUCCESS)
}

