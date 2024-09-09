// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Texture.h"
#include "Host.h"
#include "Driver.h"
#include "GPU.h"
#include "Vulkan.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/System/Thread.h"
#include "../../nh-core/Util/List.h"

#include <string.h>

// LOAD ============================================================================================

static NH_GFX_RESULT nh_vk_loadTexture(
   nh_vk_Driver *Driver_p, nh_vk_TextureInfo *Info_p, nh_vk_Texture *Texture_p, int threadIndex)
{
NH_GFX_BEGIN()

    NH_GFX_CHECK_NULL_2(NH_GFX_ERROR_PARAMETERS, Info_p)
    NH_GFX_CHECK_NULL_2(NH_GFX_ERROR_PARAMETERS, Driver_p)
    NH_GFX_CHECK_NULL_2(NH_GFX_ERROR_PARAMETERS, Texture_p)

    // prepare
    VkImageCreateInfo ImageInfo = {};
    VkImageViewCreateInfo ImageViewInfo = {};
    VkSamplerCreateInfo SamplerInfo = {};

    VkExtent3D Extent;
    Extent.width  = Info_p->width;
    Extent.height = Info_p->height;
    Extent.depth  = 1;

    ImageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ImageInfo.imageType     = VK_IMAGE_TYPE_2D;
    ImageInfo.format        = Info_p->format;
    ImageInfo.mipLevels     = 1;
    ImageInfo.arrayLayers   = 1;
    ImageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
    ImageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
    ImageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
    ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ImageInfo.extent        = Extent;
    ImageInfo.usage         = Info_p->usageFlags;

    ImageViewInfo.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ImageViewInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
    ImageViewInfo.format       = Info_p->format;
    ImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R,
    ImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G,
    ImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B,
    ImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A,

    ImageViewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
    ImageViewInfo.subresourceRange.baseMipLevel   = 0,
    ImageViewInfo.subresourceRange.levelCount     = 1,
    ImageViewInfo.subresourceRange.baseArrayLayer = 0,
    ImageViewInfo.subresourceRange.layerCount     = 1,

    SamplerInfo.sType        = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    SamplerInfo.magFilter    = VK_FILTER_LINEAR;
    SamplerInfo.minFilter    = VK_FILTER_LINEAR;
    SamplerInfo.mipmapMode   = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.mipLodBias   = 0.0f;
    SamplerInfo.compareOp    = VK_COMPARE_OP_NEVER;
    SamplerInfo.minLod       = 0.0f;
    SamplerInfo.maxLod       = 1;
    SamplerInfo.maxAnisotropy    = 1.0f;
    SamplerInfo.anisotropyEnable = false;
    SamplerInfo.borderColor      = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    Texture_p->width  = Info_p->width;
    Texture_p->height = Info_p->height;

    // get device properites for the requested texture format
    VkFormatProperties formatProperties;
    NH_VULKAN.Host.Functions.vkGetPhysicalDeviceFormatProperties(Driver_p->PhysicalDevice, ImageInfo.format, &formatProperties);
    
    VkMemoryAllocateInfo memAllocInfo = {};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VkMemoryRequirements memReqs = {};
    
    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = Driver_p->GraphicsCommandPools_p[threadIndex],  
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer CopyCmd;
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkAllocateCommandBuffers(Driver_p->Device, &AllocateInfo, &CopyCmd))

    // create a host-visible staging buffer that contains the raw image data
    VkBuffer StagingBuffer;
    VkDeviceMemory StagingMemory;
    
    VkBufferCreateInfo bufferCreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = Info_p->size,
        .usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (Driver_p->Functions.vkCreateBuffer(Driver_p->Device, &bufferCreateInfo, VK_NULL_HANDLE, &StagingBuffer) != VK_SUCCESS) {
        NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_BAD_STATE)
    }

    // get memory requirements for the staging buffer (alignment, memory type bits)
    Driver_p->Functions.vkGetBufferMemoryRequirements(Driver_p->Device, StagingBuffer, &memReqs);
    
    memAllocInfo.allocationSize = memReqs.size;
    // get memory type index for a host visible buffer
    nh_vk_findMemoryType(
        Driver_p, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &memAllocInfo.memoryTypeIndex
    );

    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkAllocateMemory(Driver_p->Device, &memAllocInfo, VK_NULL_HANDLE, &StagingMemory))
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkBindBufferMemory(Driver_p->Device, StagingBuffer, StagingMemory, 0))

    // copy texture data into staging buffer
    uint8_t *data;
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkMapMemory(Driver_p->Device, StagingMemory, 0, memReqs.size, 0, (void **)&data))
    memcpy(data, Info_p->data_p, Info_p->size);
    Driver_p->Functions.vkUnmapMemory(Driver_p->Device, StagingMemory);	
    
    // setup buffer copy regions for each mip level
    VkBufferImageCopy bufferCopyRegions_p[ImageInfo.mipLevels];
    uint32_t offset = 0;

    for (uint32_t i = 0; i < ImageInfo.mipLevels; i++)
    {
    	VkBufferImageCopy bufferCopyRegion = {};
    	bufferCopyRegion.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    	bufferCopyRegion.imageSubresource.mipLevel       = i;
    	bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
    	bufferCopyRegion.imageSubresource.layerCount     = 1;
    	bufferCopyRegion.imageExtent.width               = (uint32_t)Info_p->width;
    	bufferCopyRegion.imageExtent.height              = (uint32_t)Info_p->height;
    	bufferCopyRegion.imageExtent.depth               = 1;
    	bufferCopyRegion.bufferOffset                    = offset;
    
    	bufferCopyRegions_p[i] = bufferCopyRegion;
    
    	offset += (uint32_t)Info_p->size;
    }

    // ensure that the TRANSFER_DST bit is set for staging
    if (!(ImageInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)) {
    	ImageInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkCreateImage(Driver_p->Device, &ImageInfo, VK_NULL_HANDLE, &Texture_p->Image))
    Driver_p->Functions.vkGetImageMemoryRequirements(Driver_p->Device, Texture_p->Image, &memReqs);
    
    memAllocInfo.allocationSize = memReqs.size;
    nh_vk_findMemoryType(
        Driver_p, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memAllocInfo.memoryTypeIndex
    );

    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkAllocateMemory(Driver_p->Device, &memAllocInfo, VK_NULL_HANDLE, &Texture_p->DeviceMemory))
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkBindImageMemory(Driver_p->Device, Texture_p->Image, Texture_p->DeviceMemory, 0))
    
    VkImageSubresourceRange SubresourceRange = {};
    SubresourceRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;
    SubresourceRange.baseMipLevel = 0;
    SubresourceRange.levelCount   = ImageInfo.mipLevels;
    SubresourceRange.layerCount   = 1;

    // copy
    VkCommandBufferBeginInfo cmdBufferInfo = {
	.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };
    Driver_p->Functions.vkBeginCommandBuffer(CopyCmd, &cmdBufferInfo);

    VkImageMemoryBarrier barrier1 = 
    {
        .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask       = 0,
        .dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = Texture_p->Image,
        .subresourceRange    = SubresourceRange,
    };

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    sourceStage      = VK_PIPELINE_STAGE_HOST_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    Driver_p->Functions.vkCmdPipelineBarrier(
        CopyCmd, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier1
    );

    VkBufferImageCopy bufferCopyRegion = {0};
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferCopyRegion.imageSubresource.layerCount = 1;
    bufferCopyRegion.imageExtent.width           = Texture_p->width;
    bufferCopyRegion.imageExtent.height          = Texture_p->height;
    bufferCopyRegion.imageExtent.depth           = 1;

    Driver_p->Functions.vkCmdCopyBufferToImage(
        CopyCmd, StagingBuffer, Texture_p->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ImageInfo.mipLevels, bufferCopyRegions_p
    );

    VkImageMemoryBarrier barrier2 = 
    {
        .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask       = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout           = Info_p->imageLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = Texture_p->Image,
        .subresourceRange    = SubresourceRange,
    };
    sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    Driver_p->Functions.vkCmdPipelineBarrier(
        CopyCmd, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier2
    );

    Driver_p->Functions.vkEndCommandBuffer(CopyCmd);

    VkSubmitInfo submitInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers    = &CopyCmd
    };
    
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkQueueSubmit(Driver_p->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE))
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkQueueWaitIdle(Driver_p->GraphicsQueue))

    // clean up staging resources
    Driver_p->Functions.vkFreeMemory(Driver_p->Device, StagingMemory, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyBuffer(Driver_p->Device, StagingBuffer, VK_NULL_HANDLE);

    // misc
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkCreateSampler(Driver_p->Device, &SamplerInfo, VK_NULL_HANDLE, &Texture_p->Sampler))
    
    ImageViewInfo.image = Texture_p->Image;
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkCreateImageView(Driver_p->Device, &ImageViewInfo, VK_NULL_HANDLE, &Texture_p->ImageView))
    
    Texture_p->DescriptorImageInfo.sampler     = Texture_p->Sampler;
    Texture_p->DescriptorImageInfo.imageView   = Texture_p->ImageView;
    Texture_p->DescriptorImageInfo.imageLayout = Info_p->imageLayout;

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

