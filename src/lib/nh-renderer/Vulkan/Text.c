// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Text.h"
#include "Buffer.h"
#include "Uniform.h"
#include "Descriptor.h"

#include "../Common/Macros.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-gfx/Vulkan/Texture.h"
#include "../../nh-gfx/Common/Macros.h"

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <float.h>

// HELPER ==========================================================================================

static NH_RENDERER_RESULT nh_renderer_vk_createTextVertices(
    nh_vk_Driver *Driver_p, nh_css_Fragment *Fragment_p, nh_gfx_TextSegment *Segment_p, 
    float *vertices_p, uint32_t *indices_p, int index)
{
NH_RENDERER_BEGIN()

    VkBufferCreateInfo bufferCreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size        = Segment_p->length * 20 * sizeof(float),
        .usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };
    nh_vk_BufferInfo bufferInfo = {
        .Info_p           = &bufferCreateInfo,
        .data_p           = vertices_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_FALSE,
    };

    NH_GFX_CHECK_2(NH_RENDERER_ERROR_BAD_STATE, nh_vk_createBuffer(
        Driver_p, &bufferInfo, nh_renderer_vk_getBufferFromIndex(Fragment_p->data_p, NH_RENDERER_VK_BUFFER_TEXT_SDF, index)
    ))

    bufferCreateInfo.size  = Segment_p->length * 6 * sizeof(uint32_t);
    bufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    bufferInfo.data_p = indices_p;
    bufferInfo.Info_p = &bufferCreateInfo;
   
    NH_GFX_CHECK_2(NH_RENDERER_ERROR_BAD_STATE, nh_vk_createBuffer(
        Driver_p, &bufferInfo, nh_renderer_vk_getBufferFromIndex(Fragment_p->data_p, NH_RENDERER_VK_BUFFER_TEXT_SDF_INDEX, index)
    ))
 
NH_RENDERER_DIAGNOSTIC_END(NH_RENDERER_SUCCESS)
}

static NH_RENDERER_RESULT nh_renderer_vk_createTextUniform(
    nh_vk_Driver *Driver_p, nh_css_Fragment *Fragment_p, int index)
{
NH_RENDERER_BEGIN()

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
    nh_vk_BufferInfo bufferInfo =
    {
        .Info_p           = &vsInfo,
        .data_p           = vsData_p,
        .mapMemory        = NH_TRUE,
        .memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        .createInfo       = NH_FALSE,
    };

    NH_GFX_CHECK_2(NH_RENDERER_ERROR_BAD_STATE, nh_vk_createBuffer(
        Driver_p, &bufferInfo, nh_renderer_vk_getUniformFromIndex(Fragment_p->data_p, NH_RENDERER_VK_UNIFORM_TEXT_SDF_VS, index)
    ))

    // text color, etc.
    float fsData_p[11];
    fsData_p[0]  = Fragment_p->Text.Values.Color.r;
    fsData_p[1]  = Fragment_p->Text.Values.Color.g;
    fsData_p[2]  = Fragment_p->Text.Values.Color.b;
    fsData_p[3]  = Fragment_p->Text.Values.Color.a;
    fsData_p[10] = Fragment_p->Text.Values.fontSize;

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

    NH_GFX_CHECK_2(NH_RENDERER_ERROR_BAD_STATE, nh_vk_createBuffer(
        Driver_p, &bufferInfo, nh_renderer_vk_getUniformFromIndex(Fragment_p->data_p, NH_RENDERER_VK_UNIFORM_TEXT_SDF_FS, index)
    ))

NH_RENDERER_DIAGNOSTIC_END(NH_RENDERER_SUCCESS)
}

