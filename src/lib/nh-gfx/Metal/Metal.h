#ifndef NH_GFX_METAL_H
#define NH_GFX_METAL_H

#if defined(__APPLE__)
    #include <Metal/Metal.h>
#endif

#include "../Common/Includes.h"

#ifdef __cplusplus
extern "C" {
#endif

// Global Metal context
typedef struct nh_gfx_Metal {
    nh_gfx_MetalDevice *Device_p;
} nh_gfx_Metal;

extern nh_gfx_Metal NH_METAL;

// Metal initialization/termination
NH_API_RESULT nh_gfx_initMetal(void);
NH_API_RESULT nh_gfx_terminateMetal(void);

#ifdef __cplusplus
}
#endif

#endif // NH_GFX_METAL_H 
