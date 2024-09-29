#ifndef NH_RENDERER_VERTICES_H
#define NH_RENDERER_VERTICES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nh-css/Main/FragmentTree.h"
#include "../../nh-gfx/Base/Viewport.h"

#endif

/** @addtogroup lib_nh-renderer_structs
 *  @{
 */

    typedef struct nh_renderer_ClipBox {
        nh_ClipPosition Position;
        nh_ClipSize Size;
        float depth;
    } nh_renderer_ClipBox;

/** @} */

/** @addtogroup lib_nh-renderer_functions
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
        nh_gfx_Viewport *Viewport_p, nh_css_Fragment *Fragment_p, float *vertices_p, char *side_p, 
        int cornerTriangleCount
    );

    NH_API_RESULT nh_renderer_getTextVertices(
        nh_gfx_Viewport *Viewport_p, nh_gfx_TextSegment *Segment_p, int *x_p, int y, float *z_p,
        float **vertices_pp, uint32_t **indices_pp
    );

/** @} */

#endif 