// RELOAD ==========================================================================================

static NH_GFX_RESULT nh_vk_reloadTexture(
   nh_vk_Driver *Driver_p, nh_vk_TextureInfo *Info_p, nh_vk_Texture *Texture_p, int threadIndex)
{
NH_GFX_BEGIN()

    NH_GFX_CHECK_NULL_2(NH_GFX_ERROR_PARAMETERS, Info_p)
    NH_GFX_CHECK_NULL_2(NH_GFX_ERROR_PARAMETERS, Driver_p)
    NH_GFX_CHECK_NULL_2(NH_GFX_ERROR_PARAMETERS, Texture_p)

    // prepare
    VkImageCreateInfo ImageInfo = {};
    VkImageViewCreateInfo ImageViewInfo = {};
    VkSamplerCreateInfo SamplerInfo = {};

    VkExtent3D Extent;
    Extent.width  = Info_p->width;
    Extent.height = Info_p->height;
    Extent.depth  = 1;

    ImageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ImageInfo.imageType     = VK_IMAGE_TYPE_2D;
    ImageInfo.format        = Info_p->format;
    ImageInfo.mipLevels     = 1;
    ImageInfo.arrayLayers   = 1;
    ImageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
    ImageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
    ImageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
    ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ImageInfo.extent        = Extent;
    ImageInfo.usage         = Info_p->usageFlags;

    ImageViewInfo.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ImageViewInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
    ImageViewInfo.format       = Info_p->format;
    ImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R,
    ImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G,
    ImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B,
    ImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A,

    ImageViewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
    ImageViewInfo.subresourceRange.baseMipLevel   = 0,
    ImageViewInfo.subresourceRange.levelCount     = 1,
    ImageViewInfo.subresourceRange.baseArrayLayer = 0,
    ImageViewInfo.subresourceRange.layerCount     = 1,

    SamplerInfo.sType        = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    SamplerInfo.magFilter    = VK_FILTER_LINEAR;
    SamplerInfo.minFilter    = VK_FILTER_LINEAR;
    SamplerInfo.mipmapMode   = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.mipLodBias   = 0.0f;
    SamplerInfo.compareOp    = VK_COMPARE_OP_NEVER;
    SamplerInfo.minLod       = 0.0f;
    SamplerInfo.maxLod       = 1;
    SamplerInfo.maxAnisotropy    = 1.0f;
    SamplerInfo.anisotropyEnable = false;
    SamplerInfo.borderColor      = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    Texture_p->width  = Info_p->width;
    Texture_p->height = Info_p->height;

    // get device properites for the requested texture format
    VkFormatProperties formatProperties;
    NH_VULKAN.Host.Functions.vkGetPhysicalDeviceFormatProperties(Driver_p->PhysicalDevice, ImageInfo.format, &formatProperties);
    
    VkMemoryAllocateInfo memAllocInfo = {};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VkMemoryRequirements memReqs = {};
    
    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = Driver_p->GraphicsCommandPools_p[threadIndex],  
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer CopyCmd;
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkAllocateCommandBuffers(Driver_p->Device, &AllocateInfo, &CopyCmd))

    // create a host-visible staging buffer that contains the raw image data
    VkBuffer StagingBuffer;
    VkDeviceMemory StagingMemory;
    
    VkBufferCreateInfo bufferCreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = Info_p->size,
        .usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (Driver_p->Functions.vkCreateBuffer(Driver_p->Device, &bufferCreateInfo, VK_NULL_HANDLE, &StagingBuffer) != VK_SUCCESS) {
        NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_BAD_STATE)
    }

    // get memory requirements for the staging buffer (alignment, memory type bits)
    Driver_p->Functions.vkGetBufferMemoryRequirements(Driver_p->Device, StagingBuffer, &memReqs);
    
    memAllocInfo.allocationSize = memReqs.size;
    // get memory type index for a host visible buffer
    nh_vk_findMemoryType(
        Driver_p, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &memAllocInfo.memoryTypeIndex
    );

    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkAllocateMemory(Driver_p->Device, &memAllocInfo, VK_NULL_HANDLE, &StagingMemory))
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkBindBufferMemory(Driver_p->Device, StagingBuffer, StagingMemory, 0))

    // copy texture data into staging buffer
    uint8_t *data;
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkMapMemory(Driver_p->Device, StagingMemory, 0, memReqs.size, 0, (void **)&data))
    memcpy(data, Info_p->data_p, Info_p->size);
    Driver_p->Functions.vkUnmapMemory(Driver_p->Device, StagingMemory);	
    
    // setup buffer copy regions for each mip level
    VkBufferImageCopy bufferCopyRegions_p[ImageInfo.mipLevels];
    uint32_t offset = 0;

    for (uint32_t i = 0; i < ImageInfo.mipLevels; i++)
    {
    	VkBufferImageCopy bufferCopyRegion = {};
    	bufferCopyRegion.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    	bufferCopyRegion.imageSubresource.mipLevel       = i;
    	bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
    	bufferCopyRegion.imageSubresource.layerCount     = 1;
    	bufferCopyRegion.imageExtent.width               = (uint32_t)Info_p->width;
    	bufferCopyRegion.imageExtent.height              = (uint32_t)Info_p->height;
    	bufferCopyRegion.imageExtent.depth               = 1;
    	bufferCopyRegion.bufferOffset                    = offset;
    
    	bufferCopyRegions_p[i] = bufferCopyRegion;
    
    	offset += (uint32_t)Info_p->size;
    }

    // ensure that the TRANSFER_DST bit is set for staging
    if (!(ImageInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)) {
    	ImageInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    
    VkImageSubresourceRange SubresourceRange = {};
    SubresourceRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;
    SubresourceRange.baseMipLevel = 0;
    SubresourceRange.levelCount   = ImageInfo.mipLevels;
    SubresourceRange.layerCount   = 1;

    // copy
    VkCommandBufferBeginInfo cmdBufferInfo = {
	.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };
    Driver_p->Functions.vkBeginCommandBuffer(CopyCmd, &cmdBufferInfo);

    VkImageMemoryBarrier barrier1 = 
    {
        .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask       = 0,
        .dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = Texture_p->Image,
        .subresourceRange    = SubresourceRange,
    };

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    sourceStage      = VK_PIPELINE_STAGE_HOST_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    Driver_p->Functions.vkCmdPipelineBarrier(
        CopyCmd, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier1
    );

    VkBufferImageCopy bufferCopyRegion = {0};
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferCopyRegion.imageSubresource.layerCount = 1;
    bufferCopyRegion.imageExtent.width           = Texture_p->width;
    bufferCopyRegion.imageExtent.height          = Texture_p->height;
    bufferCopyRegion.imageExtent.depth           = 1;

    Driver_p->Functions.vkCmdCopyBufferToImage(
        CopyCmd, StagingBuffer, Texture_p->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ImageInfo.mipLevels, bufferCopyRegions_p
    );

    VkImageMemoryBarrier barrier2 = 
    {
        .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask       = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout           = Info_p->imageLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = Texture_p->Image,
        .subresourceRange    = SubresourceRange,
    };
    sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    Driver_p->Functions.vkCmdPipelineBarrier(
        CopyCmd, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier2
    );

    Driver_p->Functions.vkEndCommandBuffer(CopyCmd);

    VkSubmitInfo submitInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers    = &CopyCmd
    };
    
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkQueueSubmit(Driver_p->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE))
    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkQueueWaitIdle(Driver_p->GraphicsQueue))

    // clean up staging resources
    Driver_p->Functions.vkFreeMemory(Driver_p->Device, StagingMemory, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyBuffer(Driver_p->Device, StagingBuffer, VK_NULL_HANDLE);

NH_GFX_END(NH_GFX_SUCCESS)
}

