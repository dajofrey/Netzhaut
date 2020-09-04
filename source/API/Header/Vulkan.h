#ifndef NH_VULKAN_H
#define NH_VULKAN_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

#ifdef __cplusplus
    #define EXTERNC extern "C"
#else
    #define EXTERNC
#endif

#ifndef VOLK_VULKAN_H_PATH
    #define VOLK_VULKAN_H_PATH "vulkan.h"
#endif

#include "Netzhaut.h"
#include "External/volk.h"

#ifdef DOXYGEN_JS_API

/** \defgroup CVulkan Vulkan
 *  \ingroup C
 *  \brief C Vulkan module 
 *
 *  This is a graphics interface using the Vulkan API which lets you render custom stuff,
 *  overriding what Netzhaut would normally render. The interface can be accessed by including the 
 *  Netzhaut/Vulkan.h header.
 */

/** @addtogroup CVulkanEnums Enums
 *  \ingroup CVulkan
 *  @{
 */ 

#else

/** \defgroup APIVulkan Vulkan
 *  \ingroup API
 *  \brief API Vulkan module
 *
 *  Vulkan interface for users.
 */

/** @addtogroup APIVulkanEnums Enums
 *  \ingroup APIVulkan
 *  @{
 */ 

#endif

    /**
     * Different types of a VkDeviceMemory handle. Must be specified when
     * creating a device memory handle.
     */
    typedef enum NH_VK_DEVICE_MEMORY {
        NH_VK_DEVICE_MEMORY_IMAGE, /**<Indicates VkImage memory.*/
        NH_VK_DEVICE_MEMORY_BUFFER /**<Indicates VkBuffer memory.*/
    } NH_VK_DEVICE_MEMORY;
    
    /**
     * Different types of a VkPipeline handle. Must be specified when
     * creating a pipeline and the info struct must match accordingly.
     */
    typedef enum NH_VK_PIPELINE {
        NH_VK_PIPELINE_GRAPHICS, /*<Indicates a graphics pipeline.*/
        NH_VK_PIPELINE_COMPUTE,  /*<Indicates a compute pipeline.*/
    } NH_VK_PIPELINE;

/** @} */

#ifdef DOXYGEN_JS_API

/** @addtogroup CVulkanTypes Types
 *  \ingroup CVulkan
 *  @{
 */ 

#else

/** @addtogroup APIVulkanTypes Types
 *  \ingroup APIVulkan
 *  @{
 */ 

#endif

   typedef struct Nh_Vk_Host Nh_Vk_Host;

   typedef struct Nh_Vk_GPU Nh_Vk_GPU;

   typedef struct Nh_Vk_Driver Nh_Vk_Driver;

/** @} */

#ifdef DOXYGEN_JS_API

/** @addtogroup CVulkanStructs Structs
 *  \ingroup CVulkan
 *  @{
 */ 

#else

/** @addtogroup APIVulkanStructs Structs
 *  \ingroup APIVulkan
 *  @{
 */ 

#endif

    typedef struct Nh_Vk_DriverInfo {
        uint32_t major;    
        uint32_t minor;    
        uint32_t patch;    
        uint32_t vendorId; 
        uint32_t deviceId; 
    } Nh_Vk_DriverInfo;
    
   typedef struct NH_Vk_BufferInfo {
        VkBufferCreateInfo *Info_p;
        void *data_p;              
        NH_BOOL mapMemory;           
        int memoryProperties;    
        NH_BOOL createInfo;          
    } NH_Vk_BufferInfo;
    
   typedef struct NH_Vk_Buffer {
        int type;                                    
        int size;
        VkBuffer Buffer;                       
        VkDeviceMemory DeviceMemory;                 
        VkDescriptorBufferInfo DescriptorBufferInfo; 
    } NH_Vk_Buffer;
    
   typedef struct NH_Vk_DescriptorSet {
        int type;                           
        VkDescriptorSet DescriptorSet;      
        VkDescriptorSetLayout Layout;       
        struct NH_Vk_DescriptorSet *Next_p; 
    } NH_Vk_DescriptorSet;
    
   typedef struct NH_Vk_DeviceMemoryInfo{
        NH_VK_DEVICE_MEMORY deviceMemoryType;    
        VkMemoryPropertyFlags memoryProperties;
        void *bindSource_p;                    
    } NH_Vk_DeviceMemoryInfo;
    
   typedef struct NH_Vk_Pipeline {
        VkPipeline Pipeline;            
        VkPipelineLayout PipelineLayout;
    } NH_Vk_Pipeline;
    
   typedef struct NH_Vk_TextureInfo {
        unsigned int width;           
        unsigned int height;          
        int memoryProperties;         
        void *data_p;                 
        size_t size;                  
        VkImageLayout imageLayout;    
        VkFormat format;              
        VkImageUsageFlags usageFlags; 
    } NH_Vk_TextureInfo;
    
   typedef struct NH_Vk_Texture {
        int width;                                                  
        int height;                                
        VkImage Image;                             
        VkImageView ImageView;                     
        VkDeviceMemory DeviceMemory;               
        VkDescriptorImageInfo DescriptorImageInfo; 
        VkSampler Sampler;                         
    } NH_Vk_Texture;
    
