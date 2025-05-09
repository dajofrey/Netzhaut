// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Descriptor.h"
#include "Uniform.h"
#include "Buffer.h"

#include "../Main/Vertices.h"

#include "../../nh-gfx/Common/Macros.h"
#include "../../nh-core/Util/Array.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_renderer_createVulkanColorDescriptor(
    nh_gfx_VulkanDriver *Driver_p, nh_css_Fragment *Fragment_p, NH_RENDERER_VK_DESCRIPTOR type)
{
    nh_gfx_VulkanDescriptorSet *Descriptor_p = nh_core_incrementArray(&((nh_renderer_VulkanData*)Fragment_p->data_p)->Descriptors);
    NH_CORE_CHECK_MEM(Descriptor_p)
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
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, Descriptor_p))

    NH_RENDERER_VK_UNIFORM uniformType;
    switch (type)
    {
        case NH_RENDERER_VK_DESCRIPTOR_BACKGROUND    : uniformType = NH_RENDERER_VK_UNIFORM_BACKGROUND; break;
        case NH_RENDERER_VK_DESCRIPTOR_TOP_BORDER    : uniformType = NH_RENDERER_VK_UNIFORM_TOP_BORDER; break; 
        case NH_RENDERER_VK_DESCRIPTOR_RIGHT_BORDER  : uniformType = NH_RENDERER_VK_UNIFORM_RIGHT_BORDER; break;
        case NH_RENDERER_VK_DESCRIPTOR_BOTTOM_BORDER : uniformType = NH_RENDERER_VK_UNIFORM_BOTTOM_BORDER; break;
        case NH_RENDERER_VK_DESCRIPTOR_LEFT_BORDER   : uniformType = NH_RENDERER_VK_UNIFORM_LEFT_BORDER; break;
        default : return NH_API_ERROR_BAD_STATE;
    }

    // update
    VkDescriptorBufferInfo BufferInfo = 
    {
        .buffer = nh_renderer_getVulkanUniform(Fragment_p->data_p, uniformType)->Buffer,
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
    Driver_p->Functions.vkUpdateDescriptorSets(Driver_p->Device, 1, &Write, 0, VK_NULL_HANDLE);  

    return NH_API_SUCCESS;
}

