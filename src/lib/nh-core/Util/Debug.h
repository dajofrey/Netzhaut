#ifndef NH_CORE_UTIL_DEBUG_H
#define NH_CORE_UTIL_DEBUG_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include <stdio.h>

// DEFINES =========================================================================================

#define NH_CORE_CHECK(checkable) \
{ \
    NH_API_RESULT checkResult = checkable; \
    if (checkResult != NH_API_SUCCESS) { \
        char message_p[255]; \
        sprintf(message_p, "Check at line %d (%s) in %s failed.", __LINE__, __func__, __FILE__); \
        nh_core_debug(message_p); \
        return checkResult; \
    } \
} 
 
#define NH_CORE_CHECK_2(checkReturn, checkable) \
{ \
    NH_API_RESULT checkResult = checkable; \
    if (checkResult != NH_API_SUCCESS) { \
        char message_p[255]; \
        sprintf(message_p, "Check at line %d (%s) in %s failed.", __LINE__, __func__, __FILE__); \
        nh_core_debug(message_p); \
        return checkReturn; \
    } \
} 
 
#define NH_CORE_CHECK_NULL(checkable) \
{ \
    void *checkResult_p = checkable; \
    if (checkResult_p == NULL) { \
        char message_p[255]; \
        sprintf(message_p, "Check (NULL) at line %d (%s) in %s failed.", __LINE__, __func__, __FILE__); \
        nh_core_debug(message_p); \
        return NH_API_ERROR_NULL_POINTER; \
    } \
} 

#define NH_CORE_CHECK_NULL_2(checkReturn, checkable) \
{ \
    void *checkResult_p = checkable; \
    if (checkResult_p == NULL) { \
        char message_p[255]; \
        sprintf(message_p, "Check (NULL) at line %d (%s) in %s failed.", __LINE__, __func__, __FILE__); \
        nh_core_debug(message_p); \
        return checkReturn; \
    } \
} 
 
#define NH_CORE_CHECK_MEM(checkable) \
{ \
    void *checkResult_p = checkable; \
    if (checkResult_p == NULL) { \
        char message_p[255]; \
        sprintf(message_p, "Check (MEM) at line %d (%s) in %s failed.", __LINE__, __func__, __FILE__); \
        nh_core_debug(message_p); \
        return NH_API_ERROR_MEMORY_ALLOCATION; \
    } \
} 
 
#define NH_CORE_CHECK_MEM_2(checkReturn, checkable) \
{ \
    void *checkResult_p = checkable; \
    if (checkResult_p == NULL) { \
        char message_p[255]; \
        sprintf(message_p, "Check (MEM) at line %d (%s) in %s failed.", __LINE__, __func__, __FILE__); \
        nh_core_debug(message_p); \
        return checkReturn; \
    } \
} 

// FUNCTIONS =======================================================================================

void nh_core_debug(
    char *message_p
);

#endif // NH_CORE_UTIL_DEBUG_H
