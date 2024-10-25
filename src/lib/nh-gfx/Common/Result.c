// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"
#include "../Vulkan/Vulkan.h"

// RESULTS =========================================================================================

const char *NH_API_RESULTS_PP[] = 
{
    "NH_API_SUCCESS",                    
    "NH_API_ERROR_NULL_POINTER",         
    "NH_API_ERROR_BAD_STATE", 
    "NH_API_ERROR_PARAMETERS", 
    "NH_API_ERROR_MEMORY_ALLOCATION",
    "NH_API_ERROR_INVALID_FILE_FORMAT",
    "NH_API_ERROR_RESIZE_NEEDED",

    "NH_API_VULKAN_ERROR_VALIDATION_NOT_SUPPORTED",
    "NH_API_VULKAN_ERROR_LOADER_CANT_CREATE_DEBUG_MESSENGER",
    "NH_API_VULKAN_ERROR_NO_CORRESPONDING_RESULT",
    "NH_API_VULKAN_ERROR_DEVICE_NOT_SUITABLE",
    "NH_API_VULKAN_ERROR_HOST_CREATION_FAILED",
    "NH_API_VULKAN_ERROR_NO_DEVICE_SUITABLE",
    "NH_API_VULKAN_NOT_READY",
    "NH_API_VULKAN_TIMEOUT",
    "NH_API_VULKAN_EVENT_SET",
    "NH_API_VULKAN_EVENT_RESET",
    "NH_API_VULKAN_INCOMPLETE",
    "NH_API_VULKAN_ERROR_OUT_OF_HOST_MEMORY",
    "NH_API_VULKAN_ERROR_OUT_OF_DEVICE_MEMORY",
    "NH_API_VULKAN_ERROR_INITIALIZATION_FAILED",
    "NH_API_VULKAN_ERROR_DEVICE_LOST",
    "NH_API_VULKAN_ERROR_MEMORY_MAP_FAILED",
    "NH_API_VULKAN_ERROR_LAYER_NOT_PRESENT",
    "NH_API_VULKAN_ERROR_EXTENSION_NOT_PRESENT",
    "NH_API_VULKAN_ERROR_FEATURE_NOT_PRESENT",
    "NH_API_VULKAN_ERROR_INCOMPATIBLE_DRIVER",
    "NH_API_VULKAN_ERROR_TOO_MANY_OBJECTS",
    "NH_API_VULKAN_ERROR_FORMAT_NOT_SUPPORTED",
    "NH_API_VULKAN_ERROR_FRAGMENTED_POOL",
    "NH_API_VULKAN_ERROR_UNKNOWN",
    "NH_API_VULKAN_ERROR_OUT_OF_POOL_MEMORY",
    "NH_API_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE",
    "NH_API_VULKAN_ERROR_FRAGMENTATION",
    "NH_API_VULKAN_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS",
    "NH_API_VULKAN_ERROR_SURFACE_LOST_KHR",
    "NH_API_VULKAN_ERROR_NATIVE_WINDOW_IN_USE_KHR",
    "NH_API_VULKAN_SUBOPTIMAL_KHR",
    "NH_API_VULKAN_ERROR_OUT_OF_DATE_KHR",
    "NH_API_VULKAN_ERROR_INCOMPATIBLE_DISPLAY_KHR",
    "NH_API_VULKAN_ERROR_VALIDATION_FAILED_EXT",
    "NH_API_VULKAN_ERROR_INVALID_SHADER_NV",
    "NH_API_VULKAN_ERROR_INCOMPATIBLE_VERSION_KHR",
    "NH_API_VULKAN_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT",
    "NH_API_VULKAN_ERROR_NOT_PERMITTED_EXT",
    "NH_API_VULKAN_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT",
    "NH_API_VULKAN_THREAD_IDLE_KHR",
    "NH_API_VULKAN_THREAD_DONE_KHR",
    "NH_API_VULKAN_OPERATION_DEFERRED_KHR",
    "NH_API_VULKAN_OPERATION_NOT_DEFERRED_KHR",
    "NH_API_VULKAN_PIPELINE_COMPILE_REQUIRED_EXT",
    "NH_API_VULKAN_ERROR_OUT_OF_POOL_MEMORY_KHR",
    "NH_API_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE_KHR",
    "NH_API_VULKAN_RESULT_MAX_ENUM",
};

unsigned int NH_API_RESULTS_PP_COUNT = sizeof(NH_API_RESULTS_PP) / sizeof(NH_API_RESULTS_PP[0]);

// GET VULKAN RESULT ===============================================================================

