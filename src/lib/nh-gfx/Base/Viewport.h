#ifndef NH_GFX_VIEWPORT_H
#define NH_GFX_VIEWPORT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Surface.h"

#include "../Vulkan/Viewport.h"
#include "../OpenGL/Viewport.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_enums
 *  @{
 */

    typedef enum NH_GFX_VIEWPORT_OWNER {
        NH_GFX_VIEWPORT_OWNER_CSS,
        NH_GFX_VIEWPORT_OWNER_TERMINAL,
        NH_GFX_VIEWPORT_OWNER_USER,
        NH_GFX_VIEWPORT_OWNER_NONE,
    } NH_GFX_VIEWPORT_OWNER;

/** @} */

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_gfx_ViewportOwner {
        NH_GFX_VIEWPORT_OWNER type;
        void *p;
    } nh_gfx_ViewportOwner;

    typedef struct nh_gfx_Viewport {
        nh_gfx_ViewportOwner Owner;
        nh_gfx_Surface *Surface_p;
	nh_vk_Viewport Vulkan;
	nh_opengl_Viewport OpenGL;
        struct {
            nh_api_PixelSize Size;
            nh_api_PixelPosition Position;    
            nh_Color ClearColor;
            nh_Color BorderColor;
            bool border;
            int borderWidth;
            unsigned int priority;     /**<Defines rendering order. 0 is highest priority aka frontmost viewport. Lesser priorities ascend to +infinity.*/
        } Settings;
        struct {
            int newestBuffers_p[3];    
            int submittedBuffers_p[3]; 
            bool atomicRecording;      
            bool render;              
            bool atomic;
        } Sync;
    } nh_gfx_Viewport;

/** @} */

/** @addtogroup lib_nh-gfx_typedefs
 *  @{
 */

    typedef nh_gfx_Viewport *(*nh_gfx_createViewport_f)(
        nh_gfx_Surface *Surface_p, nh_api_PixelPosition Position, nh_api_PixelSize Size
    );

    typedef NH_API_RESULT (*nh_gfx_configureViewport_f)(
        nh_gfx_Viewport *Viewport_p, nh_api_PixelPosition Position, nh_api_PixelSize Size
    );

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_API_RESULT nh_gfx_beginRecording(
        nh_gfx_Viewport *Viewport_p
    );
    
    NH_API_RESULT nh_gfx_endRecording(
        nh_gfx_Viewport *Viewport_p, bool blockUntilRender
    );

    NH_API_RESULT nh_gfx_claimViewport(
        nh_gfx_Viewport *Viewport_p, NH_GFX_VIEWPORT_OWNER owner, void *owner_p
    );

    void nh_gfx_destroyViewport(
        nh_gfx_Surface *Surface_p, nh_gfx_Viewport *Viewport_p
    );

/** @} */

#endif
