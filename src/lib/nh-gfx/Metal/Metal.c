#import "Metal.h"

#include <stdlib.h>

nh_gfx_Metal NH_METAL = {0};

nh_gfx_MetalDevice *nh_gfx_createMetalDevice() {
    nh_gfx_MetalDevice *dev = (nh_gfx_MetalDevice *)malloc(sizeof(nh_gfx_MetalDevice));
    dev->device = MTLCreateSystemDefaultDevice();
    dev->commandQueue = [dev->device newCommandQueue];
    return dev;
}

void nh_gfx_destroyMetalDevice(nh_gfx_MetalDevice *dev) {
    if (dev) {
        dev->commandQueue = nil;
        dev->device = nil;
        free(dev);
    }
}

NH_API_RESULT nh_gfx_initMetal() {
    NH_METAL.Device = nh_gfx_createMetalDevice();
    if (!NH_METAL.Device) return NH_API_ERROR_BAD_STATE;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_terminateMetal() {
    nh_gfx_destroyMetalDevice(NH_METAL.Device);
    NH_METAL.Device = NULL;
    return NH_API_SUCCESS;
}

nh_gfx_MetalCommandBuffer *nh_gfx_createMetalCommandBuffer(nh_gfx_MetalDevice *dev) {
    nh_gfx_MetalCommandBuffer *cmd = (nh_gfx_MetalCommandBuffer *)malloc(sizeof(nh_gfx_MetalCommandBuffer));
    cmd->commandBuffer = [dev->commandQueue commandBuffer];
    return cmd;
}

void nh_gfx_commitMetalCommandBuffer(nh_gfx_MetalCommandBuffer *cmd) {
    [cmd->commandBuffer commit];
}

void nh_gfx_destroyMetalCommandBuffer(nh_gfx_MetalCommandBuffer *cmd) {
    if (cmd) {
        cmd->commandBuffer = nil;
        free(cmd);
    }
} 