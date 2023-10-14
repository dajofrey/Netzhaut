#ifndef NH_RENDERER_VERTICES_H
#define NH_RENDERER_VERTICES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nhcss/Main/FragmentTree.h"

#endif

/** @addtogroup lib_nhrenderer_structs
 *  @{
 */

    typedef struct nh_renderer_ClipBox {
        nh_ClipPosition Position;
        nh_ClipSize Size;
        float depth;
    } nh_renderer_ClipBox;

/** @} */

/** @addtogroup lib_nhrenderer_functions
 *  @{
 */

    nh_renderer_ClipBox nh_renderer_convertToClipBox(
        nh_gfx_Viewport *Viewport_p, nh_css_PixelBox PixelBox
    );

    int nh_renderer_getBackgroundVertices(
        nh_gfx_Viewport *Viewport_p, nh_css_Fragment *Fragment_p, float *vertices_p, 
        int cornerTriangleCount
    );

    int nh_renderer_getBorderVertices(
        nh_gfx_Viewport *Viewport_p, nh_css_Fragment *Fragment_p, float *vertices_p, NH_BYTE *side_p, 
        int cornerTriangleCount
    );

    NH_RENDERER_RESULT nh_renderer_getTextVertices(
        nh_gfx_Viewport *Viewport_p, nh_gfx_TextSegment *Segment_p, NH_PIXEL *x_p, NH_PIXEL y, float *z_p,
        float **vertices_pp, uint32_t **indices_pp
    );

/** @} */

#endif 
