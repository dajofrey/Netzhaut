#ifndef NH_GFX_TAB_H
#define NH_GFX_TAB_H

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

    NH_RESULT Nh_Gfx_createTab(
        Nh_Tab *Tab_p
    );

     NH_RESULT Nh_Gfx_updateTabRendering(
        Nh_Tab *Tab_p
    );
  
    void Nh_Gfx_destroyTab(
        Nh_Tab *Tab_p
    );

/** @} */

#endif 
