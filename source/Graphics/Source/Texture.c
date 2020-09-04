// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#define STB_IMAGE_IMPLEMENTATION

#include "../Header/Texture.h"
#include "../Header/Macros.h"

#include "../Vulkan/Header/Texture.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/List.h"
#include "../../Core/Header/External/freetype-gl.h"
#include "../../Core/Header/Config.h"

#include "../../CSS/Header/Box.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// DECLARE =========================================================================================

static Nh_Gfx_Texture *Nh_Gfx_allocateTexture(
    Nh_Tab *Tab_p, Nh_URI URI
);

static Nh_Gfx_Texture *Nh_Gfx_softLoadTexture(
    Nh_Gfx_Texture *Texture_p
);
static void Nh_Gfx_softFreeTexture(
    Nh_Gfx_Texture *Texture_p
);

// INIT ============================================================================================

NH_RESULT Nh_Gfx_initDefaultTextures(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Nh_Gfx_Texture *Dummy_p = Nh_Gfx_initTexture(Tab_p, NULL, NULL);
    NH_CHECK_NULL(Dummy_p)
     
    switch (Tab_p->Window_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_hardLoadTexture(Tab_p->Window_p->GPU.Pointer, Dummy_p)) break;
        case NH_API_OPENGL : break;
    }

NH_END(NH_SUCCESS)
}

Nh_Gfx_Texture *Nh_Gfx_initTexture(
    Nh_Tab *Tab_p, Nh_URI *URI_p, char *chars_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NULL, Tab_p)

    Nh_URI URI = URI_p == NULL ? Nh_createURI(chars_p, NULL, NULL) : Nh_copyURI(URI_p);

    Nh_Gfx_Texture *Texture_p = Nh_Gfx_getTexture(Tab_p, &URI);
    if (Texture_p != NULL) {
        Nh_freeURI(URI); 
        NH_END(Texture_p)
    }

NH_END(Nh_Gfx_softLoadTexture(Nh_Gfx_allocateTexture(Tab_p, URI)))
}

static Nh_Gfx_Texture *Nh_Gfx_allocateTexture(
    Nh_Tab *Tab_p, Nh_URI URI)
{
NH_BEGIN()

    Nh_Gfx_Texture *Texture_p = Nh_allocate(sizeof(Nh_Gfx_Texture));
    NH_CHECK_NULL(NULL, Texture_p)
    NH_CHECK(NULL, Nh_addListItem(&Tab_p->Textures, Texture_p))

    Texture_p->gamma    = Nh_getConfig()->Settings.gamma;
    Texture_p->pixels_p = NULL;
    Texture_p->data_pp  = NULL;
    Texture_p->data_p   = NULL;
    Texture_p->Vulkan_p = NULL;
    Texture_p->URI      = URI;
    Texture_p->width    = 0;
    Texture_p->height   = 0;

NH_END(Texture_p)
}

// SOFT ============================================================================================

static Nh_Gfx_Texture *Nh_Gfx_softLoadTexture(
    Nh_Gfx_Texture *Texture_p)
{
NH_BEGIN()

    NH_CHECK_NULL(NULL, Texture_p)

    // catch already loaded texture
    if (Texture_p->pixels_p != NULL || Texture_p->data_pp != NULL || Texture_p->data_p != NULL) {NH_END(NULL)}

    NH_MEDIA mediaType = Nh_getMediaType(Texture_p->URI);

    switch (mediaType)
    {
        case NH_MEDIA_IMAGE_JPEG :
        case NH_MEDIA_IMAGE_PNG  :
        {
            long size = 0;
            char *data_p = Nh_getData(Texture_p->URI, &size);
            Texture_p->pixels_p = stbi_load_from_memory(data_p, size, &Texture_p->width, &Texture_p->height, &Texture_p->compression, STBI_rgb_alpha);
            Nh_freeData(Texture_p->URI);
            NH_CHECK_NULL(NULL, Texture_p->pixels_p)

            break;
        }
        case NH_MEDIA_FONT_TTF : 
        case NH_MEDIA_FONT_OTF :
        { 
            long size = 0;
            char *data_p = Nh_getData(Texture_p->URI, &size);

            texture_atlas_t *atlas_p = texture_atlas_new(512, 512, 1);
            texture_font_t *font_p   = texture_font_new_from_memory(atlas_p, 48, data_p, size);
            font_p->rendermode       = RENDER_SIGNED_DISTANCE_FIELD;

            char *ascii_p = "!\"#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmno pqrstuvwxyz{|}~ ";

            float max = 0.0f;
            for (int i = 0; i < strlen(ascii_p); ++i) {
                texture_glyph_t *glyph_p = texture_font_get_glyph(font_p, ascii_p + i);
                if (glyph_p != NULL && ((float)glyph_p->height > max)) {max = (float)glyph_p->height;}
            }

            Texture_p->width    = atlas_p->width;
            Texture_p->height   = atlas_p->height;
            Texture_p->pixels_p = atlas_p->data;
            NH_CHECK_NULL(NULL, Texture_p->pixels_p)

            Texture_p->data_pp = Nh_allocate(sizeof(void*) * 3);
            NH_CHECK_NULL(NULL, Texture_p->data_pp)
            Texture_p->data_pp[0] = atlas_p;
            Texture_p->data_pp[1] = font_p; 

            Texture_p->data_pp[2] = Nh_allocate(sizeof(float));
            NH_CHECK_NULL(NULL, Texture_p->data_pp[2])
            *((float*)Texture_p->data_pp[2]) = max;

            Nh_freeData(Texture_p->URI);

            break;
        }
        case -1 :
        {
            Texture_p->data_p = Nh_allocate(sizeof(uint32_t) * 10000);
            NH_CHECK_NULL(NULL, Texture_p->data_p)
            for (int i = 0; i < 10000; ++i) {((uint32_t*)Texture_p->data_p)[i] = 255;}

            Texture_p->width  = 100;
            Texture_p->height = 100;
 
            break;
        }
    }

#include NH_DEFAULT_CHECK

NH_END(Texture_p)
}