// IMPLEMENT =======================================================================================

//static NH_GFX_RESULT nh_vk_relayLoadOperation(
//    nh_vk_GPU *GPU_p, nh_Texture *Texture_p, NH_BOOL reload)
//{
//NH_GFX_BEGIN()
//
//    nh_vk_TextureInfo Info = {};
//
//    NH_MEDIA mediaType = nh_core_getMediaType(&Texture_p->URI);
//
//    switch (mediaType)
//    {
//        case NH_MEDIA_IMAGE_JPEG :
//        case NH_MEDIA_IMAGE_PNG  :
//
//            Info.width            = Texture_p->width;
//            Info.height           = Texture_p->height;
//            Info.data_p           = Texture_p->pixels_p;
//            Info.size             = Texture_p->width * Texture_p->height * 4;
//            Info.format           = VK_FORMAT_R8G8B8A8_UNORM;
//            Info.imageLayout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//            Info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
//            Info.usageFlags       = VK_IMAGE_USAGE_SAMPLED_BIT;
//
//            if (!reload) {
//                NH_GFX_CHECK(nh_vk_loadTexture(
//                    &GPU_p->Driver, &Info, Texture_p->Vulkan_p, nh_core_getThreadIndex()
//                ))
//            }
//            else {
//                NH_GFX_CHECK(nh_vk_reloadTexture(
//                    &GPU_p->Driver, &Info, Texture_p->Vulkan_p, nh_core_getThreadIndex()
//                ))
//            }
//           
//            break;
//
//        case NH_MEDIA_FONT_TTF :
//        case NH_MEDIA_FONT_OTF :
//
//            Info.width            = Texture_p->width;
//            Info.height           = Texture_p->height;
//            Info.data_p           = Texture_p->pixels_p;
//            Info.size             = Texture_p->width * Texture_p->height;
//            Info.format           = VK_FORMAT_R8_UNORM;
//            Info.imageLayout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//            Info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
//            Info.usageFlags       = VK_IMAGE_USAGE_SAMPLED_BIT;
//
//            if (!reload) {
//                NH_GFX_CHECK(nh_vk_loadTexture(
//                    &GPU_p->Driver, &Info, Texture_p->Vulkan_p, nh_core_getThreadIndex()
//                ))
//            }
//            else {
//                NH_GFX_CHECK(nh_vk_reloadTexture(
//                    &GPU_p->Driver, &Info, Texture_p->Vulkan_p, nh_core_getThreadIndex()
//                ))
//            }            
//
//            break;
//
//        case -1 :
//
//            Info.width            = Texture_p->width;
//            Info.height           = Texture_p->height;
//            Info.data_p           = Texture_p->data_p;
//            Info.size             = sizeof(uint32_t) * (Texture_p->width * Texture_p->height);
//            Info.format           = VK_FORMAT_R8G8B8A8_UNORM;
//            Info.imageLayout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//            Info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
//            Info.usageFlags       = VK_IMAGE_USAGE_SAMPLED_BIT;
//            
//            NH_GFX_CHECK(nh_vk_loadTexture(
//                &GPU_p->Driver, &Info, Texture_p->Vulkan_p, nh_core_getThreadIndex()
//            ))
//            
//            break;
//
//        default : NH_GFX_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
//    }
//
//    Texture_p->width  = Texture_p->Vulkan_p->width;
//    Texture_p->height = Texture_p->Vulkan_p->height;
//
//NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
//}

