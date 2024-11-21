// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Host.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// DECLARE =========================================================================================

#define VALIDATION_LAYER "VK_LAYER_KHRONOS_validation"
#define VALIDATION_EXTENSION "VK_EXT_debug_utils"

static NH_API_RESULT nh_gfx_createVulkanInstance(
    nh_gfx_VulkanHost *Host_p, bool validation
);
static NH_API_RESULT nh_gfx_createVulkanMessenger(
    nh_gfx_VulkanHost *Host_p
);

static bool nh_gfx_vulkanValidationLayerSupported(
    nh_gfx_VulkanHost *Host_p
); 
static bool nh_gfx_vulkanValidationExtensionSupported(
    nh_gfx_VulkanHost *Host_p
); 

// HOST ============================================================================================

NH_API_RESULT nh_gfx_createVulkanHost(
    nh_gfx_VulkanHost *Host_p, bool validation)
{
    NH_GFX_CHECK_VULKAN(volkInitialize())

    Host_p->Functions.vkCreateInstance                       = vkCreateInstance;
    Host_p->Functions.vkEnumerateInstanceExtensionProperties = vkEnumerateInstanceExtensionProperties;
    Host_p->Functions.vkEnumerateInstanceLayerProperties     = vkEnumerateInstanceLayerProperties;
   
    NH_CORE_CHECK(nh_gfx_createVulkanInstance(Host_p, validation))

    Host_p->Functions.vkDestroyInstance                   = vkDestroyInstance;
    Host_p->Functions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
    Host_p->Functions.vkGetPhysicalDeviceFormatProperties = vkGetPhysicalDeviceFormatProperties;
    Host_p->Functions.vkGetInstanceProcAddr               = vkGetInstanceProcAddr;
    Host_p->Functions.vkCreateDevice                      = vkCreateDevice;
    Host_p->Functions.vkGetPhysicalDeviceFeatures         = vkGetPhysicalDeviceFeatures;
#ifdef __unix__
    Host_p->Functions.vkCreateXlibSurfaceKHR  = vkCreateXlibSurfaceKHR; 
#elif defined(_WIN32) || defined (WIN32)
    Host_p->Functions.vkCreateWin32SurfaceKHR = vkCreateWin32SurfaceKHR;
#endif
    Host_p->Functions.vkGetPhysicalDeviceSurfaceFormatsKHR      = vkGetPhysicalDeviceSurfaceFormatsKHR;
    Host_p->Functions.vkGetPhysicalDeviceProperties             = vkGetPhysicalDeviceProperties;
    Host_p->Functions.vkEnumeratePhysicalDevices                = vkEnumeratePhysicalDevices;
    Host_p->Functions.vkGetPhysicalDeviceQueueFamilyProperties  = vkGetPhysicalDeviceQueueFamilyProperties;
    Host_p->Functions.vkEnumerateDeviceExtensionProperties      = vkEnumerateDeviceExtensionProperties;
    Host_p->Functions.vkDestroySurfaceKHR                       = vkDestroySurfaceKHR;
    Host_p->Functions.vkGetPhysicalDeviceSurfaceSupportKHR      = vkGetPhysicalDeviceSurfaceSupportKHR;
    Host_p->Functions.vkGetPhysicalDeviceSurfaceCapabilitiesKHR = vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    Host_p->Functions.vkGetPhysicalDeviceSurfacePresentModesKHR = vkGetPhysicalDeviceSurfacePresentModesKHR;

    Host_p->validation = validation;

    if (validation) {
        Host_p->validation = nh_gfx_createVulkanMessenger(Host_p) == NH_API_SUCCESS;
    }

    return NH_API_SUCCESS;
}

void nh_gfx_destroyVulkanHost(
    nh_gfx_VulkanHost *Host_p)
{
    if (Host_p->validation) 
    {
        PFN_vkDestroyDebugUtilsMessengerEXT function = NULL;
        function = (PFN_vkDestroyDebugUtilsMessengerEXT) Host_p->Functions.vkGetInstanceProcAddr(
            Host_p->Instance, "vkDestroyDebugUtilsMessengerEXT"
        );    
        function(Host_p->Instance, Host_p->Messenger, VK_NULL_HANDLE);
    }
    Host_p->Functions.vkDestroyInstance(Host_p->Instance, VK_NULL_HANDLE);

    return;
}

// CALLBACK ========================================================================================

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) 
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        char *message_p = nh_core_allocate(sizeof(char) * (strlen(pCallbackData->pMessage) + 1));
        if (message_p) {
            sprintf(message_p, pCallbackData->pMessage);
            nh_gfx_logVulkanValidation(message_p);
            nh_core_free(message_p);
        }
    }
    return VK_FALSE;
}

// INSTANCE ========================================================================================

static NH_API_RESULT nh_gfx_createVulkanInstance(
    nh_gfx_VulkanHost *Host_p, bool validation)
{
    VkApplicationInfo VkApplicationInfo = 
    {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName   = "NULL",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 0),
        .pEngineName        = "NULL",
        .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion         = VK_API_VERSION_1_2
    };

    const char* instanceLayers_pp[1] = {VALIDATION_LAYER};
	
