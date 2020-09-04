/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef  NH_CHECK
#define NH_CHECK(...)                                                        \
{                                                                            \
    NH_RESULT results_p[] = {__VA_ARGS__};                                   \
    for (int i = 0; i < sizeof(results_p) / sizeof(results_p[0]); i++) {     \
        if (results_p[i] != NH_SUCCESS) {                                    \
            if (NH_ERROR == 0) {puts("");}                                   \
            NH_ERROR++;                                                      \
            printf("NETZHAUT: \e[1;31mERROR\e[0m %s\n", NH_RESULTS_PP[results_p[i]]); \
            return results_p[i];                                             \
        }                                                                    \
    }                                                                        \
}

#undef  NH_CHECK_NULL
#define NH_CHECK_NULL(...)                                                                     \
{                                                                                              \
    void *pointers_p[] = {__VA_ARGS__};                                                        \
    for (int i = 0; i < sizeof(pointers_p) / sizeof(pointers_p[0]); i++) {                     \
        if (pointers_p[i] == NULL || pointers_p[i] == NH_NULL) {return NH_ERROR_NULL_POINTER;} \
    }                                                                                          \
}

#undef  NH_CHECK_MEM
#define NH_CHECK_MEM(...)                                                                           \
{                                                                                                   \
    void *pointers_p[] = {__VA_ARGS__};                                                             \
    for (int i = 0; i < sizeof(pointers_p) / sizeof(pointers_p[0]); i++) {                          \
        if (pointers_p[i] == NULL || pointers_p[i] == NH_NULL) {return NH_ERROR_MEMORY_ALLOCATION;} \
    }                                                                                               \
}

#undef  NH_CHECK_VULKAN
#define NH_CHECK_VULKAN(...)                                                    \
{                                                                               \
    VkResult results_p[] = {__VA_ARGS__};                                       \
    for (int i = 0; i < sizeof(results_p) / sizeof(results_p[0]); i++) {        \
        if (results_p[i] != VK_SUCCESS) {return Nh_Vk_getResult(results_p[i]);} \
    }                                                                           \
}

#undef  NH_CHECK_TRUE
#define NH_CHECK_TRUE(...)                                               \
{                                                                        \
    NH_BOOL results_p[] = {__VA_ARGS__};                                 \
    for (int i = 0; i < sizeof(results_p) / sizeof(results_p[0]); i++) { \
        if (results_p[i] != NH_TRUE) {return NH_ERROR_EXPECTED_TRUE;}    \
    }                                                                    \
}

#undef  NH_CHECK_FALSE
#define NH_CHECK_FALSE(...)                                              \
{                                                                        \
    NH_BOOL results_p[] = {__VA_ARGS__};                                 \
    for (int i = 0; i < sizeof(results_p) / sizeof(results_p[0]); i++) { \
        if (results_p[i] != NH_FALSE) {return NH_ERROR_EXPECTED_FALSE;}  \
    }                                                                    \
}

#undef  NH_CHECK_TYPE
#define NH_CHECK_TYPE(type1, type2) if (type1 != type2) {return NH_ERROR_TYPE_MISMATCH;}

