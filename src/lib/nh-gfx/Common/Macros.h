#ifndef NH_GFX_COMMON_MACROS_H
#define NH_GFX_COMMON_MACROS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"

#ifdef NH_LOG_FLOW
    #include "Log.h"
#endif

#endif

// FLOW LOGGING ====================================================================================

#ifdef NH_LOG_FLOW
    #define NH_GFX_BEGIN() {_nh_gfx_logBegin(__FILE__, __func__);}
#else
    #define NH_GFX_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_GFX_END(result) {_nh_gfx_logEnd(__FILE__, __func__); return result;}
#else
    #define NH_GFX_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_GFX_SILENT_END() {_nh_gfx_logEnd(__FILE__, __func__); return;}
#else
    #define NH_GFX_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_GFX_DIAGNOSTIC_END(result) {return _nh_gfx_logDiagnosticEnd(__FILE__, __func__, result, __LINE__);}
#else
    #define NH_GFX_DIAGNOSTIC_END(result) {return result;} 
#endif

// ERROR LOGGING ===================================================================================

#define NH_GFX_CHECK(checkable)                                             \
{                                                                           \
    NH_API_RESULT checkResult = checkable;                                  \
    if (checkResult != NH_API_SUCCESS) {NH_GFX_DIAGNOSTIC_END(checkResult)} \
}

#define NH_GFX_CHECK_2(checkReturn, checkable)                   \
{                                                                \
    NH_API_RESULT checkResult = checkable;                       \
    if (checkResult != NH_API_SUCCESS) {NH_GFX_END(checkReturn)} \
}

#define NH_GFX_CHECK_NULL(checkable)                                              \
{                                                                                 \
    void *checkResult_p = checkable;                                              \
    if (checkResult_p == NULL) {NH_GFX_DIAGNOSTIC_END(NH_API_ERROR_NULL_POINTER)} \
}

#define NH_GFX_CHECK_NULL_2(checkReturn, checkable)      \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_GFX_END(checkReturn)} \
}

#define NH_GFX_CHECK_MEM(checkable)                                                    \
{                                                                                      \
    void *checkResult_p = checkable;                                                   \
    if (checkResult_p == NULL) {NH_GFX_DIAGNOSTIC_END(NH_API_ERROR_MEMORY_ALLOCATION)} \
}

#define NH_GFX_CHECK_MEM_2(checkReturn, checkable)       \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_GFX_END(checkReturn)} \
}

#undef  NH_GFX_CHECK_VULKAN
#define NH_GFX_CHECK_VULKAN(checkable)                                                   \
{                                                                                        \
    VkResult checkResult = checkable;                                                    \
    if (checkResult != VK_SUCCESS) {NH_GFX_DIAGNOSTIC_END(nh_vk_getResult(checkResult))} \
}

#undef  NH_GFX_CHECK_VULKAN_2
#define NH_GFX_CHECK_VULKAN_2(checkReturn, checkable)        \
{                                                            \
    VkResult checkResult = checkable;                        \
    if (checkResult != VK_SUCCESS) {NH_GFX_END(checkReturn)} \
}

#endif
