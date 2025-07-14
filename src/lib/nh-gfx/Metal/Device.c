#include "Device.h"
#include <stdlib.h>

nh_gfx_MetalDevice *nh_gfx_createMetalDevice()
{
    nh_gfx_MetalDevice *Device_p = (nh_gfx_MetalDevice *)nh_core_allocate(sizeof(nh_gfx_MetalDevice));
    Device_p->device = MTLCreateSystemDefaultDevice();
    Device_p->commandQueue = [dev->device newCommandQueue];
    return Device_p;
}

void nh_gfx_freeMetalDevice(
    nh_gfx_MetalDevice *Device_p)
{
    free(Device_p);
} 
