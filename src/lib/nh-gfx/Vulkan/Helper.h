#ifndef NH_GFX_VULKAN_HELPER_H
#define NH_GFX_VULKAN_HELPER_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Driver.h"
#include "Host.h"

#include "../Common/Includes.h"

// ENUMS ===========================================================================================

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

// STRUCTS =========================================================================================

typedef struct nh_gfx_VulkanBufferInfo {
    VkBufferCreateInfo *Info_p;
    void *data_p;              
    bool mapMemory;           
    int memoryProperties;    
    bool createInfo;          
} nh_gfx_VulkanBufferInfo;

typedef struct nh_gfx_VulkanBuffer {
    int type;                                    
    int size;
    VkBuffer Buffer;                       
    VkDeviceMemory DeviceMemory;                 
    VkDescriptorBufferInfo DescriptorBufferInfo; 
} nh_gfx_VulkanBuffer;

typedef struct nh_gfx_VulkanDescriptorSet {
    int type;                           
    VkDescriptorSet DescriptorSet;      
    VkDescriptorSetLayout Layout;       
    struct nh_gfx_VulkanDescriptorSet *Next_p; 
} nh_gfx_VulkanDescriptorSet;

typedef struct nh_gfx_VulkanDeviceMemoryInfo{
    NH_VK_DEVICE_MEMORY deviceMemoryType;    
    VkMemoryPropertyFlags memoryProperties;
    void *bindSource_p;                    
} nh_gfx_VulkanDeviceMemoryInfo;

typedef struct nh_gfx_VulkanPipelineInfo {
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
} nh_gfx_VulkanPipelineInfo;

typedef struct nh_gfx_VulkanPipeline {
    VkPipeline Pipeline;            
    VkPipelineLayout PipelineLayout;
} nh_gfx_VulkanPipeline;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_createVulkanBuffer(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanBufferInfo *BufferInfo_p, nh_gfx_VulkanBuffer *Buffer_p
);

NH_API_RESULT nh_gfx_destroyVulkanBuffer(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanBuffer *Buffer_p
);

NH_API_RESULT nh_gfx_createVulkanDescriptorSet(
    nh_gfx_VulkanDriver *Driver_p, VkDescriptorSetAllocateInfo *DescriptorSetAllocateInfo_p, 
    VkDescriptorSetLayoutCreateInfo *DescriptorSetLayoutInfo_p, nh_gfx_VulkanDescriptorSet *DescriptorSet_p
);

NH_API_RESULT nh_gfx_destroyVulkanDescriptorSet(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanDescriptorSet *DescriptorSet_p, VkDescriptorPool *DescriptorPool_p
);

NH_API_RESULT nh_gfx_createVulkanDeviceMemory(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanDeviceMemoryInfo *Info_p, VkDeviceMemory *DeviceMemory_p
);

NH_API_RESULT nh_gfx_bindVulkanDeviceMemory(
    nh_gfx_VulkanDriver *Driver_p, VkDeviceMemory *DeviceMemory_p, nh_gfx_VulkanDeviceMemoryInfo *Info_p
);

NH_API_RESULT nh_gfx_mapVulkanDeviceMemory(
    nh_gfx_VulkanDriver *Driver_p, VkDeviceMemory *DeviceMemory_p, const void *data_p, 
    size_t bufferSize, size_t offset
);

NH_API_RESULT nh_gfx_findVulkanMemoryType(
    nh_gfx_VulkanDriver *Driver_p, int typeFilter, VkMemoryPropertyFlags properties, uint32_t *result_p
); 

NH_API_RESULT nh_gfx_createVulkanShaderModule(
    nh_gfx_VulkanDriver *Driver_p, const uint32_t *code, size_t size, VkShaderModule *ShaderModule_p
);

NH_API_RESULT nh_gfx_createVulkanPipeline(
    nh_gfx_VulkanDriver *Driver_p, NH_VK_PIPELINE PipelineType, VkPipelineLayoutCreateInfo *PipelineLayoutCreateInfo_p, 
    void *pipelineInfo_p, nh_gfx_VulkanPipeline *Pipeline_p
);

void nh_gfx_destroyVulkanPipeline(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipeline_p
);

void nh_gfx_prepareVulkanPipelineInfo(
    nh_gfx_VulkanPipelineInfo *Info_p
);

#endif // NH_GFX_VULKAN_HELPER_H
