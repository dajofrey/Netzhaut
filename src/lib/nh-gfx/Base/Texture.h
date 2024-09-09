#ifndef NH_GFX_TEXTURE_H
#define NH_GFX_TEXTURE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Window.h"
#include "Tab.h"
#include "Content.h"

#include "External/stb_image.h"

#include "../Common/Includes.h"

typedef struct nh_html_PixelBox nh_html_PixelBox;
typedef struct nh_vk_Texture nh_vk_Texture;

#endif

/** @addtogroup lib_nh-core_structs
 *  @{
 */

    typedef struct nh_Texture {
        NH_BYTE *id_p;
        nh_vk_Texture *Vulkan_p; 
        stbi_uc *pixels_p;         
        NH_PIXEL width;                 
        NH_PIXEL height;                
        int compression;           
        float gamma;
        void *data_p;               
    } nh_Texture;
    
///** @} */
//
///** @addtogroup lib_nh-core_functions
// *  @{
// */
//
//    NH_CORE_RESULT nh_core_initDefaultTextures(
//        nh_Content *Content_p
//    );
//
//    nh_Texture *nh_core_initTexture(
//        nh_Content *Content_p, nh_URI *URI_p, char *chars_p
//    );
//  
//    NH_CORE_RESULT nh_deleteTextures(
//       nh_Content *Content_p
//    );
//   
//    NH_CORE_RESULT nh_deleteTexture(
//        nh_Content *Content_p, nh_Texture *Texture_p
//    );
//   
//    NH_CORE_RESULT nh_core_updateTexture(
//        nh_Content *Content_p, nh_Texture *Texture_p
//    );
//
//    NH_BOOL nh_textureLoaded(
//        nh_Content *Content_p, char *location_p, char *base_p
//    );
//
//    nh_Texture *nh_core_getTextureFromLocation(
//        nh_Content *Content_p, char *location_p, char *base_p
//    );
//
//    nh_Texture *nh_core_getTexture(
//        nh_Content *Content_p, nh_URI *URI_p 
//    );
//    
//    float nh_core_getTextureWidth(
//        nh_Content *Content_p, nh_Texture *Texture_p
//    );
//    
//    float nh_core_getTextureHeight(
//        nh_Content *Content_p, nh_Texture *Texture_p
//    );
//
////    void nh_core_getTextureScale(
////        nh_Content *Content_p, nh_html_PixelBox *Box_p, nh_Texture *Texture_p, float scale_p[2]
////    );
////
////    void nh_core_getScaledTextureRepeat(
////        nh_Content *Content_p, nh_html_PixelBox *Box_p, nh_Texture *Texture_p, float repeat_p[2]
////    );
//
///** @} */
//
//#endif
