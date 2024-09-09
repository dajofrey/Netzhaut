// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "FontStyle.h"
#include "FontManager.h"

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

// FONT WEIGHT =====================================================================================

static NH_GFX_FONT_WEIGHT fontWeightValues_p[] = {
    NH_GFX_FONT_WEIGHT_THIN,
    NH_GFX_FONT_WEIGHT_EXTRALIGHT,
    NH_GFX_FONT_WEIGHT_ULTRALIGHT,
    NH_GFX_FONT_WEIGHT_LIGHT,
    NH_GFX_FONT_WEIGHT_NORMAL,
    NH_GFX_FONT_WEIGHT_REGULAR,
    NH_GFX_FONT_WEIGHT_MEDIUM,
    NH_GFX_FONT_WEIGHT_SEMIBOLD,
    NH_GFX_FONT_WEIGHT_DEMIBOLD,
    NH_GFX_FONT_WEIGHT_BOLD,
    NH_GFX_FONT_WEIGHT_EXTRABOLD,
    NH_GFX_FONT_WEIGHT_ULTRABOLD,
    NH_GFX_FONT_WEIGHT_BLACK,
    NH_GFX_FONT_WEIGHT_HEAVY,
};

static NH_BYTE *fontWeights_pp[] = {
    "thin",
    "extralight",
    "ultralight",
    "light",
    "normal",
    "regular",
    "medium",
    "semibold",
    "demibold",
    "bold",
    "extrabold",
    "ultrabold",
    "black",
    "heavy",
};

static int nh_gfx_parseFontWeight(
    NH_BYTE *weight_p)
{
NH_GFX_BEGIN()

    for (int i = 0; i < sizeof(fontWeights_pp)/sizeof(fontWeights_pp[0]); ++i) {
        if (strstr(weight_p, fontWeights_pp[i])) {NH_GFX_END(fontWeightValues_p[i])}
    }

NH_GFX_END(400)
}

nh_gfx_Font *nh_gfx_getLighterFontVariant(
    nh_gfx_Font *Font_p)
{
NH_GFX_BEGIN()

    for (int weight = Font_p->Style.weight - 100; weight > 0; weight -= 100) {
        for (int font = 0; font < NH_GFX_FONT_MANAGER.Fonts.size; ++font) {
            nh_gfx_Font *Tmp_p = NH_GFX_FONT_MANAGER.Fonts.pp[font];
            if (!strcmp(Font_p->Family.name_p, Tmp_p->Family.name_p) && Tmp_p->Style.weight == weight) {
                NH_GFX_END(Tmp_p)
            }
        }
    }

NH_GFX_END(NULL)
}

nh_gfx_Font *nh_gfx_getBolderFontVariant(
    nh_gfx_Font *Font_p)
{
NH_GFX_BEGIN()

    for (int weight = Font_p->Style.weight + 100; weight < 1000; weight += 100) {
        for (int font = 0; font < NH_GFX_FONT_MANAGER.Fonts.size; ++font) {
            nh_gfx_Font *Tmp_p = NH_GFX_FONT_MANAGER.Fonts.pp[font];
            if (!strcmp(Font_p->Family.name_p, Tmp_p->Family.name_p) && Tmp_p->Style.weight == weight) {
                NH_GFX_END(Tmp_p)
            }
        }
    }

NH_GFX_END(NULL)
}

// FONT STYLE ======================================================================================

NH_GFX_RESULT nh_gfx_parseFontStyle(
    nh_gfx_FontStyle *Style_p, NH_BYTE *name_p)
{
NH_GFX_BEGIN()

    Style_p->name_p = nh_core_allocateBytes(name_p);
    NH_GFX_CHECK_MEM(Style_p->name_p)
    for (int i = 0; i < strlen(Style_p->name_p); ++i) {
        Style_p->name_p[i] = tolower(Style_p->name_p[i]);
    }

    Style_p->weight = nh_gfx_parseFontWeight(Style_p->name_p);
    Style_p->oblique = strstr(Style_p->name_p, "oblique") ? NH_TRUE : NH_FALSE;
    Style_p->italic = strstr(Style_p->name_p, "italic") ? NH_TRUE : NH_FALSE;

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

void nh_gfx_freeFontStyle(
    nh_gfx_FontStyle *Style_p)
{
NH_GFX_BEGIN()

    nh_core_free(Style_p->name_p);

NH_GFX_SILENT_END()
}

