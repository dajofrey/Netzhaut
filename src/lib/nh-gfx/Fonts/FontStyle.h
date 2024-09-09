#ifndef NH_GFX_FONT_STYLE_H
#define NH_GFX_FONT_STYLE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

typedef struct nh_gfx_Font nh_gfx_Font;

#endif

/** @addtogroup lib_nh-gfx_enums
 *  @{
 */

    typedef enum NH_GFX_FONT_WEIGHT {
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
    } NH_GFX_FONT_WEIGHT;

/** @} */

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_gfx_FontStyle {
        NH_BYTE *name_p;
        NH_GFX_FONT_WEIGHT weight;
        NH_BOOL oblique;
        NH_BOOL italic;
    } nh_gfx_FontStyle;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    nh_gfx_Font *nh_gfx_getLighterFontVariant(
        nh_gfx_Font *Font_p
    );
    
    nh_gfx_Font *nh_gfx_getBolderFontVariant(
        nh_gfx_Font *Font_p
    );

    NH_GFX_RESULT nh_gfx_parseFontStyle(
        nh_gfx_FontStyle *Style_p, NH_BYTE *name_p
    );

    void nh_gfx_freeFontStyle(
        nh_gfx_FontStyle *Style_p
    );

/** @} */

#endif 
