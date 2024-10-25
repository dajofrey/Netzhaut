#ifndef NH_RENDERER_BOX_TRIANGULATION_H
#define NH_RENDERER_BOX_TRIANGULATION_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Vertices.h"
#include "../Common/Includes.h"
#include "../../nh-core/Util/Math.h"

// FUNCTIONS =======================================================================================

int nh_renderer_triangulateBox(
    nh_renderer_ClipBox EnclosingBox, nh_renderer_ClipBox EnclosedBox, float radii_p[4], int cornerTriangleCount, 
    char *side_p, nh_Triangle *Triangles_p, bool enclosedPoint, int offset, float z
);

#endif // NH_RENDERER_BOX_TRIANGULATION_H
