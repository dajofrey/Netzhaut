#ifndef NH_ECMASCRIPT_ENGINE_RUNTIME_H
#define NH_ECMASCRIPT_ENGINE_RUNTIME_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nh-core/Util/Stack.h"
#include "../../nh-core/Util/Array.h"

// FUNCTIONS =======================================================================================

nh_api_Runtime *nh_ecmascript_startRuntime(
    void *args_p
);

nh_api_Agent *nh_ecmascript_createAgent(
    nh_api_Runtime *Runtime_p
);

#endif
