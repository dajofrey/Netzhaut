#ifndef NH_WSI_PLATFORMS_ANDROID_ONYX_H
#define NH_WSI_PLATFORMS_ANDROID_ONYX_H

#include "../../Common/Includes.h"
#include <jni.h>

typedef struct nh_wsi_AndroidOnyx {
    jobject touchHelperGlobalRef;
    jobject rawInputCallbackGlobalRef;
    bool isOnyxDevice;
    int currentRefreshMode; // e.g., REGAL, DU, A2
    int currentStrokeStyle;
    int currentStrokeWidth;
} nh_wsi_AndroidOnyx;

NH_API_RESULT nh_wsi_setAndroidRawDrawingEnabled(
    nh_wsi_AndroidOnyx *Onyx_p, bool enabled
);

NH_API_RESULT nh_wsi_setAndroidRawDrawingRenderEnabled(
    nh_wsi_AndroidOnyx *Onyx_p, bool enabled
);

NH_API_RESULT nh_wsi_setAndroidRawDrawingLimitRect(
    nh_wsi_AndroidOnyx *Onyx_p, int left, int top, int right, int bottom,
    const nh_api_Rect *excludeRects_p, int excludeCount
);

NH_API_RESULT nh_wsi_setAndroidRawDrawingStrokeWidth(
    nh_wsi_AndroidOnyx *Onyx_p, float width
);

NH_API_RESULT nh_wsi_setAndroidRawDrawingStrokeStyle(
    nh_wsi_AndroidOnyx *Onyx_p, int style
);

#endif