static NH_RENDERER_RESULT nh_renderer_vk_createTextDescriptor(
    nh_vk_GPU *GPU_p, nh_css_Fragment *Fragment_p, nh_vk_Texture *Texture_p, int index)
{
NH_RENDERER_BEGIN()

    nh_vk_Driver *Driver_p = &GPU_p->Driver;

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

    NH_GFX_CHECK_2(NH_RENDERER_ERROR_BAD_STATE, nh_vk_createDescriptorSet(Driver_p, &AllocateInfo, &LayoutInfo, nh_renderer_vk_getDescriptorFromIndex(Fragment_p->data_p, NH_RENDERER_VK_DESCRIPTOR_TEXT_SDF, index)))

    // update
    VkDescriptorBufferInfo vsDescrBufferInfo = 
    {
        .buffer = nh_renderer_vk_getUniformFromIndex(Fragment_p->data_p, NH_RENDERER_VK_UNIFORM_TEXT_SDF_VS, index)->Buffer,
        .offset = 0,
        .range  = sizeof(float) * 16
    };
    VkDescriptorBufferInfo fsDescrBufferInfo = 
    {
        .buffer = nh_renderer_vk_getUniformFromIndex(Fragment_p->data_p, NH_RENDERER_VK_UNIFORM_TEXT_SDF_FS, index)->Buffer,
        .offset = 0,
        .range  = sizeof(float) * 11
    };        
    VkDescriptorImageInfo ImageInfo = 
    {
        .sampler     = Texture_p->Sampler,
        .imageView   = Texture_p->ImageView,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    VkWriteDescriptorSet vsUniformBufferDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = nh_renderer_vk_getDescriptorFromIndex(Fragment_p->data_p, NH_RENDERER_VK_DESCRIPTOR_TEXT_SDF, index)->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .dstBinding      = 0,
        .pBufferInfo     = &vsDescrBufferInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet fsTextureSamplerDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = nh_renderer_vk_getDescriptorFromIndex(Fragment_p->data_p, NH_RENDERER_VK_DESCRIPTOR_TEXT_SDF, index)->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .dstBinding      = 1,
        .pImageInfo      = &ImageInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet fsUniformBufferDescriptorSet = 
    {
        .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet          = nh_renderer_vk_getDescriptorFromIndex(Fragment_p->data_p, NH_RENDERER_VK_DESCRIPTOR_TEXT_SDF, index)->DescriptorSet,
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .dstBinding      = 2,
        .pBufferInfo     = &fsDescrBufferInfo,
        .descriptorCount = 1
    };
    VkWriteDescriptorSet Sets_p[3] = {vsUniformBufferDescriptorSet, fsTextureSamplerDescriptorSet, fsUniformBufferDescriptorSet};
    Driver_p->Functions.vkUpdateDescriptorSets(Driver_p->Device, 3, Sets_p, 0, VK_NULL_HANDLE);

NH_RENDERER_DIAGNOSTIC_END(NH_RENDERER_SUCCESS)
}

// CREATE AND UPDATE ===============================================================================

NH_RENDERER_RESULT nh_renderer_vk_createText(
    nh_vk_GPU *GPU_p, nh_css_Fragment *Fragment_p, nh_gfx_TextSegment *Segment_p, NH_BOOL allocate, 
    float *vertices_p, uint32_t *indices_p, int index)
{
NH_RENDERER_BEGIN()

    nh_vk_Texture *Texture_p = nh_vk_loadFontTexture(GPU_p, Segment_p->FontInstance_p->Font_p);

    if (allocate) 
    {
        nh_vk_Buffer *Buffer_p = nh_core_incrementArray(&((nh_renderer_vk_Data*)Fragment_p->data_p)->Buffers);
        NH_RENDERER_CHECK_MEM(Buffer_p)
        Buffer_p->type = NH_RENDERER_VK_BUFFER_TEXT_SDF;

        Buffer_p = nh_core_incrementArray(&((nh_renderer_vk_Data*)Fragment_p->data_p)->Buffers);
        NH_RENDERER_CHECK_MEM(Buffer_p)
        Buffer_p->type = NH_RENDERER_VK_BUFFER_TEXT_SDF_INDEX;

        Buffer_p = nh_core_incrementArray(&((nh_renderer_vk_Data*)Fragment_p->data_p)->Uniforms);
        NH_RENDERER_CHECK_MEM(Buffer_p)
        Buffer_p->type = NH_RENDERER_VK_UNIFORM_TEXT_SDF_VS;

        Buffer_p = nh_core_incrementArray(&((nh_renderer_vk_Data*)Fragment_p->data_p)->Uniforms);
        NH_RENDERER_CHECK_MEM(Buffer_p)
        Buffer_p->type = NH_RENDERER_VK_UNIFORM_TEXT_SDF_FS;

        nh_vk_DescriptorSet *Descriptor_p = nh_core_incrementArray(&((nh_renderer_vk_Data*)Fragment_p->data_p)->Descriptors);
        NH_RENDERER_CHECK_MEM(Descriptor_p)
        Descriptor_p->type = NH_RENDERER_VK_DESCRIPTOR_TEXT_SDF;
    }

    NH_RENDERER_CHECK(nh_renderer_vk_createTextVertices(&GPU_p->Driver, Fragment_p, Segment_p, vertices_p, indices_p, index))
    NH_RENDERER_CHECK(nh_renderer_vk_createTextUniform(&GPU_p->Driver, Fragment_p, index))
    NH_RENDERER_CHECK(nh_renderer_vk_createTextDescriptor(GPU_p, Fragment_p, Texture_p, index))

NH_RENDERER_DIAGNOSTIC_END(NH_RENDERER_SUCCESS)
}

//NH_RENDERER_RESULT nh_vk_updateText(
//    nh_Tab *Tab_p, nh_renderer_FormattingNodeFragment *Fragment_p)
//{
//NH_RENDERER_BEGIN()
//
//    NH_RENDERER_CHECK_NULL(Tab_p, Fragment_p)
//    nh_vk_GPU *GPU_p = Tab_p->Window_p->GPU.Pointer;
//
//    if (Fragment_p->Computed.Text.lineCount == 0) 
//    {
//        NH_RENDERER_CHECK(nh_html_createText(Tab_p, Fragment_p, &Fragment_p->Computed.Text))
//        NH_RENDERER_CHECK(nh_vk_createText(Tab_p, Fragment_p, true))
//        NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//    } 
//    else {NH_RENDERER_CHECK(nh_html_updateText(Tab_p, Fragment_p))}
//
//    nh_vk_destroyBufferFromType(&GPU_p->Driver, Fragment_p, NH_VK_BUFFER_TEXT_SDF); 
//    nh_vk_destroyBufferFromType(&GPU_p->Driver, Fragment_p, NH_VK_BUFFER_TEXT_SDF_INDEX);
//
//    nh_vk_destroyUniform(&GPU_p->Driver, Fragment_p, NH_VK_UNIFORM_TEXT_SDF_VS);
//    nh_vk_destroyUniform(&GPU_p->Driver, Fragment_p, NH_VK_UNIFORM_TEXT_SDF_FS);
//    nh_vk_destroyDescriptor(&GPU_p->Driver, Fragment_p, NH_VK_DESCRIPTOR_TEXT_SDF);
//
//    NH_RENDERER_CHECK(nh_vk_createText(Tab_p, Fragment_p, false))
//
//NH_RENDERER_DIAGNOSTIC_END(NH_SUCCESS)
//}

