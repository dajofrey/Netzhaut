#ifndef NH_VK_HOST_H
#define NH_VK_HOST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#define NETZHAUT_VULKAN
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

    typedef struct nh_vk_HostFunctions {
        PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
        PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
        PFN_vkCreateInstance vkCreateInstance;
        PFN_vkDestroyInstance vkDestroyInstance;
        PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
        PFN_vkCreateDevice vkCreateDevice;
        PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
#ifdef VK_USE_PLATFORM_XLIB_KHR
        PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR; 
#elif VK_USE_PLATFORM_WIN32_KHR 
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
    } nh_vk_HostFunctions;
    
    typedef struct nh_vk_Host {
        NH_BOOL validation;
        VkInstance Instance;
        nh_vk_HostFunctions Functions;
        VkDebugUtilsMessengerEXT Messenger;
    } nh_vk_Host;

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    NH_GFX_RESULT nh_vk_createHost(
        nh_vk_Host *Host_p, NH_BOOL validation
    );

    void nh_vk_destroyHost(
        nh_vk_Host *Host_p
    );

/** @} */

#endif 
