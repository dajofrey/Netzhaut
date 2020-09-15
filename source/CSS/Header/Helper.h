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
    
    bool Nh_CSS_respectFlow(
        Nh_HTML_Node *Node_p, NH_BOOL plain
    );
    
    int Nh_CSS_getPrecedence(
        NH_CSS_SELECTOR selector
    );
    
    void Nh_CSS_strip(
        NH_CSS_STRIP type, char *in_p, char *out_p
    );

/** @} */

#endif 
