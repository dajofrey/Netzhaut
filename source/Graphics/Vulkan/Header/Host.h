#ifndef NH_VK_HOST_H
#define NH_VK_HOST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../API/Header/Vulkan.h"
#include "../../../API/Header/Netzhaut.h"

#include <stdbool.h>

#endif

/** @addtogroup VulkanStructs Structs
 *  \ingroup Vulkan
 *  @{
 */

    typedef struct NH_Vk_Func {
        PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
        PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
        PFN_vkCreateInstance vkCreateInstance;
        PFN_vkDestroyInstance vkDestroyInstance;
        PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
        PFN_vkCreateDevice vkCreateDevice;
        PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
#ifdef __unix__
        PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR; 
#elif defined(_WIN32) || defined (WIN32)
        PFN_VkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
#endif
        PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
        PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
        PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
        PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
        PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
        PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
        PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
        PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
        PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
    } NH_Vk_Func;
    
    struct Nh_Vk_Host {
        bool validation;
        VkInstance Instance;                
        NH_Vk_Func Func;                    
        VkDebugUtilsMessengerEXT Messenger; 
    };

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createHost(
        bool validation
    );

    void Nh_Vk_destroyHost(
    );

/** @} */

#endif 