NH_API_RESULT nh_vk_getResult(
    int result)
{
    switch (result)
    {
         case VK_SUCCESS                                            : return NH_API_SUCCESS;
         case VK_NOT_READY                                          : return NH_API_VULKAN_NOT_READY;
         case VK_TIMEOUT                                            : return NH_API_VULKAN_TIMEOUT;
         case VK_EVENT_SET                                          : return NH_API_VULKAN_EVENT_SET;
         case VK_EVENT_RESET                                        : return NH_API_VULKAN_EVENT_RESET;
         case VK_INCOMPLETE                                         : return NH_API_VULKAN_INCOMPLETE;
         case VK_ERROR_OUT_OF_HOST_MEMORY                           : return NH_API_VULKAN_ERROR_OUT_OF_HOST_MEMORY;
         case VK_ERROR_OUT_OF_DEVICE_MEMORY                         : return NH_API_VULKAN_ERROR_OUT_OF_DEVICE_MEMORY;
         case VK_ERROR_INITIALIZATION_FAILED                        : return NH_API_VULKAN_ERROR_INITIALIZATION_FAILED;
         case VK_ERROR_DEVICE_LOST                                  : return NH_API_VULKAN_ERROR_DEVICE_LOST;
         case VK_ERROR_MEMORY_MAP_FAILED                            : return NH_API_VULKAN_ERROR_MEMORY_MAP_FAILED;
         case VK_ERROR_LAYER_NOT_PRESENT                            : return NH_API_VULKAN_ERROR_LAYER_NOT_PRESENT;
         case VK_ERROR_EXTENSION_NOT_PRESENT                        : return NH_API_VULKAN_ERROR_EXTENSION_NOT_PRESENT;
         case VK_ERROR_FEATURE_NOT_PRESENT                          : return NH_API_VULKAN_ERROR_FEATURE_NOT_PRESENT;
         case VK_ERROR_INCOMPATIBLE_DRIVER                          : return NH_API_VULKAN_ERROR_INCOMPATIBLE_DRIVER;
         case VK_ERROR_TOO_MANY_OBJECTS                             : return NH_API_VULKAN_ERROR_TOO_MANY_OBJECTS;
         case VK_ERROR_FORMAT_NOT_SUPPORTED                         : return NH_API_VULKAN_ERROR_FORMAT_NOT_SUPPORTED;
         case VK_ERROR_FRAGMENTED_POOL                              : return NH_API_VULKAN_ERROR_FRAGMENTED_POOL;
         case VK_ERROR_UNKNOWN                                      : return NH_API_VULKAN_ERROR_UNKNOWN;
         case VK_ERROR_OUT_OF_POOL_MEMORY                           : return NH_API_VULKAN_ERROR_OUT_OF_POOL_MEMORY;
         case VK_ERROR_INVALID_EXTERNAL_HANDLE                      : return NH_API_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE;
         case VK_ERROR_FRAGMENTATION                                : return NH_API_VULKAN_ERROR_FRAGMENTATION;
         case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS               : return NH_API_VULKAN_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS;
         case VK_ERROR_SURFACE_LOST_KHR                             : return NH_API_VULKAN_ERROR_SURFACE_LOST_KHR;
         case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR                     : return NH_API_VULKAN_ERROR_NATIVE_WINDOW_IN_USE_KHR;
         case VK_SUBOPTIMAL_KHR                                     : return NH_API_VULKAN_SUBOPTIMAL_KHR;
         case VK_ERROR_OUT_OF_DATE_KHR                              : return NH_API_VULKAN_ERROR_OUT_OF_DATE_KHR;
         case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR                     : return NH_API_VULKAN_ERROR_INCOMPATIBLE_DISPLAY_KHR;
         case VK_ERROR_VALIDATION_FAILED_EXT                        : return NH_API_VULKAN_ERROR_VALIDATION_FAILED_EXT;
         case VK_ERROR_INVALID_SHADER_NV                            : return NH_API_VULKAN_ERROR_INVALID_SHADER_NV;
//         case VK_ERROR_INCOMPATIBLE_VERSION_KHR                     : NH_GFX_DIAGNOSTIC_END(NH_API_VULKAN_ERROR_INCOMPATIBLE_VERSION_KHR)
         case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT : return NH_API_VULKAN_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT;
         case VK_ERROR_NOT_PERMITTED_EXT                            : return NH_API_VULKAN_ERROR_NOT_PERMITTED_EXT;
         case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT          : return NH_API_VULKAN_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT;
         case VK_THREAD_IDLE_KHR                                    : return NH_API_VULKAN_THREAD_IDLE_KHR;
         case VK_THREAD_DONE_KHR                                    : return NH_API_VULKAN_THREAD_DONE_KHR;
         case VK_OPERATION_DEFERRED_KHR                             : return NH_API_VULKAN_OPERATION_DEFERRED_KHR;
         case VK_OPERATION_NOT_DEFERRED_KHR                         : return NH_API_VULKAN_OPERATION_NOT_DEFERRED_KHR;
         case VK_PIPELINE_COMPILE_REQUIRED_EXT                      : return NH_API_VULKAN_PIPELINE_COMPILE_REQUIRED_EXT;
         case VK_RESULT_MAX_ENUM                                    : return NH_API_VULKAN_RESULT_MAX_ENUM;
         default                                                    : return NH_API_VULKAN_ERROR_NO_CORRESPONDING_RESULT;
    }

    return -1;
}

