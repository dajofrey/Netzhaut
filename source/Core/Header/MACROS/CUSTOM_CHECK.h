/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef  NH_CHECK
#define NH_CHECK(_return, ...)                                             \
{                                                                          \
    NH_RESULT results_p[] = {__VA_ARGS__};                                 \
    for (int i = 0; i < sizeof(results_p) / sizeof(results_p[0]); i++) {   \
        if (results_p[i] != NH_SUCCESS) {return _return;}                  \
    }                                                                      \
}

#undef  NH_CHECK_NULL
#define NH_CHECK_NULL(_return, ...)                                              \
{                                                                                \
    void *pointers_p[] = {__VA_ARGS__};                                          \
    for (int i = 0; i < sizeof(pointers_p) / sizeof(pointers_p[0]); i++) {       \
        if (pointers_p[i] == NULL || pointers_p[i] == NH_NULL) {return _return;} \
    }                                                                            \
}

#undef  NH_CHECK_MEM
#define NH_CHECK_MEM(_return, ...)                                               \
{                                                                                \
    void *pointers_p[] = {__VA_ARGS__};                                          \
    for (int i = 0; i < sizeof(pointers_p) / sizeof(pointers_p[0]); i++) {       \
        if (pointers_p[i] == NULL || pointers_p[i] == NH_NULL) {return _return;} \
    }                                                                            \
}

#undef  NH_CHECK_VULKAN
#define NH_CHECK_VULKAN(_return, ...)                                    \
{                                                                        \
    VkResult results_p[] = {__VA_ARGS__};                                \
    for (int i = 0; i < sizeof(results_p) / sizeof(results_p[0]); i++) { \
        if (results_p[i] != VK_SUCCESS) {return _return;}                \
    }                                                                    \
}

#undef  NH_CHECK_TRUE
#define NH_CHECK_TRUE(_return, ...)                                      \
{                                                                        \
    NH_BOOL results_p[] = {__VA_ARGS__};                                 \
    for (int i = 0; i < sizeof(results_p) / sizeof(results_p[0]); i++) { \
        if (results_p[i] != NH_TRUE) {return _return;}                   \
    }                                                                    \
}

#undef  NH_CHECK_FALSE
#define NH_CHECK_FALSE(_return, ...)                                     \
{                                                                        \
    NH_BOOL results_p[] = {__VA_ARGS__};                                 \
    for (int i = 0; i < sizeof(results_p) / sizeof(results_p[0]); i++) { \
        if (results_p[i] != NH_FALSE) {return _return;}                  \
    }                                                                    \
}

#undef  NH_CHECK_TYPE
#define NH_CHECK_TYPE(_return, type1, type2) if (type1 != type2) {return _return;}

