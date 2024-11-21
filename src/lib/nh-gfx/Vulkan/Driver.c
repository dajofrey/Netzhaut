// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Driver.h"
#include "GPU.h"
#include "Host.h"
#include "Vulkan.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Thread.h"
#include "../../nh-core/System/Memory.h"

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN32) || defined (WIN32)
    #include <windows.h>
#endif

// DECLARE =========================================================================================

static NH_API_RESULT nh_gfx_createVulkanPhysicalDevice(
    nh_gfx_VulkanHost *Host, nh_gfx_VulkanDriver *Driver_p, char *name_p
);
static NH_API_RESULT nh_gfx_createVulkanDevice(
    nh_gfx_VulkanHost *Host, nh_gfx_VulkanDriver *Driver_p, uint32_t *graphicsQueue_p, uint32_t *computeQueue_p
);
static NH_API_RESULT nh_gfx_createVulkanDescriptorPool(
    nh_gfx_VulkanDriver *Driver_p
);
static NH_API_RESULT nh_gfx_createVulkanCommandPools(
    nh_gfx_VulkanDriver *Driver_p, uint32_t graphicsQueue, uint32_t computeQueue
);
static NH_API_RESULT nh_gfx_createVulkanRenderPass(
    nh_gfx_VulkanHost *Host, nh_gfx_VulkanDriver *Driver_p
);

static NH_API_RESULT nh_gfx_getVulkanSurfaceFormat(
    nh_gfx_VulkanHost *Host, nh_gfx_VulkanDriver *Driver_p, VkSurfaceFormatKHR *surfaceFormat_p
);
static bool nh_gfx_vulkanDeviceIsSuitable(
    nh_gfx_VulkanHost *Host_p, VkPhysicalDevice Device
);
static bool nh_gfx_vulkanDeviceSupportsExtensions(
    nh_gfx_VulkanHost *Host_p, VkPhysicalDevice Device
);
static uint32_t nh_gfx_getVulkanQueueFamily(
    nh_gfx_VulkanHost *Host, nh_gfx_VulkanDriver *Driver_p, VkQueueFlagBits queueFlag
);
static NH_API_RESULT nh_gfx_chooseVulkanSwapSurfaceFormat(
    VkSurfaceFormatKHR availableFormats[], VkSurfaceFormatKHR *surfaceFormat_p
);

#if defined(_WIN32) || defined (WIN32)
LRESULT CALLBACK WindowProc(
    NHND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam
);
#endif

// VALIDATE ========================================================================================

static NH_API_RESULT nh_gfx_validateVulkanFunctions(
    nh_gfx_VulkanDriver *Driver_p)
{
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateFence)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateSemaphore)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkAllocateCommandBuffers)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyFence)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroySemaphore)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyFramebuffer)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkFreeMemory)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyImageView)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyImage)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroySwapchainKHR)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateSwapchainKHR)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkGetSwapchainImagesKHR)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateImageView)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateImage)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkBeginCommandBuffer)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdPipelineBarrier)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkEndCommandBuffer)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkQueueSubmit)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkQueueWaitIdle)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkFreeCommandBuffers)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateFramebuffer)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyDescriptorPool)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyCommandPool)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyRenderPass)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyDevice)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkGetDeviceQueue)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateDescriptorPool)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateCommandPool)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateRenderPass)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkUpdateDescriptorSets)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateBuffer)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyBuffer)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkFreeMemory)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateDescriptorSetLayout)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkAllocateDescriptorSets)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyDescriptorSetLayout)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkFreeDescriptorSets)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkGetImageMemoryRequirements)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkGetBufferMemoryRequirements)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkAllocateMemory)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkBindImageMemory)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkBindBufferMemory)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkMapMemory)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkUnmapMemory)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateShaderModule)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreatePipelineLayout)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateGraphicsPipelines)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateComputePipelines)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyPipeline)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyPipelineLayout)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkAcquireNextImageKHR)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkBeginCommandBuffer)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdPipelineBarrier)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdClearColorImage)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkAllocateCommandBuffers)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkGetBufferMemoryRequirements)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdPipelineBarrier)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdCopyBufferToImage)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkQueueSubmit)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkQueueWaitIdle)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCreateSampler)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdBeginRenderPass)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdSetViewport)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdSetScissor)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdClearAttachments)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdEndRenderPass)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkResetCommandBuffer)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdBindPipeline)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdBindDescriptorSets)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdBindVertexBuffers)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkCmdDraw)
    NH_CORE_CHECK_NULL(Driver_p->Functions.vkDestroyShaderModule)

    return NH_API_SUCCESS;
}

