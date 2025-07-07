#import "Texture.h"
#include <stdlib.h>

nh_gfx_MetalTexture *nh_gfx_createMetalTexture(nh_gfx_MetalDevice *device, int width, int height) {
    nh_gfx_MetalTexture *tex = (nh_gfx_MetalTexture *)malloc(sizeof(nh_gfx_MetalTexture));
    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm
                                                                                     width:width
                                                                                    height:height
                                                                                 mipmapped:NO];
    tex->texture = [device->device newTextureWithDescriptor:desc];
    return tex;
}

void nh_gfx_destroyMetalTexture(nh_gfx_MetalTexture *tex) {
    if (tex) {
        tex->texture = nil;
        free(tex);
    }
} 