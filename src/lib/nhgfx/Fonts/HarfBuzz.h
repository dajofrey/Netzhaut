#ifndef NH_GFX_HARFBUZZ_H
#define NH_GFX_HARFBUZZ_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "FontManager.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhgfx_structs
 *  @{
 */

    typedef struct nh_gfx_HarfBuzzBuffer {
        void *external_p;
    } nh_gfx_HarfBuzzBuffer;

    typedef struct nh_gfx_HarfBuzzGlyphInfo {
        NH_ENCODING_UTF32 id;
        int32_t xOffset;
        int32_t yOffset;
        int32_t xAdvance;
        int32_t yAdvance;
    } nh_gfx_HarfBuzzGlyphInfo;

/** @} */

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    nh_gfx_HarfBuzzBuffer nh_gfx_createHarfBuzzBuffer(
        nh_gfx_FontInstance *Instance_p, NH_ENCODING_UTF32 *text_p, unsigned int textLength
    );
    
    void nh_gfx_destroyHarfBuzzBuffer(
        nh_gfx_HarfBuzzBuffer Buffer
    );

    nh_gfx_HarfBuzzGlyphInfo *nh_gfx_getHarfBuzzGlyphInfos(
        nh_gfx_HarfBuzzBuffer Buffer, unsigned int *glyphs_p
    );

/** @} */

#endif 