// IMPLEMENT =======================================================================================

NH_API_RESULT nh_gfx_createVulkanDriver(
    nh_gfx_VulkanHost *Host_p, nh_gfx_VulkanDriver *Driver_p, char *name_p)
{
    // allocate
    Driver_p->DescriptorPool_p = nh_core_allocate(sizeof(VkDescriptorPool));
    NH_CORE_CHECK_MEM(Driver_p->DescriptorPool_p);
    Driver_p->descriptorPoolCount = 1;

    Driver_p->RenderPass_p = nh_core_allocate(sizeof(VkRenderPass));
    NH_CORE_CHECK_MEM(Driver_p->RenderPass_p);
    Driver_p->renderPassCount = 1;

    Driver_p->ComputeCommandPools_p = nh_core_allocate(sizeof(VkCommandPool) * NH_MAX_THREADS);
    NH_CORE_CHECK_MEM(Driver_p->ComputeCommandPools_p);
    Driver_p->GraphicsCommandPools_p = nh_core_allocate(sizeof(VkCommandPool) * NH_MAX_THREADS);
    NH_CORE_CHECK_MEM(Driver_p->GraphicsCommandPools_p);

    // create
    uint32_t graphicsQueue = 0, computeQueue = 0;

    NH_CORE_CHECK(nh_gfx_createVulkanPhysicalDevice(Host_p, Driver_p, name_p))
    NH_CORE_CHECK(nh_gfx_createVulkanDevice(Host_p, Driver_p, &graphicsQueue, &computeQueue))
    NH_CORE_CHECK(nh_gfx_createVulkanDescriptorPool(Driver_p))
    NH_CORE_CHECK(nh_gfx_createVulkanCommandPools(Driver_p, graphicsQueue, computeQueue))
    NH_CORE_CHECK(nh_gfx_createVulkanRenderPass(Host_p, Driver_p))

    return NH_API_SUCCESS;;
}

