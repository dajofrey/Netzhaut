#ifndef NH_GFX_FONTS_FONT_STYLE_H
#define NH_GFX_FONTS_FONT_STYLE_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

// ENUMS ===========================================================================================

typedef enum NH_GFX_FONT_WEIGHT_E {
    NH_GFX_FONT_WEIGHT_THIN = 100,
    NH_GFX_FONT_WEIGHT_EXTRALIGHT = 200,
    NH_GFX_FONT_WEIGHT_ULTRALIGHT = 200,
    NH_GFX_FONT_WEIGHT_LIGHT = 300,
    NH_GFX_FONT_WEIGHT_NORMAL = 400,
    NH_GFX_FONT_WEIGHT_REGULAR = 400,
    NH_GFX_FONT_WEIGHT_MEDIUM = 500,
    NH_GFX_FONT_WEIGHT_SEMIBOLD = 600,
    NH_GFX_FONT_WEIGHT_DEMIBOLD = 600,
    NH_GFX_FONT_WEIGHT_BOLD = 700,
    NH_GFX_FONT_WEIGHT_EXTRABOLD = 800,
    NH_GFX_FONT_WEIGHT_ULTRABOLD = 800,
    NH_GFX_FONT_WEIGHT_BLACK = 900,
    NH_GFX_FONT_WEIGHT_HEAVY = 900,
} NH_GFX_FONT_WEIGHT_E;

// STRUCTS =========================================================================================

typedef struct nh_gfx_FontStyle {
    char *name_p;
    NH_GFX_FONT_WEIGHT_E weight;
    bool oblique;
    bool italic;
} nh_gfx_FontStyle;

// FUNCTIONS =======================================================================================

typedef struct nh_gfx_Font nh_gfx_Font;

nh_gfx_Font *nh_gfx_getLighterFontVariant(
    nh_gfx_Font *Font_p
);

nh_gfx_Font *nh_gfx_getBolderFontVariant(
    nh_gfx_Font *Font_p
);

NH_API_RESULT nh_gfx_parseFontStyle(
    nh_gfx_FontStyle *Style_p, char *name_p
);

void nh_gfx_freeFontStyle(
    nh_gfx_FontStyle *Style_p
);


#endif // NH_GFX_FONTS_FONT_STYLE_H
