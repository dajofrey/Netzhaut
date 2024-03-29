#ifndef NH_GFX_FONT_MANAGER_H
#define NH_GFX_FONT_MANAGER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "FontFamily.h"
#include "FontStyle.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

    typedef struct nh_gfx_Glyph {
        void *external_p;
        NH_ENCODING_UTF32 codepoint;
        size_t width;
        size_t height;
        int xOffset;
        int yOffset;
        float u0;
        float v0;
        float u1;
        float v1;
    } nh_gfx_Glyph;

    typedef struct nh_gfx_Atlas {
        void *external_p;
        void *data_p;
        size_t width;
        size_t height;
    } nh_gfx_Atlas;

    typedef struct nh_gfx_Font {
        NH_BYTE *id_p;
        NH_BOOL internal;
        nh_gfx_Atlas Atlas;
        nh_Array Instances; 
        void *file_p;
        size_t fileSize;
        nh_gfx_FontFamily Family;
        nh_gfx_FontStyle Style;
    } nh_gfx_Font;

    typedef struct nh_gfx_FontInstance {
        nh_gfx_Font *Font_p;
        unsigned int fontSize;
        void *external_p;
        float ascender;
        float descender;
        float hres;
        float linegap;
    } nh_gfx_FontInstance;

    typedef struct nh_gfx_FontManager {
        nh_List Fonts;
    } nh_gfx_FontManager;

/** @} */

/** @addtogroup lib_nhgfx_vars
 *  @{
 */

    extern nh_gfx_FontManager NH_GFX_FONT_MANAGER;

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    NH_GFX_RESULT nh_gfx_initializeFontManager(
    );

    void nh_gfx_terminateFontManager(
    );

    NH_GFX_RESULT nh_gfx_addFontDirectory(
        NH_BYTE *dirPath_p
    );
    
    NH_GFX_RESULT nh_gfx_addFontUsingAbsolutePath(
        NH_BYTE *absolutePath_p
    );
    
    nh_gfx_FontInstance *nh_gfx_claimFontInstance(
        nh_gfx_Font *Font_p, unsigned int fontSize
    );

    NH_GFX_RESULT nh_gfx_unclaimFontInstance(
        nh_gfx_FontInstance *Instance_p
    );

    nh_List nh_gfx_getFonts(
        nh_gfx_FontFamily FontFamily, nh_gfx_FontStyle FontStyle, NH_BOOL internal
    );

    nh_gfx_Font *nh_gfx_getFontUsingId(
        NH_BYTE *id_p
    );

    nh_gfx_Font *nh_gfx_getDefaultFont(
        NH_GFX_GENERIC_FONT_FAMILY generic, NH_GFX_FONT_WEIGHT weight, NH_BOOL italic, NH_BOOL oblique
    );

    NH_GFX_RESULT nh_gfx_loadGlyphs(
        nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 *text_p, unsigned int textLength
    );
   
    NH_BOOL nh_gfx_hasGlyph(
        nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 codepoint
    );

    nh_gfx_Glyph nh_gfx_getGlyph(
        nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 id 
    );

/** @} */

#endif 
