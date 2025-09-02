#ifndef NH_ECMASCRIPT_ENGINE_RUNTIME_H
#define NH_ECMASCRIPT_ENGINE_RUNTIME_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../../nh-core/Util/Stack.h"
#include "../../nh-core/Util/Array.h"

// TYPES ===========================================================================================

typedef enum NH_ECMASCRIPT_RUNTIME_COMMAND_E {
    NH_ECMASCRIPT_RUNTIME_COMMAND_START_AGENT_CLUSTER,
    NH_ECMASCRIPT_RUNTIME_COMMAND_GLOBAL_DECLARATION_INSTANTIATION,
    NH_ECMASCRIPT_RUNTIME_COMMAND_PARSE_SCRIPT,
    NH_ECMASCRIPT_RUNTIME_COMMAND_SCRIPT_EVALUATION,
} NH_ECMASCRIPT_RUNTIME_COMMAND_E;

// FUNCTIONS =======================================================================================

void nh_ecmascript_startRuntime(
    void *args_p
);

void nh_ecmascript_enqueueRuntimeCommand(
);

#endif
