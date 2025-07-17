#ifndef NH_GFX_COMMON_CONFIG_H
#define NH_GFX_COMMON_CONFIG_H

#include "Includes.h"
#include "../Base/Surface.h"

typedef struct nh_gfx_Config {
    NH_GFX_API_E api;
    nh_api_PixelSize ViewportSize;
    nh_api_PixelPosition ViewportPosition;
} nh_gfx_Config;

nh_gfx_Config nh_gfx_getConfig(
);

#endif
