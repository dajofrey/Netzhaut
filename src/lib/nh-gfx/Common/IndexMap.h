#ifndef NH_GFX_COMMON_INDEXMAP_H
#define NH_GFX_COMMON_INDEXMAP_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Result.h"
#include "../../nh-core/Util/HashMap.h"

// STRUCTS =========================================================================================

typedef struct nh_gfx_IndexMap {
    nh_core_HashMap OpenGLCommandNames;
} nh_gfx_IndexMap;

extern nh_gfx_IndexMap NH_GFX_INDEXMAP;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_createIndexMap(
);

void nh_gfx_freeIndexMap(
);

#endif // NH_GFX_COMMON_INDEXMAP_H 
