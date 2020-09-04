// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Texture.h"
#include "../Header/Host.h"
#include "../Header/Driver.h"
#include "../Header/GPU.h"
#include "../Header/Utils.h"
#include "../Header/Macros.h"

#include "../../../API/Header/Vulkan.h"

#include "../../../Core/Header/Thread.h"
#include "../../../Core/Header/List.h"
#include "../../../Core/Header/Memory.h"

#include "../../../CSS/Header/Box.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// IMPLEMENT =======================================================================================

NH_RESULT Nh_Vk_hardLoadTexture(
    Nh_Vk_GPU *GPU_p, Nh_Gfx_Texture *Texture_p)
{
NH_BEGIN()

    NH_CHECK_NULL(GPU_p, Texture_p)

    if (Texture_p->Vulkan_p != NULL) {NH_END(NH_SUCCESS)}

    Texture_p->Vulkan_p = Nh_allocate(sizeof(NH_Vk_Texture));
    NH_CHECK_MEM(Texture_p->Vulkan_p)
    
    NH_Vk_TextureInfo Info = {};

    NH_MEDIA mediaType = Nh_getMediaType(Texture_p->URI);

    switch (mediaType)
    {
        case NH_MEDIA_IMAGE_JPEG :
        case NH_MEDIA_IMAGE_PNG  :

            Info.width            = Texture_p->width;
            Info.height           = Texture_p->height;
            Info.data_p           = Texture_p->pixels_p;
            Info.size             = Texture_p->width * Texture_p->height * 4;
            Info.format           = VK_FORMAT_R8G8B8A8_UNORM;
            Info.imageLayout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            Info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            Info.usageFlags       = VK_IMAGE_USAGE_SAMPLED_BIT;

            NH_CHECK(Nh_Vk_loadTexture(
                Nh_Vk_getHost(), &GPU_p->Driver, &Info, Texture_p->Vulkan_p, Nh_getKernelThread()->id
            ))
            
            break;

        case NH_MEDIA_FONT_TTF :
        case NH_MEDIA_FONT_OTF :

            Info.width            = Texture_p->width;
            Info.height           = Texture_p->height;
            Info.data_p           = Texture_p->pixels_p;
            Info.size             = Texture_p->width * Texture_p->height;
            Info.format           = VK_FORMAT_R8_UNORM;
            Info.imageLayout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            Info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            Info.usageFlags       = VK_IMAGE_USAGE_SAMPLED_BIT;

            NH_CHECK(Nh_Vk_loadTexture(
                Nh_Vk_getHost(), &GPU_p->Driver, &Info, Texture_p->Vulkan_p, Nh_getKernelThread()->id
            ))
            
            break;

        case -1 :

            Info.width            = Texture_p->width;
            Info.height           = Texture_p->height;
            Info.data_p           = Texture_p->data_p;
            Info.size             = sizeof(uint32_t) * (Texture_p->width * Texture_p->height);
            Info.format           = VK_FORMAT_R8G8B8A8_UNORM;
            Info.imageLayout      = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            Info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            Info.usageFlags       = VK_IMAGE_USAGE_SAMPLED_BIT;
            
            NH_CHECK(Nh_Vk_loadTexture(
                Nh_Vk_getHost(), &GPU_p->Driver, &Info, Texture_p->Vulkan_p, Nh_getKernelThread()->id
            ))
            
            break;

        default : NH_END(NH_ERROR_BAD_STATE)
    }

    Texture_p->width  = Texture_p->Vulkan_p->width;
    Texture_p->height = Texture_p->Vulkan_p->height;

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_loadTexture(
   Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, NH_Vk_TextureInfo *Info_p, 
   NH_Vk_Texture *Texture_p, int threadId)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_ERROR_PARAMETERS, Info_p, Host_p, Driver_p, Texture_p)

