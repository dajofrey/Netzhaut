// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Text.h"
#include "HarfBuzz.h"

#include "External/freetype.h"
#include "External/freetype-gl.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-core/Util/File.h"
#include "../../nh-core/Util/String.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/File.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#ifdef __unix__ 	
    #include <dirent.h>
#elif defined(_WIN32) || defined(WIN32)
#endif

// CREATE/FREE =====================================================================================

static nh_gfx_FontInstance *nh_gfx_getAnyFontInstance(
    NH_ENCODING_UTF32 codepoint, int fontSize)
{
    for (int i = 0; i < NH_GFX_FONT_MANAGER.Fonts.size; ++i)
    {
        nh_gfx_Font *Font_p = NH_GFX_FONT_MANAGER.Fonts.pp[i];

        nh_gfx_FontInstance *Instance_p = nh_gfx_claimFontInstance(Font_p, fontSize);
        if (!Instance_p) {continue;}

        if (nh_gfx_hasGlyph(Instance_p, codepoint)) {
            if (nh_gfx_loadGlyphs(Instance_p, &codepoint, 1) == NH_API_SUCCESS) {return Instance_p;}
        }
    }

    return NULL;
}

NH_API_RESULT nh_gfx_createText(
    nh_gfx_Text *Text_p, NH_ENCODING_UTF32 *codepoints_p, unsigned int length, int fontSize, 
    nh_core_Array *FontFamilies_p, nh_gfx_FontStyle FontStyle)
{
    Text_p->Segments = nh_core_initArray(sizeof(nh_gfx_TextSegment), 1);
    nh_gfx_FontInstance *PrevInstance_p = NULL;

    for (int i = 0; i < length; ++i)
    {
        NH_ENCODING_UTF32 codepoint = codepoints_p[i];
        nh_gfx_FontInstance *Instance_p = NULL;
        nh_gfx_FontInstance *Fallback_p = NULL;

        for (int j = 0; j < FontFamilies_p->length; ++j)
        {
            nh_gfx_FontFamily *FontFamily_p = &((nh_gfx_FontFamily*)FontFamilies_p->p)[j];
            nh_core_List Fonts = nh_gfx_getFonts(*FontFamily_p, FontStyle, false);

            for (int k = 0; k < Fonts.size; ++k) 
            {
                nh_gfx_Font *Font_p = Fonts.pp[k];

                Instance_p = nh_gfx_claimFontInstance(Font_p, fontSize);
                NH_CORE_CHECK_NULL(Instance_p)

                if (nh_gfx_hasGlyph(Instance_p, codepoint)) {
                    if (nh_gfx_loadGlyphs(Instance_p, &codepoint, 1) == NH_API_SUCCESS) {break;}
                }
                if (!Fallback_p) {Fallback_p = Instance_p;}

                Instance_p = NULL;
            }

            nh_core_freeList(&Fonts, false);
            if (Instance_p) {break;}
        }

        if (!Instance_p) {Instance_p = nh_gfx_getAnyFontInstance(codepoint, fontSize);}
        if (!Instance_p) {
            Instance_p = Fallback_p;
            NH_CORE_CHECK(nh_gfx_loadGlyphs(Instance_p, &codepoint, 1))
        }
        if (!Instance_p) {return NH_API_ERROR_BAD_STATE;}

        if (Instance_p != PrevInstance_p) {
            nh_gfx_TextSegment *Segment_p = (nh_gfx_TextSegment*)nh_core_incrementArray(&Text_p->Segments);
            Segment_p->FontInstance_p = Instance_p;
            Segment_p->length = 1;
            Segment_p->codepoints_p = codepoints_p + i;
        }
        else {
            nh_gfx_TextSegment *Segment_p = &((nh_gfx_TextSegment*)Text_p->Segments.p)[Text_p->Segments.length - 1];
            Segment_p->length++;
        }

        PrevInstance_p = Instance_p;
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_createTextFromFont(
    nh_gfx_Text *Text_p, NH_ENCODING_UTF32 *codepoints_p, unsigned int length, int fontSize, 
    nh_gfx_Font *Font_p)
{
    nh_gfx_FontInstance *Instance_p = nh_gfx_claimFontInstance(Font_p, fontSize);
    NH_CORE_CHECK_NULL(Instance_p)

    Text_p->Segments = nh_core_initArray(sizeof(nh_gfx_TextSegment), 1);

    nh_gfx_TextSegment *Segment_p = (nh_gfx_TextSegment*)nh_core_incrementArray(&Text_p->Segments);
    NH_CORE_CHECK_MEM(Segment_p)

    Segment_p->FontInstance_p = Instance_p;
    Segment_p->length = 0;
    Segment_p->codepoints_p = codepoints_p;

    for (int i = 0; i < length; ++i)
    {
        NH_ENCODING_UTF32 codepoint = codepoints_p[i];

        if (!nh_gfx_hasGlyph(Instance_p, codepoint) || nh_gfx_loadGlyphs(Instance_p, &codepoint, 1)) {
            return NH_API_ERROR_BAD_STATE;
        }

        Segment_p->length++;
    }

    return NH_API_SUCCESS;
}

void nh_gfx_freeText(
    nh_gfx_Text *Text_p)
{
    nh_core_freeArray(&Text_p->Segments);
    return;
}

// MEASURE =========================================================================================

int nh_gfx_getTextWidth(
    nh_gfx_Text *Text_p)
{
    int width = 0;

    for (int s = 0; s < Text_p->Segments.length; ++s)
    {
        nh_gfx_TextSegment *Segment_p = &((nh_gfx_TextSegment*)Text_p->Segments.p)[s];

        nh_gfx_HarfBuzzBuffer Buffer = 
            nh_gfx_createHarfBuzzBuffer(Segment_p->FontInstance_p, Segment_p->codepoints_p, Segment_p->length);

        unsigned int glyphs;
        nh_gfx_HarfBuzzGlyphInfo *Infos_p = nh_gfx_getHarfBuzzGlyphInfos(Buffer, &glyphs);

        for (int i = 0; i < glyphs; ++i) {
            width += Infos_p[i].xAdvance;
        }

        nh_core_free(Infos_p);
        nh_gfx_destroyHarfBuzzBuffer(Buffer);
    }

    return width;
}

int nh_gfx_getTextHeight(
    nh_gfx_Text *Text_p)
{
    int fontSize = 0;

    for (int i = 0; i < Text_p->Segments.length; ++i)
    {
        nh_gfx_TextSegment *Segment_p = &((nh_gfx_TextSegment*)Text_p->Segments.p)[i];
        if (Segment_p->FontInstance_p->fontSize > fontSize) {fontSize = Segment_p->FontInstance_p->fontSize;}
    }

    return fontSize;
}

unsigned int nh_gfx_getFittingTextLength(
    nh_gfx_Text *Text_p, int maxWidth)
{
    unsigned int fittingGlyphs = 0;

    for (int s = 0; s < Text_p->Segments.length; ++s) 
    {
        nh_gfx_TextSegment *Segment_p = &((nh_gfx_TextSegment*)Text_p->Segments.p)[s];
        nh_gfx_HarfBuzzBuffer Buffer = nh_gfx_createHarfBuzzBuffer(Segment_p->FontInstance_p, Segment_p->codepoints_p, Segment_p->length);
    
        unsigned int glyphs = 0;
        nh_gfx_HarfBuzzGlyphInfo *Infos_p = nh_gfx_getHarfBuzzGlyphInfos(Buffer, &glyphs);
    
        int width = 0;
    
        for (int i = 0; i < glyphs; ++i) {
            width += Infos_p[i].xAdvance;
            if (width > maxWidth) {break;}
            fittingGlyphs++;
        }

        nh_core_free(Infos_p);
        nh_gfx_destroyHarfBuzzBuffer(Buffer);
    }

    // TODO this is problematic for complex fonts

    return fittingGlyphs;
}

