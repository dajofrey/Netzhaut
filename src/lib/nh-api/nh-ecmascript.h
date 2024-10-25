#ifndef NH_API_NH_ECMASCRIPT_H
#define NH_API_NH_ECMASCRIPT_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-core.h"

// TYPEDEFS ========================================================================================

typedef struct nh_api_Script nh_api_Script;
typedef struct nh_api_Realm nh_api_Realm;

// FUNCTIONS =======================================================================================

/**
 * Todo.
 */
nh_api_Script *nh_api_parseScript(
    char *sourceText_p, nh_api_Realm *Realm_p, int encoding
);

#endif // NH_API_NH_ECMASCRIPT_H
