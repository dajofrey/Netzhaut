#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef NH_CSS_ASSOCIATE_H
#define NH_CSS_ASSOCIATE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"

#include "../../Core/Header/Tab.h"
#include "../../API/Header/Netzhaut.h"
#include "../../HTML/Header/Document.h"

#endif

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_associateSheets(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_CSS_deactivate(
        Nh_Tab *Tab_p, NH_CSS_PSEUDO_CLASS pseudoClass
    );
    
    NH_RESULT Nh_CSS_activate(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p, NH_CSS_PSEUDO_CLASS pseudoClass
    );

    NH_RESULT Nh_CSS_activateChild(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p, NH_CSS_PSEUDO_CLASS pseudoClass
    );

/** @} */

#endif 