nh_vk_Texture *nh_vk_loadFontTexture(
    nh_vk_GPU *GPU_p, nh_gfx_Font *Font_p)
{
NH_GFX_BEGIN()

    NH_GFX_CHECK_NULL_2(NULL, GPU_p)
    NH_GFX_CHECK_NULL_2(NULL, Font_p)

    for (int i = 0; i < GPU_p->Textures.size; ++i) {
        if (((nh_vk_Texture*)GPU_p->Textures.pp[i])->source_p == Font_p) {
            NH_GFX_END(GPU_p->Textures.pp[i])
        }
    }

    nh_vk_Texture *Texture_p = nh_core_allocate(sizeof(nh_vk_Texture));
    NH_GFX_CHECK_MEM_2(NULL, Texture_p)

    Texture_p->source_p = Font_p;

    nh_vk_TextureInfo Info = {};

    Info.width            = Font_p->Atlas.width;
    Info.height           = Font_p->Atlas.height;
    Info.data_p           = Font_p->Atlas.data_p;
    Info.size             = Font_p->Atlas.width * Font_p->Atlas.height;
    Info.format           = VK_FORMAT_R8_UNORM;
    Info.imageLayout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    Info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    Info.usageFlags       = VK_IMAGE_USAGE_SAMPLED_BIT;

    NH_GFX_CHECK_2(NULL, nh_vk_loadTexture(
        &GPU_p->Driver, &Info, Texture_p, nh_core_getThreadIndex()
    ))

    nh_core_appendToList(&GPU_p->Textures, Texture_p);

NH_GFX_END(Texture_p)
}

