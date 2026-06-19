#import "Device.h"
#import "DeviceImpl.h"

#include "../../nh-core/System/Memory.h"

#include <stdlib.h>

nh_gfx_MetalDevice *nh_gfx_createMetalDevice()
{
    nh_gfx_MetalDevice *Device_p = (nh_gfx_MetalDevice *)nh_core_allocate(sizeof(nh_gfx_MetalDevice));
    Device_p->device = MTLCreateSystemDefaultDevice();
    Device_p->commandQueue = [Device_p->device newCommandQueue];
    return Device_p;
}

void nh_gfx_freeMetalDevice(
    nh_gfx_MetalDevice *Device_p)
{
    free(Device_p);
} 
