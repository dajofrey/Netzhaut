#ifndef NH_ECMASCRIPT_INTERPRETER_EVALUATION_H
#define NH_ECMASCRIPT_INTERPRETER_EVALUATION_H

#include "../Engine/Completion.h"
#include "../Parser/Script.h"

nh_ecmascript_Completion nh_ecmascript_evaluateScript(
    nh_ecmascript_Script *Script_p
);

#endif
