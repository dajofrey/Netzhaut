#ifndef NH_GFX_SURFACE_H
#define NH_GFX_SURFACE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    
/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../Vulkan/Surface.h"
#include "../OpenGL/Surface.h"

#include "../../nh-wsi/Window/Window.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_gfx_Surface {
        void *args_p;
        NH_SIGNAL signal;
        int renderRequests;
        NH_API_GRAPHICS_BACKEND_E api;
        nh_wsi_Window *Window_p;
        nh_vk_Surface Vulkan;
	nh_opengl_Surface OpenGL;
        nh_List Viewports;
        struct {
            nh_Color BackgroundColor;
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

/** @} */

/** @addtogroup lib_nh-gfx_typedefs
 *  @{
 */

    typedef nh_gfx_Surface *(*nh_gfx_createSurface_f)(
        nh_wsi_Window *Window_p, NH_API_GRAPHICS_BACKEND_E api
    );

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    nh_gfx_Surface *nh_gfx_createSurface(
        nh_wsi_Window *Window_p, NH_API_GRAPHICS_BACKEND_E api
    );

/** @} */

#endif 
