#ifndef NH_MAKE_COMMON_MACROS_H
#define NH_MAKE_COMMON_MACROS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Result.h"
#include "../UI/Message.h"

#endif

// FLOW LOGGING ====================================================================================

#ifdef NH_LOG_FLOW
    #define NH_MAKE_BEGIN() {_nh_make_logBegin(__FILE__, __func__);}
#else
    #define NH_MAKE_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_MAKE_END(result) {_nh_make_logEnd(__FILE__, __func__); return result;}
#else
    #define NH_MAKE_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_MAKE_SILENT_END() {_nh_make_logEnd(__FILE__, __func__); return;}
#else
    #define NH_MAKE_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_MAKE_DIAGNOSTIC_END(result) {return _nh_make_logDiagnosticEnd(__FILE__, __func__, result, __LINE__);}
#else
    #define NH_MAKE_DIAGNOSTIC_END(result) {return result;} 
#endif

// ERROR LOGGING ===================================================================================

#undef  NH_MAKE_CHECK
#define NH_MAKE_CHECK(checkable)                                                       \
{                                                                                      \
    NH_MAKE_RESULT checkResult = checkable;                                            \
    if (checkResult != NH_MAKE_SUCCESS) {                                              \
        nh_make_messagef("-> \e[1;31mERROR\e[0m %s", NH_MAKE_RESULTS_PP[checkResult]); \
        return checkResult;                                                            \
    }                                                                                  \
}

#undef  NH_MAKE_CHECK_2
#define NH_MAKE_CHECK_2(checkReturn, checkable)                                        \
{                                                                                      \
    NH_MAKE_RESULT checkResult = checkable;                                            \
    if (checkResult != NH_MAKE_SUCCESS) {                                              \
        nh_make_messagef("-> \e[1;31mERROR\e[0m %s", NH_MAKE_RESULTS_PP[checkResult]); \
        return checkReturn;                                                            \
    }                                                                                  \
}

#undef  NH_MAKE_CHECK_NULL
#define NH_MAKE_CHECK_NULL(checkable)                                                                 \
{                                                                                                     \
    void *checkResult_p = checkable;                                                                  \
    if (checkResult_p == NULL) {                                                                      \
        nh_make_messagef("-> \e[1;31mERROR\e[0m %s", NH_MAKE_RESULTS_PP[NH_MAKE_ERROR_NULL_POINTER]); \
        return NH_MAKE_ERROR_NULL_POINTER;                                                            \
    }                                                                                                 \
}

#undef  NH_MAKE_CHECK_NULL_2
#define NH_MAKE_CHECK_NULL_2(checkReturn, checkable)                                                  \
{                                                                                                     \
    void *checkResult_p = checkable;                                                                  \
    if (checkResult_p == NULL) {                                                                      \
        nh_make_messagef("-> \e[1;31mERROR\e[0m %s", NH_MAKE_RESULTS_PP[NH_MAKE_ERROR_NULL_POINTER]); \
        return checkReturn;                                                                           \
    }                                                                                                 \
}

#endif
