#ifndef NH_GFX_VULKAN_HOST_H
#define NH_GFX_VULKAN_HOST_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

// STRUCTS =========================================================================================

typedef struct nh_gfx_VulkanHostFunctions {
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
} nh_gfx_VulkanHostFunctions;

typedef struct nh_gfx_VulkanHost {
    bool validation;
    VkInstance Instance;
    nh_gfx_VulkanHostFunctions Functions;
    VkDebugUtilsMessengerEXT Messenger;
} nh_gfx_VulkanHost;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_createVulkanHost(
    nh_gfx_VulkanHost *Host_p, bool validation
);

void nh_gfx_destroyVulkanHost(
    nh_gfx_VulkanHost *Host_p
);

#endif // NH_GFX_VULKAN_HOST_H 
