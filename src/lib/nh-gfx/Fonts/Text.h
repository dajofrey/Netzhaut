#ifndef NH_GFX_TEXT_H
#define NH_GFX_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "FontManager.h"
#include "FontFamily.h"
#include "FontStyle.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_gfx_TextSegment {
        nh_gfx_FontInstance *FontInstance_p;
        NH_ENCODING_UTF32 *codepoints_p;
        unsigned int length;
    } nh_gfx_TextSegment;

    typedef struct nh_gfx_Text {
        nh_Array Segments;
    } nh_gfx_Text;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_API_RESULT nh_gfx_createText(
        nh_gfx_Text *Text_p, NH_ENCODING_UTF32 *codepoints_p, unsigned int length, int fontSize, 
        nh_Array *FontFamilies_p, nh_gfx_FontStyle FontStyle
    );

    NH_API_RESULT nh_gfx_createTextFromFont(
        nh_gfx_Text *Text_p, NH_ENCODING_UTF32 *codepoints_p, unsigned int length, int fontSize, 
        nh_gfx_Font *Font_p
    );

    void nh_gfx_freeText(
        nh_gfx_Text *Text_p
    );

    int nh_gfx_getTextWidth(
        nh_gfx_Text *Text_p
    );
    
    int nh_gfx_getTextHeight(
        nh_gfx_Text *Text_p
    );

    unsigned int nh_gfx_getFittingTextLength(
        nh_gfx_Text *Text_p, int maxWidth
    );

/** @} */

#endif 
