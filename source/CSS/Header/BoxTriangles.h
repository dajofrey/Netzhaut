#ifndef NH_CSS_BOX_TRIANGLES_H
#define NH_CSS_BOX_TRIANGLES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Box.h"

#endif

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    int Nh_CSS_getBoxTriangles(
        Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, float radii_p[4], int cornerTriangleCount, 
        int side, Nh_Triangle *Triangles_p, bool enclosedPoint, int offset, float z
    );

/** @} */

#endif
