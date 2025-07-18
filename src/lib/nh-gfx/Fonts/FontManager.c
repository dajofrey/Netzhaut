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
#include "../Common/Data/SourceCodePro-Bold.ttf.inc"
#include "../Common/Data/SourceCodePro-ExtraBold.ttf.inc"

#include "../../nh-core/Util/File.h"
#include "../../nh-core/Util/String.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-encoding/Encodings/UTF8.h"

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

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_gfx_addFont(
    char *data_p, size_t size, char *id_p, bool internal)
{
    FT_Face Face; 
    int error = FT_New_Memory_Face(FreeType, data_p, size, 0, &Face);

    if (error == FT_Err_Unknown_File_Format) {
        return NH_API_ERROR_INVALID_FILE_FORMAT;
    }
    else if (error) {
        return NH_API_SUCCESS;
    }

    nh_gfx_Font *Font_p = (nh_gfx_Font*)nh_core_allocate(sizeof(nh_gfx_Font));
    NH_CORE_CHECK_MEM(Font_p)

    Font_p->file_p           = internal ? data_p : NULL;
    Font_p->fileSize         = internal ? size : 0;
    Font_p->Atlas.external_p = NULL;
    Font_p->Atlas.data_p     = NULL;
    Font_p->Instances        = nh_core_initArray(sizeof(nh_gfx_FontInstance), 8);
    Font_p->internal         = internal;

    Font_p->id_p = nh_core_allocateBytes(id_p);
    NH_CORE_CHECK_MEM(Font_p->id_p)

    NH_CORE_CHECK(nh_gfx_parseFontFamily(&Font_p->Family, Face->family_name))
    NH_CORE_CHECK(nh_gfx_parseFontStyle(&Font_p->Style, Face->style_name))

    nh_core_appendToList(&NH_GFX_FONT_MANAGER.Fonts, Font_p);
    nh_gfx_logFont(Font_p);

    FT_Done_Face(Face);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_addFontDirectory(
    char *dirPath_p)
{
#ifdef __unix__ 

    struct dirent *entry_p = NULL;
    char absolutePath_p[1024] = {'\0'};

    DIR *dir_p = opendir(dirPath_p); 
    if (dir_p == NULL) {return NH_API_SUCCESS;}
  
    while ((entry_p = readdir(dir_p)) != NULL) 
    {
        if (dirPath_p[strlen(dirPath_p) - 1] != '/') {
            sprintf(absolutePath_p, "%s/%s", dirPath_p, entry_p->d_name);
        } else {
            sprintf(absolutePath_p, "%s%s", dirPath_p, entry_p->d_name);
        }

        NH_API_RESULT result = nh_gfx_addFontUsingAbsolutePath(absolutePath_p);
        if (result != NH_API_SUCCESS && result != NH_API_ERROR_INVALID_FILE_FORMAT) {
            closedir(dir_p); 
            return result;
        }

        if (strcmp(entry_p->d_name, "..") != 0 && strcmp(entry_p->d_name, ".") != 0) {
            NH_CORE_CHECK(nh_gfx_addFontDirectory(absolutePath_p))
        }
    }
  
    closedir(dir_p);    

#elif defined(_WIN32) || defined(WIN32)

    printf("windows not supported\n");
    exit(0);

#endif

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_addFontUsingAbsolutePath(
    char *absolutePath_p)
{
    if (!nh_core_isRegularFile(absolutePath_p)) {return NH_API_ERROR_INVALID_FILE_FORMAT;}

    if (!strstr(absolutePath_p, ".ttf") && !strstr(absolutePath_p, ".otf")) {
        return NH_API_ERROR_INVALID_FILE_FORMAT;
    }

    size_t size = 0;
    char *data_p = nh_core_getFileData(absolutePath_p, &size);

    if (!data_p) {return NH_API_ERROR_INVALID_FILE_FORMAT;}

    nh_gfx_addFont(data_p, size, absolutePath_p, false);

    nh_core_free(data_p);

    return NH_API_SUCCESS;
}

static nh_gfx_FontInstance *nh_gfx_loadFontInstance(
    nh_gfx_Font *Font_p, unsigned int fontSize, nh_gfx_FontInstance *Instance_p)
{
    if (!Font_p->Atlas.external_p) 
    {
        Font_p->Atlas.external_p = texture_atlas_new(1024, 1024, 1);
        NH_CORE_CHECK_NULL_2(NULL, Font_p->Atlas.external_p)
        Font_p->Atlas.width  = ((texture_atlas_t*)Font_p->Atlas.external_p)->width; 
        Font_p->Atlas.height = ((texture_atlas_t*)Font_p->Atlas.external_p)->height;
        Font_p->Atlas.data_p = ((texture_atlas_t*)Font_p->Atlas.external_p)->data;
    }

    if (!Instance_p) {
        Instance_p = (nh_gfx_FontInstance*)nh_core_incrementArray(&Font_p->Instances);
        NH_CORE_CHECK_MEM_2(NULL, Instance_p)
    }

    if (!Font_p->file_p) {
        Font_p->file_p = nh_core_getFileData(Font_p->id_p, &Font_p->fileSize);
        NH_CORE_CHECK_MEM_2(NULL, Font_p->file_p)
    }
    Instance_p->external_p = texture_font_new_from_memory(&FreeType, Font_p->Atlas.external_p, fontSize, Font_p->file_p, Font_p->fileSize);

    NH_CORE_CHECK_NULL_2(NULL, Instance_p->external_p)
    ((texture_font_t*)Instance_p->external_p)->rendermode = RENDER_SIGNED_DISTANCE_FIELD;

    Instance_p->Font_p    = Font_p;
    Instance_p->fontSize  = fontSize;
    Instance_p->ascender  = ((texture_font_t*)Instance_p->external_p)->ascender;
    Instance_p->linegap   = ((texture_font_t*)Instance_p->external_p)->linegap;
    Instance_p->descender = ((texture_font_t*)Instance_p->external_p)->descender;
    Instance_p->hres      = ((texture_font_t*)Instance_p->external_p)->hres;

    return Instance_p;
}

nh_gfx_FontInstance *nh_gfx_claimFontInstance(
    nh_gfx_Font *Font_p, unsigned int fontSize)
{
    nh_gfx_FontInstance *Instance_p = NULL;
    for (int i = 0; i < Font_p->Instances.length; ++i) {
        Instance_p = &((nh_gfx_FontInstance*)Font_p->Instances.p)[i];
        if (Instance_p->fontSize == fontSize) {break;}
        Instance_p = NULL;
    }

    if (!Instance_p || !Instance_p->external_p) {
        return nh_gfx_loadFontInstance(Font_p, fontSize, Instance_p);
    }

    // TODO claim counter

    return Instance_p;
}

NH_API_RESULT nh_gfx_unclaimFontInstance(
    nh_gfx_FontInstance *Instance_p)
{
    // TODO unclaim check

    texture_atlas_clear(Instance_p->Font_p->Atlas.external_p);
    texture_font_delete(Instance_p->external_p);
    Instance_p->external_p = NULL;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_loadGlyphs(
    nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 *text_p, unsigned int textLength)
{
    if (texture_font_load_glyphs(&FreeType, Instance_p->external_p, text_p, textLength, "en")) {
        return NH_API_ERROR_BAD_STATE;
    }

    return NH_API_SUCCESS;
}

bool nh_gfx_hasGlyph(
    nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 codepoint)
{
    if (FT_Get_Char_Index(((texture_font_t*)Instance_p->external_p)->ft_face, codepoint)) {
        return true;
    }

    return false;
}

nh_gfx_Glyph nh_gfx_getGlyph(
    nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 id)
{
    nh_gfx_Glyph Glyph = {0};

    char bytes_p[4];
    memset(bytes_p, 0, 4);

    texture_glyph_t *Glyph_p = texture_font_get_glyph(Instance_p->external_p, id);
    if (!Glyph_p) {return Glyph;}

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

    return Glyph;
}

nh_core_List nh_gfx_getFonts(
    nh_gfx_FontFamily FontFamily, nh_gfx_FontStyle FontStyle, bool internal)
{
    nh_core_List Fonts = nh_core_initList(64);

    for (int i = 0; i < NH_GFX_FONT_MANAGER.Fonts.size; ++i) 
    {
        nh_gfx_Font *Font_p = NH_GFX_FONT_MANAGER.Fonts.pp[i];
        if (!Font_p->internal && internal) {continue;}

        bool familyMatch = false;

        if (FontFamily.name_p) {
            familyMatch = !strcmp(Font_p->Family.name_p, FontFamily.name_p);
        }
        else {
            for (NH_GFX_GENERIC_FONT_FAMILY generic = 0; generic < NH_GFX_GENERIC_FONT_FAMILY_COUNT; ++generic) {
                if (FontFamily.generic_p[generic] && Font_p->Family.generic_p[generic]) {
                    familyMatch = true;
                }
            }
        }

        if (familyMatch) {
            if (Font_p->Style.weight != FontStyle.weight
            ||  Font_p->Style.italic != FontStyle.italic 
            ||  Font_p->Style.oblique != FontStyle.oblique) {familyMatch = false;}
        }

        if (familyMatch) {
            nh_core_appendToList(&Fonts, Font_p);
        }
    }

    return Fonts;
}

nh_gfx_Font *nh_gfx_getFontUsingId(
    char *id_p)
{
    for (int i = 0; i < NH_GFX_FONT_MANAGER.Fonts.size; ++i) {
        nh_gfx_Font *Font_p = NH_GFX_FONT_MANAGER.Fonts.pp[i];
        if (!strcmp(Font_p->id_p, id_p)) {return Font_p;}
    }

    return NULL;
}

nh_gfx_Font *nh_gfx_getDefaultFont(
    NH_GFX_GENERIC_FONT_FAMILY generic, NH_GFX_FONT_WEIGHT_E weight, bool italic, bool oblique)
{
    nh_gfx_FontFamily Family = nh_gfx_initFontFamily(NULL);
    Family.generic_p[generic] = true;

    nh_gfx_FontStyle Style;
    Style.weight = weight;
    Style.italic = italic;
    Style.oblique = oblique;

    nh_core_List Fonts = nh_gfx_getFonts(Family, Style, false);
    nh_gfx_Font *Font_p = Fonts.size <= 0 ? NULL : Fonts.pp[0]; // TODO define default fonts
    nh_core_freeList(&Fonts, false);

    return Font_p;
}

static NH_API_RESULT nh_gfx_addInitialFonts()
{
    // First system fonts.
#if defined(__APPLE__)
    NH_CORE_CHECK(nh_gfx_addFontDirectory("/System/Library/Fonts/"))
    NH_CORE_CHECK(nh_gfx_addFontDirectory("/Library/Fonts/"))
    NH_CORE_CHECK(nh_gfx_addFontDirectory("/System/Library/Fonts/Supplemental/"))
    NH_CORE_CHECK(nh_gfx_addFontDirectory("/Users/Shared/Fonts/"))
    const char* home = getenv("HOME");
    if (home) {
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/Library/Fonts/", home);
        NH_CORE_CHECK(nh_gfx_addFontDirectory(path))
    }
#elif defined(__unix__)
    NH_CORE_CHECK(nh_gfx_addFontDirectory("/usr/share/fonts/"))
    NH_CORE_CHECK(nh_gfx_addFontDirectory("/usr/local/share/fonts/"))
    NH_CORE_CHECK(nh_gfx_addFontDirectory("~/.fonts/"))
#elif defined(_WIN32) || defined (WIN32)
    // TODO
#endif

    // Then internal fonts.
    NH_CORE_CHECK(nh_gfx_addFont(source_code_pro_regular_ttf_inc, source_code_pro_regular_ttf_inc_len, "SourceCodePro", true))
    NH_CORE_CHECK(nh_gfx_addFont(source_code_pro_bold_ttf, source_code_pro_bold_ttf_len, "SourceCodePro", true))
    NH_CORE_CHECK(nh_gfx_addFont(source_code_pro_extrabold_ttf, source_code_pro_extrabold_ttf_len, "SourceCodePro", true))

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_initializeFontManager()
{
    NH_GFX_FONT_MANAGER.Fonts = nh_core_initList(255);

    if (FT_Init_FreeType(&FreeType)) {return NH_API_ERROR_BAD_STATE;}

    return nh_gfx_addInitialFonts();
}

void nh_gfx_terminateFontManager()
{
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

    nh_core_freeList(&NH_GFX_FONT_MANAGER.Fonts, true);

    FT_Done_FreeType(FreeType);

    return;
}
