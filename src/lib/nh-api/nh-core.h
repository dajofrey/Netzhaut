#ifndef NH_API_NH_CORE_H 
#define NH_API_NH_CORE_H 

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include <stdint.h>
#include <stdbool.h>

// ENUMS ===========================================================================================

    /** 
     * Return values for functions. 
     */ 
    typedef enum NH_API_RESULT 
    { 
        NH_API_SUCCESS,

        // general errors
        NH_API_ERROR_NULL_POINTER, 
        NH_API_ERROR_BAD_STATE, 
        NH_API_ERROR_THREAD_INITIALIZATION, 
        NH_API_ERROR_THREADPOOL_CREATION, 
        NH_API_ERROR_MEMORY_INITIALIZATION, 
        NH_API_ERROR_HTML_BASE_CREATION_FAILED, 
        NH_API_ERROR_HTML_THREAD_START_FAILED, 
        NH_API_ERROR_WINDOW_THREAD_START_FAILED, 
        NH_API_ERROR_INSUFFICIENT_RESOURCES, 
        NH_API_ERROR_MEMORY_ALLOCATION, 
        NH_API_ERROR_PARAMETERS, 
        NH_API_ERROR_NOT_ENOUGH_THREADS, 
        NH_API_ERROR_UNKNOWN_FILE_FORMAT, 
        NH_API_ERROR_TYPE_MISMATCH, 
        NH_API_ERROR_STRING_LENGTH, 
        NH_API_ERROR_INVALID_FILE_FORMAT, 
        NH_API_ERROR_EXPECTED_TRUE, 
        NH_API_ERROR_EXPECTED_FALSE, 
        NH_API_ERROR_INVALID_ARGUMENT, 
        NH_API_ERROR_RESIZE_NEEDED, 

        // usage errors
        NH_API_USAGE_ERROR_ALREADY_INITIALIZED, 
        NH_API_USAGE_ERROR_INIT_MISSING, 
        NH_API_USAGE_ERROR_STILL_RUNNING, 

        // signals
        NH_API_SIGNAL_SPLIT_INLINE_BOX,

        // vulkan related
        NH_API_VULKAN_ERROR_HOST_CREATION_FAILED, 
        NH_API_VULKAN_ERROR_VALIDATION_NOT_SUPPORTED, 
        NH_API_VULKAN_ERROR_LOADER_CANT_CREATE_DEBUG_MESSENGER, 
        NH_API_VULKAN_ERROR_NO_CORRESPONDING_RESULT, 
        NH_API_VULKAN_ERROR_DEVICE_NOT_SUITABLE, 
        NH_API_VULKAN_ERROR_NO_DEVICE_SUITABLE, 
        NH_API_VULKAN_ERROR_OUT_OF_HOST_MEMORY, 
        NH_API_VULKAN_ERROR_OUT_OF_DEVICE_MEMORY, 
        NH_API_VULKAN_ERROR_INITIALIZATION_FAILED, 
        NH_API_VULKAN_ERROR_DEVICE_LOST, 
        NH_API_VULKAN_ERROR_MEMORY_MAP_FAILED, 
        NH_API_VULKAN_ERROR_LAYER_NOT_PRESENT, 
        NH_API_VULKAN_ERROR_EXTENSION_NOT_PRESENT, 
        NH_API_VULKAN_ERROR_FEATURE_NOT_PRESENT, 
        NH_API_VULKAN_ERROR_INCOMPATIBLE_DRIVER, 
        NH_API_VULKAN_ERROR_TOO_MANY_OBJECTS, 
        NH_API_VULKAN_ERROR_FORMAT_NOT_SUPPORTED, 
        NH_API_VULKAN_ERROR_FRAGMENTED_POOL, 
        NH_API_VULKAN_ERROR_UNKNOWN, 
        NH_API_VULKAN_ERROR_OUT_OF_POOL_MEMORY, 
        NH_API_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE, 
        NH_API_VULKAN_ERROR_FRAGMENTATION, 
        NH_API_VULKAN_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS, 
        NH_API_VULKAN_ERROR_SURFACE_LOST_KHR, 
        NH_API_VULKAN_ERROR_NATIVE_WINDOW_IN_USE_KHR, 
        NH_API_VULKAN_ERROR_OUT_OF_DATE_KHR, 
        NH_API_VULKAN_ERROR_INCOMPATIBLE_DISPLAY_KHR, 
        NH_API_VULKAN_ERROR_VALIDATION_FAILED_EXT, 
        NH_API_VULKAN_ERROR_INVALID_SHADER_NV, 
        NH_API_VULKAN_ERROR_INCOMPATIBLE_VERSION_KHR, 
        NH_API_VULKAN_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT, 
        NH_API_VULKAN_ERROR_NOT_PERMITTED_EXT, 
        NH_API_VULKAN_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, 
        NH_API_VULKAN_ERROR_OUT_OF_POOL_MEMORY_KHR, 
        NH_API_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE_KHR, 
        NH_API_VULKAN_THREAD_IDLE_KHR, 
        NH_API_VULKAN_THREAD_DONE_KHR, 
        NH_API_VULKAN_OPERATION_DEFERRED_KHR, 
        NH_API_VULKAN_OPERATION_NOT_DEFERRED_KHR, 
        NH_API_VULKAN_PIPELINE_COMPILE_REQUIRED_EXT, 
        NH_API_VULKAN_SUBOPTIMAL_KHR, 
        NH_API_VULKAN_RESULT_MAX_ENUM, 
        NH_API_VULKAN_NOT_READY, 
        NH_API_VULKAN_TIMEOUT, 
        NH_API_VULKAN_EVENT_SET, 
        NH_API_VULKAN_EVENT_RESET, 
        NH_API_VULKAN_INCOMPLETE, 
 
    } NH_API_RESULT; 

    typedef enum NH_SIGNAL {
        NH_SIGNAL_OK,
        NH_SIGNAL_DONE,
        NH_SIGNAL_FINISH,
        NH_SIGNAL_FREE,
        NH_SIGNAL_ERROR,
        NH_SIGNAL_IDLE,
        NH_SIGNAL_OPEN,
        NH_SIGNAL_CLOSE,
        NH_SIGNAL_UPDATE,
        NH_SIGNAL_INACTIVE,
        NH_SIGNAL_INIT,
        NH_SIGNAL_RESIZE,
        NH_SIGNAL_RESTART,
        NH_SIGNAL_SET_BACKGROUND_COLOR,
        NH_SIGNAL_SET_TITLE,
    } NH_SIGNAL;

