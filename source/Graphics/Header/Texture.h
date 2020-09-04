#ifndef NH_GFX_TEXTURE_H
#define NH_GFX_TEXTURE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "External/stb_image.h"

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/URI.h"

#include <stdbool.h>

typedef struct Nh_Window Nh_Window;
typedef struct Nh_CSS_Box Nh_CSS_Box;
typedef struct Nh_Tab Nh_Tab;
typedef struct NH_Vk_Texture NH_Vk_Texture;

#endif

/** @addtogroup GraphicsStructs Structs
 *  \ingroup Graphics
 *  @{
 */

    typedef struct Nh_Gfx_Texture {
        Nh_URI URI;    
        NH_Vk_Texture *Vulkan_p; 
        stbi_uc *pixels_p;         
        int width;                 
        int height;                
        int compression;           
        float gamma;
        void *data_p;               
        void **data_pp;            
    } Nh_Gfx_Texture;
    
/** @} */

/** @addtogroup GraphicsFunctions Functions
 *  \ingroup Graphics
 *  @{
 */

    NH_RESULT Nh_Gfx_initDefaultTextures(
        Nh_Tab *Tab_p
    );

    Nh_Gfx_Texture *Nh_Gfx_initTexture(
        Nh_Tab *Tab_p, Nh_URI *URI_p, char *chars_p
    );
  
    NH_RESULT Nh_Gfx_deleteTextures(
       Nh_Tab *Tab_p
    );
   
    NH_RESULT Nh_Gfx_deleteTexture(
        Nh_Tab *Tab_p, Nh_Gfx_Texture *Texture_p
    );
   
    Nh_Gfx_Texture *Nh_Gfx_getTexture(
        Nh_Tab *Tab_p, Nh_URI *URI_p 
    );
    
    float Nh_Gfx_getTextureWidth(
        Nh_Window *Window_p, Nh_Gfx_Texture *Texture_p
    );
    
    float Nh_Gfx_getTextureHeight(
        Nh_Window *Window_p, Nh_Gfx_Texture *Texture_p
    );

    void Nh_Gfx_getTextureScale(
        Nh_Window *Window_p, Nh_CSS_Box *Box_p, Nh_Gfx_Texture *Texture_p, float scale_p[2]
    );

    void Nh_Gfx_getScaledTextureRepeat(
        Nh_Window *Window_p, Nh_CSS_Box *Box_p, Nh_Gfx_Texture *Texture_p, float repeat_p[2]
    );

/** @} */

#endif
