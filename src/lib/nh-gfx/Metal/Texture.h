#ifndef NH_GFX_METAL_TEXTURE_H
#define NH_GFX_METAL_TEXTURE_H

#import <Metal/Metal.h>
#include "Metal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nh_gfx_MetalTexture {
    id<MTLTexture> texture;
} nh_gfx_MetalTexture;

nh_gfx_MetalTexture *nh_gfx_createMetalTexture(nh_gfx_MetalDevice *device, int width, int height);
void nh_gfx_destroyMetalTexture(nh_gfx_MetalTexture *texture);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_TEXTURE_H 