#ifndef NH_CORE_PLATFORM_H
#define NH_CORE_PLATFORM_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#if defined(__APPLE__)
    #include <TargetConditionals.h>
#endif

#if defined(__unix__) && !defined(__APPLE__)
    #define NH_PLATFORM_UNIX 1
#elif defined(__APPLE__) && defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
    #define NH_PLATFORM_IOS 1
#elif defined(__APPLE__) && defined(TARGET_OS_OSX) && TARGET_OS_OSX
    #define NH_PLATFORM_MACOS 1
#endif

#endif // NH_CORE_PLATFORM_H
