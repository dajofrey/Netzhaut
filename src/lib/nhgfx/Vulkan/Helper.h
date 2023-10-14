#ifndef NH_VK_HELPER_H
#define NH_VK_HELPER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Driver.h"
#include "Host.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhgfx_enums 
 *  @{
 */

    /**
     * Different types of a VkDeviceMemory handle. Must be specified when
     * creating a device memory handle.
     */
    typedef enum NH_VK_DEVICE_MEMORY {
        NH_VK_DEVICE_MEMORY_IMAGE, /**<Indicates VkImage memory.*/
        NH_VK_DEVICE_MEMORY_BUFFER /**<Indicates VkBuffer memory.*/
    } NH_VK_DEVICE_MEMORY;

    typedef enum NH_VK_PIPELINE {
        NH_VK_PIPELINE_GRAPHICS,
        NH_VK_PIPELINE_COMPUTE,
    } NH_VK_PIPELINE;

/** @} */

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

   typedef struct nh_vk_BufferInfo {
        VkBufferCreateInfo *Info_p;
        void *data_p;              
        NH_BOOL mapMemory;           
        int memoryProperties;    
        NH_BOOL createInfo;          
    } nh_vk_BufferInfo;
    
   typedef struct nh_vk_Buffer {
        int type;                                    
        int size;
        VkBuffer Buffer;                       
        VkDeviceMemory DeviceMemory;                 
        VkDescriptorBufferInfo DescriptorBufferInfo; 
    } nh_vk_Buffer;

    typedef struct nh_vk_DescriptorSet {
        int type;                           
        VkDescriptorSet DescriptorSet;      
        VkDescriptorSetLayout Layout;       
        struct nh_vk_DescriptorSet *Next_p; 
    } nh_vk_DescriptorSet;

    typedef struct nh_vk_DeviceMemoryInfo{
        NH_VK_DEVICE_MEMORY deviceMemoryType;    
        VkMemoryPropertyFlags memoryProperties;
        void *bindSource_p;                    
    } nh_vk_DeviceMemoryInfo;

    typedef struct nh_vk_PipelineInfo {
        VkViewport Viewport;
        VkRect2D Scissor;
        VkDynamicState dynamic_p[2];
        VkPipelineViewportStateCreateInfo ViewportState;
        VkPipelineDynamicStateCreateInfo DynamicState;
        VkPipelineDepthStencilStateCreateInfo DepthStencilState;
        VkPipelineRasterizationStateCreateInfo RasterizationState;
        VkPipelineMultisampleStateCreateInfo MultisampleState;
        VkPipelineColorBlendStateCreateInfo ColorBlendState;
        VkPipelineColorBlendAttachmentState ColorBlendAttachment;
    } nh_vk_PipelineInfo;

    typedef struct nh_vk_Pipeline {
        VkPipeline Pipeline;            
        VkPipelineLayout PipelineLayout;
    } nh_vk_Pipeline;

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    NH_GFX_RESULT nh_vk_createBuffer(
        nh_vk_Driver *Driver_p, nh_vk_BufferInfo *BufferInfo_p, nh_vk_Buffer *Buffer_p
    );
    
    NH_GFX_RESULT nh_vk_destroyBuffer(
        nh_vk_Driver *Driver_p, nh_vk_Buffer *Buffer_p
    );

    NH_GFX_RESULT nh_vk_createDescriptorSet(
        nh_vk_Driver *Driver_p, VkDescriptorSetAllocateInfo *DescriptorSetAllocateInfo_p, 
        VkDescriptorSetLayoutCreateInfo *DescriptorSetLayoutInfo_p, nh_vk_DescriptorSet *DescriptorSet_p
    );
    
    NH_GFX_RESULT nh_vk_destroyDescriptorSet(
        nh_vk_Driver *Driver_p, nh_vk_DescriptorSet *DescriptorSet_p, VkDescriptorPool *DescriptorPool_p
    );
    
    NH_GFX_RESULT nh_vk_createDeviceMemory(
        nh_vk_Driver *Driver_p, nh_vk_DeviceMemoryInfo *Info_p, VkDeviceMemory *DeviceMemory_p
    );
    
    NH_GFX_RESULT nh_vk_bindDeviceMemory(
        nh_vk_Driver *Driver_p, VkDeviceMemory *DeviceMemory_p, nh_vk_DeviceMemoryInfo *Info_p
    );
    
    NH_GFX_RESULT nh_vk_mapDeviceMemory(
        nh_vk_Driver *Driver_p, VkDeviceMemory *DeviceMemory_p, const void *data_p, 
        size_t bufferSize, size_t offset
    );
    
    NH_GFX_RESULT nh_vk_findMemoryType(
        nh_vk_Driver *Driver_p, int typeFilter, VkMemoryPropertyFlags properties, uint32_t *result_p
    ); 

    NH_GFX_RESULT nh_vk_createShaderModule(
        nh_vk_Driver *Driver_p, const uint32_t *code, size_t size, VkShaderModule *ShaderModule_p
    );

    NH_GFX_RESULT nh_vk_createPipeline(
        nh_vk_Driver *Driver_p, NH_VK_PIPELINE PipelineType, VkPipelineLayoutCreateInfo *PipelineLayoutCreateInfo_p, 
        void *pipelineInfo_p, nh_vk_Pipeline *Pipeline_p
    );
    
    void nh_vk_destroyPipeline(
        nh_vk_Driver *Driver_p, nh_vk_Pipeline *Pipeline_p
    );

    void nh_vk_prepareDefaultPipelineInfo(
        nh_vk_PipelineInfo *Info_p
    );

/** @} */

#endif
