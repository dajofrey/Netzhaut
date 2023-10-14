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

#endif

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

    typedef struct nh_gfx_SurfaceRequirements {
        nh_opengl_SurfaceRequirements OpenGL;
    } nh_gfx_SurfaceRequirements;

    typedef struct nh_gfx_Surface {
        void *args_p;
        NH_SIGNAL signal;
        int renderRequests;
        NH_GFX_API_E api;
        nh_wsi_Window *Window_p;
        nh_vk_Surface Vulkan;
	nh_opengl_Surface OpenGL;
        nh_List Viewports;
        struct {
            nh_Color BackgroundColor;
        } Settings;
        struct {
            NH_BOOL halt;      
            NH_BOOL undoHalt;  
            NH_BOOL rendering; 
            NH_BOOL resize;   
            NH_BOOL ready;    
            NH_BOOL atomicRender;
        } Sync;
        struct {
            NH_BOOL getNextImage;       
            NH_BOOL skipRender;          
            NH_BOOL atomic;
        } Internal;
    } nh_gfx_Surface;           

/** @} */

/** @addtogroup lib_nhgfx_typedefs
 *  @{
 */

    typedef nh_gfx_SurfaceRequirements *(*nh_gfx_getSurfaceRequirements_f)(
    );

    typedef nh_gfx_Surface *(*nh_gfx_createSurface_f)(
        nh_wsi_Window *Window_p, NH_GFX_API_E api
    );

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    nh_gfx_Surface *nh_gfx_createSurface(
        nh_wsi_Window *Window_p, NH_GFX_API_E api
    );

    NH_GFX_RESULT nh_gfx_createSurfaceRequirements(
    );

    NH_GFX_RESULT nh_gfx_freeSurfaceRequirements(
    );

/** @} */

#endif 
