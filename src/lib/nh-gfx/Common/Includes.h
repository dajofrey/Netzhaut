#ifndef NH_GFX_COMMON_INCLUDES_H
#define NH_GFX_COMMON_INCLUDES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

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
    #include "volk/volk.h"
#endif

#ifdef NETZHAUT_OPENGL_IMPLEMENTATION
    #define SOGL_IMPLEMENTATION_X11
#endif

//#ifdef NETZHAUT_OPENGL
    #define SOGL_MAJOR_VERSION 4
    #define SOGL_MINOR_VERSION 6
    #include "simple-opengl-loader-master/simple-opengl-loader.h"
    #include "glx/glx.h"
//#endif

#endif // DOXYGEN_SHOULD_SKIP_THIS

#endif // NH_GFX_COMMON_INCLUDES_H
