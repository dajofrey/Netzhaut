#ifndef NH_CSS_BOX_H
#define NH_CSS_BOX_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Image.h"

#include "../../Core/Header/Math.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_Tab Nh_Tab;

#endif

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct Nh_CSS_Box {
        Nh_Vector3D TopLeft;
        Nh_Vector3D BottomRight;
    } Nh_CSS_Box;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_getEnclosingBox(
        Nh_Tab *Tab_p, Nh_CSS_Box *Box_p
    );

    Nh_CSS_Box Nh_CSS_getBackgroundImageBox(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p, float subtractFromZ
    );

    Nh_CSS_Box Nh_CSS_getContentBox(
        Nh_HTML_Node *Node_p
    );
        
    Nh_CSS_Box Nh_CSS_getPaddingBox(
        Nh_HTML_Node *Node_p
    );
    
    Nh_CSS_Box Nh_CSS_getBorderBox(
        Nh_HTML_Node *Node_p
    );

    void Nh_CSS_getBoxOffsetRelToTab(
        Nh_Tab *Tab_p, Nh_CSS_Box *Box_p, int xy_p[2]
    );
    
    void Nh_CSS_getBoxSize(
        Nh_Tab *Tab_p, Nh_CSS_Box *Box_p, int wh_p[2]
    );
    
    void Nh_CSS_getBoxOffsetRelToWindow(
        Nh_Tab *Tab_p, Nh_CSS_Box *Box_p, int xy_p[2]
    );
    
    void Nh_CSS_resize(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_Box *Box_p
    );

    Nh_CSS_Box Nh_CSS_initBox(
    );

/** @} */

#endif
