// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Descriptors.h"
#include "../Header/Uniforms.h"
#include "../Header/Driver.h"
#include "../Header/Texture.h"
#include "../Header/Surface.h"
#include "../Header/GPU.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/Tab.h"
#include "../../../Core/Header/Memory.h"
#include "../../../API/Header/Vulkan.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// DECLARE ========================================================================================

static NH_RESULT Nh_Vk_allocateDescriptor(
    Nh_HTML_Node *Node_p, NH_Vk_DescriptorSet **Data_pp
);

// COLOR ===========================================================================================

NH_RESULT Nh_Vk_createColorDescriptor(
    Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_DESCRIPTOR type)
{
NH_BEGIN()

    NH_Vk_DescriptorSet *Descriptor_p;
    NH_CHECK(Nh_Vk_allocateDescriptor(Node_p, &Descriptor_p))
    Descriptor_p->type = type;

// create
    VkDescriptorSetLayoutBinding LayoutBinding = 
    {
        .binding            = 0,
        .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = VK_NULL_HANDLE,
    };
    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
    {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings    = &LayoutBinding,
    };

    VkDescriptorSetAllocateInfo AllocateInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool     = Driver_p->DescriptorPool_p[0],
        .descriptorSetCount = 1,
    };
    NH_CHECK(Nh_Vk_createDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, Descriptor_p))

    NH_VK_UNIFORM uniformType;
    switch (type)
    {
        case NH_VK_DESCRIPTOR_BACKGROUND    : uniformType = NH_VK_UNIFORM_BACKGROUND; break;
        case NH_VK_DESCRIPTOR_TOP_BORDER    : uniformType = NH_VK_UNIFORM_TOP_BORDER; break; 
        case NH_VK_DESCRIPTOR_RIGHT_BORDER  : uniformType = NH_VK_UNIFORM_RIGHT_BORDER; break;
        case NH_VK_DESCRIPTOR_BOTTOM_BORDER : uniformType = NH_VK_UNIFORM_BOTTOM_BORDER; break;
        case NH_VK_DESCRIPTOR_LEFT_BORDER   : uniformType = NH_VK_UNIFORM_LEFT_BORDER; break;
        default : NH_END(NH_ERROR_PARAMETERS)
    }

// update
    VkDescriptorBufferInfo BufferInfo = 
    {
        .buffer = Nh_Vk_getUniform(Node_p, uniformType)->Buffer,
        .offset = 0,
        .range  = sizeof(float) * 20
    };
    VkWriteDescriptorSet Write = 
    {
        .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet           = Descriptor_p->DescriptorSet,
        .dstBinding       = 0,
        .dstArrayElement  = 0,
        .descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount  = 1,
        .pBufferInfo      = &BufferInfo,
        .pImageInfo       = VK_NULL_HANDLE,
        .pTexelBufferView = VK_NULL_HANDLE
    };
    Driver_p->Func.vkUpdateDescriptorSets(Driver_p->Device, 1, &Write, 0, VK_NULL_HANDLE);  

NH_END(NH_SUCCESS)
}

// IMAGE ===========================================================================================

NH_RESULT Nh_Vk_createImageDescriptor(
    Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_Vk_Driver *Driver_p = &GPU_p->Driver;

    NH_Vk_DescriptorSet *Descriptor_p;
    NH_CHECK(Nh_Vk_allocateDescriptor(Node_p, &Descriptor_p))
    Descriptor_p->type = NH_VK_DESCRIPTOR_IMAGE;

// create
    VkDescriptorSetLayoutBinding setLayoutBindings[2];
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
    setLayoutBindings[0] = setLayoutBinding1;
    setLayoutBindings[1] = setLayoutBinding2;

    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
    {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pBindings    = setLayoutBindings,
        .bindingCount = 2
    };
    VkDescriptorSetAllocateInfo AllocateInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool     = Driver_p->DescriptorPool_p[0],
        .descriptorSetCount = 1,
    };

    NH_CHECK(Nh_Vk_createDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, Descriptor_p))

// update
    VkDescriptorBufferInfo vsDescrBufferInfo = 
    {
        .buffer = Nh_Vk_getUniform(Node_p, NH_VK_UNIFORM_IMAGE)->Buffer,
        .offset = 0,
        .range  = sizeof(float) * 17
    };
    VkDescriptorImageInfo ImageInfo = 
    {
        .sampler     = Node_p->Computed.Attributes.Texture_p->Vulkan_p->Sampler,
        .imageView   = Node_p->Computed.Attributes.Texture_p->Vulkan_p->ImageView,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    VkWriteDescriptorSet vsUniformBufferDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = Descriptor_p->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .dstBinding      = 0,
        .pBufferInfo     = &vsDescrBufferInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet fsTextureSamplerDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = Descriptor_p->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .dstBinding      = 1,
        .pImageInfo      = &ImageInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet Sets_p[2] = {vsUniformBufferDescriptorSet, fsTextureSamplerDescriptorSet};
    Driver_p->Func.vkUpdateDescriptorSets(Driver_p->Device, 2, Sets_p, 0, VK_NULL_HANDLE);

NH_END(NH_SUCCESS)
}

// BACKGROUND IMAGE ================================================================================

