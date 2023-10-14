// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "FontManager.h"
#include "HarfBuzz.h"

#include "External/freetype.h"
#include "External/freetype-gl.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../Common/Data/SourceCodePro-Regular.ttf.inc"

#include "../../nhcore/Util/File.h"
#include "../../nhcore/Util/String.h"
#include "../../nhcore/Util/List.h"
#include "../../nhcore/System/Memory.h"

#include "../../nhencoding/Encodings/UTF8.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#ifdef __unix__ 	
    #include <dirent.h>
#elif defined(_WIN32) || defined(WIN32)
#endif

// DATA ============================================================================================

nh_gfx_FontManager NH_GFX_FONT_MANAGER;
static FT_Library FreeType;

// ADD =============================================================================================

static NH_GFX_RESULT nh_gfx_addFont(
    NH_BYTE *data_p, size_t size, NH_BYTE *id_p, NH_BOOL internal)
{
NH_GFX_BEGIN()

    FT_Face Face; 
    int error = FT_New_Memory_Face(FreeType, data_p, size, 0, &Face);

    if (error == FT_Err_Unknown_File_Format) {
        NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_INVALID_FILE_FORMAT)
    }
    else if (error) {
        NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
    }

    nh_gfx_Font *Font_p = nh_core_allocate(sizeof(nh_gfx_Font));
    NH_GFX_CHECK_MEM(Font_p)

    Font_p->file_p           = internal ? data_p : NULL;
    Font_p->fileSize         = internal ? size : 0;
    Font_p->Atlas.external_p = NULL;
    Font_p->Atlas.data_p     = NULL;
    Font_p->Instances        = nh_core_initArray(sizeof(nh_gfx_FontInstance), 8);
    Font_p->internal         = internal;

    Font_p->id_p = nh_core_allocateBytes(id_p);
    NH_GFX_CHECK_MEM(Font_p->id_p)

    NH_GFX_CHECK(nh_gfx_parseFontFamily(&Font_p->Family, Face->family_name))
    NH_GFX_CHECK(nh_gfx_parseFontStyle(&Font_p->Style, Face->style_name))

    nh_core_appendToList(&NH_GFX_FONT_MANAGER.Fonts, Font_p);
    nh_gfx_logFont(Font_p);

    FT_Done_Face(Face);

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

NH_GFX_RESULT nh_gfx_addFontDirectory(
    NH_BYTE *dirPath_p)
{
NH_GFX_BEGIN()

#ifdef __unix__ 

    struct dirent *entry_p = NULL;
    NH_BYTE absolutePath_p[1024] = {'\0'};

    DIR *dir_p = opendir(dirPath_p); 
    if (dir_p == NULL) {NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)}
  
    while ((entry_p = readdir(dir_p)) != NULL) 
    {
        if (dirPath_p[strlen(dirPath_p) - 1] != '/') {
            sprintf(absolutePath_p, "%s/%s", dirPath_p, entry_p->d_name);
        } else {
            sprintf(absolutePath_p, "%s%s", dirPath_p, entry_p->d_name);
        }

        NH_GFX_RESULT result = nh_gfx_addFontUsingAbsolutePath(absolutePath_p);
        if (result != NH_GFX_SUCCESS && result != NH_GFX_ERROR_INVALID_FILE_FORMAT) {
            closedir(dir_p); 
            NH_GFX_END(result)
        }

        if (strcmp(entry_p->d_name, "..") != 0 && strcmp(entry_p->d_name, ".") != 0) {
            NH_GFX_CHECK(nh_gfx_addFontDirectory(absolutePath_p))
        }
    }
  
    closedir(dir_p);    

#elif defined(_WIN32) || defined(WIN32)

    printf("windows not supported\n");
    exit(0);

#endif

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

NH_GFX_RESULT nh_gfx_addFontUsingAbsolutePath(
    NH_BYTE *absolutePath_p)
{
NH_GFX_BEGIN()

    if (!nh_core_isRegularFile(absolutePath_p)) {NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_INVALID_FILE_FORMAT)}

    if (!strstr(absolutePath_p, ".ttf") && !strstr(absolutePath_p, ".otf")) {
        NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_INVALID_FILE_FORMAT)
    }

    size_t size = 0;
    NH_BYTE *data_p = nh_core_getFileData(absolutePath_p, &size);

    if (!data_p) {NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_INVALID_FILE_FORMAT)}

    nh_gfx_addFont(data_p, size, absolutePath_p, NH_FALSE);

    nh_core_free(data_p);

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

// FONT INSTANCE ===================================================================================

