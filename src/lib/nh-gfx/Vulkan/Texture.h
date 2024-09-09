#ifndef NH_VK_TEXTURE_H
#define NH_VK_TEXTURE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "GPU.h"
#include "../Fonts/FontManager.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

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

/** @} */

/** @addtogroup lib_nh-gfx_functions 
 *  @{
 */

    nh_vk_Texture *nh_vk_loadFontTexture(
        nh_vk_GPU *GPU_p, nh_gfx_Font *Font_p
    );

    nh_vk_Texture *nh_vk_reloadFontTexture(
        nh_vk_GPU *GPU_p, nh_gfx_Font *Font_p
    );

    void nh_vk_destroyTexture(
        nh_vk_GPU *GPU_p, nh_vk_Texture *Texture_p
    );

/** @} */

#endif