void nh_gfx_destroyVulkanDriver(
    nh_gfx_VulkanDriver *Driver_p)
{
    Driver_p->Functions.vkDestroyDescriptorPool(Driver_p->Device, Driver_p->DescriptorPool_p[0], VK_NULL_HANDLE);

    for (int i = 0; i < NH_MAX_THREADS; ++i) {
        Driver_p->Functions.vkDestroyCommandPool(
            Driver_p->Device, Driver_p->GraphicsCommandPools_p[i], VK_NULL_HANDLE);
        Driver_p->Functions.vkDestroyCommandPool(
            Driver_p->Device, Driver_p->ComputeCommandPools_p[i], VK_NULL_HANDLE);
    }

    Driver_p->Functions.vkDestroyRenderPass(Driver_p->Device, Driver_p->RenderPass_p[0], VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyDevice(Driver_p->Device, VK_NULL_HANDLE);

    nh_core_free(Driver_p->DescriptorPool_p);
    nh_core_free(Driver_p->RenderPass_p);
    nh_core_free(Driver_p->ComputeCommandPools_p);
    nh_core_free(Driver_p->GraphicsCommandPools_p);

    return;
}

// HELPER ==========================================================================================

static NH_API_RESULT nh_gfx_createVulkanPhysicalDevice(
    nh_gfx_VulkanHost *Host_p, nh_gfx_VulkanDriver *Driver_p, char *name_p)
{
    VkInstance *Window_p = &Host_p->Instance;
    VkPhysicalDevice *Device_p = NULL;

    // get physical device
    uint32_t deviceCount;
    Host_p->Functions.vkEnumeratePhysicalDevices(*Window_p, &deviceCount, VK_NULL_HANDLE); 

    VkPhysicalDevice *Physical_p = nh_core_allocate(sizeof(VkPhysicalDevice) * deviceCount);
    NH_CORE_CHECK_MEM(Physical_p)
    
    Host_p->Functions.vkEnumeratePhysicalDevices(*Window_p, &deviceCount, Physical_p);

    for (int i = 0; i < deviceCount; ++i) 
    {
        VkPhysicalDeviceProperties Properties;
        Host_p->Functions.vkGetPhysicalDeviceProperties(Physical_p[i], &Properties);
        
        if (!strcmp(Properties.deviceName, name_p)) {Device_p = &Physical_p[i];}
    }

    NH_CORE_CHECK_NULL(Device_p)

    // add if suitable
    if (nh_gfx_vulkanDeviceIsSuitable(Host_p, *Device_p))
    {        
        Driver_p->PhysicalDevice = *Device_p;

        VkPhysicalDeviceFeatures Features;
        Host_p->Functions.vkGetPhysicalDeviceFeatures(*Device_p, &Features);
          
        VkPhysicalDeviceProperties Properties;
        Host_p->Functions.vkGetPhysicalDeviceProperties(*Device_p, &Properties);

        Driver_p->Info.major    = VK_VERSION_MAJOR(Properties.apiVersion);
        Driver_p->Info.minor    = VK_VERSION_MINOR(Properties.apiVersion);
        Driver_p->Info.patch    = VK_VERSION_PATCH(Properties.apiVersion);
        Driver_p->Info.vendorId = Properties.vendorID;
        Driver_p->Info.deviceId = Properties.deviceID;
    } 
    else {return NH_API_VULKAN_ERROR_DEVICE_NOT_SUITABLE;}

    nh_core_free(Physical_p);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_createVulkanDevice(
    nh_gfx_VulkanHost *Host_p, nh_gfx_VulkanDriver *Driver_p, uint32_t *graphicsQueue_p, uint32_t *computeQueue_p)
{
    *graphicsQueue_p = nh_gfx_getVulkanQueueFamily(Host_p, Driver_p, VK_QUEUE_GRAPHICS_BIT);
    *computeQueue_p  = nh_gfx_getVulkanQueueFamily(Host_p, Driver_p, VK_QUEUE_COMPUTE_BIT);

    if (*graphicsQueue_p == -1 || *computeQueue_p == -1) {return NH_API_ERROR_BAD_STATE;}

    int queueCount = *graphicsQueue_p == *computeQueue_p ? 1 : 2;
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo *QueueInfos_p = nh_core_allocate(sizeof(VkDeviceQueueCreateInfo) * queueCount);
    NH_CORE_CHECK_MEM(QueueInfos_p)

    for (int i = 0; i < queueCount; ++i)
    {
        QueueInfos_p[i].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        QueueInfos_p[i].pNext            = VK_NULL_HANDLE;
        QueueInfos_p[i].flags            = 0;
        QueueInfos_p[i].queueFamilyIndex = i == 0 ? *graphicsQueue_p : *computeQueue_p;
        QueueInfos_p[i].queueCount       = 1;
        QueueInfos_p[i].pQueuePriorities = &queuePriority;
    }

    VkPhysicalDeviceFeatures DeviceFeatures;
    Host_p->Functions.vkGetPhysicalDeviceFeatures(Driver_p->PhysicalDevice, &DeviceFeatures);

    const char* deviceExtensions_pp[1] = {"VK_KHR_swapchain"};

    VkDeviceCreateInfo DeviceInfo = 
    {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = VK_NULL_HANDLE,
        .queueCreateInfoCount    = queueCount,
        .pQueueCreateInfos       = QueueInfos_p,
        .pEnabledFeatures        = &DeviceFeatures,
        .enabledExtensionCount   = 1,
        .ppEnabledExtensionNames = deviceExtensions_pp,
        .enabledLayerCount       = 0
    };
    NH_GFX_CHECK_VULKAN(
        Host_p->Functions.vkCreateDevice(Driver_p->PhysicalDevice, &DeviceInfo, VK_NULL_HANDLE, &Driver_p->Device))

    volkLoadDeviceTable(&Driver_p->Functions, Driver_p->Device);
    NH_CORE_CHECK(nh_gfx_validateVulkanFunctions(Driver_p))

    for (unsigned int i = 0; i < queueCount; ++i) {
        Driver_p->Functions.vkGetDeviceQueue(
            Driver_p->Device, i == 0 ? 
            *graphicsQueue_p : *computeQueue_p, 0, i == 0 ? &Driver_p->GraphicsQueue : &Driver_p->ComputeQueue
        );
    }

    nh_core_free(QueueInfos_p);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_createVulkanDescriptorPool(
    nh_gfx_VulkanDriver *Driver_p)
{
    VkDescriptorPoolSize PoolSizes_p[] =
    {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100000 }, // TODO hardcoded... 
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 10000 }
    };

    VkDescriptorPoolCreateInfo PoolInfo = 
    {
        .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext         = NULL,
        .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .poolSizeCount = 2,
        .pPoolSizes    = PoolSizes_p,
        .maxSets       = 110000
    };

    NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkCreateDescriptorPool(
        Driver_p->Device, &PoolInfo, VK_NULL_HANDLE, &(Driver_p->DescriptorPool_p[0])
    ))

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_createVulkanCommandPools(
    nh_gfx_VulkanDriver *Driver_p, uint32_t graphicsQueue, uint32_t computeQueue)
{
    for (int i = 0; i < NH_MAX_THREADS; ++i)
    {
        VkCommandPoolCreateInfo CommandPoolInfo = 
        {    
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .queueFamilyIndex = graphicsQueue,
            .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        };
        NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkCreateCommandPool(Driver_p->Device, &CommandPoolInfo, VK_NULL_HANDLE, &Driver_p->GraphicsCommandPools_p[i]))

        CommandPoolInfo.queueFamilyIndex = computeQueue;
        NH_GFX_CHECK_VULKAN(Driver_p->Functions.vkCreateCommandPool(Driver_p->Device, &CommandPoolInfo, VK_NULL_HANDLE, &Driver_p->ComputeCommandPools_p[i]))
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_getVulkanSurfaceFormat(
    nh_gfx_VulkanHost *Host_p, nh_gfx_VulkanDriver *Driver_p, VkSurfaceFormatKHR *surfaceFormat_p)
{
    VkInstance *Window_p = &Host_p->Instance;
    VkPhysicalDevice *Physical_p = &Driver_p->PhysicalDevice;

    VkSurfaceKHR Surface;

#ifdef __unix__

    Display *display = XOpenDisplay(NULL);
    NH_CORE_CHECK_NULL(display) 
  
    int screen = DefaultScreen(display);
    VkXlibSurfaceCreateInfoKHR SurfaceInfo = 
    { 
        .sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .pNext  = VK_NULL_HANDLE,
        .dpy    = display,
        .window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 100, 100, 
                    1, BlackPixel(display, screen), WhitePixel(display, screen))
    };

    Host_p->Functions.vkCreateXlibSurfaceKHR(*Window_p, &SurfaceInfo, VK_NULL_HANDLE, &Surface); 

#elif defined(_WIN32) || defined (WIN32)

    HINSTANCE hinstance = (HINSTANCE) GetModuleHandle(NULL);
    
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";    
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hinstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    NHND hwnd = CreateWindowEx(
        0, 
        CLASS_NAME,
        L"test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, 
        NULL, 
        hinstance,
        NULL 
    );

    NH_CORE_CHECK_NULL(hwnd)

    VkWin32SurfaceCreateInfoKHR surfaceInfo =
    {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = VK_NULL_HANDLE,
        .flags = 0,
        .hinstance = hinstance,
        .hwnd = hwnd, 

    };

    Host_p->Functions.vkCreateWin32SurfaceKHR(*Window_p, &surfaceInfo, VK_NULL_HANDLE, &Surface);

#endif
   
    uint32_t formatCount;
    Host_p->Functions.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Physical_p, Surface, &formatCount, VK_NULL_HANDLE
    );

    VkSurfaceFormatKHR *SurfaceFormats_p = nh_core_allocate(sizeof(VkSurfaceFormatKHR) * formatCount);
    NH_CORE_CHECK_MEM(SurfaceFormats_p)

    Host_p->Functions.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Physical_p, Surface, &formatCount, SurfaceFormats_p
    ); 
    Host_p->Functions.vkDestroySurfaceKHR(*Window_p, Surface, VK_NULL_HANDLE);

