// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HarfBuzz.h"

#include "External/freetype.h"
#include "External/freetype-gl.h"
#include "External/HarfBuzz.h"

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

// HARFBUZZ ========================================================================================

nh_gfx_HarfBuzzGlyphInfo *nh_gfx_getHarfBuzzGlyphInfos(
    nh_gfx_HarfBuzzBuffer Buffer, unsigned int *glyphs_p)
{
NH_GFX_BEGIN()

    hb_glyph_position_t *positions_p = hb_buffer_get_glyph_positions(Buffer.external_p, glyphs_p);
    NH_GFX_CHECK_NULL_2(NULL, positions_p)

    hb_glyph_info_t *infos_p = hb_buffer_get_glyph_infos(Buffer.external_p, glyphs_p);
    NH_GFX_CHECK_NULL_2(NULL, infos_p)

    nh_gfx_HarfBuzzGlyphInfo *Infos_p = nh_core_allocate(sizeof(nh_gfx_HarfBuzzGlyphInfo) * (*glyphs_p));
    NH_GFX_CHECK_NULL_2(NULL, Infos_p)

    for (int i = 0 ; i < *glyphs_p; ++i) {
        Infos_p[i].xAdvance  = positions_p[i].x_advance / 64;
        Infos_p[i].xOffset   = positions_p[i].x_offset / 64;
        Infos_p[i].yAdvance  = positions_p[i].y_advance / 64;
        Infos_p[i].yOffset   = positions_p[i].y_offset / 64;
        Infos_p[i].id        = infos_p[i].codepoint;
    }

NH_GFX_END(Infos_p)
}

nh_gfx_HarfBuzzBuffer nh_gfx_createHarfBuzzBuffer(
    nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 *text_p, unsigned int textLength)
{
NH_GFX_BEGIN()

    hb_buffer_t *buffer_p = hb_buffer_create();

//    hb_buffer_set_direction(buffer_p, HB_DIRECTION_LTR);
//    hb_buffer_set_script(buffer_p, HB_SCRIPT_LATIN);
//    hb_buffer_set_language(buffer_p, hb_language_from_string("en", 2));

    hb_buffer_add_utf32(buffer_p, text_p, textLength, 0, textLength);
    hb_buffer_guess_segment_properties(buffer_p);

    hb_shape(((texture_font_t*)Instance_p->external_p)->hb_ft_font, buffer_p, NULL, 0);

    nh_gfx_HarfBuzzBuffer Buffer;
    Buffer.external_p = buffer_p;

NH_GFX_END(Buffer)
}

void nh_gfx_destroyHarfBuzzBuffer(
    nh_gfx_HarfBuzzBuffer Buffer)
{
NH_GFX_BEGIN()

    hb_buffer_destroy(Buffer.external_p);

NH_GFX_SILENT_END()
}

