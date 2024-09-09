#ifndef NH_GFX_H
#define NH_GFX_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-wsi.h"

#endif

/** @defgroup api_nh-gfx nh-gfx
 *  @brief Provide surface and viewport for rendering.
 */

/** @addtogroup api_nh-gfx
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_GFX_RESULT {
        NH_GFX_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_GFX_ERROR_NULL_POINTER,
        NH_GFX_ERROR_BAD_STATE,
        NH_GFX_ERROR_PARAMETERS,
        NH_GFX_ERROR_MEMORY_ALLOCATION,
        NH_GFX_ERROR_INVALID_FILE_FORMAT,
        NH_GFX_ERROR_RESIZE_NEEDED,
        NH_GFX_VULKAN_ERROR_VALIDATION_NOT_SUPPORTED,
        NH_GFX_VULKAN_ERROR_LOADER_CANT_CREATE_DEBUG_MESSENGER,
        NH_GFX_VULKAN_ERROR_NO_CORRESPONDING_RESULT,
        NH_GFX_VULKAN_ERROR_DEVICE_NOT_SUITABLE,
        NH_GFX_VULKAN_ERROR_HOST_CREATION_FAILED,
        NH_GFX_VULKAN_ERROR_NO_DEVICE_SUITABLE,
        NH_GFX_VULKAN_NOT_READY,
        NH_GFX_VULKAN_TIMEOUT,
        NH_GFX_VULKAN_EVENT_SET,
        NH_GFX_VULKAN_EVENT_RESET,
        NH_GFX_VULKAN_INCOMPLETE,
        NH_GFX_VULKAN_ERROR_OUT_OF_HOST_MEMORY,
        NH_GFX_VULKAN_ERROR_OUT_OF_DEVICE_MEMORY,
        NH_GFX_VULKAN_ERROR_INITIALIZATION_FAILED,
        NH_GFX_VULKAN_ERROR_DEVICE_LOST,
        NH_GFX_VULKAN_ERROR_MEMORY_MAP_FAILED,
        NH_GFX_VULKAN_ERROR_LAYER_NOT_PRESENT,
        NH_GFX_VULKAN_ERROR_EXTENSION_NOT_PRESENT,
        NH_GFX_VULKAN_ERROR_FEATURE_NOT_PRESENT,
        NH_GFX_VULKAN_ERROR_INCOMPATIBLE_DRIVER,
        NH_GFX_VULKAN_ERROR_TOO_MANY_OBJECTS,
        NH_GFX_VULKAN_ERROR_FORMAT_NOT_SUPPORTED,
        NH_GFX_VULKAN_ERROR_FRAGMENTED_POOL,
        NH_GFX_VULKAN_ERROR_UNKNOWN,
        NH_GFX_VULKAN_ERROR_OUT_OF_POOL_MEMORY,
        NH_GFX_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE,
        NH_GFX_VULKAN_ERROR_FRAGMENTATION,
        NH_GFX_VULKAN_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
        NH_GFX_VULKAN_ERROR_SURFACE_LOST_KHR,
        NH_GFX_VULKAN_ERROR_NATIVE_WINDOW_IN_USE_KHR,
        NH_GFX_VULKAN_SUBOPTIMAL_KHR,
        NH_GFX_VULKAN_ERROR_OUT_OF_DATE_KHR,
        NH_GFX_VULKAN_ERROR_INCOMPATIBLE_DISPLAY_KHR,
        NH_GFX_VULKAN_ERROR_VALIDATION_FAILED_EXT,
        NH_GFX_VULKAN_ERROR_INVALID_SHADER_NV,
        NH_GFX_VULKAN_ERROR_INCOMPATIBLE_VERSION_KHR,
        NH_GFX_VULKAN_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT,
        NH_GFX_VULKAN_ERROR_NOT_PERMITTED_EXT,
        NH_GFX_VULKAN_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT,
        NH_GFX_VULKAN_THREAD_IDLE_KHR,
        NH_GFX_VULKAN_THREAD_DONE_KHR,
        NH_GFX_VULKAN_OPERATION_DEFERRED_KHR,
        NH_GFX_VULKAN_OPERATION_NOT_DEFERRED_KHR,
        NH_GFX_VULKAN_PIPELINE_COMPILE_REQUIRED_EXT,
        NH_GFX_VULKAN_ERROR_OUT_OF_POOL_MEMORY_KHR,
        NH_GFX_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE_KHR,
        NH_GFX_VULKAN_RESULT_MAX_ENUM,
    } NH_GFX_RESULT;

    typedef enum NH_GFX_API_E {
        NH_GFX_API_VULKAN,
        NH_GFX_API_OPENGL,
    } NH_GFX_API_E;

// TYPEDEFS ========================================================================================

    typedef struct nh_gfx_SurfaceRequirements nh_gfx_SurfaceRequirements;
    typedef struct nh_gfx_Surface nh_gfx_Surface;
    typedef struct nh_gfx_Viewport nh_gfx_Viewport;

// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    nh_gfx_SurfaceRequirements *nh_api_getSurfaceRequirements(
    );

    /**
     * Todo.
     */
    nh_gfx_Surface *nh_api_createSurface(
        nh_wsi_Window *Window_p, NH_GFX_API_E api
    );

    /**
     * Todo.
     */
    nh_gfx_Viewport *nh_api_createViewport(
        nh_gfx_Surface *Surface_p, nh_PixelPosition Position, nh_PixelSize Size
    );

    /**
     * Todo.
     */
    NH_GFX_RESULT nh_api_configureViewport(
        nh_gfx_Viewport *Viewport_p, nh_PixelPosition Position, nh_PixelSize Size
    );

/** @} */

#endif // NH_GFX_H