#ifdef __unix__
    const char* instanceExtensions_pp[]  = {VALIDATION_EXTENSION, "VK_KHR_surface", "VK_KHR_xlib_surface", "VK_KHR_portability_enumeration"};
    const char* instanceExtensions2_pp[] = {"VK_KHR_surface", "VK_KHR_xlib_surface", "VK_KHR_portability_enumeration"};
#elif defined(_WIN32) || defined (WIN32)
    const char* instanceExtensions_pp[]  = {VALIDATION_EXTENSION, "VK_KHR_surface", "VK_KHR_win32_surface"};
    const char* instanceExtensions2_pp[] = {"VK_KHR_surface", "VK_KHR_win32_surface"};
#endif

    VkInstanceCreateInfo InstanceInfo = 
    {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo        = &VkApplicationInfo,
        .enabledExtensionCount   = 4,
        .ppEnabledExtensionNames = instanceExtensions_pp,
        .enabledLayerCount       = 1,
        .ppEnabledLayerNames     = instanceLayers_pp,
        .flags                   = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
    };

    if ((nh_gfx_vulkanValidationLayerSupported(Host_p) == false) || !validation) {
        InstanceInfo.enabledLayerCount = 0;
    }
    
    if ((nh_gfx_vulkanValidationExtensionSupported(Host_p) == false) || !validation) {
        InstanceInfo.enabledExtensionCount   = 3;
        InstanceInfo.ppEnabledExtensionNames = instanceExtensions2_pp;
    }

    NH_GFX_CHECK_VULKAN(Host_p->Functions.vkCreateInstance(&InstanceInfo, VK_NULL_HANDLE, &Host_p->Instance))
    
    volkLoadInstanceOnly(Host_p->Instance);

    return NH_API_SUCCESS;;
}

// MESSENGER =======================================================================================

static NH_API_RESULT nh_gfx_createVulkanMessenger(
    nh_gfx_VulkanHost* Host_p) 
{
    VkDebugUtilsMessengerCreateInfoEXT Info = 
    {
        .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext           = VK_NULL_HANDLE,
        .flags           = 0,
        .messageSeverity = 
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
        .messageType     = 
          VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback,
        .pUserData       = VK_NULL_HANDLE
    };
   
    if (nh_gfx_vulkanValidationLayerSupported(Host_p) == false ||  nh_gfx_vulkanValidationExtensionSupported(Host_p) == false) {
        return NH_API_VULKAN_ERROR_VALIDATION_NOT_SUPPORTED;
    }

    PFN_vkCreateDebugUtilsMessengerEXT create_f;
    create_f = (PFN_vkCreateDebugUtilsMessengerEXT) Host_p->Functions.vkGetInstanceProcAddr(
        Host_p->Instance, "vkCreateDebugUtilsMessengerEXT"
    );    

    if (!create_f) {return NH_API_VULKAN_ERROR_LOADER_CANT_CREATE_DEBUG_MESSENGER;}

    create_f(Host_p->Instance, &Info, VK_NULL_HANDLE, &Host_p->Messenger); 

    return NH_API_SUCCESS;
}

// VALIDATION LAYER ================================================================================

static bool nh_gfx_vulkanValidationLayerSupported(
    nh_gfx_VulkanHost *Host_p) 
{
    int availableLayerCount = 0;

    Host_p->Functions.vkEnumerateInstanceLayerProperties(&availableLayerCount, VK_NULL_HANDLE);
    
    VkLayerProperties *availableLayers_p = nh_core_allocate(sizeof(VkLayerProperties) * availableLayerCount);
    if (availableLayers_p == NULL) {return false;}

    Host_p->Functions.vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers_p);

    for(int i = 0; i < availableLayerCount; ++i) {
        if (!strcmp(VALIDATION_LAYER, availableLayers_p[i].layerName)) {
            nh_core_free(availableLayers_p); 
            return true;
        }
    }

    nh_core_free(availableLayers_p);

    return false;
}

// VALIDATION EXTENSION ============================================================================

static bool nh_gfx_vulkanValidationExtensionSupported(
    nh_gfx_VulkanHost *Host_p) 
{
    uint32_t propCount = 0; 

    Host_p->Functions.vkEnumerateInstanceExtensionProperties(VALIDATION_LAYER, &propCount, VK_NULL_HANDLE);

    VkExtensionProperties *extensionProps_p = nh_core_allocate(sizeof(VkExtensionProperties) * propCount);
    if (extensionProps_p == NULL) {return false;}

    Host_p->Functions.vkEnumerateInstanceExtensionProperties(VALIDATION_LAYER, &propCount, extensionProps_p);

    for (unsigned int i = 0; i < propCount; ++i) {
        if (!strcmp(extensionProps_p[i].extensionName, VALIDATION_EXTENSION)) {
            nh_core_free(extensionProps_p); 
            return true;
        }
    }

    nh_core_free(extensionProps_p);

    return false;
}

