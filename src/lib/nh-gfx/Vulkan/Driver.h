#ifndef NH_VK_DRIVER_H
#define NH_VK_DRIVER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Host.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_vk_DriverInfo {
        uint32_t major;
        uint32_t minor;
        uint32_t patch;
        uint32_t vendorId;
        uint32_t deviceId;
    } nh_vk_DriverInfo;

    typedef struct nh_vk_Driver {
        nh_vk_DriverInfo Info;
        VkPhysicalDevice PhysicalDevice;
        VkDevice Device;
        struct VolkDeviceTable Functions;
        VkCommandPool *ComputeCommandPools_p;
        VkCommandPool *GraphicsCommandPools_p;
        VkDescriptorPool *DescriptorPool_p;
        VkRenderPass *RenderPass_p;
        VkQueue GraphicsQueue;
        VkQueue ComputeQueue;
        unsigned int descriptorPoolCount;
        unsigned int renderPassCount;
    } nh_vk_Driver;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_GFX_RESULT nh_vk_createDriver(
        nh_vk_Host *Host_p, nh_vk_Driver *Driver_p, NH_BYTE *name_p
    );

    void nh_vk_destroyDriver(
        nh_vk_Driver *Driver_p
    );

/** @} */

#endif
