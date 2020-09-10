#ifndef NH_CSS_BORDER_H
#define NH_CSS_BORDER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Utils.h"
#include "GenericProperty.h"

#include "../../API/Header/Netzhaut.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_Tab Nh_Tab;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_BORDER_STYLE {
        NH_CSS_BORDER_STYLE_NONE,
        NH_CSS_BORDER_STYLE_HIDDEN,
        NH_CSS_BORDER_STYLE_DOTTED,
        NH_CSS_BORDER_STYLE_DASHED,
        NH_CSS_BORDER_STYLE_SOLID,
        NH_CSS_BORDER_STYLE_DOUBLE,
        NH_CSS_BORDER_STYLE_GROOVE,
        NH_CSS_BORDER_STYLE_RIDGE,
        NH_CSS_BORDER_STYLE_INSET,
        NH_CSS_BORDER_STYLE_OUTSET,
    } NH_CSS_BORDER_STYLE;

/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_Border {
        
        struct Radii {
            NH_CSS_Corners Values_p[2];
        } Radii;

        NH_CSS_SidesColor Color;
        NH_CSS_Sides Width;
        NH_CSS_Types Style;

    } NH_CSS_Border;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_setDefaultBorderProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );
    
    void Nh_CSS_setDefaultBorderProperty(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type
    );
    
    NH_RESULT Nh_CSS_computeBorderProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp
    );

/** @} */

#endif 
