#ifndef NH_GFX_NODE_H
#define NH_GFX_NODE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../HTML/Header/Document.h"
#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/Tab.h"

#endif

/** @addtogroup GraphicsFunctions Functions
 *  \ingroup Graphics
 *  @{
 */

    NH_RESULT Nh_Gfx_createNode(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );

    void Nh_Gfx_destroyNode(
        Nh_GPU *GPU_p, Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_Gfx_updateNodeProperty(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p
    );

/** @} */

#endif 
