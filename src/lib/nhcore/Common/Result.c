// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_CORE_RESULTS_PP[] = 
{
    "NH_CORE_SUCCESS",                    
    "NH_PENDING",
    "NH_CORE_ERROR_NULL_POINTER",         
    "NH_CORE_ERROR_BAD_STATE", 
    "NH_CORE_ERROR_THREAD_INITIALIZATION",
    "NH_CORE_ERROR_THREADPOOL_CREATION",
    "NH_CORE_ERROR_MEMORY_INITIALIZATION",
    "NH_CORE_ERROR_VULKAN_HOST_CREATION_FAILED",
    "NH_CORE_ERROR_HTML_BASE_CREATION_FAILED",
    "NH_CORE_ERROR_HTML_THREAD_START_FAILED",
    "NH_CORE_ERROR_WINDOW_THREAD_START_FAILED",
    "NH_CORE_ERROR_INSUFFICIENT_RESOURCES",
    "NH_CORE_ERROR_MEMORY_ALLOCATION",
    "NH_CORE_ERROR_PARAMETERS",
    "NH_CORE_ERROR_NOT_ENOUGH_THREADS",
    "NH_CORE_ERROR_UNKNOWN_FILE_FORMAT",
    "NH_CORE_ERROR_TYPE_MISMATCH",
    "NH_CORE_ERROR_STRING_LENGTH",
    "NH_CORE_ERROR_INVALID_FILE_FORMAT",
    "NH_CORE_ERROR_EXPECTED_TRUE",
    "NH_CORE_ERROR_EXPECTED_FALSE",
    "NH_CORE_ERROR_INVALID_ARGUMENT",

//    "NH_JS_ERROR_BAD_ALLOCATION",
//    "NH_JS_ERROR_UNEXPECTED_TOKEN",             
//    "NH_JS_ERROR_UNEXPECTED_NUMBER",            
//    "NH_JS_ERROR_UNEXPECTED_STRING",            
//    "NH_JS_ERROR_UNEXPECTED_IDENTIFIER",        
//    "NH_JS_ERROR_UNEXPECTED_RESERVED",          
//    "NH_JS_ERROR_UNEXPECTED_EOS",               
//    "NH_JS_ERROR_NEWLINE_AFTER_THROW",          
//    "NH_JS_ERROR_INVALID_REG_EXP",              
//    "NH_JS_ERROR_UNTERMINATED_REG_EXP",         
//    "NH_JS_ERROR_INVALIDL_HS_IN_ASSIGNMENT",    
//    "NH_JS_ERROR_INVALIDL_HS_IN_FOR_IN",        
//    "NH_JS_ERROR_MULTIPLE_DEFAULTS_IN_SWITCH",  
//    "NH_JS_ERROR_NO_CATCH_OR_FINALLY",          
//    "NH_JS_ERROR_UNKNOWN_LABEL",                
//    "NH_JS_ERROR_REDECLARATION",                
//    "NH_JS_ERROR_ILLEGAL_CONTINUE",             
//    "NH_JS_ERROR_ILLEGAL_BREAK",                
//    "NH_JS_ERROR_ILLEGAL_RETURN",               
//    "NH_JS_ERROR_STRICT_MODE_WITH",             
//    "NH_JS_ERROR_STRICT_CATCH_VARIABLE",        
//    "NH_JS_ERROR_STRICT_VAR_NAME",              
//    "NH_JS_ERROR_STRICT_PARAM_NAME",            
//    "NH_JS_ERROR_STRICT_PARAM_DUPE",            
//    "NH_JS_ERROR_STRICT_FUNCTION_NAME",         
//    "NH_JS_ERROR_STRICT_OCTAL_LITERAL",         
//    "NH_JS_ERROR_STRICT_DELETE",                
//    "NH_JS_ERROR_STRICT_DUPLICATE_PROPERTY",    
//    "NH_JS_ERROR_ACCESSOR_DATA_PROPERTY",       
//    "NH_JS_ERROR_ACCESSOR_GET_SET",             
//    "NH_JS_ERROR_STRICT_LHS_ASSIGNMENT",        
//    "NH_JS_ERROR_STRICT_LHS_POSTFIX",           
//    "NH_JS_ERROR_STRICT_LHS_PREFIX",            
//    "NH_JS_ERROR_STRICT_RESERVED_WORD", 

    "NH_USAGE_ERROR_ALREADY_INITIALIZED",
    "NH_USAGE_ERROR_INIT_MISSING",
    "NH_USAGE_ERROR_STILL_RUNNING",

//    "NH_VULKAN_ERROR_VALIDATION_NOT_SUPPORTED",
//    "NH_VULKAN_ERROR_LOADER_CANT_CREATE_DEBUG_MESSENGER",
//    "NH_VULKAN_ERROR_NO_CORRESPONDING_RESULT",
//    "NH_VULKAN_ERROR_DEVICE_NOT_SUITABLE",
//    "NH_VULKAN_ERROR_HOST_CREATION_FAILED",
//    "NH_VULKAN_ERROR_NO_DEVICE_SUITABLE",
//
//    "NH_VULKAN_NOT_READY",
//    "NH_VULKAN_TIMEOUT",
//    "NH_VULKAN_EVENT_SET",
//    "NH_VULKAN_EVENT_RESET",
//    "NH_VULKAN_INCOMPLETE",
//    "NH_VULKAN_ERROR_OUT_OF_HOST_MEMORY",
//    "NH_VULKAN_ERROR_OUT_OF_DEVICE_MEMORY",
//    "NH_VULKAN_ERROR_INITIALIZATION_FAILED",
//    "NH_VULKAN_ERROR_DEVICE_LOST",
//    "NH_VULKAN_ERROR_MEMORY_MAP_FAILED",
//    "NH_VULKAN_ERROR_LAYER_NOT_PRESENT",
//    "NH_VULKAN_ERROR_EXTENSION_NOT_PRESENT",
//    "NH_VULKAN_ERROR_FEATURE_NOT_PRESENT",
//    "NH_VULKAN_ERROR_INCOMPATIBLE_DRIVER",
//    "NH_VULKAN_ERROR_TOO_MANY_OBJECTS",
//    "NH_VULKAN_ERROR_FORMAT_NOT_SUPPORTED",
//    "NH_VULKAN_ERROR_FRAGMENTED_POOL",
//    "NH_VULKAN_ERROR_UNKNOWN",
//    "NH_VULKAN_ERROR_OUT_OF_POOL_MEMORY",
//    "NH_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE",
//    "NH_VULKAN_ERROR_FRAGMENTATION",
//    "NH_VULKAN_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS",
//    "NH_VULKAN_ERROR_SURFACE_LOST_KHR",
//    "NH_VULKAN_ERROR_NATIVE_WINDOW_IN_USE_KHR",
//    "NH_VULKAN_SUBOPTIMAL_KHR",
//    "NH_VULKAN_ERROR_OUT_OF_DATE_KHR",
//    "NH_VULKAN_ERROR_INCOMPATIBLE_DISPLAY_KHR",
//    "NH_VULKAN_ERROR_VALIDATION_FAILED_EXT",
//    "NH_VULKAN_ERROR_INVALID_SHADER_NV",
//    "NH_VULKAN_ERROR_INCOMPATIBLE_VERSION_KHR",
//    "NH_VULKAN_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT",
//    "NH_VULKAN_ERROR_NOT_PERMITTED_EXT",
//    "NH_VULKAN_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT",
//    "NH_VULKAN_THREAD_IDLE_KHR",
//    "NH_VULKAN_THREAD_DONE_KHR",
//    "NH_VULKAN_OPERATION_DEFERRED_KHR",
//    "NH_VULKAN_OPERATION_NOT_DEFERRED_KHR",
//    "NH_VULKAN_PIPELINE_COMPILE_REQUIRED_EXT",
//    "NH_VULKAN_ERROR_OUT_OF_POOL_MEMORY_KHR",
//    "NH_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE_KHR",
//    "NH_VULKAN_RESULT_MAX_ENUM",
//
//    "NH_HTML_ERROR_DOCTYPE_PARSING",
//    "NH_HTML_ERROR_DOCUMENT_EMPTY",
//    "NH_HTML_ERROR_INVALID_START_TAG",
//    "NH_HTML_ERROR_ATTRIBUTE_UNKNOWN",
};

