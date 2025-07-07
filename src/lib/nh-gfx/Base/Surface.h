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

#include "../Vulkan/Surface.h"
#include "../OpenGL/Surface.h"
#include "../Metal/Surface.h"

#include "../../nh-wsi/Window/Window.h"
#include "../../nh-css/Properties/Color.h"

// STRUCTS =========================================================================================

typedef struct nh_gfx_Surface {
    void *args_p;
    NH_SIGNAL signal;
    int renderRequests;
    NH_API_GRAPHICS_BACKEND_E api;
    nh_wsi_Window *Window_p;
    nh_gfx_VulkanSurface Vulkan;
    nh_gfx_OpenGLSurface OpenGL;
    nh_gfx_MetalSurface Metal;
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
    nh_wsi_Window *Window_p, NH_API_GRAPHICS_BACKEND_E api
);

#endif 
