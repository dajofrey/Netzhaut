#ifndef NH_RENDERER_BOX_TRIANGULATION_H
#define NH_RENDERER_BOX_TRIANGULATION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Vertices.h"

#include "../Common/Includes.h"

#include "../../nh-core/Util/Math.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    int nh_renderer_triangulateBox(
        nh_renderer_ClipBox EnclosingBox, nh_renderer_ClipBox EnclosedBox, float radii_p[4], int cornerTriangleCount, 
        NH_BYTE *side_p, nh_Triangle *Triangles_p, NH_BOOL enclosedPoint, int offset, float z
    );

/** @} */

#endif
