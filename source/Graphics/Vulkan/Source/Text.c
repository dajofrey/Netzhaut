// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Text.h"
#include "../Header/Driver.h"
#include "../Header/Texture.h"
#include "../Header/Uniforms.h"
#include "../Header/Descriptors.h"
#include "../Header/Buffers.h"
#include "../Header/GPU.h"
#include "../Header/Macros.h"

#include "../../../HTML/Header/Text.h"
#include "../../../API/Header/Vulkan.h"
#include "../../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <float.h>

// DECLARE ========================================================================================

static NH_RESULT Nh_Vk_createTextVertices(
    Nh_Window *Window_p, Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p,  
    float *vertices_p, uint32_t *indices_p
);
static NH_RESULT Nh_Vk_createTextUniform(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p
);
static NH_RESULT Nh_Vk_createTextDescriptor(
    Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p, Nh_Gfx_Texture *Texture_p
);
static NH_RESULT Nh_Vk_allocateTextResources(
    Nh_HTML_Node *Node_p
);

// CREATE AND UPDATE ===============================================================================

NH_RESULT Nh_Vk_createText(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, bool allocate, float *vertices_p, uint32_t *indices_p)
{
NH_BEGIN()

    Nh_Vk_GPU *GPU_p = Tab_p->Window_p->GPU.Pointer;

    Nh_Gfx_Texture *Texture_p = Nh_Gfx_getTexture(Tab_p, &Node_p->Computed.Properties.Text.Font_p->URI);
    NH_CHECK(Nh_Vk_hardLoadTexture(GPU_p, Texture_p))
    if (allocate) {NH_CHECK(Nh_Vk_allocateTextResources(Node_p))}

    NH_CHECK(Nh_Vk_createTextVertices(
        Tab_p->Window_p, Nh_Vk_getHost(), &GPU_p->Driver, Node_p, vertices_p, indices_p
    ))
    NH_CHECK(Nh_Vk_createTextUniform(Nh_Vk_getHost(), &GPU_p->Driver, Node_p))
    NH_CHECK(Nh_Vk_createTextDescriptor(GPU_p, Node_p, Texture_p))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_updateText(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

//    NH_CHECK_NULL(Tab_p, Node_p)
//    Nh_Vk_GPU *GPU_p = Tab_p->Window_p->GPU.Pointer;
//
//    if (Node_p->Computed.Text.lineCount == 0) 
//    {
//        NH_CHECK(Nh_HTML_createText(Tab_p, Node_p, &Node_p->Computed.Text))
//        NH_CHECK(Nh_Vk_createText(Tab_p, Node_p, true))
//        NH_END(NH_SUCCESS)
//    } 
//    else {NH_CHECK(Nh_HTML_updateText(Tab_p, Node_p))}
//
//    Nh_Vk_destroyBufferFromType(&GPU_p->Driver, Node_p, NH_VK_BUFFER_TEXT_SDF); 
//    Nh_Vk_destroyBufferFromType(&GPU_p->Driver, Node_p, NH_VK_BUFFER_TEXT_SDF_INDEX);
//
//    Nh_Vk_destroyUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_TEXT_SDF_VS);
//    Nh_Vk_destroyUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_TEXT_SDF_FS);
//    Nh_Vk_destroyDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_TEXT_SDF);
//
//    NH_CHECK(Nh_Vk_createText(Tab_p, Node_p, false))

NH_END(NH_SUCCESS)
}

// HELPER ==========================================================================================

static NH_RESULT Nh_Vk_createTextVertices(
    Nh_Window *Window_p, Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p,  
    float *vertices_p, uint32_t *indices_p)
{
NH_BEGIN()

    int len = strlen(Node_p->text_p);

    VkBufferCreateInfo bufferCreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size        = len * 20 * sizeof(float),
        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };
    NH_Vk_BufferInfo bufferInfo = {
        .Info_p           = &bufferCreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_FALSE,
    };

    NH_CHECK(Nh_Vk_createBuffer(
        Host_p, Driver_p, &bufferInfo, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXT_SDF)
    ))

    bufferCreateInfo.size  = len * 6 * sizeof(uint32_t);
    bufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    bufferInfo.data_p = indices_p;
    bufferInfo.Info_p = &bufferCreateInfo;
   
    NH_CHECK(Nh_Vk_createBuffer(
        Host_p, Driver_p, &bufferInfo, Nh_Vk_getBuffer(Node_p, NH_VK_BUFFER_TEXT_SDF_INDEX)
    ))
 
NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_createTextUniform(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    // MVP
    float vsData_p[16];
    for (int i = 0; i < 16; ++i) {vsData_p[i] = 0.0;}
    vsData_p[0] = vsData_p[5] = vsData_p[10] = vsData_p[15] = 1.0;
    
    VkBufferCreateInfo vsInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = sizeof(float) * 16,  // MVP
        .usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,  
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    NH_Vk_BufferInfo bufferInfo =
    {
        .Info_p           = &vsInfo,
        .data_p           = vsData_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_FALSE,
    };

    NH_CHECK(Nh_Vk_createBuffer(
        Host_p, Driver_p, &bufferInfo, Nh_Vk_getUniform(Node_p, NH_VK_UNIFORM_TEXT_SDF_VS)
    ))

    // text color, etc.
    float fsData_p[11];
    fsData_p[0] = Node_p->Computed.Properties.Text.color_p[0];
    fsData_p[1] = Node_p->Computed.Properties.Text.color_p[1];
    fsData_p[2] = Node_p->Computed.Properties.Text.color_p[2];
    fsData_p[3] = Node_p->Computed.Properties.Text.color_p[3];
    fsData_p[10] = Node_p->Computed.Properties.Text.fontSize;

    for (int i = 4; i < 10; ++i) {fsData_p[i] = 0.0;}

    VkBufferCreateInfo fsInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = sizeof(float) * 11,
        .usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    bufferInfo.data_p = fsData_p;
    bufferInfo.Info_p = &fsInfo;

    NH_CHECK(Nh_Vk_createBuffer(
        Host_p, Driver_p, &bufferInfo, Nh_Vk_getUniform(Node_p, NH_VK_UNIFORM_TEXT_SDF_FS)
    ))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_createTextDescriptor(
    Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p, Nh_Gfx_Texture *Texture_p)
{
NH_BEGIN()

    Nh_Vk_Driver *Driver_p = &GPU_p->Driver;

    // create
    VkDescriptorSetLayoutBinding setLayoutBindings[3];
    VkDescriptorSetLayoutBinding setLayoutBinding1 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .stageFlags      = VK_SHADER_STAGE_VERTEX_BIT,
        .binding         = 0,
        .descriptorCount = 1 
    };
    VkDescriptorSetLayoutBinding setLayoutBinding2 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
        .binding         = 1,
        .descriptorCount = 1 
    };
    VkDescriptorSetLayoutBinding setLayoutBinding3 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
        .binding         = 2,
        .descriptorCount = 1 
    };
    setLayoutBindings[0] = setLayoutBinding1;
    setLayoutBindings[1] = setLayoutBinding2;
    setLayoutBindings[2] = setLayoutBinding3;

    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
    {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pBindings    = setLayoutBindings,
        .bindingCount = 3
    };
    VkDescriptorSetAllocateInfo AllocateInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool     = Driver_p->DescriptorPool_p[0],
        .descriptorSetCount = 1,
    };

    NH_CHECK(Nh_Vk_createDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_TEXT_SDF)))

    // update
    VkDescriptorBufferInfo vsDescrBufferInfo = 
    {
        .buffer = Nh_Vk_getUniform(Node_p, NH_VK_UNIFORM_TEXT_SDF_VS)->Buffer,
        .offset = 0,
        .range  = sizeof(float) * 16
    };
    VkDescriptorBufferInfo fsDescrBufferInfo = 
    {
        .buffer = Nh_Vk_getUniform(Node_p, NH_VK_UNIFORM_TEXT_SDF_FS)->Buffer,
        .offset = 0,
        .range  = sizeof(float) * 11
    };        
    VkDescriptorImageInfo ImageInfo = 
    {
        .sampler     = Texture_p->Vulkan_p->Sampler,
        .imageView   = Texture_p->Vulkan_p->ImageView,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    VkWriteDescriptorSet vsUniformBufferDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_TEXT_SDF)->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .dstBinding      = 0,
        .pBufferInfo     = &vsDescrBufferInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet fsTextureSamplerDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_TEXT_SDF)->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .dstBinding      = 1,
        .pImageInfo      = &ImageInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet fsUniformBufferDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = Nh_Vk_getDescriptor(Node_p, NH_VK_DESCRIPTOR_TEXT_SDF)->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .dstBinding      = 2,
        .pBufferInfo     = &fsDescrBufferInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet Sets_p[3] = {vsUniformBufferDescriptorSet, fsTextureSamplerDescriptorSet, fsUniformBufferDescriptorSet};
    Driver_p->Func.vkUpdateDescriptorSets(Driver_p->Device, 3, Sets_p, 0, VK_NULL_HANDLE);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_allocateTextResources(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    // vertex buffers
    NH_Vk_Buffer *Buffer_p = Nh_allocate(sizeof(NH_Vk_Buffer));
    NH_CHECK_MEM(Buffer_p)
    Buffer_p->type = NH_VK_BUFFER_TEXT_SDF;

    Nh_addListItem(&Node_p->Vulkan.Buffers, Buffer_p);

    Buffer_p = Nh_allocate(sizeof(NH_Vk_Buffer));
    NH_CHECK_MEM(Buffer_p)
    Buffer_p->type = NH_VK_BUFFER_TEXT_SDF_INDEX;

    Nh_addListItem(&Node_p->Vulkan.Buffers, Buffer_p);

    // uniforms
    Buffer_p = Nh_allocate(sizeof(NH_Vk_Buffer));
    NH_CHECK_MEM(Buffer_p)
    Buffer_p->type = NH_VK_UNIFORM_TEXT_SDF_VS;

    Nh_addListItem(&Node_p->Vulkan.Uniforms, Buffer_p);

    Buffer_p = Nh_allocate(sizeof(NH_Vk_Buffer));
    NH_CHECK_MEM(Buffer_p)
    Buffer_p->type = NH_VK_UNIFORM_TEXT_SDF_FS;

    Nh_addListItem(&Node_p->Vulkan.Uniforms, Buffer_p);

    // descriptor
    NH_Vk_DescriptorSet *Descriptor_p = Nh_allocate(sizeof(NH_Vk_DescriptorSet));
    NH_CHECK_MEM(Descriptor_p)
    Descriptor_p->type = NH_VK_DESCRIPTOR_TEXT_SDF;

    Nh_addListItem(&Node_p->Vulkan.Descriptors, Descriptor_p);

NH_END(NH_SUCCESS)
}

