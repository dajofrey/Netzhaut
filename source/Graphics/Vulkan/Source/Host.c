// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Host.h"
#include "../Header/Utils.h"
#include "../Header/Macros.h"
#include "../Header/Vulkan.h"

#include "../../Header/Log.h"

#include "../../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// DECLARE =========================================================================================

#define VALIDATION_LAYER "VK_LAYER_KHRONOS_validation"
#define VALIDATION_EXTENSION "VK_EXT_debug_utils"

static NH_RESULT Nh_Vk_createInstance(
    Nh_Vk_Host *Host_p, bool validation
);
static NH_RESULT Nh_Vk_createMessenger(
    Nh_Vk_Host *Host_p
);

static NH_BOOL Nh_Vk_validationLayerSupported(
    Nh_Vk_Host *Host_p
); 
static NH_BOOL Nh_Vk_validationExtensionSupported(
    Nh_Vk_Host *Host_p
); 

// HOST ============================================================================================

NH_RESULT Nh_Vk_createHost(
    bool validation)
{
NH_BEGIN()

    Nh_Vk_Host *Host_p = Nh_Vk_getHost();

    NH_CHECK_VULKAN(volkInitialize())
    
    Host_p->Func.vkCreateInstance                       = vkCreateInstance;
    Host_p->Func.vkEnumerateInstanceExtensionProperties = vkEnumerateInstanceExtensionProperties;
    Host_p->Func.vkEnumerateInstanceLayerProperties     = vkEnumerateInstanceLayerProperties;
   
    NH_CHECK(Nh_Vk_createInstance(Host_p, validation))

    Host_p->Func.vkDestroyInstance                   = vkDestroyInstance;
    Host_p->Func.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
    Host_p->Func.vkGetPhysicalDeviceFormatProperties = vkGetPhysicalDeviceFormatProperties;
    Host_p->Func.vkGetInstanceProcAddr               = vkGetInstanceProcAddr;
    Host_p->Func.vkCreateDevice                      = vkCreateDevice;
    Host_p->Func.vkGetPhysicalDeviceFeatures         = vkGetPhysicalDeviceFeatures;
#ifdef __unix__
    Host_p->Func.vkCreateXlibSurfaceKHR  = vkCreateXlibSurfaceKHR; 
#elif defined(_WIN32) || defined (WIN32)
    Host_p->Func.vkCreateWin32SurfaceKHR = vkCreateWin32SurfaceKHR;
#endif
    Host_p->Func.vkGetPhysicalDeviceSurfaceFormatsKHR      = vkGetPhysicalDeviceSurfaceFormatsKHR;
    Host_p->Func.vkGetPhysicalDeviceProperties             = vkGetPhysicalDeviceProperties;
    Host_p->Func.vkEnumeratePhysicalDevices                = vkEnumeratePhysicalDevices;
    Host_p->Func.vkGetPhysicalDeviceQueueFamilyProperties  = vkGetPhysicalDeviceQueueFamilyProperties;
    Host_p->Func.vkEnumerateDeviceExtensionProperties      = vkEnumerateDeviceExtensionProperties;
    Host_p->Func.vkDestroySurfaceKHR                       = vkDestroySurfaceKHR;
    Host_p->Func.vkGetPhysicalDeviceSurfaceSupportKHR      = vkGetPhysicalDeviceSurfaceSupportKHR;
    Host_p->Func.vkGetPhysicalDeviceSurfaceCapabilitiesKHR = vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    Host_p->Func.vkGetPhysicalDeviceSurfacePresentModesKHR = vkGetPhysicalDeviceSurfacePresentModesKHR;

    Host_p->validation = validation;

    if (validation) {
        Host_p->validation = Nh_Vk_createMessenger(Host_p) == NH_SUCCESS;
    }

NH_END(NH_SUCCESS)
}

void Nh_Vk_destroyHost()
{
NH_BEGIN()

    Nh_Vk_Host *Host_p = Nh_Vk_getHost();

    if (Host_p->validation) 
    {
        PFN_vkDestroyDebugUtilsMessengerEXT function = NULL;
        function = (PFN_vkDestroyDebugUtilsMessengerEXT) Host_p->Func.vkGetInstanceProcAddr(
            Host_p->Instance, "vkDestroyDebugUtilsMessengerEXT"
        );    
        function(Host_p->Instance, Host_p->Messenger, VK_NULL_HANDLE);
    }
    Host_p->Func.vkDestroyInstance(Host_p->Instance, VK_NULL_HANDLE);

NH_SILENT_END()
}

// CALLBACK ========================================================================================

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) 
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        char *message_p = Nh_allocate(sizeof(char) * (strlen(pCallbackData->pMessage) + 2));
        if (message_p != NULL) {
            strcpy(message_p, pCallbackData->pMessage);
            message_p[strlen(pCallbackData->pMessage)] = '\n';
            message_p[strlen(pCallbackData->pMessage) + 1] = '\0';
            Nh_Gfx_logVulkan(message_p);
        }
    }
    return VK_FALSE;
}

