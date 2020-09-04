/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_UTILITY_MACROS_H
#define NH_JS_UTILITY_MACROS_H

#define NH_JS_IS_BOOL(t)                                          \
(                                                                 \
    t == NH_JS_TYPE_BOOLEAN_TRUE || t == NH_JS_TYPE_BOOLEAN_FALSE \
)

#define NH_JS_TRUE(t)            \
(                                \
    t == NH_JS_TYPE_BOOLEAN_TRUE \
)

#define NH_JS_FALSE(t)            \
(                                 \
    t == NH_JS_TYPE_BOOLEAN_FALSE \
)

#define NH_JS_IS_SCRIPT(path)        \
(                                    \
    strlen(path) >= 3                \
    && path[strlen(path) - 1] == 's' \
    && path[strlen(path) - 2] == 'j' \
    && path[strlen(path) - 3] == '.' \
)

#endif