// TYPEDEFS ========================================================================================

    /**
     * Opaque structure of the loader data.
     */
    typedef struct nh_Loader nh_Loader;
    
    typedef float         NH_CLIP;

    typedef struct nh_api_Workload nh_api_Workload;

    typedef void (*nh_api_logCallback_f)(
        char *node_p, char *options_p, char *message_p
    );

    typedef NH_API_RESULT (*nh_api_addLogCallback_f)(
        nh_api_logCallback_f logCallback_f
    );

// DATA ============================================================================================

    extern nh_Loader *NH_LOADER_P;

// STRUCTS =========================================================================================

    typedef struct nh_BigInt {
        int TODO;
    } nh_BigInt;

    typedef struct nh_ClipPosition {
        NH_CLIP x; 
        NH_CLIP y; 
    } nh_ClipPosition; 
 
    typedef struct nh_ClipSize { 
        NH_CLIP width; 
        NH_CLIP height; 
    } nh_ClipSize; 
 
    typedef struct nh_Color { 
        float r, g, b, a; 
    } nh_Color; 
 
// GENERAL FUNCTIONS ===============================================================================

    /**
     * Todo. 
     */
    NH_API_RESULT nh_api_initialize(
        char *path_p, char *config_p, int length
    );

    /**
     * Todo. 
     */
    NH_API_RESULT nh_api_terminate(
    );

    /**
     * Todo. 
     */
    unsigned int nh_api_run(
    );

    /**
     * Todo. 
     */
    bool nh_api_keepRunning(
    );

    /**
     * Todo. 
     */
    NH_API_RESULT nh_api_terminate(
    );

    /**
     * Todo. 
     */
    NH_API_RESULT nh_api_addLogCallback(
        nh_api_logCallback_f logCallback_f
    );

    /**
     * Todo.
     */
    nh_api_Workload *nh_api_getWorkload(
        void *args_p
    );

// FILE FUNCTIONS ==================================================================================

    char *nh_api_getFileData(
        const char* path_p, long *size_p
    );

    NH_API_RESULT nh_api_writeBytesToFile(
        char *filePath_p, char *bytes_p
    );

// CONFIG FUNCTIONS ================================================================================

    NH_API_RESULT nh_api_registerConfig(
        char *absolutePath_p, int length
    );

    NH_API_RESULT nh_api_loadConfig(
        char *data_p, int length
    );

    void *nh_api_createMonitorInterface(
    );
    
    void nh_api_freeMonitorInterface(
        void *Interface_p
    );

/** @} */

#endif // NH_API_NH_CORE_H
