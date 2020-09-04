// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Driver.h"
#include "../Header/GPU.h"
#include "../Header/Host.h"
#include "../Header/Utils.h"
#include "../Header/Macros.h"
#include "../Header/Vulkan.h"

#include "../../../Core/Header/Thread.h"
#include "../../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN32) || defined (WIN32)
    #include <windows.h>
#endif

// DECLARE =========================================================================================

static NH_RESULT Nh_Vk_createPhysicalDevice(
    Nh_Vk_Host *Host, Nh_Vk_GPU *GPU_p
);
static NH_RESULT Nh_Vk_createDevice(
    Nh_Vk_Host *Host, Nh_Vk_Driver *Driver_p, uint32_t *graphicsQueue_p, uint32_t *computeQueue_p
);
static NH_RESULT Nh_Vk_createDescriptorPool(
    Nh_Vk_Driver *Driver_p
);
static NH_RESULT Nh_Vk_createCommandPools(
    Nh_Vk_Driver *Driver_p, uint32_t graphicsQueue, uint32_t computeQueue
);
static NH_RESULT Nh_Vk_createRenderPass(
    Nh_Vk_Host *Host, Nh_Vk_Driver *Driver_p
);

static NH_RESULT Nh_Vk_getSurfaceFormat(
    Nh_Vk_Host *Host, Nh_Vk_Driver *Driver_p, VkSurfaceFormatKHR *surfaceFormat_p
);
static bool Nh_Vk_deviceIsSuitable(
    Nh_Vk_Host *Host_p, VkPhysicalDevice Device
);
static bool Nh_Vk_deviceSupportsExtensions(
    Nh_Vk_Host *Host_p, VkPhysicalDevice Device
);
static uint32_t Nh_Vk_getQueueFamily(
    Nh_Vk_Host *Host, Nh_Vk_Driver *Driver_p, VkQueueFlagBits queueFlag
);
static NH_RESULT Nh_Vk_chooseSwapSurfaceFormat(
    VkSurfaceFormatKHR availableFormats[], VkSurfaceFormatKHR *surfaceFormat_p
);

#if defined(_WIN32) || defined (WIN32)
LRESULT CALLBACK WindowProc(
    NHND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam
);
#endif

// IMPLEMENT =======================================================================================

NH_RESULT Nh_Vk_createDriver(
    Nh_Vk_Host *Host_p, Nh_Vk_GPU *GPU_p)
{
NH_BEGIN()

    // allocate
    GPU_p->Driver.DescriptorPool_p = Nh_allocate(sizeof(VkDescriptorPool));
    NH_CHECK_MEM(GPU_p->Driver.DescriptorPool_p);
    GPU_p->Driver.descriptorPoolCount = 1;

    GPU_p->Driver.RenderPass_p = Nh_allocate(sizeof(VkRenderPass));
    NH_CHECK_MEM(GPU_p->Driver.RenderPass_p);
    GPU_p->Driver.renderPassCount = 1;

    GPU_p->Driver.ComputeCommandPools_p = Nh_allocate(sizeof(VkCommandPool) * Nh_getThreadPool()->maxKernelThreads);
    NH_CHECK_MEM(GPU_p->Driver.ComputeCommandPools_p);
    GPU_p->Driver.GraphicsCommandPools_p = Nh_allocate(sizeof(VkCommandPool) * Nh_getThreadPool()->maxKernelThreads);
    NH_CHECK_MEM(GPU_p->Driver.GraphicsCommandPools_p);

    // create
    uint32_t graphicsQueue = 0, computeQueue = 0;

    NH_CHECK(Nh_Vk_createPhysicalDevice(Host_p, GPU_p))
    NH_CHECK(Nh_Vk_createDevice(Host_p, &GPU_p->Driver, &graphicsQueue, &computeQueue))
    NH_CHECK(Nh_Vk_createDescriptorPool(&GPU_p->Driver))
    NH_CHECK(Nh_Vk_createCommandPools(&GPU_p->Driver, graphicsQueue, computeQueue))
    NH_CHECK(Nh_Vk_createRenderPass(Host_p, &GPU_p->Driver))

NH_END(NH_SUCCESS);
}