/** @} */

#ifdef DOXYGEN_JS_API

/** @addtogroup CVulkanFunctions Functions
 *  \ingroup CVulkan
 *  @{
 */ 

#else

/** @addtogroup APIVulkanFunctions Functions
 *  \ingroup APIVulkan
 *  @{
 */ 

#endif

    EXTERNC NH_RESULT Nh_Vk_createBuffer(
        Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, NH_Vk_BufferInfo *BufferInfo_p, 
        NH_Vk_Buffer *Buffer_p
    );
    
    EXTERNC NH_RESULT Nh_Vk_destroyBuffer(
        Nh_Vk_Driver *Driver_p, NH_Vk_Buffer *Buffer_p
    );
    
    EXTERNC NH_RESULT Nh_Vk_createDescriptorSet(
        Nh_Vk_Driver *Driver_p, VkDescriptorSetAllocateInfo *DescriptorSetAllocateInfo_p, 
        VkDescriptorSetLayoutCreateInfo *DescriptorSetLayoutInfo_p, NH_Vk_DescriptorSet *DescriptorSet_p
    );
    
    EXTERNC NH_RESULT Nh_Vk_destroyDescriptorSet(
        Nh_Vk_Driver *Driver_p, NH_Vk_DescriptorSet *DescriptorSet_p, VkDescriptorPool *DescriptorPool_p
    );
    
    EXTERNC NH_RESULT Nh_Vk_createDeviceMemory(
        Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, NH_Vk_DeviceMemoryInfo *Info_p, 
        VkDeviceMemory *DeviceMemory_p
    );
    
    EXTERNC NH_RESULT Nh_Vk_bindDeviceMemory(
        Nh_Vk_Driver *Driver_p, VkDeviceMemory *DeviceMemory_p, NH_Vk_DeviceMemoryInfo *Info_p
    );
    
    EXTERNC NH_RESULT Nh_Vk_mapDeviceMemory(
        Nh_Vk_Driver *Driver_p, VkDeviceMemory *DeviceMemory_p, const void *data_p, 
        size_t bufferSize, size_t offset
    );
    
    EXTERNC NH_RESULT Nh_Vk_createPipeline(
        Nh_Vk_Driver *Driver_p, NH_VK_PIPELINE PipelineType, VkPipelineLayoutCreateInfo *PipelineLayoutCreateInfo_p, 
        void *pipelineInfo_p, NH_Vk_Pipeline *Pipeline_p
    );
    
    EXTERNC void Nh_Vk_destroyPipeline(
        Nh_Vk_Driver *Driver_p, NH_Vk_Pipeline *Pipeline_p
    );
    
    EXTERNC NH_RESULT Nh_Vk_createShaderModule(
        Nh_Vk_Driver *Driver_p, const uint32_t *code, size_t size, VkShaderModule *ShaderModule_p
    );
   
    EXTERNC NH_RESULT Nh_Vk_loadTexture(
        Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, NH_Vk_TextureInfo *TextureInfo_p, 
        NH_Vk_Texture *Texture_p, int threadId
    );
    
    EXTERNC void Nh_Vk_destroyTexture(
        Nh_Vk_GPU *GPU_p, NH_Vk_Texture *Texture_p
    );


    EXTERNC Nh_Vk_Host *Nh_Vk_getHost(
    );

    EXTERNC int Nh_Vk_getDriver(
        Nh_Vk_Driver **Driver_pp
    );


//    EXTERNC NH_RESULT Nh_setVulkanCallback(
//        Nh_Tab *Tab_p, char *elementId_p, NH_RESULT (*callback_p)(struct VolkDeviceTable, VkCommandBuffer*)
//    );

    EXTERNC NH_RESULT Nh_Vk_findMemoryType(
        Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, int typeFilter, VkMemoryPropertyFlags properties, 
        uint32_t *result_p
    ); 

/** @} */

#undef EXTERNC

#endif // NH_VULKAN_H
