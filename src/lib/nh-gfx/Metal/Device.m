#import "Device.h"
#import "DeviceImpl.h"

#include "../../nh-core/System/Memory.h"

#include <stdlib.h>
#include <string.h>

nh_gfx_MetalDevice *nh_gfx_createMetalDevice()
{
    nh_gfx_MetalDevice *Device_p = (nh_gfx_MetalDevice *)nh_core_allocate(sizeof(nh_gfx_MetalDevice));
    if (!Device_p) return NULL;
    memset(Device_p, 0, sizeof(*Device_p));
    Device_p->device = MTLCreateSystemDefaultDevice();
    Device_p->commandQueue = [Device_p->device newCommandQueue];
    if (!Device_p->device || !Device_p->commandQueue) {
        NSLog(@"Netzhaut: could not initialize Metal device and command queue");
        nh_gfx_freeMetalDevice(Device_p);
        return NULL;
    }
    return Device_p;
}

void nh_gfx_freeMetalDevice(
    nh_gfx_MetalDevice *Device_p)
{
    if (!Device_p) return;
    Device_p->commandQueue = nil;
    Device_p->device = nil;
    nh_core_free(Device_p);
} 
