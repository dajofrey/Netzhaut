#ifndef NH_NETWORK_COMMON_MACROS_H
#define NH_NETWORK_COMMON_MACROS_H

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
    #define NH_NETWORK_BEGIN() {_nh_network_logBegin(__FILE__, __func__);} 
#else 
    #define NH_NETWORK_BEGIN() 
#endif 

#ifdef NH_LOG_FLOW 
    #define NH_NETWORK_END(result) {_nh_network_logEnd(__FILE__, __func__); return result;} 
#else 
    #define NH_NETWORK_END(result) {return result;} 
#endif 

#ifdef NH_LOG_FLOW 
    #define NH_NETWORK_SILENT_END() {_nh_network_logEnd(__FILE__, __func__); return;} 
#else 
    #define NH_NETWORK_SILENT_END() {return;} 
#endif 

#ifdef NH_LOG_FLOW 
    #define NH_NETWORK_DIAGNOSTIC_END(result) {return _nh_network_logDiagnosticEnd(__FILE__, _
#else 
    #define NH_NETWORK_DIAGNOSTIC_END(result) {return result;} 
#endif 

// ERROR LOGGING ===================================================================================

#define NH_NETWORK_CHECK(checkable)                                             \
{                                                                           \
    NH_NETWORK_RESULT checkResult = checkable;                                  \
    if (checkResult != NH_NETWORK_SUCCESS) {NH_NETWORK_DIAGNOSTIC_END(checkResult)} \
} 

#define  NH_NETWORK_CHECK_2(checkReturn, checkable)                  \
{                                                                \
    NH_NETWORK_RESULT checkResult = checkable;                       \
    if (checkResult != NH_NETWORK_SUCCESS) {NH_NETWORK_END(checkReturn)} \
} 

#define NH_NETWORK_SILENT_CHECK(checkable)                       \
{                                                            \
    NH_NETWORK_RESULT checkResult = checkable;                   \
    if (checkResult != NH_NETWORK_SUCCESS) {NH_NETWORK_SILENT_END()} \
} 

#define NH_NETWORK_CHECK_NULL(checkable)                                              \
{                                                                                 \
    void *checkResult_p = checkable;                                              \
    if (checkResult_p == NULL) {NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_ERROR_NULL_POINTER)} \
} 

#define NH_NETWORK_CHECK_NULL_2(checkReturn, checkable)      \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_NETWORK_END(checkReturn)} \
} 

#define NH_NETWORK_CHECK_MEM(checkable)                                                    \
{                                                                                      \
    void *checkResult_p = checkable;                                                   \
    if (checkResult_p == NULL) {NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_ERROR_MEMORY_ALLOCATION)} \
} 

#define NH_NETWORK_CHECK_MEM_2(checkReturn, checkable)       \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_NETWORK_END(checkReturn)} \
} 

#endif // NH_NETWORK_COMMON_MACROS_H