unsigned int NH_CORE_RESULTS_PP_COUNT = sizeof(NH_CORE_RESULTS_PP) / sizeof(NH_CORE_RESULTS_PP[0]);




//    /**
//     * Boolean values.
//     */
//    typedef enum NH_BOOL {
//        NH_FALSE = 0, /**<Indicates false.*/
//        NH_TRUE = 1,  /**<Indicates true.*/
//    } NH_BOOL;
//
//    /**
//     * Return values for functions.
//     */
//    typedef enum NH_CORE_RESULT 
//    {
//        NH_CORE_SUCCESS, /**<Indicates that something worked as planned.*/                   
//        NH_PENDING, 
//        NH_CORE_ERROR_NULL_POINTER,         
//        NH_CORE_ERROR_BAD_STATE, 
//        NH_CORE_ERROR_THREAD_INITIALIZATION,
//        NH_CORE_ERROR_THREADPOOL_CREATION,
//        NH_CORE_ERROR_MEMORY_INITIALIZATION,
//        NH_CORE_ERROR_VULKAN_HOST_CREATION_FAILED,
//        NH_CORE_ERROR_HTML_BASE_CREATION_FAILED,
//        NH_CORE_ERROR_HTML_THREAD_START_FAILED,
//        NH_CORE_ERROR_WINDOW_THREAD_START_FAILED,
//        NH_CORE_ERROR_INSUFFICIENT_RESOURCES,
//        NH_CORE_ERROR_MEMORY_ALLOCATION,
//        NH_CORE_ERROR_PARAMETERS,
//        NH_CORE_ERROR_NOT_ENOUGH_THREADS,
//        NH_CORE_ERROR_UNKNOWN_FILE_FORMAT,
//        NH_CORE_ERROR_TYPE_MISMATCH,
//        NH_CORE_ERROR_STRING_LENGTH,
//        NH_CORE_ERROR_RESIZE_NEEDED,
//        NH_CORE_ERROR_INVALID_FILE_FORMAT,
//        NH_CORE_ERROR_EXPECTED_TRUE,
//        NH_CORE_ERROR_EXPECTED_FALSE,
//
//        NH_JS_ERROR_BAD_ALLOCATION,
//        NH_JS_ERROR_UNEXPECTED_TOKEN,             
//        NH_JS_ERROR_UNEXPECTED_NUMBER,            
//        NH_JS_ERROR_UNEXPECTED_STRING,            
//        NH_JS_ERROR_UNEXPECTED_IDENTIFIER,        
//        NH_JS_ERROR_UNEXPECTED_RESERVED,          
//        NH_JS_ERROR_UNEXPECTED_EOS,               
//        NH_JS_ERROR_NEWLINE_AFTER_THROW,          
//        NH_JS_ERROR_INVALID_REG_EXP,              
//        NH_JS_ERROR_UNTERMINATED_REG_EXP,         
//        NH_JS_ERROR_INVALIDL_HS_IN_ASSIGNMENT,    
//        NH_JS_ERROR_INVALIDL_HS_IN_FOR_IN,        
//        NH_JS_ERROR_MULTIPLE_DEFAULTS_IN_SWITCH,  
//        NH_JS_ERROR_NO_CATCH_OR_FINALLY,          
//        NH_JS_ERROR_UNKNOWN_LABEL,                
//        NH_JS_ERROR_REDECLARATION,                
//        NH_JS_ERROR_ILLEGAL_CONTINUE,             
//        NH_JS_ERROR_ILLEGAL_BREAK,                
//        NH_JS_ERROR_ILLEGAL_RETURN,               
//        NH_JS_ERROR_STRICT_MODE_WITH,             
//        NH_JS_ERROR_STRICT_CATCH_VARIABLE,        
//        NH_JS_ERROR_STRICT_VAR_NAME,              
//        NH_JS_ERROR_STRICT_PARAM_NAME,            
//        NH_JS_ERROR_STRICT_PARAM_DUPE,            
//        NH_JS_ERROR_STRICT_FUNCTION_NAME,         
//        NH_JS_ERROR_STRICT_OCTAL_LITERAL,         
//        NH_JS_ERROR_STRICT_DELETE,                
//        NH_JS_ERROR_STRICT_DUPLICATE_PROPERTY,    
//        NH_JS_ERROR_ACCESSOR_DATA_PROPERTY,       
//        NH_JS_ERROR_ACCESSOR_GET_SET,             
//        NH_JS_ERROR_STRICT_LHS_ASSIGNMENT,        
//        NH_JS_ERROR_STRICT_LHS_POSTFIX,           
//        NH_JS_ERROR_STRICT_LHS_PREFIX,            
//        NH_JS_ERROR_STRICT_RESERVED_WORD, 
//
//        NH_USAGE_ERROR_ALREADY_INITIALIZED,
//        NH_USAGE_ERROR_INIT_MISSING,
//        NH_USAGE_ERROR_NULL_INPUT,
//        NH_USAGE_ERROR_STILL_RUNNING,
//
//        NH_VULKAN_ERROR_VALIDATION_NOT_SUPPORTED,
//        NH_VULKAN_ERROR_LOADER_CANT_CREATE_DEBUG_MESSENGER,
//        NH_VULKAN_ERROR_NO_CORRESPONDING_RESULT,
//        NH_VULKAN_ERROR_DEVICE_NOT_SUITABLE,
//        NH_VULKAN_ERROR_HOST_CREATION_FAILED,
//        NH_VULKAN_ERROR_NO_DEVICE_SUITABLE,
//
//        NH_VULKAN_NOT_READY,
//        NH_VULKAN_TIMEOUT,
//        NH_VULKAN_EVENT_SET,
//        NH_VULKAN_EVENT_RESET,
//        NH_VULKAN_INCOMPLETE,
//        NH_VULKAN_ERROR_OUT_OF_HOST_MEMORY,
//        NH_VULKAN_ERROR_OUT_OF_DEVICE_MEMORY,
//        NH_VULKAN_ERROR_INITIALIZATION_FAILED,
//        NH_VULKAN_ERROR_DEVICE_LOST,
//        NH_VULKAN_ERROR_MEMORY_MAP_FAILED,
//        NH_VULKAN_ERROR_LAYER_NOT_PRESENT,
//        NH_VULKAN_ERROR_EXTENSION_NOT_PRESENT,
//        NH_VULKAN_ERROR_FEATURE_NOT_PRESENT,
//        NH_VULKAN_ERROR_INCOMPATIBLE_DRIVER,
//        NH_VULKAN_ERROR_TOO_MANY_OBJECTS,
//        NH_VULKAN_ERROR_FORMAT_NOT_SUPPORTED,
//        NH_VULKAN_ERROR_FRAGMENTED_POOL,
//        NH_VULKAN_ERROR_UNKNOWN,
//        NH_VULKAN_ERROR_OUT_OF_POOL_MEMORY,
//        NH_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE,
//        NH_VULKAN_ERROR_FRAGMENTATION,
//        NH_VULKAN_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
//        NH_VULKAN_ERROR_SURFACE_LOST_KHR,
//        NH_VULKAN_ERROR_NATIVE_WINDOW_IN_USE_KHR,
//        NH_VULKAN_SUBOPTIMAL_KHR,
//        NH_VULKAN_ERROR_OUT_OF_DATE_KHR,
//        NH_VULKAN_ERROR_INCOMPATIBLE_DISPLAY_KHR,
//        NH_VULKAN_ERROR_VALIDATION_FAILED_EXT,
//        NH_VULKAN_ERROR_INVALID_SHADER_NV,
//        NH_VULKAN_ERROR_INCOMPATIBLE_VERSION_KHR,
//        NH_VULKAN_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT,
//        NH_VULKAN_ERROR_NOT_PERMITTED_EXT,
//        NH_VULKAN_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT,
//        NH_VULKAN_THREAD_IDLE_KHR,
//        NH_VULKAN_THREAD_DONE_KHR,
//        NH_VULKAN_OPERATION_DEFERRED_KHR,
//        NH_VULKAN_OPERATION_NOT_DEFERRED_KHR,
//        NH_VULKAN_PIPELINE_COMPILE_REQUIRED_EXT,
//        NH_VULKAN_ERROR_OUT_OF_POOL_MEMORY_KHR,
//        NH_VULKAN_ERROR_INVALID_EXTERNAL_HANDLE_KHR,
//        NH_VULKAN_RESULT_MAX_ENUM,
//
//        NH_HTML_ERROR_DOCTYPE_PARSING,
//        NH_HTML_ERROR_DOCUMENT_EMPTY,
//        NH_HTML_ERROR_INVALID_START_TAG,
//        NH_HTML_ERROR_ATTRIBUTE_UNKNOWN,
//
//    } NH_CORE_RESULT;

/** @} */

