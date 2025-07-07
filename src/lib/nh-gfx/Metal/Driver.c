#include "Driver.h"
#include <stdlib.h>

nh_gfx_MetalDriver *nh_gfx_createMetalDriver() {
    nh_gfx_MetalDriver *driver = (nh_gfx_MetalDriver *)malloc(sizeof(nh_gfx_MetalDriver));
    driver->device = nh_gfx_createMetalDevice();
    return driver;
}

void nh_gfx_destroyMetalDriver(nh_gfx_MetalDriver *driver) {
    if (driver) {
        nh_gfx_destroyMetalDevice(driver->device);
        free(driver);
    }
} 