void Nh_Vk_destroyDriver(
    Nh_Vk_Driver *Driver_p)
{
NH_BEGIN()

    Driver_p->Func.vkDestroyDescriptorPool(Driver_p->Device, Driver_p->DescriptorPool_p[0], VK_NULL_HANDLE);

    for (int i = 0; i < Nh_getThreadPool()->maxKernelThreads; ++i) {
        Driver_p->Func.vkDestroyCommandPool(Driver_p->Device, Driver_p->GraphicsCommandPools_p[i], VK_NULL_HANDLE);
        Driver_p->Func.vkDestroyCommandPool(Driver_p->Device, Driver_p->ComputeCommandPools_p[i], VK_NULL_HANDLE);
    }

    Driver_p->Func.vkDestroyRenderPass(Driver_p->Device, Driver_p->RenderPass_p[0], VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyDevice(Driver_p->Device, VK_NULL_HANDLE);

    Nh_free(Driver_p->DescriptorPool_p);
    Nh_free(Driver_p->RenderPass_p);
    Nh_free(Driver_p->ComputeCommandPools_p);
    Nh_free(Driver_p->GraphicsCommandPools_p);

NH_SILENT_END()
}

int Nh_Vk_getDriver( // defined in API
    Nh_Vk_Driver **Driver_pp)
{
NH_BEGIN()

    int count = Nh_Vk_getGPUCount();

    if (Driver_pp != NULL) {
        for (int i = 0; i < count; ++i) {
            Driver_pp[i] = &Nh_Vk_getGPU(i)->Driver;
        }
    }

NH_END(count)
}

// HELPER ==========================================================================================

static NH_RESULT Nh_Vk_createPhysicalDevice(
    Nh_Vk_Host *Host_p, Nh_Vk_GPU *GPU_p)
{
NH_BEGIN()

    Nh_Vk_Driver *Driver_p     = &GPU_p->Driver;
    VkInstance *Window_p       = &Host_p->Instance;
    VkPhysicalDevice *Device_p = NULL;

    // get physical device
    uint32_t deviceCount;
    Host_p->Func.vkEnumeratePhysicalDevices(*Window_p, &deviceCount, VK_NULL_HANDLE); 

    VkPhysicalDevice *Physical_p = Nh_allocate(sizeof(VkPhysicalDevice) * deviceCount);
    NH_CHECK_MEM(Physical_p)
    
    Host_p->Func.vkEnumeratePhysicalDevices(*Window_p, &deviceCount, Physical_p);

    for (int i = 0; i < deviceCount; ++i) 
    {
        VkPhysicalDeviceProperties Properties;
        Host_p->Func.vkGetPhysicalDeviceProperties(Physical_p[i], &Properties);
        
        if (!strcmp(Properties.deviceName, GPU_p->name_p)) {Device_p = &Physical_p[i];}
    }

    NH_CHECK_NULL(Device_p)

    // add if suitable
    if (Nh_Vk_deviceIsSuitable(Host_p, *Device_p))
    {        
        Driver_p->PhysicalDevice = *Device_p;

        VkPhysicalDeviceFeatures Features;
        Host_p->Func.vkGetPhysicalDeviceFeatures(*Device_p, &Features);
          
        VkPhysicalDeviceProperties Properties;
        Host_p->Func.vkGetPhysicalDeviceProperties(*Device_p, &Properties);

        Driver_p->Info.major    = VK_VERSION_MAJOR(Properties.apiVersion);
        Driver_p->Info.minor    = VK_VERSION_MINOR(Properties.apiVersion);
        Driver_p->Info.patch    = VK_VERSION_PATCH(Properties.apiVersion);
        Driver_p->Info.vendorId = Properties.vendorID;
        Driver_p->Info.deviceId = Properties.deviceID;
    } 
    else {NH_END(NH_VULKAN_ERROR_DEVICE_NOT_SUITABLE)}

    Nh_free(Physical_p);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_createDevice(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, uint32_t *graphicsQueue_p, uint32_t *computeQueue_p)
{
NH_BEGIN()

    *graphicsQueue_p = Nh_Vk_getQueueFamily(Host_p, Driver_p, VK_QUEUE_GRAPHICS_BIT);
    *computeQueue_p  = Nh_Vk_getQueueFamily(Host_p, Driver_p, VK_QUEUE_COMPUTE_BIT);

    if (*graphicsQueue_p == -1 || *computeQueue_p == -1) {NH_END(NH_ERROR_BAD_STATE)}

    int queueCount = *graphicsQueue_p == *computeQueue_p ? 1 : 2;
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo *QueueInfos_p = Nh_allocate(sizeof(VkDeviceQueueCreateInfo) * queueCount);
    NH_CHECK_MEM(QueueInfos_p)

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
    Host_p->Func.vkGetPhysicalDeviceFeatures(Driver_p->PhysicalDevice, &DeviceFeatures);

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
    NH_CHECK_VULKAN(Host_p->Func.vkCreateDevice(Driver_p->PhysicalDevice, &DeviceInfo, VK_NULL_HANDLE, &Driver_p->Device))

    volkLoadDeviceTable(&Driver_p->Func, Driver_p->Device);

    for (unsigned int i = 0; i < queueCount; ++i) {
        Driver_p->Func.vkGetDeviceQueue(
            Driver_p->Device, i == 0 ? 
            *graphicsQueue_p : *computeQueue_p, 0, i == 0 ? &Driver_p->GraphicsQueue : &Driver_p->ComputeQueue
        );
    }

    Nh_free(QueueInfos_p);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_createDescriptorPool(
    Nh_Vk_Driver *Driver_p)
{
NH_BEGIN()

    VkDescriptorPoolSize PoolSizes_p[] =
    {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 }
    };

    VkDescriptorPoolCreateInfo PoolInfo = 
    {
        .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext         = NULL,
        .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .poolSizeCount = 2,
        .pPoolSizes    = PoolSizes_p,
        .maxSets       = 1100
    };

    NH_CHECK_VULKAN(Driver_p->Func.vkCreateDescriptorPool(
        Driver_p->Device, &PoolInfo, VK_NULL_HANDLE, &(Driver_p->DescriptorPool_p[0])
    ))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_createCommandPools(
    Nh_Vk_Driver *Driver_p, uint32_t graphicsQueue, uint32_t computeQueue)
{
NH_BEGIN()

    for (int i = 0; i < Nh_getThreadPool()->maxKernelThreads; ++i)
    {
        VkCommandPoolCreateInfo CommandPoolInfo = 
        {    
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .queueFamilyIndex = graphicsQueue,
            .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        };
        NH_CHECK_VULKAN(Driver_p->Func.vkCreateCommandPool(Driver_p->Device, &CommandPoolInfo, VK_NULL_HANDLE, &Driver_p->GraphicsCommandPools_p[i]))

        CommandPoolInfo.queueFamilyIndex = computeQueue;
        NH_CHECK_VULKAN(Driver_p->Func.vkCreateCommandPool(Driver_p->Device, &CommandPoolInfo, VK_NULL_HANDLE, &Driver_p->ComputeCommandPools_p[i]))
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_getSurfaceFormat(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, VkSurfaceFormatKHR *surfaceFormat_p)
{
NH_BEGIN()

    VkInstance *Window_p = &Host_p->Instance;
    VkPhysicalDevice *Physical_p = &Driver_p->PhysicalDevice;

    VkSurfaceKHR Surface;

#ifdef __unix__

    Display *display = XOpenDisplay(NULL);
    NH_CHECK_NULL(display) 
  
    int screen = DefaultScreen(display);
    VkXlibSurfaceCreateInfoKHR SurfaceInfo = 
    { 
        .sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .pNext  = VK_NULL_HANDLE,
        .dpy    = display,
        .window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 100, 100, 
                    1, BlackPixel(display, screen), WhitePixel(display, screen))
    };

    Host_p->Func.vkCreateXlibSurfaceKHR(*Window_p, &SurfaceInfo, VK_NULL_HANDLE, &Surface); 

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

    NH_CHECK_NULL(hwnd)

    VkWin32SurfaceCreateInfoKHR surfaceInfo =
    {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = VK_NULL_HANDLE,
        .flags = 0,
        .hinstance = hinstance,
        .hwnd = hwnd, 

    };

    Host_p->Func.vkCreateWin32SurfaceKHR(*Window_p, &surfaceInfo, VK_NULL_HANDLE, &Surface);

#endif
   
    uint32_t formatCount;
    Host_p->Func.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Physical_p, Surface, &formatCount, VK_NULL_HANDLE
    );

    VkSurfaceFormatKHR *SurfaceFormats_p = Nh_allocate(sizeof(VkSurfaceFormatKHR) * formatCount);
    NH_CHECK_MEM(SurfaceFormats_p)

    Host_p->Func.vkGetPhysicalDeviceSurfaceFormatsKHR(
        *Physical_p, Surface, &formatCount, SurfaceFormats_p
    ); 
    Host_p->Func.vkDestroySurfaceKHR(*Window_p, Surface, VK_NULL_HANDLE);

#ifdef __unix__
    XCloseDisplay(display);
#elif defined(_WIN32) || defined (WIN32)
    DestroyWindow(hwnd);
    UnregisterClassA(CLASS_NAME, hinstance);
#endif

    Nh_Vk_chooseSwapSurfaceFormat(SurfaceFormats_p, surfaceFormat_p);

    Nh_free(SurfaceFormats_p);

NH_END(NH_SUCCESS)
}

static bool Nh_Vk_deviceIsSuitable(
    Nh_Vk_Host *Host_p, VkPhysicalDevice Device)
{
NH_BEGIN()

    uint32_t queueFamilyCount = 0;
    Host_p->Func.vkGetPhysicalDeviceQueueFamilyProperties(Device, &queueFamilyCount, VK_NULL_HANDLE);

#include NH_CUSTOM_CHECK

    VkQueueFamilyProperties *queueFamilies_p = 
        Nh_allocate(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    NH_CHECK_NULL(false, queueFamilies_p)

    Host_p->Func.vkGetPhysicalDeviceQueueFamilyProperties(Device, &queueFamilyCount, queueFamilies_p);

#include NH_DEFAULT_CHECK
   
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
    
    Nh_free(queueFamilies_p);

#undef GRAPHICS
#undef COMPUTE

NH_END(Nh_Vk_deviceSupportsExtensions(Host_p, Device) && queueFamiliesSupported)
}

static bool Nh_Vk_deviceSupportsExtensions(
    Nh_Vk_Host *Host_p, VkPhysicalDevice Device) 
{
NH_BEGIN()

    const char* requiredExtensions_p = {"VK_KHR_swapchain"};
    uint32_t requiredExtensionsCount = 1;

    uint32_t availableExtensionsCount = 0;
    Host_p->Func.vkEnumerateDeviceExtensionProperties(
        Device, VK_NULL_HANDLE, &availableExtensionsCount, VK_NULL_HANDLE
    ); 

#include NH_CUSTOM_CHECK

    VkExtensionProperties *AvailableExtensions_p = 
        Nh_allocate(sizeof(VkExtensionProperties) * availableExtensionsCount);
    NH_CHECK_NULL(false, AvailableExtensions_p)

#include NH_DEFAULT_CHECK

    Host_p->Func.vkEnumerateDeviceExtensionProperties(
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
            Nh_free(AvailableExtensions_p);
            NH_END(true)
        }
    }

    Nh_free(AvailableExtensions_p);

NH_END(false)
}

static uint32_t Nh_Vk_getQueueFamily(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p, VkQueueFlagBits queueFlag) 
{   
NH_BEGIN();

    VkInstance *Window_p = &Host_p->Instance;
    VkPhysicalDevice *Physical_p = &Driver_p->PhysicalDevice;

    uint32_t queueFamilyCount = 0;
    Host_p->Func.vkGetPhysicalDeviceQueueFamilyProperties(*Physical_p, &queueFamilyCount, VK_NULL_HANDLE);
    
    if (queueFamilyCount == 0) {return -1;}

#include NH_CUSTOM_CHECK

    VkQueueFamilyProperties *QueueFamilyProperties_p = Nh_allocate(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    NH_CHECK_NULL(-1, QueueFamilyProperties_p)

    Host_p->Func.vkGetPhysicalDeviceQueueFamilyProperties(*Physical_p, &queueFamilyCount, QueueFamilyProperties_p);

    for (uint32_t i = 0; i < queueFamilyCount; ++i) 
    {
        if (QueueFamilyProperties_p[i].queueCount <= 0 && !(QueueFamilyProperties_p[i].queueFlags & queueFlag)) {
            continue;
        }
        if (queueFlag == VK_QUEUE_COMPUTE_BIT)
        {
            Nh_free(QueueFamilyProperties_p); 
            NH_END(i);
        }
        if (queueFlag == VK_QUEUE_GRAPHICS_BIT)
        {
            VkBool32 presentSupport = VK_FALSE;
            VkSurfaceKHR testSurface;

#ifdef __unix__

            Display *display = XOpenDisplay(NULL);
            NH_CHECK_NULL(-1, display)

            int screen = DefaultScreen(display);
            VkXlibSurfaceCreateInfoKHR SurfaceInfo = 
            { 
                .sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
                .pNext  = VK_NULL_HANDLE,
                .dpy    = display,
                .window = XCreateSimpleWindow(display, RootWindow(display, screen), 
                        10, 10, 100, 100, 1,BlackPixel(display, screen), WhitePixel(display, screen))
            };

            Host_p->Func.vkCreateXlibSurfaceKHR(*Window_p, &SurfaceInfo, VK_NULL_HANDLE, &testSurface); 
            Host_p->Func.vkGetPhysicalDeviceSurfaceSupportKHR(*Physical_p, i, testSurface, &presentSupport); 
            Host_p->Func.vkDestroySurfaceKHR(*Window_p, testSurface, VK_NULL_HANDLE);

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
            NH_CHECK_NULL(-1, hwnd)

            VkWin32SurfaceCreateInfoKHR surfaceInfo =
            {
                .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                .pNext     = VK_NULL_HANDLE,
                .flags     = 0,
                .hinstance = hinstance,
                .hwnd      = hwnd, 
            };

            Host_p->Func.vkCreateWin32SurfaceKHR(*Window_p, &surfaceInfo, VK_NULL_HANDLE, &testSurface);
            Host_p->Func.vkGetPhysicalDeviceSurfaceSupportKHR(*Physical_p, i, testSurface, &presentSupport);
            Host_p->Func.vkDestroySurfaceKHR(*Window_p, testSurface, VK_NULL_HANDLE);

            DestroyWindow(hwnd);
            UnregisterClassA(CLASS_NAME, hinstance);

#endif

            if (presentSupport == VK_TRUE) {
                Nh_free(QueueFamilyProperties_p); 
                NH_END(i)
            }
        }
    }

#include NH_DEFAULT_CHECK
 
    Nh_free(QueueFamilyProperties_p);

NH_END(-1)
}

static NH_RESULT Nh_Vk_chooseSwapSurfaceFormat(
    VkSurfaceFormatKHR availableFormats[], VkSurfaceFormatKHR *surfaceFormat_p) 
{
NH_BEGIN()

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
    
NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Vk_createRenderPass(
    Nh_Vk_Host *Host_p, Nh_Vk_Driver *Driver_p)
{
NH_BEGIN()

    VkSurfaceFormatKHR SurfaceFormat;
    NH_CHECK(Nh_Vk_getSurfaceFormat(Host_p, Driver_p, &SurfaceFormat))

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

    Driver_p->Func.vkCreateRenderPass(
        Driver_p->Device, &RenderPassInfo, VK_NULL_HANDLE, &Driver_p->RenderPass_p[0]
    );

NH_END(NH_SUCCESS)
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

