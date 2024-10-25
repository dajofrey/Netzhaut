#ifndef NH_GFX_VULKAN_TEXTURE_H
#define NH_GFX_VULKAN_TEXTURE_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "GPU.h"
#include "../Fonts/FontManager.h"

// STRUCTS =========================================================================================

typedef struct nh_vk_TextureInfo {
    unsigned int width;           
    unsigned int height;          
    int memoryProperties;         
    void *data_p;                 
    size_t size;                  
    VkImageLayout imageLayout;    
    VkFormat format;              
    VkImageUsageFlags usageFlags; 
} nh_vk_TextureInfo;

typedef struct nh_vk_Texture {
    void *source_p;
    unsigned int width;                                                  
    unsigned int height;                                
    VkImage Image;                             
    VkImageView ImageView;                     
    VkDeviceMemory DeviceMemory;
    VkDescriptorImageInfo DescriptorImageInfo; 
    VkSampler Sampler;                         
} nh_vk_Texture;

// FUNCTIONS =======================================================================================

nh_vk_Texture *nh_vk_loadFontTexture(
    nh_gfx_VulkanGPU *GPU_p, nh_gfx_Font *Font_p
);

nh_vk_Texture *nh_vk_reloadFontTexture(
    nh_gfx_VulkanGPU *GPU_p, nh_gfx_Font *Font_p
);

void nh_vk_destroyTexture(
    nh_gfx_VulkanGPU *GPU_p, nh_vk_Texture *Texture_p
);

#endif // NH_GFX_VULKAN_TEXTURE_H
