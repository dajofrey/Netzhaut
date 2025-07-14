#import "Metal.h"

#include <stdlib.h>

nh_gfx_Metal NH_METAL = {0};

NH_API_RESULT nh_gfx_initMetal() {
    NH_METAL.Device_p = nh_gfx_createMetalDevice();
    if (!NH_METAL.Device_p) return NH_API_ERROR_BAD_STATE;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_terminateMetal() {
    nh_gfx_freeMetalDevice(NH_METAL.Device_p);
    NH_METAL.Device_p = NULL;
    return NH_API_SUCCESS;
}