#ifdef __unix__
    XCloseDisplay(display);
#elif defined(_WIN32) || defined (WIN32)
    DestroyWindow(hwnd);
    UnregisterClassA(CLASS_NAME, hinstance);
#endif

    nh_gfx_chooseVulkanSwapSurfaceFormat(SurfaceFormats_p, surfaceFormat_p);

    nh_core_free(SurfaceFormats_p);

    return NH_API_SUCCESS;
}

static bool nh_gfx_vulkanDeviceIsSuitable(
    nh_gfx_VulkanHost *Host_p, VkPhysicalDevice Device)
{
    uint32_t queueFamilyCount = 0;
    Host_p->Functions.vkGetPhysicalDeviceQueueFamilyProperties(Device, &queueFamilyCount, VK_NULL_HANDLE);

    VkQueueFamilyProperties *queueFamilies_p = 
        nh_core_allocate(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    NH_CORE_CHECK_NULL_2(false, queueFamilies_p)

    Host_p->Functions.vkGetPhysicalDeviceQueueFamilyProperties(Device, &queueFamilyCount, queueFamilies_p);
   
#define GRAPHICS 0
#define COMPUTE 1 

    int familyTypes_p[2] = {0, 0};

    for (int i = 0; i < queueFamilyCount; ++i) 
    {
        if (queueFamilies_p[i].queueCount > 0 &&  queueFamilies_p[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            familyTypes_p[GRAPHICS] = 1;  
        }
        if (queueFamilies_p[i].queueCount > 0 &&  queueFamilies_p[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            familyTypes_p[COMPUTE] = 1;  
        }
    }
    
    bool queueFamiliesSupported = familyTypes_p[GRAPHICS] != 0 && familyTypes_p[COMPUTE] != 0;
    
    nh_core_free(queueFamilies_p);

#undef GRAPHICS
#undef COMPUTE

    return nh_gfx_vulkanDeviceSupportsExtensions(Host_p, Device) && queueFamiliesSupported;
}

static bool nh_gfx_vulkanDeviceSupportsExtensions(
    nh_gfx_VulkanHost *Host_p, VkPhysicalDevice Device) 
{
    const char* requiredExtensions_p = {"VK_KHR_swapchain"};
    uint32_t requiredExtensionsCount = 1;

    uint32_t availableExtensionsCount = 0;
    Host_p->Functions.vkEnumerateDeviceExtensionProperties(
        Device, VK_NULL_HANDLE, &availableExtensionsCount, VK_NULL_HANDLE
    ); 

    VkExtensionProperties *AvailableExtensions_p = 
        nh_core_allocate(sizeof(VkExtensionProperties) * availableExtensionsCount);
    NH_CORE_CHECK_NULL_2(false, AvailableExtensions_p)

    Host_p->Functions.vkEnumerateDeviceExtensionProperties(
        Device, VK_NULL_HANDLE, &availableExtensionsCount, AvailableExtensions_p
    ); 
    
    for (unsigned int i = 0; i < requiredExtensionsCount; ++i)
    {
        bool requiredExtensionAvailable = false;
        for (unsigned int j = 0; j < availableExtensionsCount; ++j) {
            if (!strcmp(&requiredExtensions_p[i], AvailableExtensions_p[j].extensionName)) {
                requiredExtensionAvailable = true;
            }
        }
        if (requiredExtensionAvailable) 
        {
            nh_core_free(AvailableExtensions_p);
            return true;
        }
    }

    nh_core_free(AvailableExtensions_p);

    return false;
}

static uint32_t nh_gfx_getVulkanQueueFamily(
    nh_gfx_VulkanHost *Host_p, nh_gfx_VulkanDriver *Driver_p, VkQueueFlagBits queueFlag) 
{   
    VkInstance *Window_p = &Host_p->Instance;
    VkPhysicalDevice *Physical_p = &Driver_p->PhysicalDevice;

    uint32_t queueFamilyCount = 0;
    Host_p->Functions.vkGetPhysicalDeviceQueueFamilyProperties(*Physical_p, &queueFamilyCount, VK_NULL_HANDLE);
    
    if (queueFamilyCount == 0) {return -1;}

    VkQueueFamilyProperties *QueueFamilyProperties_p = 
        nh_core_allocate(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    NH_CORE_CHECK_NULL_2(-1, QueueFamilyProperties_p)

    Host_p->Functions.vkGetPhysicalDeviceQueueFamilyProperties(
        *Physical_p, &queueFamilyCount, QueueFamilyProperties_p);

    for (uint32_t i = 0; i < queueFamilyCount; ++i) 
    {
        if (QueueFamilyProperties_p[i].queueCount <= 0 && !(QueueFamilyProperties_p[i].queueFlags & queueFlag)) {
            continue;
        }
        if (queueFlag == VK_QUEUE_COMPUTE_BIT)
        {
            nh_core_free(QueueFamilyProperties_p); 
            return i;;
        }
        if (queueFlag == VK_QUEUE_GRAPHICS_BIT)
        {
            VkBool32 presentSupport = VK_FALSE;
            VkSurfaceKHR testSurface;

#ifdef __unix__

            Display *display = XOpenDisplay(NULL);
            NH_CORE_CHECK_NULL_2(-1, display)

            int screen = DefaultScreen(display);
            VkXlibSurfaceCreateInfoKHR SurfaceInfo = 
            { 
                .sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
                .pNext  = VK_NULL_HANDLE,
                .dpy    = display,
                .window = XCreateSimpleWindow(display, RootWindow(display, screen), 
                        10, 10, 100, 100, 1,BlackPixel(display, screen), WhitePixel(display, screen))
            };

            Host_p->Functions.vkCreateXlibSurfaceKHR(*Window_p, &SurfaceInfo, VK_NULL_HANDLE, &testSurface); 
            Host_p->Functions.vkGetPhysicalDeviceSurfaceSupportKHR(*Physical_p, i, testSurface, &presentSupport); 
            Host_p->Functions.vkDestroySurfaceKHR(*Window_p, testSurface, VK_NULL_HANDLE);

            XCloseDisplay(display);

#elif defined(_WIN32) || defined (WIN32)

            HINSTANCE hinstance = (HINSTANCE) GetModuleHandle(NULL);

            const wchar_t CLASS_NAME[]  = L"Sample Window Class";    
            WNDCLASS wc = {0};
            wc.lpfnWndProc   = WindowProc;
            wc.hInstance     = hinstance;
            wc.lpszClassName = CLASS_NAME;
            RegisterClass(&wc);

            NHND hwnd = CreateWindowEx(
                0,
                CLASS_NAME,
                L"test",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                NULL,
                NULL,
                hinstance,
                NULL
            );
            NH_CORE_CHECK_NULL_2(-1, hwnd)

            VkWin32SurfaceCreateInfoKHR surfaceInfo =
            {
                .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                .pNext     = VK_NULL_HANDLE,
                .flags     = 0,
                .hinstance = hinstance,
                .hwnd      = hwnd, 
            };

            Host_p->Functions.vkCreateWin32SurfaceKHR(*Window_p, &surfaceInfo, VK_NULL_HANDLE, &testSurface);
            Host_p->Functions.vkGetPhysicalDeviceSurfaceSupportKHR(*Physical_p, i, testSurface, &presentSupport);
            Host_p->Functions.vkDestroySurfaceKHR(*Window_p, testSurface, VK_NULL_HANDLE);

            DestroyWindow(hwnd);
            UnregisterClassA(CLASS_NAME, hinstance);

#endif

            if (presentSupport == VK_TRUE) {
                nh_core_free(QueueFamilyProperties_p); 
                return i;
            }
        }
    }
 
    nh_core_free(QueueFamilyProperties_p);

    return -1;
}

static NH_API_RESULT nh_gfx_chooseVulkanSwapSurfaceFormat(
    VkSurfaceFormatKHR availableFormats[], VkSurfaceFormatKHR *surfaceFormat_p) 
{
    if (availableFormats[0].format == VK_FORMAT_UNDEFINED) 
    {
        *surfaceFormat_p = (VkSurfaceFormatKHR) {
            VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        };
    }

    for(int i = 0; i < sizeof(*availableFormats) / sizeof(availableFormats[0]); ++i) 
    {
        if (availableFormats[i].format     == VK_FORMAT_B8G8R8A8_UNORM 
        &&  availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
        {
            *surfaceFormat_p = availableFormats[i];
        }
    }

    *surfaceFormat_p = availableFormats[0];
    
    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_createVulkanRenderPass(
    nh_gfx_VulkanHost *Host_p, nh_gfx_VulkanDriver *Driver_p)
{
    VkSurfaceFormatKHR SurfaceFormat;
    NH_CORE_CHECK(nh_gfx_getVulkanSurfaceFormat(Host_p, Driver_p, &SurfaceFormat))

    VkAttachmentDescription colorAttachment = 
    {
        .format         = SurfaceFormat.format,
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };
    VkAttachmentReference colorAttachmentRef = 
    {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    // depth
    VkAttachmentDescription depthAttachment = 
    {
        .format         = VK_FORMAT_D32_SFLOAT_S8_UINT,
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };
    VkAttachmentReference depthAttachmentRef = 
    {
        .attachment = 1,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    // subpass
    VkSubpassDescription subpass = 
    {
        .pipelineBindPoint          = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount       = 1,
        .pColorAttachments          = &colorAttachmentRef,
        .pDepthStencilAttachment    = &depthAttachmentRef
    };

    VkSubpassDependency dependencies[2];

    dependencies[1].srcSubpass      = 0;
    dependencies[1].dstSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask    = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    dependencies[1].dstAccessMask   = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[0].srcSubpass      = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass      = 0;
    dependencies[0].srcStageMask    = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[0].dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    // create render pass
    VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo RenderPassInfo = 
    {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 2,
        .pAttachments    = attachments,
        .subpassCount    = 1,
        .pSubpasses      = &subpass,
        .dependencyCount = 2,
        .pDependencies   = dependencies
    };

    Driver_p->Functions.vkCreateRenderPass(
        Driver_p->Device, &RenderPassInfo, VK_NULL_HANDLE, &Driver_p->RenderPass_p[0]
    );

    return NH_API_SUCCESS;
}

#if defined(_WIN32) || defined (WIN32)

    LRESULT CALLBACK WindowProc(
        NHND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
    
            case WM_PAINT:
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
                EndPaint(hwnd, &ps);
                return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

#endif

