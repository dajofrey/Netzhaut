//// LICENSE NOTICE ==================================================================================
//
///**
// * Netzhaut - Web Browser Engine
// * Copyright (C) 2022  Dajo Frey
// * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
// */
//
//// INCLUDES ========================================================================================
//
//#define STB_IMAGE_IMPLEMENTATION
//
//#include "Texture.h"
//#include "Memory.h"
//#include "Tab.h"
//#include "LinkedList.h"
//#include "Font.h"
//
//#include "../nh-gfx/Vulkan/Header/Texture.h"
//
//#include "Common/Macros.h"
//#include NH_FLOW
//#include NH_GFX_DEFAULT_CHECK
//
//#include <string.h>
//
//// DECLARE =========================================================================================
//
//static nh_Texture *nh_core_allocateTexture(
//    nh_Content *Content_p, nh_URI URI
//);
//
//static nh_Texture *nh_softLoadTexture(
//    nh_Texture *Texture_p
//);
//static void nh_softFreeTexture(
//    nh_Texture *Texture_p
//);
//
//// INIT ============================================================================================
//
//NH_GFX_RESULT nh_core_initDefaultTextures(
//    nh_Content *Content_p)
//{
//NH_GFX_BEGIN()
//
////    nh_Texture *Dummy_p = nh_core_initTexture(Content_p, NULL, NULL);
////    NH_GFX_CHECK_NULL(Dummy_p)
////     
////    switch (Content_p->Tab_p->GPU.API)
////    {
////        case NH_API_VULKAN : NH_GFX_CHECK(nh_vk_hardLoadTexture(Content_p->Tab_p->GPU.Pointer, Dummy_p)) break;
////        case NH_API_DIRECTX : break;
////    }
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//nh_gfx_Texture *nh_gfx_initTexture(
//    nh_Content *Content_p, NH_BYTE *id_p)
//{
//NH_GFX_BEGIN()
//
//#include NH_GFX_CUSTOM_CHECK
//
//    NH_GFX_CHECK_NULL(NULL, Content_p)
//
//    nh_URI URI = URI_p == NULL ? nh_core_createURI(location_p, NULL, NULL, -1) : nh_copyURI(URI_p);
//
//    nh_Texture *Texture_p = nh_core_getTexture(Content_p, &URI);
//    if (Texture_p != NULL) {
//        nh_core_freeURI(&URI); 
//        NH_GFX_END(Texture_p)
//    }
//
//NH_GFX_END(nh_softLoadTexture(nh_core_allocateTexture(Content_p, URI)))
//}
//
//static nh_Texture *nh_core_allocateTexture(
//    nh_Content *Content_p, nh_URI URI)
//{
//NH_GFX_BEGIN()
//
//    nh_Texture *Texture_p = nh_core_allocate(sizeof(nh_Texture));
//    NH_GFX_CHECK_NULL(NULL, Texture_p)
//    NH_GFX_CHECK(NULL, nh_core_appendToLinkedList(&Content_p->Textures, Texture_p))
//
//    Texture_p->gamma    = NH_CONFIG.Settings.gamma;
//    Texture_p->pixels_p = NULL;
//    Texture_p->data_p   = NULL;
//    Texture_p->Vulkan_p = NULL;
//    Texture_p->URI      = URI;
//    Texture_p->width    = 0;
//    Texture_p->height   = 0;
//
//NH_GFX_END(Texture_p)
//}
//
//// PREPARE =========================================================================================
//
//static nh_gfx_Texture *nh_gfx_prepareFontTexture(
//    nh_gfx_Surface *Surface_p, nh_gfx_Font *Font_p)
//{
//NH_GFX_BEGIN()
//
//#include NH_GFX_CUSTOM_CHECK
//
//    NH_GFX_CHECK_NULL(NULL, Texture_p)
//
//    switch (Surface_p->api)
//    {
//        case NH_GFX_API_VULKAN :
//            for (int i = 0; i < Surface_p->Vulkan_p->GPU_p->Textures.size; ++i) {
//
//            }
//    }
//
//    // catch already loaded texture
//    if (Texture_p->pixels_p != NULL || Texture_p->data_p != NULL) {NH_GFX_END(NULL)}
//
//    NH_GFX_CHECK(NULL, nh_gfx_loadFont(Font_p))
//
//    Texture_p->width    = Font_p->Atlas.width;
//    Texture_p->height   = Font_p->Atlas.height;
//    Texture_p->pixels_p = Font_p->Atlas.data_p;
//    NH_GFX_CHECK_NULL(NULL, Texture_p->pixels_p)
//
//#include NH_GFX_DEFAULT_CHECK
//
//NH_GFX_END(Texture_p)
//}
//
//static nh_gfx_Texture *nh_gfx_softLoadTexture(
//    nh_gfx_Texture *Texture_p)
//{
//NH_GFX_BEGIN()
//
//    NH_GFX_CHECK_NULL(NULL, Texture_p)
//
//    // catch already loaded texture
//    if (Texture_p->pixels_p != NULL || Texture_p->data_p != NULL) {NH_GFX_END(NULL)}
//
//    NH_MEDIA mediaType = nh_core_getMediaType(&Texture_p->URI);
//
//    switch (mediaType)
//    {
//        case NH_MEDIA_IMAGE_JPEG :
//        case NH_MEDIA_IMAGE_PNG  :
//        {
//            NH_BYTE *data_p = nh_core_getPayload(&Texture_p->URI);
//            Texture_p->pixels_p = stbi_load_from_memory(data_p, Texture_p->URI.size, &Texture_p->width, &Texture_p->height, &Texture_p->compression, STBI_rgb_alpha);
//            nh_core_freePayload(&Texture_p->URI);
//            NH_GFX_CHECK_NULL(NULL, Texture_p->pixels_p)
//
//            break;
//        }
//        case NH_MEDIA_FONT_TTF : 
//        case NH_MEDIA_FONT_OTF :
//        { 
//            nh_Font *Font_p = nh_core_getFontUsingURI(&Texture_p->URI);
//            NH_GFX_CHECK(NULL, nh_core_loadFont(Font_p))
//
//            Texture_p->width    = Font_p->Atlas.width;
//            Texture_p->height   = Font_p->Atlas.height;
//            Texture_p->pixels_p = Font_p->Atlas.data_p;
//            NH_GFX_CHECK_NULL(NULL, Texture_p->pixels_p)
//
//            break;
//        }
////        case -1 :
////        {
////            Texture_p->data_p = nh_core_allocate(sizeof(uint32_t) * 10000);
////            NH_GFX_CHECK_NULL(NULL, Texture_p->data_p)
////            for (int i = 0; i < 10000; ++i) {((uint32_t*)Texture_p->data_p)[i] = 255;}
////
////            Texture_p->width  = 100;
////            Texture_p->height = 100;
//// 
////            break;
////        }
//    }
//
//#include NH_GFX_DEFAULT_CHECK
//
//NH_GFX_END(Texture_p)
//}
//
//static void nh_softFreeTexture(
//    nh_Texture *Texture_p)
//{
//NH_GFX_BEGIN()
//
//    if (Texture_p == NULL) {NH_GFX_SILENT_END()}
//
//    NH_MEDIA mediaType = nh_core_getMediaType(&Texture_p->URI);
//   
//    switch (mediaType)
//    {
//        case NH_MEDIA_IMAGE_JPEG :
//        case NH_MEDIA_IMAGE_PNG  :
//
//            stbi_image_free(Texture_p->pixels_p);
//            break;
//
//        case NH_MEDIA_FONT_TTF : 
//        case NH_MEDIA_FONT_OTF :
//            
//            Texture_p->pixels_p = NULL;
//            Texture_p->data_p = NULL;
//            break;
//
//        case -1 : 
//        
//            nh_core_free(Texture_p->data_p);
//            break;
//    }
//
//NH_GFX_SILENT_END()
//}
//
//// DELETE ==========================================================================================
//
//NH_GFX_RESULT nh_deleteTextures(
//    nh_Content *Content_p)
//{
//NH_GFX_BEGIN()
//
//    while (Content_p->Textures.count > 0) {
//        NH_GFX_CHECK(nh_deleteTexture(Content_p, nh_core_getFromLinkedList(&Content_p->Textures, 0)))
//    }
//
//    nh_core_destroyLinkedList(&Content_p->Textures, true);
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//NH_GFX_RESULT nh_deleteTexture(
//    nh_Content *Content_p, nh_Texture *Texture_p)
//{
//NH_GFX_BEGIN()
//
////    NH_GFX_CHECK_NULL(Content_p)
////    NH_GFX_CHECK_NULL(Texture_p)
////
////    int index = -1;
////    for (int i = 0; i < Content_p->Textures.count; ++i) {
////        if (Texture_p == nh_core_getFromLinkedList(&Content_p->Textures, i)) {index = i; break;}
////    }
////
////    if (index == -1) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
////
////    if (Texture_p->Vulkan_p != NULL) {nh_vk_destroyTexture(Content_p->Tab_p->GPU.Pointer, Texture_p->Vulkan_p);}
////
////    nh_softFreeTexture(Texture_p);
////    nh_core_freeURI(&Texture_p->URI);
////    nh_core_removeFromLinkedList(&Content_p->Textures, index, true);
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}
//
//// UPDATE ==========================================================================================
//
//NH_GFX_RESULT nh_core_updateTexture(
//    nh_Content *Content_p, nh_Texture *Texture_p)
//{
//NH_GFX_BEGIN()
//
////    if (Content_p == NULL || Texture_p == NULL) {NH_GFX_END(NH_SUCCESS)}
////
////    nh_softFreeTexture(Texture_p);
////    NH_GFX_CHECK_NULL(nh_softLoadTexture(Texture_p))
////
////    if (Texture_p->Vulkan_p != NULL) {
////        NH_GFX_CHECK(nh_vk_hardReloadTexture(Content_p->Tab_p->GPU.Pointer, Texture_p))
////    }
//
//NH_GFX_END(NH_SUCCESS)
//}
//
//// CHECK ===========================================================================================
//
//NH_BOOL nh_textureLoaded(
//    nh_Content *Content_p, NH_BYTE *location_p, NH_BYTE *base_p)
//{
//NH_GFX_BEGIN()
//
//    nh_Texture *Texture_p = nh_core_getTextureFromLocation(Content_p, location_p, base_p);
//
//NH_GFX_END(Texture_p != NULL && Texture_p->pixels_p != NULL)
//}
//
//// GET =============================================================================================
//
//nh_Texture *nh_core_getTextureFromLocation(
//    nh_Content *Content_p, NH_BYTE *location_p, NH_BYTE *base_p)
//{
//NH_GFX_BEGIN()
//
//    for (int i = 0; i < Content_p->Textures.count; ++i) 
//    {
//        nh_Texture *Texture_p = nh_core_getFromLinkedList(&Content_p->Textures, i);
//        if (nh_correspondingURI(&Texture_p->URI, location_p, base_p)) {NH_GFX_END(Texture_p)}
//    }
//
//NH_GFX_END(NULL)
//}
//
//nh_Texture *nh_core_getTexture(
//    nh_Content *Content_p, nh_URI *URI_p)
//{
//NH_GFX_BEGIN()
//
//    if (URI_p == NULL) {NH_GFX_END(nh_core_getFromLinkedList(&Content_p->Textures, 0))}
//
//    for (int i = 0; i < Content_p->Textures.count; ++i) 
//    {
//        nh_Texture *Texture_p = nh_core_getFromLinkedList(&Content_p->Textures, i);
//        if (nh_equivalentURI(URI_p, &Texture_p->URI)) {NH_GFX_END(Texture_p)}
//    }
//
//NH_GFX_END(NULL)
//}
//
//float nh_core_getTextureWidth(
//    nh_Content *Content_p, nh_Texture *Texture_p)
//{
//NH_GFX_BEGIN()
//
//    float width = (((float)Texture_p->width) / (float)Content_p->Tab_p->Size.width) * 2.0f;
//
//NH_GFX_END(width)
//}
//
//float nh_core_getTextureHeight(
//    nh_Content *Content_p, nh_Texture *Texture_p)
//{
//NH_GFX_BEGIN()
//
//    float height = (((float)Texture_p->height) / (float)Content_p->Tab_p->Size.height) * 2.0f;
//
//NH_GFX_END(height)
//}
//
////void nh_core_getTextureScale(
////    nh_Content *Content_p, nh_html_PixelBox *Box_p, nh_Texture *Texture_p, float scale_p[2])
////{
////NH_GFX_BEGIN()
////    
//////    scale_p[0] = ((float)nh_core_getTextureWidth(Content_p, Texture_p)) / (float)Box_p->Size.width;
//////    scale_p[1] = ((float)nh_core_getTextureHeight(Content_p, Texture_p)) / (float)Box_p->Size.height;
////
////NH_GFX_SILENT_END()
////}
////
////void nh_core_getScaledTextureRepeat(
////    nh_Content *Content_p, nh_html_PixelBox *Box_p, nh_Texture *Texture_p, float repeat_p[2])
////{
////NH_GFX_BEGIN()
////    
////    float scale_p[2];
////    nh_core_getTextureScale(Content_p, Box_p, Texture_p, scale_p);
////
////    if (scale_p[1] > scale_p[0]) {
////        repeat_p[0] = scale_p[1];
////        repeat_p[1] = 1.0f;
////    }
////    else {
////        repeat_p[0] = 1.0f;
////        repeat_p[1] = scale_p[0];
////    }
////
////NH_GFX_SILENT_END()
////}
//
