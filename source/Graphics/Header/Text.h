#ifndef NH_GFX_TEXT_H
#define NH_GFX_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Window.h"

#include "../../HTML/Header/Document.h"
#include "../../API/Header/Netzhaut.h"

#include <stdint.h>

#endif

/** @addtogroup GraphicsFunctions Functions
 *  \ingroup Graphics
 *  @{
 */

    NH_RESULT Nh_Gfx_updateText(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );

/** @} */

#endif 
