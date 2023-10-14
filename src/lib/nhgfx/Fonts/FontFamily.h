#ifndef NH_GFX_FONT_FAMILY_H
#define NH_GFX_FONT_FAMILY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhgfx_enums
 *  @{
 */

    typedef enum NH_GFX_GENERIC_FONT_FAMILY {
        NH_GFX_GENERIC_FONT_FAMILY_SERIF = 0,
        NH_GFX_GENERIC_FONT_FAMILY_SANS_SERIF,
        NH_GFX_GENERIC_FONT_FAMILY_CURSIVE,
        NH_GFX_GENERIC_FONT_FAMILY_FANTASY,
        NH_GFX_GENERIC_FONT_FAMILY_MONOSPACE,
        NH_GFX_GENERIC_FONT_FAMILY_SYSTEM_UI,
        NH_GFX_GENERIC_FONT_FAMILY_EMOJI,
        NH_GFX_GENERIC_FONT_FAMILY_MATH,
        NH_GFX_GENERIC_FONT_FAMILY_FANGSONG,
        NH_GFX_GENERIC_FONT_FAMILY_UI_SERIF,
        NH_GFX_GENERIC_FONT_FAMILY_UI_SANS_SERIF,
        NH_GFX_GENERIC_FONT_FAMILY_UI_MONOSPACE,
        NH_GFX_GENERIC_FONT_FAMILY_UI_ROUNDED,
        NH_GFX_GENERIC_FONT_FAMILY_COUNT,
    } NH_GFX_GENERIC_FONT_FAMILY;

/** @} */

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

    typedef struct nh_gfx_FontFamily {
        NH_BYTE *name_p;
        NH_BOOL generic_p[NH_GFX_GENERIC_FONT_FAMILY_COUNT];
    } nh_gfx_FontFamily;

/** @} */

/** @addtogroup lib_nhgfx_vars
 *  @{
 */

    extern NH_BYTE *NH_GFX_GENERIC_FONT_FAMILY_NAMES_PP[];

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    nh_gfx_FontFamily nh_gfx_initFontFamily(
        NH_BYTE *name_p
    );

    NH_GFX_RESULT nh_gfx_parseFontFamily(
        nh_gfx_FontFamily *FontFamily_p, NH_BYTE *name_p
    );

    void nh_gfx_freeFontFamily(
        nh_gfx_FontFamily *Family_p
    );

/** @} */

#endif 
