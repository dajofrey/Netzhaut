// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Utils.h"
#include "../Header/Host.h"
#include "../Header/Driver.h"
#include "../Header/Macros.h"

#include "../../../API/Header/Vulkan.h"
#include "../../../API/Header/Netzhaut.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// BUFFER ==========================================================================================

NH_RESULT Nh_Vk_createBuffer(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, NH_Vk_BufferInfo *BufferInfo_p, 
    NH_Vk_Buffer *Buffer_p)
{
NH_BEGIN()

    NH_CHECK_VULKAN(Driver_p->Func.vkCreateBuffer(Driver_p->Device, BufferInfo_p->Info_p, VK_NULL_HANDLE, &Buffer_p->Buffer))

    NH_Vk_DeviceMemoryInfo Info = 
    {
        .deviceMemoryType = NH_VK_DEVICE_MEMORY_BUFFER,
        .memoryProperties = BufferInfo_p->memoryProperties,  
        .bindSource_p     = &Buffer_p->Buffer
    };

    NH_CHECK(Nh_Vk_createDeviceMemory(Host_p, Driver_p, &Info, &Buffer_p->DeviceMemory))

    if (BufferInfo_p->mapMemory) { 
        Nh_Vk_mapDeviceMemory(
            Driver_p, &Buffer_p->DeviceMemory, BufferInfo_p->data_p, 
            (size_t) BufferInfo_p->Info_p->size, 0
        );
    }
    
    Nh_Vk_bindDeviceMemory(Driver_p, &Buffer_p->DeviceMemory, &Info);

    if (BufferInfo_p->createInfo) { 
        Buffer_p->DescriptorBufferInfo.buffer = Buffer_p->Buffer;
        Buffer_p->DescriptorBufferInfo.offset = 0;
        Buffer_p->DescriptorBufferInfo.range  = VK_WHOLE_SIZE;
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_destroyBuffer(
    Nh_Vk_Driver *Driver_p, NH_Vk_Buffer *Buffer_p)
{
NH_BEGIN();

    Driver_p->Func.vkDestroyBuffer(Driver_p->Device, Buffer_p->Buffer, VK_NULL_HANDLE);
    Driver_p->Func.vkFreeMemory(Driver_p->Device, Buffer_p->DeviceMemory, VK_NULL_HANDLE); 

NH_END(NH_SUCCESS);
}

// DESCRIPTOR SET ==================================================================================

NH_RESULT Nh_Vk_createDescriptorSet(
    Nh_Vk_Driver *Driver_p, VkDescriptorSetAllocateInfo *DescriptorSetAllocateInfo_p, VkDescriptorSetLayoutCreateInfo *DescriptorSetLayoutInfo_p, 
    NH_Vk_DescriptorSet *DescriptorSet_p)
{
NH_BEGIN()

    NH_CHECK_VULKAN(Driver_p->Func.vkCreateDescriptorSetLayout(Driver_p->Device, DescriptorSetLayoutInfo_p, VK_NULL_HANDLE, &DescriptorSet_p->Layout))

    DescriptorSetAllocateInfo_p->pSetLayouts = &DescriptorSet_p->Layout;
    DescriptorSetAllocateInfo_p->descriptorSetCount = 1;

    NH_CHECK_VULKAN(Driver_p->Func.vkAllocateDescriptorSets(Driver_p->Device, DescriptorSetAllocateInfo_p, &DescriptorSet_p->DescriptorSet))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_destroyDescriptorSet(
    Nh_Vk_Driver *Driver_p, NH_Vk_DescriptorSet *DescriptorSet_p, VkDescriptorPool *DescriptorPool_p)
{
NH_BEGIN()

    Driver_p->Func.vkDestroyDescriptorSetLayout(
        Driver_p->Device, DescriptorSet_p->Layout, VK_NULL_HANDLE
    );
    Driver_p->Func.vkFreeDescriptorSets(
        Driver_p->Device, *DescriptorPool_p, 1, &DescriptorSet_p->DescriptorSet
    );

NH_END(NH_SUCCESS)
}

// MEMORY ==========================================================================================

NH_RESULT Nh_Vk_createDeviceMemory(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, NH_Vk_DeviceMemoryInfo *Info_p, 
    VkDeviceMemory *DeviceMemory_p)
{
NH_BEGIN()

    VkMemoryRequirements memoryRequirements;
    if (Info_p->deviceMemoryType == NH_VK_DEVICE_MEMORY_IMAGE)
    {
        VkImage *VkImage_p = (VkImage*) Info_p->bindSource_p;
        Driver_p->Func.vkGetImageMemoryRequirements(
            Driver_p->Device, 
            *VkImage_p, 
            &memoryRequirements
        );
    }
    else if (Info_p->deviceMemoryType == NH_VK_DEVICE_MEMORY_BUFFER)
    {
        VkBuffer *VkBuffer_p = (VkBuffer*) Info_p->bindSource_p;
        Driver_p->Func.vkGetBufferMemoryRequirements(
            Driver_p->Device, 
            *VkBuffer_p, 
            &memoryRequirements
         );
    }
    else {NH_END(NH_ERROR_BAD_STATE)}
    
    int i;
    Nh_Vk_findMemoryType(
        Host_p, Driver_p, memoryRequirements.memoryTypeBits, Info_p->memoryProperties, &i
    );
    
    VkMemoryAllocateInfo memoryInfo = 
    { 
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = VK_NULL_HANDLE,
        .allocationSize  = memoryRequirements.size,
        .memoryTypeIndex = i
    };
   
#define CREATION_FAILED Driver_p->Func.vkAllocateMemory(Driver_p->Device, &memoryInfo, VK_NULL_HANDLE, DeviceMemory_p) != VK_SUCCESS 
    if (CREATION_FAILED) {
        NH_END(NH_ERROR_BAD_STATE)
    }

NH_END(NH_SUCCESS);
}

NH_RESULT Nh_Vk_bindDeviceMemory(
    Nh_Vk_Driver *Driver_p, VkDeviceMemory *DeviceMemory_p, NH_Vk_DeviceMemoryInfo *Info_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Info_p->bindSource_p) 

    if (Info_p->deviceMemoryType == NH_VK_DEVICE_MEMORY_IMAGE)
    { 
        VkImage *VkImage_p = (VkImage*) Info_p->bindSource_p;
        Driver_p->Func.vkBindImageMemory(
            Driver_p->Device, 
            *VkImage_p, 
            *DeviceMemory_p, 
            0
        ); 
    }    
    else if (Info_p->deviceMemoryType == NH_VK_DEVICE_MEMORY_BUFFER)
    {
        VkBuffer *VkBuffer_p = (VkBuffer*) Info_p->bindSource_p;
        Driver_p->Func.vkBindBufferMemory(
            Driver_p->Device, 
            *VkBuffer_p, 
            *DeviceMemory_p, 
            0
        ); 
    }
    
NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Vk_mapDeviceMemory(
    Nh_Vk_Driver *Driver_p, VkDeviceMemory *DeviceMemory_p, const void *data_p, size_t bufferSize, 
    size_t offset)
{
NH_BEGIN();
    
    if (bufferSize <= 0) {NH_END(NH_ERROR_PARAMETERS)}

    void *memory_p;
    Driver_p->Func.vkMapMemory(Driver_p->Device, *DeviceMemory_p, offset, bufferSize, 0, &memory_p); 
    memcpy(memory_p, data_p, bufferSize);
    Driver_p->Func.vkUnmapMemory(Driver_p->Device, *DeviceMemory_p);

NH_END(NH_SUCCESS);
}

NH_RESULT Nh_Vk_findMemoryType(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, int typeFilter, VkMemoryPropertyFlags properties, 
    uint32_t *result_p) 
{
NH_BEGIN();

    *result_p = -1;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    
    Host_p->Func.vkGetPhysicalDeviceMemoryProperties(Driver_p->PhysicalDevice, &memoryProperties);
    
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) 
    { 
        if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            *result_p = i;
        }
    }

NH_END(NH_SUCCESS);
}

// SHADER MODULE ===================================================================================

NH_RESULT Nh_Vk_createShaderModule(
    Nh_Vk_Driver *Driver_p, const uint32_t *code, size_t size, VkShaderModule *ShaderModule_p)
{
NH_BEGIN()

    VkShaderModuleCreateInfo shaderCreateInfo = 
    {
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext    = VK_NULL_HANDLE,
        .flags    = 0,
        .codeSize = size,
        .pCode    = code,
    };
    Driver_p->Func.vkCreateShaderModule(Driver_p->Device, &shaderCreateInfo, VK_NULL_HANDLE, ShaderModule_p);

NH_END(NH_SUCCESS)
}

// PIPELINE ========================================================================================

NH_RESULT Nh_Vk_createPipeline(
    Nh_Vk_Driver *Driver_p, NH_VK_PIPELINE PipelineType, VkPipelineLayoutCreateInfo *PipelineLayoutCreateInfo_p, 
    void *pipelineInfo_p, NH_Vk_Pipeline *Pipeline_p)
{
NH_BEGIN()

#undef LAYOUT_CREATION_FAILED
#define LAYOUT_CREATION_FAILED Driver_p->Func.vkCreatePipelineLayout(Driver_p->Device, PipelineLayoutCreateInfo_p, VK_NULL_HANDLE, &Pipeline_p->PipelineLayout) != VK_SUCCESS
    if (LAYOUT_CREATION_FAILED) {
        NH_END(NH_ERROR_BAD_STATE)
    }
   
    if (PipelineType == NH_VK_PIPELINE_GRAPHICS) 
    {
        VkGraphicsPipelineCreateInfo *info_p = (VkGraphicsPipelineCreateInfo*) pipelineInfo_p;
        info_p->layout = Pipeline_p->PipelineLayout;
        if (Driver_p->Func.vkCreateGraphicsPipelines(Driver_p->Device, VK_NULL_HANDLE, 1, info_p, 
            VK_NULL_HANDLE, &Pipeline_p->Pipeline) != VK_SUCCESS) 
        {
            NH_END(NH_ERROR_BAD_STATE)
        }
    }

    if (PipelineType == NH_VK_PIPELINE_COMPUTE) 
    {
        VkComputePipelineCreateInfo *info_p = (VkComputePipelineCreateInfo*) pipelineInfo_p;
        info_p->layout = Pipeline_p->PipelineLayout;
        if (Driver_p->Func.vkCreateComputePipelines(Driver_p->Device, VK_NULL_HANDLE, 1, info_p, 
            VK_NULL_HANDLE, &Pipeline_p->Pipeline) != VK_SUCCESS) 
        {
            NH_END(NH_ERROR_BAD_STATE)
        }
    }

NH_END(NH_SUCCESS)
}

void Nh_Vk_destroyPipeline(
    Nh_Vk_Driver *Driver_p, NH_Vk_Pipeline *Pipeline_p)
{
NH_BEGIN()

    Driver_p->Func.vkDestroyPipeline(Driver_p->Device, Pipeline_p->Pipeline, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyPipelineLayout(Driver_p->Device, Pipeline_p->PipelineLayout, VK_NULL_HANDLE);
 
NH_SILENT_END()
}

// RESULT ==========================================================================================

NH_RESULT Nh_Vk_getResult(
    VkResult result)
{
NH_BEGIN()

    switch (result)
    {
         case VK_SUCCESS                                            : NH_END(NH_SUCCESS)
         case VK_NOT_READY                                          : NH_END(NH_VULKAN_NOT_READY)
         case VK_TIMEOUT                                            : NH_END(NH_VULKAN_TIMEOUT)
         case VK_EVENT_SET                                          : NH_END(NH_VULKAN_EVENT_SET)
         case VK_EVENT_RESET                                        : NH_END(NH_VULKAN_EVENT_RESET)
         case VK_INCOMPLETE                                         : NH_END(NH_VULKAN_INCOMPLETE)
         case VK_ERROR_OUT_OF_HOST_MEMORY                           : NH_END(NH_VULKAN_ERROR_OUT_OF_HOST_MEMORY)
         case VK_ERROR_OUT_OF_DEVICE_MEMORY                         : NH_END(NH_VULKAN_ERROR_OUT_OF_DEVICE_MEMORY)
         case VK_ERROR_INITIALIZATION_FAILED                        : NH_END(NH_VULKAN_ERROR_INITIALIZATION_FAILED)
         case VK_ERROR_DEVICE_LOST                                  : NH_END(NH_VULKAN_ERROR_DEVICE_LOST)
         case VK_ERROR_MEMORY_MAP_FAILED                            : NH_END(NH_VULKAN_ERROR_MEMORY_MAP_FAILED)
         case VK_ERROR_LAYER_NOT_PRESENT                            : NH_END(NH_VULKAN_ERROR_LAYER_NOT_PRESENT)
         case VK_ERROR_EXTENSION_NOT_PRESENT                        : NH_END(NH_VULKAN_ERROR_EXTENSION_NOT_PRESENT)
         case VK_ERROR_FEATURE_NOT_PRESENT                          : NH_END(NH_VULKAN_ERROR_FEATURE_NOT_PRESENT)
         case VK_ERROR_INCOMPATIBLE_DRIVER                          : NH_END(NH_VULKAN_ERROR_INCOMPATIBLE_DRIVER)
         case VK_ERROR_TOO_MANY_OBJECTS                             : NH_END(NH_VULKAN_ERROR_TOO_MANY_OBJECTS)
         case VK_ERROR_FORMAT_NOT_SUPPORTED                         : NH_END(NH_VULKAN_ERROR_FORMAT_NOT_SUPPORTED)
         case VK_ERROR_FRAGMENTED_POOL                              : NH_END(NH_VULKAN_ERROR_FRAGMENTED_POOL)
         case VK_ERROR_UNKNOWN                                      : NH_END(NH_VULKAN_ERROR_UNKNOWN)
         case VK_ERROR_OUT_OF_POOL_MEMORY                           : NH_END(NH_VULKAN_ERROR_OUT_OF_POOL_MEMORY)
         case VK_ERROR_INVALID_EXTERNAL_HANDLE                      : NH_END(NH_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE)
         case VK_ERROR_FRAGMENTATION                                : NH_END(NH_VULKAN_ERROR_FRAGMENTATION)
         case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS               : NH_END(NH_VULKAN_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS)
         case VK_ERROR_SURFACE_LOST_KHR                             : NH_END(NH_VULKAN_ERROR_SURFACE_LOST_KHR)
         case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR                     : NH_END(NH_VULKAN_ERROR_NATIVE_WINDOW_IN_USE_KHR)
         case VK_SUBOPTIMAL_KHR                                     : NH_END(NH_VULKAN_SUBOPTIMAL_KHR)
         case VK_ERROR_OUT_OF_DATE_KHR                              : NH_END(NH_VULKAN_ERROR_OUT_OF_DATE_KHR)
         case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR                     : NH_END(NH_VULKAN_ERROR_INCOMPATIBLE_DISPLAY_KHR)
         case VK_ERROR_VALIDATION_FAILED_EXT                        : NH_END(NH_VULKAN_ERROR_VALIDATION_FAILED_EXT)
         case VK_ERROR_INVALID_SHADER_NV                            : NH_END(NH_VULKAN_ERROR_INVALID_SHADER_NV)
         case VK_ERROR_INCOMPATIBLE_VERSION_KHR                     : NH_END(NH_VULKAN_ERROR_INCOMPATIBLE_VERSION_KHR)
         case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT : NH_END(NH_VULKAN_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT)
         case VK_ERROR_NOT_PERMITTED_EXT                            : NH_END(NH_VULKAN_ERROR_NOT_PERMITTED_EXT)
         case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT          : NH_END(NH_VULKAN_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT)
         case VK_THREAD_IDLE_KHR                                    : NH_END(NH_VULKAN_THREAD_IDLE_KHR)
         case VK_THREAD_DONE_KHR                                    : NH_END(NH_VULKAN_THREAD_DONE_KHR)
         case VK_OPERATION_DEFERRED_KHR                             : NH_END(NH_VULKAN_OPERATION_DEFERRED_KHR)
         case VK_OPERATION_NOT_DEFERRED_KHR                         : NH_END(NH_VULKAN_OPERATION_NOT_DEFERRED_KHR)
         case VK_PIPELINE_COMPILE_REQUIRED_EXT                      : NH_END(NH_VULKAN_PIPELINE_COMPILE_REQUIRED_EXT)
         case VK_RESULT_MAX_ENUM                                    : NH_END(NH_VULKAN_RESULT_MAX_ENUM)
         default                                                    : NH_END(NH_VULKAN_ERROR_NO_CORRESPONDING_RESULT)
    }

NH_END(-1)
}