NH_RESULT Nh_Vk_createBackgroundImageDescriptor(
    Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p)
{
NH_BEGIN()

    Nh_Vk_Driver *Driver_p = &GPU_p->Driver;

    NH_Vk_DescriptorSet *Descriptor_p;
    NH_CHECK(Nh_Vk_allocateDescriptor(Node_p, &Descriptor_p))
    Descriptor_p->type = NH_VK_DESCRIPTOR_BACKGROUND_IMAGE;

    Image_p->descriptor = Node_p->Vulkan.Descriptors.count - 1;

// create
    VkDescriptorSetLayoutBinding setLayoutBindings[2];
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
    setLayoutBindings[0] = setLayoutBinding1;
    setLayoutBindings[1] = setLayoutBinding2;

    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
    {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pBindings    = setLayoutBindings,
        .bindingCount = 2
    };
    VkDescriptorSetAllocateInfo AllocateInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool     = Driver_p->DescriptorPool_p[0],
        .descriptorSetCount = 1,
    };

    NH_CHECK(Nh_Vk_createDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, Descriptor_p))

// update
    VkDescriptorBufferInfo vsDescrBufferInfo = 
    {
        .buffer = Nh_Vk_getUniformFromIndex(Node_p, Image_p->uniform)->Buffer,
        .offset = 0,
        .range  = sizeof(float) * 17
    };
    VkDescriptorImageInfo ImageInfo = 
    {
        .sampler     = Image_p->Texture_p->Vulkan_p->Sampler,
        .imageView   = Image_p->Texture_p->Vulkan_p->ImageView,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    VkWriteDescriptorSet vsUniformBufferDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = Descriptor_p->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .dstBinding      = 0,
        .pBufferInfo     = &vsDescrBufferInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet fsTextureSamplerDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = Descriptor_p->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .dstBinding      = 1,
        .pImageInfo      = &ImageInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet Sets_p[2] = {vsUniformBufferDescriptorSet, fsTextureSamplerDescriptorSet};
    Driver_p->Func.vkUpdateDescriptorSets(Driver_p->Device, 2, Sets_p, 0, VK_NULL_HANDLE);

NH_END(NH_SUCCESS)
}

// CANVAS RENDERING CONTEXT 2D ======================================================================

NH_RESULT Nh_Vk_createCanvasRenderingContext2DDescriptor(
    Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, NH_JS_CanvasRenderingContext2D *Context_p, 
    NH_Vk_Buffer *Uniform_p, size_t size)
{
NH_BEGIN()

    NH_Vk_DescriptorSet *Descriptor_p = Nh_allocate(sizeof(NH_Vk_DescriptorSet));
    NH_CHECK_MEM(Descriptor_p)
    NH_CHECK(Nh_addListItem(&Context_p->Vulkan.Descriptors, Descriptor_p))

    VkDescriptorSetLayoutBinding setLayoutBindings[2];
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
    setLayoutBindings[0] = setLayoutBinding1;
    setLayoutBindings[1] = setLayoutBinding2;

    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
    {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pBindings    = setLayoutBindings,
        .bindingCount = 2
    };
    VkDescriptorSetAllocateInfo AllocateInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool     = Driver_p->DescriptorPool_p[0],
        .descriptorSetCount = 1,
    };

    NH_CHECK(Nh_Vk_createDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, Descriptor_p))

    VkDescriptorBufferInfo vsDescrBufferInfo = 
    {
        .buffer = Uniform_p->Buffer,
        .offset = 0,
        .range  = size
    };
    VkWriteDescriptorSet vsUniformBufferDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = Descriptor_p->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .dstBinding      = 0,
        .pBufferInfo     = &vsDescrBufferInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet Sets_p[1] = {vsUniformBufferDescriptorSet};
    Driver_p->Func.vkUpdateDescriptorSets(Driver_p->Device, 1, Sets_p, 0, VK_NULL_HANDLE);

NH_END(NH_SUCCESS)
}

// LIST ============================================================================================

void Nh_Vk_destroyDescriptorList(
    Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Vulkan.Descriptors.count; ++i) {
        NH_Vk_DescriptorSet *Descriptor_p = Nh_getListItem(&Node_p->Vulkan.Descriptors, i);
        Nh_Vk_destroyDescriptorSet(Driver_p, Descriptor_p, &(Driver_p->DescriptorPool_p[0])); // TODO
    }

    Nh_destroyList(&Node_p->Vulkan.Descriptors, true);

NH_SILENT_END()
}

void Nh_Vk_destroyDescriptor(
    Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_DESCRIPTOR type)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Vulkan.Descriptors.count; ++i) {
        NH_Vk_DescriptorSet *Descriptor_p = Nh_getListItem(&Node_p->Vulkan.Descriptors, i);
        if (Descriptor_p->type == type) {Nh_Vk_destroyDescriptorSet(Driver_p, Descriptor_p, &(Driver_p->DescriptorPool_p[0]));} // TODO
    }

NH_SILENT_END()
}

NH_Vk_DescriptorSet *Nh_Vk_getDescriptor(
    Nh_HTML_Node *Node_p, NH_VK_DESCRIPTOR type)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Vulkan.Descriptors.count; ++i) {
        NH_Vk_DescriptorSet *Descriptor_p = Nh_getListItem(&Node_p->Vulkan.Descriptors, i);
        if (Descriptor_p->type == type) {NH_END(Descriptor_p)}
    }

NH_END(NULL)
}

NH_Vk_DescriptorSet *Nh_Vk_getDescriptorFromIndex(
    Nh_HTML_Node *Node_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(&Node_p->Vulkan.Descriptors, index))
}

// HELPER ==========================================================================================

static NH_RESULT Nh_Vk_allocateDescriptor(
    Nh_HTML_Node *Node_p, NH_Vk_DescriptorSet **Data_pp)
{
NH_BEGIN()

    *Data_pp = Nh_allocate(sizeof(NH_Vk_DescriptorSet));
    NH_CHECK_MEM(*Data_pp)
    Nh_addListItem(&Node_p->Vulkan.Descriptors, *Data_pp);

NH_END(NH_SUCCESS)
}