//// IMAGE ===========================================================================================
//
//NH_API_RESULT nh_vk_createImageDescriptor(
//    nh_gfx_VulkanGPU *GPU_p, nh_html_Node *Node_p, nh_Texture *Texture_p)
//{
//    nh_gfx_VulkanDriver *Driver_p = &GPU_p->Driver;
//
//    nh_gfx_VulkanDescriptorSet *Descriptor_p;
//    NH_CHECK(nh_vk_allocateDescriptor(Node_p, &Descriptor_p))
//    Descriptor_p->type = NH_VK_DESCRIPTOR_IMAGE;
//
//// create
//    VkDescriptorSetLayoutBinding setLayoutBindings[2];
//    VkDescriptorSetLayoutBinding setLayoutBinding1 =
//    {
//        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//        .stageFlags      = VK_SHADER_STAGE_VERTEX_BIT,
//        .binding         = 0,
//        .descriptorCount = 1 
//    };
//    VkDescriptorSetLayoutBinding setLayoutBinding2 =
//    {
//        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//        .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
//        .binding         = 1,
//        .descriptorCount = 1 
//    };
//    setLayoutBindings[0] = setLayoutBinding1;
//    setLayoutBindings[1] = setLayoutBinding2;
//
//    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
//    {
//        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
//        .pBindings    = setLayoutBindings,
//        .bindingCount = 2
//    };
//    VkDescriptorSetAllocateInfo AllocateInfo = 
//    {
//        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
//        .descriptorPool     = Driver_p->DescriptorPool_p[0],
//        .descriptorSetCount = 1,
//    };
//
//    NH_CHECK(nh_gfx_createVulkanDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, Descriptor_p))
//
//// update
//
//    VkDescriptorBufferInfo vsDescrBufferInfo = 
//    {
//        .buffer = nh_vk_getUniform(Node_p, NH_VK_UNIFORM_IMAGE)->Buffer,
//        .offset = 0,
//        .range  = sizeof(float) * 17
//    };
//    VkDescriptorImageInfo ImageInfo = 
//    {
//        .sampler     = Texture_p->Vulkan_p->Sampler,
//        .imageView   = Texture_p->Vulkan_p->ImageView,
//        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
//    };
//    VkWriteDescriptorSet vsUniformBufferDescriptorSet = 
//    {
//        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
//        .dstSet          = Descriptor_p->DescriptorSet,
//        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//        .dstBinding      = 0,
//        .pBufferInfo     = &vsDescrBufferInfo,
//        .descriptorCount = 1
//    };
//    VkWriteDescriptorSet fsTextureSamplerDescriptorSet = 
//    {
//        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
//        .dstSet          = Descriptor_p->DescriptorSet,
//        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//        .dstBinding      = 1,
//        .pImageInfo      = &ImageInfo,
//        .descriptorCount = 1
//    };
//    VkWriteDescriptorSet Sets_p[2] = {vsUniformBufferDescriptorSet, fsTextureSamplerDescriptorSet};
//    Driver_p->Functions.vkUpdateDescriptorSets(Driver_p->Device, 2, Sets_p, 0, VK_NULL_HANDLE);
//
//return NH_SUCCESS;
//}
//
//// BACKGROUND IMAGE ================================================================================
//
//NH_API_RESULT nh_vk_createBackgroundImageDescriptor(
//    nh_gfx_VulkanGPU *GPU_p, nh_html_Node *Node_p, NH_RENDERER_Image *Image_p)
//{
//    nh_gfx_VulkanDriver *Driver_p = &GPU_p->Driver;
//
//    nh_gfx_VulkanDescriptorSet *Descriptor_p;
//    NH_CHECK(nh_vk_allocateDescriptor(Node_p, &Descriptor_p))
//    Descriptor_p->type = NH_VK_DESCRIPTOR_BACKGROUND_IMAGE;
//
//    Image_p->descriptor = Node_p->Vulkan.Descriptors.count - 1;
//
//// create
//    VkDescriptorSetLayoutBinding setLayoutBindings[2];
//    VkDescriptorSetLayoutBinding setLayoutBinding1 =
//    {
//        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//        .stageFlags      = VK_SHADER_STAGE_VERTEX_BIT,
//        .binding         = 0,
//        .descriptorCount = 1 
//    };
//    VkDescriptorSetLayoutBinding setLayoutBinding2 =
//    {
//        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//        .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
//        .binding         = 1,
//        .descriptorCount = 1 
//    };
//    setLayoutBindings[0] = setLayoutBinding1;
//    setLayoutBindings[1] = setLayoutBinding2;
//
//    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
//    {
//        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
//        .pBindings    = setLayoutBindings,
//        .bindingCount = 2
//    };
//    VkDescriptorSetAllocateInfo AllocateInfo = 
//    {
//        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
//        .descriptorPool     = Driver_p->DescriptorPool_p[0],
//        .descriptorSetCount = 1,
//    };
//
//    NH_CHECK(nh_gfx_createVulkanDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, Descriptor_p))
//
//// update
//    VkDescriptorBufferInfo vsDescrBufferInfo = 
//    {
//        .buffer = nh_vk_getUniformFromIndex(Node_p, Image_p->uniform)->Buffer,
//        .offset = 0,
//        .range  = sizeof(float) * 17
//    };
//    VkDescriptorImageInfo ImageInfo = 
//    {
//        .sampler     = Image_p->Texture_p->Vulkan_p->Sampler,
//        .imageView   = Image_p->Texture_p->Vulkan_p->ImageView,
//        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
//    };
//    VkWriteDescriptorSet vsUniformBufferDescriptorSet = 
//    {
//        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
//        .dstSet          = Descriptor_p->DescriptorSet,
//        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//        .dstBinding      = 0,
//        .pBufferInfo     = &vsDescrBufferInfo,
//        .descriptorCount = 1
//    };
//    VkWriteDescriptorSet fsTextureSamplerDescriptorSet = 
//    {
//        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
//        .dstSet          = Descriptor_p->DescriptorSet,
//        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//        .dstBinding      = 1,
//        .pImageInfo      = &ImageInfo,
//        .descriptorCount = 1
//    };
//    VkWriteDescriptorSet Sets_p[2] = {vsUniformBufferDescriptorSet, fsTextureSamplerDescriptorSet};
//    Driver_p->Functions.vkUpdateDescriptorSets(Driver_p->Device, 2, Sets_p, 0, VK_NULL_HANDLE);
//
//return NH_SUCCESS;
//}
//
//// CANVAS RENDERING CONTEXT 2D ======================================================================
//
//NH_API_RESULT nh_vk_createCanvasRenderingContext2DDescriptor(
//    nh_Tab *Tab_p, nh_gfx_VulkanDriver *Driver_p, NH_JS_CanvasRenderingContext2D *Context_p, 
//    nh_gfx_VulkanBuffer *Uniform_p, size_t size)
//{
//    nh_gfx_VulkanDescriptorSet *Descriptor_p = nh_core_allocate(sizeof(nh_gfx_VulkanDescriptorSet));
//    NH_CHECK_MEM(Descriptor_p)
//    NH_CHECK(nh_core_appendToLinkedList(&Context_p->Vulkan.Descriptors, Descriptor_p))
//
//    VkDescriptorSetLayoutBinding setLayoutBindings[2];
//    VkDescriptorSetLayoutBinding setLayoutBinding1 =
//    {
//        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//        .stageFlags      = VK_SHADER_STAGE_VERTEX_BIT,
//        .binding         = 0,
//        .descriptorCount = 1 
//    };
//    VkDescriptorSetLayoutBinding setLayoutBinding2 =
//    {
//        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//        .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
//        .binding         = 1,
//        .descriptorCount = 1 
//    };
//    setLayoutBindings[0] = setLayoutBinding1;
//    setLayoutBindings[1] = setLayoutBinding2;
//
//    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
//    {
//        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
//        .pBindings    = setLayoutBindings,
//        .bindingCount = 2
//    };
//    VkDescriptorSetAllocateInfo AllocateInfo = 
//    {
//        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
//        .descriptorPool     = Driver_p->DescriptorPool_p[0],
//        .descriptorSetCount = 1,
//    };
//
//    NH_CHECK(nh_gfx_createVulkanDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, Descriptor_p))
//
//    VkDescriptorBufferInfo vsDescrBufferInfo = 
//    {
//        .buffer = Uniform_p->Buffer,
//        .offset = 0,
//        .range  = size
//    };
//    VkWriteDescriptorSet vsUniformBufferDescriptorSet = 
//    {
//        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
//        .dstSet          = Descriptor_p->DescriptorSet,
//        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//        .dstBinding      = 0,
//        .pBufferInfo     = &vsDescrBufferInfo,
//        .descriptorCount = 1
//    };
//    VkWriteDescriptorSet Sets_p[1] = {vsUniformBufferDescriptorSet};
//    Driver_p->Functions.vkUpdateDescriptorSets(Driver_p->Device, 1, Sets_p, 0, VK_NULL_HANDLE);
//
//return NH_SUCCESS;
//}