static nh_gfx_FontInstance *nh_gfx_loadFontInstance(
    nh_gfx_Font *Font_p, unsigned int fontSize, nh_gfx_FontInstance *Instance_p)
{
NH_GFX_BEGIN()

    if (!Font_p->Atlas.external_p) 
    {
        Font_p->Atlas.external_p = texture_atlas_new(1024, 1024, 1);
        NH_GFX_CHECK_NULL_2(NULL, Font_p->Atlas.external_p)
        Font_p->Atlas.width  = ((texture_atlas_t*)Font_p->Atlas.external_p)->width; 
        Font_p->Atlas.height = ((texture_atlas_t*)Font_p->Atlas.external_p)->height;
        Font_p->Atlas.data_p = ((texture_atlas_t*)Font_p->Atlas.external_p)->data;
    }

    if (!Instance_p) {
        Instance_p = nh_core_incrementArray(&Font_p->Instances);
        NH_GFX_CHECK_MEM_2(NULL, Instance_p)
    }

    if (!Font_p->file_p) {
        Font_p->file_p = nh_core_getFileData(Font_p->id_p, &Font_p->fileSize);
        NH_GFX_CHECK_MEM_2(NULL, Font_p->file_p)
    }
    Instance_p->external_p = texture_font_new_from_memory(&FreeType, Font_p->Atlas.external_p, fontSize, Font_p->file_p, Font_p->fileSize);

    NH_GFX_CHECK_NULL_2(NULL, Instance_p->external_p)
    ((texture_font_t*)Instance_p->external_p)->rendermode = RENDER_SIGNED_DISTANCE_FIELD;

    Instance_p->Font_p    = Font_p;
    Instance_p->fontSize  = fontSize;
    Instance_p->ascender  = ((texture_font_t*)Instance_p->external_p)->ascender;
    Instance_p->linegap   = ((texture_font_t*)Instance_p->external_p)->linegap;
    Instance_p->descender = ((texture_font_t*)Instance_p->external_p)->descender;
    Instance_p->hres      = ((texture_font_t*)Instance_p->external_p)->hres;

NH_GFX_END(Instance_p)
}

nh_gfx_FontInstance *nh_gfx_claimFontInstance(
    nh_gfx_Font *Font_p, unsigned int fontSize)
{
NH_GFX_BEGIN()

    nh_gfx_FontInstance *Instance_p = NULL;
    for (int i = 0; i < Font_p->Instances.length; ++i) {
        Instance_p = &((nh_gfx_FontInstance*)Font_p->Instances.p)[i];
        if (Instance_p->fontSize == fontSize) {break;}
        Instance_p = NULL;
    }

    if (!Instance_p || !Instance_p->external_p) {
        NH_GFX_END(nh_gfx_loadFontInstance(Font_p, fontSize, Instance_p))
    }

    // TODO claim counter

NH_GFX_END(Instance_p)
}

NH_GFX_RESULT nh_gfx_unclaimFontInstance(
    nh_gfx_FontInstance *Instance_p)
{
NH_GFX_BEGIN()

    // TODO unclaim check

    texture_atlas_clear(Instance_p->Font_p->Atlas.external_p);
    texture_font_delete(Instance_p->external_p);
    Instance_p->external_p = NULL;

NH_GFX_END(NH_GFX_SUCCESS)
}

// GLYPH ===========================================================================================

NH_GFX_RESULT nh_gfx_loadGlyphs(
    nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 *text_p, unsigned int textLength)
{
NH_GFX_BEGIN()

    if (texture_font_load_glyphs(&FreeType, Instance_p->external_p, text_p, textLength, "en")) {
        NH_GFX_END(NH_GFX_ERROR_BAD_STATE)
    }

NH_GFX_END(NH_GFX_SUCCESS)
}

NH_BOOL nh_gfx_hasGlyph(
    nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 codepoint)
{
NH_GFX_BEGIN()

    if (FT_Get_Char_Index(((texture_font_t*)Instance_p->external_p)->ft_face, codepoint)) {
        NH_GFX_END(NH_TRUE)
    }

NH_GFX_END(NH_FALSE)
}

nh_gfx_Glyph nh_gfx_getGlyph(
    nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 id)
{
NH_GFX_BEGIN()

    nh_gfx_Glyph Glyph = {0};

    NH_BYTE bytes_p[4];
    memset(bytes_p, 0, 4);

    texture_glyph_t *Glyph_p = texture_font_get_glyph(Instance_p->external_p, id);
    if (!Glyph_p) {NH_GFX_END(Glyph)}

    Glyph.external_p = Glyph_p;

    Glyph.codepoint = Glyph_p->codepoint;
    Glyph.width     = Glyph_p->width;
    Glyph.height    = Glyph_p->height;
    Glyph.xOffset   = Glyph_p->offset_x;
    Glyph.yOffset   = Glyph_p->offset_y;

    Glyph.u0 = Glyph_p->s0;
    Glyph.v0 = Glyph_p->t0;
    Glyph.u1 = Glyph_p->s1;
    Glyph.v1 = Glyph_p->t1;

NH_GFX_END(Glyph)
}

// GET =============================================================================================

