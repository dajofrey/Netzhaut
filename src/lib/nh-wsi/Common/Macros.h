#ifndef NH_WSI_COMMON_MACROS_H
#define NH_WSI_COMMON_MACROS_H

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
    #define NH_WSI_BEGIN() {_nh_wsi_logBegin(__FILE__, __func__);}
#else
    #define NH_WSI_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_WSI_END(result) {_nh_wsi_logEnd(__FILE__, __func__); return result;}
#else
    #define NH_WSI_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_WSI_SILENT_END() {_nh_wsi_logEnd(__FILE__, __func__); return;}
#else
    #define NH_WSI_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_WSI_DIAGNOSTIC_END(result) {return _nh_wsi_logDiagnosticEnd(__FILE__, __func__, result, __LINE__);}
#else
    #define NH_WSI_DIAGNOSTIC_END(result) {return result;} 
#endif

// ERROR LOGGING ===================================================================================

#define NH_WSI_CHECK(checkable)                                             \
{                                                                           \
    NH_WSI_RESULT_E checkResult = checkable;                                  \
    if (checkResult != NH_WSI_SUCCESS) {NH_WSI_DIAGNOSTIC_END(checkResult)} \
}

#define NH_WSI_CHECK_2(checkReturn, checkable)                   \
{                                                                \
    NH_WSI_RESULT_E checkResult = checkable;                       \
    if (checkResult != NH_WSI_SUCCESS) {NH_WSI_END(checkReturn)} \
}

#define NH_WSI_CHECK_NULL(checkable)                                              \
{                                                                                 \
    void *checkResult_p = checkable;                                              \
    if (checkResult_p == NULL) {NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_NULL_POINTER)} \
}

#define NH_WSI_CHECK_NULL_2(checkReturn, checkable)      \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_WSI_END(checkReturn)} \
}

#define NH_WSI_CHECK_MEM(checkable)                                                    \
{                                                                                      \
    void *checkResult_p = checkable;                                                   \
    if (checkResult_p == NULL) {NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_MEMORY_ALLOCATION)} \
}

#define NH_WSI_CHECK_MEM_2(checkReturn, checkable)       \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_WSI_END(checkReturn)} \
}

#endif