void nh_renderer_destroyVulkanDescriptorArray(
    nh_gfx_VulkanDriver *Driver_p, nh_renderer_VulkanData *Data_p)
{
    for (int i = 0; i < Data_p->Descriptors.length; ++i) {
        nh_gfx_VulkanDescriptorSet *Descriptor_p = &((nh_gfx_VulkanDescriptorSet*)Data_p->Descriptors.p)[i];
        nh_gfx_destroyVulkanDescriptorSet(Driver_p, Descriptor_p, &(Driver_p->DescriptorPool_p[0])); // TODO
    }

    nh_core_freeArray(&Data_p->Descriptors);
}

//void nh_vk_destroyDescriptor(
//    nh_gfx_VulkanDriver *Driver_p, nh_html_Node *Node_p, NH_VK_DESCRIPTOR type)
//{
//    for (int i = 0; i < Node_p->Vulkan.Descriptors.count; ++i) {
//        nh_gfx_VulkanDescriptorSet *Descriptor_p = nh_core_getFromLinkedList(&Node_p->Vulkan.Descriptors, i);
//        if (Descriptor_p->type == type) {nh_gfx_destroyVulkanDescriptorSet(Driver_p, Descriptor_p, &(Driver_p->DescriptorPool_p[0]));} // TODO
//    }
//
//NH_RENDERER_SILENT_END()
//}

nh_gfx_VulkanDescriptorSet *nh_renderer_getVulkanDescriptor(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VK_DESCRIPTOR type)
{
    for (int i = 0; i < Data_p->Descriptors.length; ++i) {
        nh_gfx_VulkanDescriptorSet *Descriptor_p = &((nh_gfx_VulkanDescriptorSet*)Data_p->Descriptors.p)[i];
        if (Descriptor_p->type == type) {return Descriptor_p;}
    }

    return NULL;
}

nh_gfx_VulkanDescriptorSet *nh_renderer_getVulkanDescriptorFromIndex(
    nh_renderer_VulkanData *Data_p, NH_RENDERER_VK_DESCRIPTOR type, unsigned int index)
{
    int count = 0;
    for (int i = 0; i < Data_p->Descriptors.length; ++i) {
        nh_gfx_VulkanDescriptorSet *Descriptor_p = &((nh_gfx_VulkanDescriptorSet*)Data_p->Descriptors.p)[i]; 
        if (Descriptor_p != NULL && Descriptor_p->type == type) {
            if (count == index) {return Descriptor_p;}
            else {count++;}
        }
    }

    return NULL;
}

