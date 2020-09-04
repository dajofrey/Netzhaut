#ifndef NH_CSS_HELPER_H
#define NH_CSS_HELPER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../HTML/Header/Document.h"

typedef struct Nh_Tab Nh_Tab;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_STRIP {
        NH_CSS_STRIP_URL,
    } NH_CSS_STRIP;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    int Nh_CSS_getLengthInPixel(
        Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, char *str_p
    );
    
    void Nh_CSS_getColor(
        char *color_p, float values_p[4]
    );
    
    bool Nh_CSS_respectFlow(
        Nh_HTML_Node *Node_p, NH_BOOL plain
    );
    
    int Nh_CSS_getPrecedence(
        NH_CSS_SELECTOR selector
    );
    
    /**
     * Converts a string with hex color syntax to float values.
     * A hexadecimal color is specified with: #RRGGBB, where the RR (red), GG (green) and BB 
     * (blue) hexadecimal integers specify the components of the color. All values must be 
     * between 00 and FF.
     */ 
    void Nh_CSS_parseHexColor(
        char *str_p, float rgba_p[4]
    );

    void Nh_CSS_strip(
        NH_CSS_STRIP type, char *in_p, char *out_p
    );

/** @} */

#endif 