// INSTANCE ========================================================================================

static NH_RESULT Nh_Vk_createInstance(
    Nh_Vk_Host *Host_p, bool validation)
{
NH_BEGIN();

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
    const char* instanceExtensions_pp[3]  = {VALIDATION_EXTENSION, "VK_KHR_surface", "VK_KHR_xlib_surface"};
    const char* instanceExtensions2_pp[2] = {"VK_KHR_surface", "VK_KHR_xlib_surface"};
#elif defined(_WIN32) || defined (WIN32)
    const char* instanceExtensions_pp[3]  = {VALIDATION_EXTENSION, "VK_KHR_surface", "VK_KHR_win32_surface"};
    const char* instanceExtensions2_pp[2] = {"VK_KHR_surface", "VK_KHR_win32_surface"};
#endif

    VkInstanceCreateInfo InstanceInfo = 
    {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo        = &VkApplicationInfo,
        .enabledExtensionCount   = 3,
        .ppEnabledExtensionNames = instanceExtensions_pp,
        .enabledLayerCount       = 1,
        .ppEnabledLayerNames     = instanceLayers_pp
    };

    if ((Nh_Vk_validationLayerSupported(Host_p) == NH_FALSE) || !validation) {
        InstanceInfo.enabledLayerCount = 0;
    }
    
    if ((Nh_Vk_validationExtensionSupported(Host_p) == NH_FALSE) || !validation) {
        InstanceInfo.enabledExtensionCount   = 2;
        InstanceInfo.ppEnabledExtensionNames = instanceExtensions2_pp;
    }

    NH_CHECK_VULKAN(Host_p->Func.vkCreateInstance(&InstanceInfo, VK_NULL_HANDLE, &Host_p->Instance))
    
    volkLoadInstance(Host_p->Instance);

NH_END(NH_SUCCESS);
}

// MESSENGER =======================================================================================

static NH_RESULT Nh_Vk_createMessenger(
    Nh_Vk_Host* Host_p) 
{
NH_BEGIN();

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
   
    if (Nh_Vk_validationLayerSupported(Host_p) == NH_FALSE ||  Nh_Vk_validationExtensionSupported(Host_p) == NH_FALSE) {
        NH_END(NH_VULKAN_ERROR_VALIDATION_NOT_SUPPORTED)
    }

    PFN_vkCreateDebugUtilsMessengerEXT function = NULL;
    function = (PFN_vkCreateDebugUtilsMessengerEXT) Host_p->Func.vkGetInstanceProcAddr(
        Host_p->Instance, "vkCreateDebugUtilsMessengerEXT"
    );    

    if (function == NULL) {NH_END(NH_VULKAN_ERROR_LOADER_CANT_CREATE_DEBUG_MESSENGER)}

    function(Host_p->Instance, &Info, VK_NULL_HANDLE, &Host_p->Messenger); 

NH_END(NH_SUCCESS);
}

// VALIDATION LAYER ================================================================================

static NH_BOOL Nh_Vk_validationLayerSupported(
    Nh_Vk_Host *Host_p) 
{
NH_BEGIN();

    int availableLayerCount = 0;

    Host_p->Func.vkEnumerateInstanceLayerProperties(&availableLayerCount, VK_NULL_HANDLE);
    
    VkLayerProperties *availableLayers_p = Nh_allocate(sizeof(VkLayerProperties) * availableLayerCount);
    if (availableLayers_p == NULL) {NH_END(NH_FALSE)}

    Host_p->Func.vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers_p);

    for(int i = 0; i < availableLayerCount; ++i) {
        if (!strcmp(VALIDATION_LAYER, availableLayers_p[i].layerName)) {
            Nh_free(availableLayers_p); 
            NH_END(NH_TRUE)
        }
    }

    Nh_free(availableLayers_p);

NH_END(NH_FALSE);
}

// VALIDATION EXTENSION ============================================================================

static NH_BOOL Nh_Vk_validationExtensionSupported(
    Nh_Vk_Host *Host_p) 
{
NH_BEGIN()

    uint32_t propCount = 0; 

    Host_p->Func.vkEnumerateInstanceExtensionProperties(VALIDATION_LAYER, &propCount, VK_NULL_HANDLE);

    VkExtensionProperties *extensionProps_p = Nh_allocate(sizeof(VkExtensionProperties) * propCount);
    if (extensionProps_p == NULL) {NH_END(NH_FALSE)}

    Host_p->Func.vkEnumerateInstanceExtensionProperties(VALIDATION_LAYER, &propCount, extensionProps_p);

    for (unsigned int i = 0; i < propCount; ++i) {
        if (!strcmp(extensionProps_p[i].extensionName, VALIDATION_EXTENSION)) {
            Nh_free(extensionProps_p); 
            NH_END(NH_TRUE)
        }
    }

    Nh_free(extensionProps_p);

NH_END(NH_FALSE)
}