static void Nh_Gfx_softFreeTexture(
    Nh_Gfx_Texture *Texture_p)
{
NH_BEGIN()

    if (Texture_p == NULL) {NH_SILENT_END()}

    NH_MEDIA mediaType = Nh_getMediaType(Texture_p->URI);
   
    switch (mediaType)
    {
        case NH_MEDIA_IMAGE_JPEG :
        case NH_MEDIA_IMAGE_PNG  :

            stbi_image_free(Texture_p->pixels_p);
            break;

        case NH_MEDIA_FONT_TTF : 
        case NH_MEDIA_FONT_OTF : ;
            
            if (Texture_p->data_pp != NULL) 
            {
                texture_atlas_delete(Texture_p->data_pp[0]);
                texture_font_delete(Texture_p->data_pp[1]);
                Nh_free(Texture_p->data_pp[2]);
                Nh_free(Texture_p->data_pp);
            }
            break;

        case -1 : 
        
            Nh_free(Texture_p->data_p);
            break;
    }

    Nh_freeURI(Texture_p->URI);

NH_SILENT_END()
}

// DELETE ==========================================================================================

NH_RESULT Nh_Gfx_deleteTextures(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    while (Tab_p->Textures.count > 0) {
        NH_CHECK(Nh_Gfx_deleteTexture(Tab_p, Nh_getListItem(&Tab_p->Textures, 0)))
    }

    Nh_destroyList(&Tab_p->Textures, true);

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Gfx_deleteTexture(
    Nh_Tab *Tab_p, Nh_Gfx_Texture *Texture_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p, Texture_p)

    int index = -1;
    for (int i = 0; i < Tab_p->Textures.count; ++i) {
        if (Texture_p == Nh_getListItem(&Tab_p->Textures, i)) {index = i; break;}
    }

    if (index == -1) {NH_END(NH_ERROR_BAD_STATE)}

    if (Texture_p->Vulkan_p != NULL) {Nh_Vk_destroyTexture(Tab_p->Window_p->GPU.Pointer, Texture_p->Vulkan_p);}

    Nh_Gfx_softFreeTexture(Texture_p);
    Nh_destroyListItem(&Tab_p->Textures, index, true);

NH_END(NH_SUCCESS)
}

// GET =============================================================================================

Nh_Gfx_Texture *Nh_Gfx_getTexture(
    Nh_Tab *Tab_p, Nh_URI *URI_p)
{
NH_BEGIN()

    if (URI_p == NULL) {NH_END(Nh_getListItem(&Tab_p->Textures, 0))}

    for (int i = 0; i < Tab_p->Textures.count; ++i) 
    {
        Nh_Gfx_Texture *Texture_p = Nh_getListItem(&Tab_p->Textures, i);
        if (Nh_equivalentURI(*URI_p, Texture_p->URI)) {NH_END(Texture_p)}
    }

NH_END(NULL)
}

float Nh_Gfx_getTextureWidth(
    Nh_Window *Window_p, Nh_Gfx_Texture *Texture_p)
{
NH_BEGIN()

    float originalWidth = ((float)Window_p->Info.pxSize_p[0]) / ((float)Window_p->Internal.resizeFactors_p[0]);
    float width = (((float)Texture_p->width) / originalWidth) * 2.0f;

NH_END(width)
}

float Nh_Gfx_getTextureHeight(
    Nh_Window *Window_p, Nh_Gfx_Texture *Texture_p)
{
NH_BEGIN()

    float originalHeight = ((float)Window_p->Info.pxSize_p[1]) / ((float)Window_p->Internal.resizeFactors_p[1]);
    float height = (((float)Texture_p->height) / originalHeight) * 2.0f;

NH_END(height)
}

void Nh_Gfx_getTextureScale(
    Nh_Window *Window_p, Nh_CSS_Box *Box_p, Nh_Gfx_Texture *Texture_p, float scale_p[2])
{
NH_BEGIN()
    
    scale_p[0] = Nh_Gfx_getTextureWidth(Window_p, Texture_p) / ((1.0f + Box_p->BottomRight.x) - (1.0f + Box_p->TopLeft.x));
    scale_p[1] = Nh_Gfx_getTextureHeight(Window_p, Texture_p) / ((1.0f + Box_p->BottomRight.y) - (1.0f + Box_p->TopLeft.y));

NH_SILENT_END()
}

void Nh_Gfx_getScaledTextureRepeat(
    Nh_Window *Window_p, Nh_CSS_Box *Box_p, Nh_Gfx_Texture *Texture_p, float repeat_p[2])
{
NH_BEGIN()
    
    float scale_p[2];
    Nh_Gfx_getTextureScale(Window_p, Box_p, Texture_p, scale_p);

    if (scale_p[1] > scale_p[0]) {
        repeat_p[0] = scale_p[1];
        repeat_p[1] = 1.0f;
    }
    else {
        repeat_p[0] = 1.0f;
        repeat_p[1] = scale_p[0];
    }

NH_SILENT_END()
}