#include NH_DEFAULT_CHECK

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
    Host_p->Func.vkGetPhysicalDeviceFormatProperties(Driver_p->PhysicalDevice, ImageInfo.format, &formatProperties);
    
    VkMemoryAllocateInfo memAllocInfo = {};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VkMemoryRequirements memReqs = {};
    
    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = Driver_p->GraphicsCommandPools_p[threadId],  
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer copyCmd;
    NH_CHECK_VULKAN(Driver_p->Func.vkAllocateCommandBuffers(Driver_p->Device, &AllocateInfo, &copyCmd))

    // create a host-visible staging buffer that contains the raw image data
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;
    
    VkBufferCreateInfo bufferCreateInfo = 
    {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext       = VK_NULL_HANDLE,
        .size        = Info_p->size,
        .usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (Driver_p->Func.vkCreateBuffer(Driver_p->Device, &bufferCreateInfo, VK_NULL_HANDLE, &stagingBuffer) != VK_SUCCESS) {
        NH_END(NH_ERROR_BAD_STATE)
    }

    // get memory requirements for the staging buffer (alignment, memory type bits)
    Driver_p->Func.vkGetBufferMemoryRequirements(Driver_p->Device, stagingBuffer, &memReqs);
    
    memAllocInfo.allocationSize = memReqs.size;
    // get memory type index for a host visible buffer
    Nh_Vk_findMemoryType(
        Host_p, Driver_p, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &memAllocInfo.memoryTypeIndex
    );

    NH_CHECK_VULKAN(Driver_p->Func.vkAllocateMemory(Driver_p->Device, &memAllocInfo, VK_NULL_HANDLE, &stagingMemory))
    NH_CHECK_VULKAN(Driver_p->Func.vkBindBufferMemory(Driver_p->Device, stagingBuffer, stagingMemory, 0))

    // copy texture data into staging buffer
    uint8_t *data;
    NH_CHECK_VULKAN(Driver_p->Func.vkMapMemory(Driver_p->Device, stagingMemory, 0, memReqs.size, 0, (void **)&data))
    memcpy(data, Info_p->data_p, Info_p->size);
    Driver_p->Func.vkUnmapMemory(Driver_p->Device, stagingMemory);	
    
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
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateImage(Driver_p->Device, &ImageInfo, VK_NULL_HANDLE, &Texture_p->Image))

    Driver_p->Func.vkGetImageMemoryRequirements(Driver_p->Device, Texture_p->Image, &memReqs);
    
    memAllocInfo.allocationSize = memReqs.size;
    Nh_Vk_findMemoryType(
        Host_p, Driver_p, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memAllocInfo.memoryTypeIndex
    );

    NH_CHECK_VULKAN(Driver_p->Func.vkAllocateMemory(Driver_p->Device, &memAllocInfo, VK_NULL_HANDLE, &Texture_p->DeviceMemory))
    NH_CHECK_VULKAN(Driver_p->Func.vkBindImageMemory(Driver_p->Device, Texture_p->Image, Texture_p->DeviceMemory, 0))
    
    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount   = ImageInfo.mipLevels;
    subresourceRange.layerCount   = 1;

    // copy
    VkCommandBufferBeginInfo cmdBufferInfo = {
	.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };
    Driver_p->Func.vkBeginCommandBuffer(copyCmd, &cmdBufferInfo);

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
        .subresourceRange    = subresourceRange,
    };

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    sourceStage      = VK_PIPELINE_STAGE_HOST_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    Driver_p->Func.vkCmdPipelineBarrier(
        copyCmd, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier1
    );

    VkBufferImageCopy bufferCopyRegion = {0};
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferCopyRegion.imageSubresource.layerCount = 1;
    bufferCopyRegion.imageExtent.width           = Texture_p->width;
    bufferCopyRegion.imageExtent.height          = Texture_p->height;
    bufferCopyRegion.imageExtent.depth           = 1;

    Driver_p->Func.vkCmdCopyBufferToImage(
        copyCmd, stagingBuffer, Texture_p->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ImageInfo.mipLevels, bufferCopyRegions_p
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
        .subresourceRange    = subresourceRange,
    };
    sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    Driver_p->Func.vkCmdPipelineBarrier(
        copyCmd, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier2
    );

    Driver_p->Func.vkEndCommandBuffer(copyCmd);

    VkSubmitInfo submitInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers    = &copyCmd
    };
    
    NH_CHECK_VULKAN(Driver_p->Func.vkQueueSubmit(Driver_p->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE))
    NH_CHECK_VULKAN(Driver_p->Func.vkQueueWaitIdle(Driver_p->GraphicsQueue))

    // clean up staging resources
    Driver_p->Func.vkFreeMemory(Driver_p->Device, stagingMemory, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyBuffer(Driver_p->Device, stagingBuffer, VK_NULL_HANDLE);

    // misc
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateSampler(Driver_p->Device, &SamplerInfo, VK_NULL_HANDLE, &Texture_p->Sampler))
    
    ImageViewInfo.image = Texture_p->Image;
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateImageView(Driver_p->Device, &ImageViewInfo, VK_NULL_HANDLE, &Texture_p->ImageView))
    
    Texture_p->DescriptorImageInfo.sampler     = Texture_p->Sampler;
    Texture_p->DescriptorImageInfo.imageView   = Texture_p->ImageView;
    Texture_p->DescriptorImageInfo.imageLayout = Info_p->imageLayout;

NH_END(NH_SUCCESS)
}

void Nh_Vk_destroyTexture(
    Nh_Vk_GPU *GPU_p, NH_Vk_Texture *Texture_p)
{
NH_BEGIN()
   
    GPU_p->Driver.Func.vkFreeMemory(GPU_p->Driver.Device, Texture_p->DeviceMemory, VK_NULL_HANDLE); 
    GPU_p->Driver.Func.vkDestroyImageView(GPU_p->Driver.Device, Texture_p->ImageView, VK_NULL_HANDLE);
    GPU_p->Driver.Func.vkDestroyImage(GPU_p->Driver.Device, Texture_p->Image, VK_NULL_HANDLE);
    GPU_p->Driver.Func.vkDestroySampler(GPU_p->Driver.Device, Texture_p->Sampler, VK_NULL_HANDLE);

    Nh_free(Texture_p);

NH_SILENT_END()
}

