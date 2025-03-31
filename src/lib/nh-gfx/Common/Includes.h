#ifndef NH_GFX_COMMON_INCLUDES_H
#define NH_GFX_COMMON_INCLUDES_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../../nh-api/nh-gfx.h"
#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-core/Util/Debug.h"

#ifndef VOLK_VULKAN_H_PATH
    #define VOLK_VULKAN_H_PATH "Vulkan-Headers/include/vulkan/vulkan.h"
#endif

#ifdef INCLUDE_VOLK
    #include "../../../../external/volk/volk.h"
#endif

#ifdef NETZHAUT_OPENGL_IMPLEMENTATION
    #define SOGL_IMPLEMENTATION_X11
#endif

#ifdef __unix__
    #define SOGL_MAJOR_VERSION 4
    #define SOGL_MINOR_VERSION 6
    #include "simple-opengl-loader-master/simple-opengl-loader.h"
    #include "glx/glx.h"
#endif

#ifdef __APPLE__
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/CGLCurrent.h>  // For managing OpenGL contexts
#endif

#endif // NH_GFX_COMMON_INCLUDES_H
