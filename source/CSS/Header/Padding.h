#ifndef NH_CSS_PADDING_H
#define NH_CSS_PADDING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"
#include "../../HTML/Header/Document.h"

#endif

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_setDefaultPaddingProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );
    
    void Nh_CSS_setDefaultPaddingProperty(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type
    );
    
    NH_RESULT Nh_CSS_computePaddingProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp
    );

/** @} */

#endif
