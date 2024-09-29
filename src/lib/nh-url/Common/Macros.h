#ifndef NH_URL_COMMON_MACROS_H
#define NH_URL_COMMON_MACROS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS 

/** 
 * Netzhaut - Web Browser Engine 
 * Copyright (C) 2022  Dajo Frey 
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file. 
 */ 

#ifdef NH_LOG_FLOW 
    #include "Log.h" 
#endif 

#endif // DOXYGEN_SHOULD_SKIP_THIS 

// FLOW LOGGING ====================================================================================

#ifdef NH_LOG_FLOW 
    #define NH_URL_BEGIN() {_nh_url_logBegin(__FILE__, __func__);} 
#else 
    #define NH_URL_BEGIN() 
#endif 

#ifdef NH_LOG_FLOW 
    #define NH_URL_END(result) {_nh_url_logEnd(__FILE__, __func__); return result;} 
#else 
    #define NH_URL_END(result) {return result;} 
#endif 

#ifdef NH_LOG_FLOW 
    #define NH_URL_SILENT_END() {_nh_url_logEnd(__FILE__, __func__); return;} 
#else 
    #define NH_URL_SILENT_END() {return;} 
#endif 

#ifdef NH_LOG_FLOW 
    #define NH_URL_DIAGNOSTIC_END(result) {return _nh_url_logDiagnosticEnd(__FILE__, _
#else 
    #define NH_URL_DIAGNOSTIC_END(result) {return result;} 
#endif 

// ERROR LOGGING ===================================================================================

#define NH_URL_CHECK(checkable)                                             \
{                                                                           \
    NH_API_RESULT checkResult = checkable;                                  \
    if (checkResult != NH_API_SUCCESS) {NH_URL_DIAGNOSTIC_END(checkResult)} \
} 

#define  NH_URL_CHECK_2(checkReturn, checkable)                  \
{                                                                \
    NH_API_RESULT checkResult = checkable;                       \
    if (checkResult != NH_API_SUCCESS) {NH_URL_END(checkReturn)} \
} 

#define NH_URL_SILENT_CHECK(checkable)                       \
{                                                            \
    NH_API_RESULT checkResult = checkable;                   \
    if (checkResult != NH_API_SUCCESS) {NH_URL_SILENT_END()} \
} 

#define NH_URL_CHECK_NULL(checkable)                                              \
{                                                                                 \
    void *checkResult_p = checkable;                                              \
    if (checkResult_p == NULL) {NH_URL_DIAGNOSTIC_END(NH_API_ERROR_NULL_POINTER)} \
} 

#define NH_URL_CHECK_NULL_2(checkReturn, checkable)      \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_URL_END(checkReturn)} \
} 

#define NH_URL_CHECK_MEM(checkable)                                                    \
{                                                                                      \
    void *checkResult_p = checkable;                                                   \
    if (checkResult_p == NULL) {NH_URL_DIAGNOSTIC_END(NH_API_ERROR_MEMORY_ALLOCATION)} \
} 

#define NH_URL_CHECK_MEM_2(checkReturn, checkable)       \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_URL_END(checkReturn)} \
} 

#endif // NH_URL_COMMON_MACROS_H
