#ifndef NH_CSS_VERTICES_H
#define NH_CSS_VERTICES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../HTML/Header/Document.h"
#include "../../API/Header/Netzhaut.h"
#include "../../CSS/Header/Image.h"

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_BORDER {
        NH_CSS_BORDER_TOP,
        NH_CSS_BORDER_RIGHT,
        NH_CSS_BORDER_BOTTOM,
        NH_CSS_BORDER_LEFT,
    } NH_CSS_BORDER;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    int Nh_CSS_getBackgroundVertices(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float *vertices_p, int cornerTriangleCount
    );
    
    NH_RESULT Nh_CSS_getBackgroundImageVertices(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float vertices_p[30], NH_CSS_Image *Image_p,
        float subtractFromZ
    );
    
    int Nh_CSS_getBorderVertices(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float *vertices_p, NH_CSS_BORDER type,
        int cornerTriangleCount
    );
    
    NH_RESULT Nh_CSS_getImageVertices(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float vertices_p[30], float subtractFromZ
    );

    NH_RESULT Nh_CSS_getTextVertices(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float **vertices_pp, uint32_t **indices_pp
    );

/** @} */

#endif 
