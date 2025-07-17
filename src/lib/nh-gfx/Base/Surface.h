#ifndef NH_GFX_SURFACE_H
#define NH_GFX_SURFACE_H

// LICENSE =========================================================================================
 
/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

#if defined(_WIN32) || defined (WIN32) || defined(__unix__)
    #include "../Vulkan/Surface.h"
#endif

#include "../OpenGL/Surface.h"

#include "../../nh-wsi/Window/Window.h"
#include "../../nh-css/Properties/Color.h"

// ENUMS ==========================================================================================

typedef enum NH_GFX_API_E { 
    NH_GFX_API_VULKAN, 
    NH_GFX_API_OPENGL, 
    NH_GFX_API_METAL, 
    NH_GFX_API_DIRECTX, 
} NH_GFX_API_E; 

// STRUCTS =========================================================================================

typedef struct nh_gfx_Surface {
    void *args_p;
    NH_SIGNAL signal;
    int renderRequests;
    NH_GFX_API_E api;
    nh_wsi_Window *Window_p;
#if defined(__unix__)
    nh_gfx_VulkanSurface Vulkan;
#endif
    nh_gfx_OpenGLSurface OpenGL;
#if defined(__APPLE__)
//    #include "../Metal/Surface.h"
//    nh_gfx_MetalSurface Metal;
#endif
    nh_core_List Viewports;
    struct {
        nh_css_Color BackgroundColor;
    } Settings;
    struct {
        bool halt;      
        bool undoHalt;  
        bool rendering; 
        bool resize;   
        bool ready;    
        bool atomicRender;
    } Sync;
    struct {
        bool getNextImage;       
        bool skipRender;          
        bool atomic;
    } Internal;
} nh_gfx_Surface;           

// FUNCTIONS =======================================================================================

nh_gfx_Surface *nh_gfx_createSurface(
    nh_wsi_Window *Window_p
);

#endif 