nh_vk_Texture *nh_vk_reloadFontTexture(
    nh_vk_GPU *GPU_p, nh_gfx_Font *Font_p)
{
NH_GFX_BEGIN()

    NH_GFX_CHECK_NULL_2(NULL, GPU_p)
    NH_GFX_CHECK_NULL_2(NULL, Font_p)

    nh_vk_Texture *Texture_p = NULL;
    for (int i = 0; i < GPU_p->Textures.size; ++i) {
        if (((nh_vk_Texture*)GPU_p->Textures.pp[i])->source_p == Font_p) {
            Texture_p = GPU_p->Textures.pp[i];
        }
    }

    if (Texture_p) {nh_vk_destroyTexture(GPU_p, Texture_p);}

NH_GFX_END(nh_vk_loadFontTexture(GPU_p, Font_p))
}

void nh_vk_destroyTexture(
    nh_vk_GPU *GPU_p, nh_vk_Texture *Texture_p)
{
NH_GFX_BEGIN()
   
    GPU_p->Driver.Functions.vkFreeMemory(GPU_p->Driver.Device, Texture_p->DeviceMemory, VK_NULL_HANDLE); 
    GPU_p->Driver.Functions.vkDestroyImageView(GPU_p->Driver.Device, Texture_p->ImageView, VK_NULL_HANDLE);
    GPU_p->Driver.Functions.vkDestroyImage(GPU_p->Driver.Device, Texture_p->Image, VK_NULL_HANDLE);
    GPU_p->Driver.Functions.vkDestroySampler(GPU_p->Driver.Device, Texture_p->Sampler, VK_NULL_HANDLE);

    nh_core_removeFromList2(&GPU_p->Textures, NH_TRUE, Texture_p);

NH_GFX_SILENT_END()
}

