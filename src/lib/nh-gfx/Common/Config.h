#ifndef NH_GFX_CONFIG_H
#define NH_GFX_CONFIG_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include <stddef.h>

typedef struct nh_gfx_Config {
    nh_api_PixelSize ViewportSize;
    nh_api_PixelPosition ViewportPosition;
} nh_gfx_Config;

nh_gfx_Config nh_gfx_getConfig(
);

#endif