nh_List nh_gfx_getFonts(
    nh_gfx_FontFamily FontFamily, nh_gfx_FontStyle FontStyle, NH_BOOL internal)
{
NH_GFX_BEGIN()

    nh_List Fonts = nh_core_initList(64);

    for (int i = 0; i < NH_GFX_FONT_MANAGER.Fonts.size; ++i) 
    {
        nh_gfx_Font *Font_p = NH_GFX_FONT_MANAGER.Fonts.pp[i];
        if (!Font_p->internal && internal) {continue;}

        NH_BOOL familyMatch = NH_FALSE;

        if (FontFamily.name_p) {
            familyMatch = !strcmp(Font_p->Family.name_p, FontFamily.name_p);
        }
        else {
            for (NH_GFX_GENERIC_FONT_FAMILY generic = 0; generic < NH_GFX_GENERIC_FONT_FAMILY_COUNT; ++generic) {
                if (FontFamily.generic_p[generic] && Font_p->Family.generic_p[generic]) {
                    familyMatch = NH_TRUE;
                }
            }
        }

        if (familyMatch) {
            if (Font_p->Style.weight != FontStyle.weight
            ||  Font_p->Style.italic != FontStyle.italic 
            ||  Font_p->Style.oblique != FontStyle.oblique) {familyMatch = NH_FALSE;}
        }

        if (familyMatch) {
            nh_core_appendToList(&Fonts, Font_p);
        }
    }

NH_GFX_END(Fonts)
}

nh_gfx_Font *nh_gfx_getFontUsingId(
    NH_BYTE *id_p)
{
NH_GFX_BEGIN()

    for (int i = 0; i < NH_GFX_FONT_MANAGER.Fonts.size; ++i) {
        nh_gfx_Font *Font_p = NH_GFX_FONT_MANAGER.Fonts.pp[i];
        if (!strcmp(Font_p->id_p, id_p)) {NH_GFX_END(Font_p)}
    }

NH_GFX_END(NULL)
}

nh_gfx_Font *nh_gfx_getDefaultFont(
    NH_GFX_GENERIC_FONT_FAMILY generic, NH_GFX_FONT_WEIGHT weight, NH_BOOL italic, NH_BOOL oblique)
{
NH_GFX_BEGIN()

    nh_gfx_FontFamily Family = nh_gfx_initFontFamily(NULL);
    Family.generic_p[generic] = NH_TRUE;

    nh_gfx_FontStyle Style;
    Style.weight = weight;
    Style.italic = italic;
    Style.oblique = oblique;

    nh_List Fonts = nh_gfx_getFonts(Family, Style, NH_FALSE);
    nh_gfx_Font *Font_p = Fonts.size <= 0 ? NULL : Fonts.pp[0]; // TODO define default fonts
    nh_core_freeList(&Fonts, NH_FALSE);

NH_GFX_END(Font_p)
}

// FONT MANAGER ====================================================================================

static NH_GFX_RESULT nh_gfx_addInitialFonts()
{
NH_GFX_BEGIN()

    // First system fonts.

#ifdef __unix__
    NH_GFX_CHECK(nh_gfx_addFontDirectory("/usr/share/fonts/"))
    NH_GFX_CHECK(nh_gfx_addFontDirectory("/usr/local/share/fonts/"))
    NH_GFX_CHECK(nh_gfx_addFontDirectory("~/.fonts/"))
#elif defined(_WIN32) || defined (WIN32)
    // TODO
#endif

    // Then internal fonts.

    NH_GFX_CHECK(nh_gfx_addFont(source_code_pro_ttf_inc, source_code_pro_ttf_inc_len, "SourceCodePro", NH_TRUE))

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

NH_GFX_RESULT nh_gfx_initializeFontManager()
{
NH_GFX_BEGIN()

    NH_GFX_FONT_MANAGER.Fonts = nh_core_initList(255);

    if (FT_Init_FreeType(&FreeType)) {NH_GFX_END(NH_GFX_ERROR_BAD_STATE)}

NH_GFX_DIAGNOSTIC_END(nh_gfx_addInitialFonts())
}

void nh_gfx_terminateFontManager()
{
NH_GFX_BEGIN()

    for (int i = 0; i < NH_GFX_FONT_MANAGER.Fonts.size; ++i) 
    {
        nh_gfx_Font *Font_p = NH_GFX_FONT_MANAGER.Fonts.pp[i];

        nh_core_free(Font_p->id_p);
        nh_gfx_freeFontFamily(&Font_p->Family);
        nh_gfx_freeFontStyle(&Font_p->Style);

        for (int j = 0; j < Font_p->Instances.length; ++j) {
            nh_gfx_FontInstance *Instance_p = &((nh_gfx_FontInstance*)Font_p->Instances.p)[j];
            texture_font_delete(Instance_p->external_p);
        }

        nh_core_freeArray(&Font_p->Instances);

        if (Font_p->Atlas.external_p != NULL) {
            texture_atlas_delete(Font_p->Atlas.external_p);
            Font_p->Atlas.external_p = NULL;
        }
    }

    nh_core_freeList(&NH_GFX_FONT_MANAGER.Fonts, NH_TRUE);

    FT_Done_FreeType(FreeType);

NH_GFX_SILENT_END()
}

