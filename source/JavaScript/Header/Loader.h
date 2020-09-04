#ifndef NH_JS_LOADER_H
#define NH_JS_LOADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Script.h"

#include "../../Core/Header/Tab.h"
#include "../../API/Header/Netzhaut.h"

#endif

/** @addtogroup JSFunctions Functions
 *  \ingroup JS
 *  @{
 */

    NH_RESULT Nh_JS_loadScript(
        Nh_Tab *Tab_p, Nh_JS_Script *Script_p
    );
    
    void Nh_JS_unloadScript(
        Nh_JS_Script *Script_p
    );

/** @} */

#endif
