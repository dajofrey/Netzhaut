#ifndef NH_WSI_PLATFORMS_ANDROID_ONYX_H
#define NH_WSI_PLATFORMS_ANDROID_ONYX_H

#include "../../Common/Includes.h"
#include <jni.h>

// Boox/Onyx e-paper raw drawing.
// Talks directly to the firmware class android.onyx.ViewUpdateHelper (the same class the
// Onyx pen SDK reflects into), so no Onyx SDK jars are required. SurfaceFlinger then renders
// the stylus strokes in hardware.

typedef enum NH_WSI_ONYX_PEN_STATE_E {
    NH_WSI_ONYX_PEN_STATE_STOP    = 0,
    NH_WSI_ONYX_PEN_STATE_START   = 1,
    NH_WSI_ONYX_PEN_STATE_DRAWING = 2,
    NH_WSI_ONYX_PEN_STATE_PAUSE   = 3,
} NH_WSI_ONYX_PEN_STATE_E;

typedef struct nh_wsi_AndroidOnyx {
    bool initialized;
    bool isOnyxDevice;
    jclass helperClass;
    jobject hostView;
    jmethodID setPenState;
    jmethodID setRegionLimit;
    jmethodID setRegionExclude;
    jmethodID setStrokeWidth;
    jmethodID setStrokeStyle;
    jmethodID setStrokeColor;
    jmethodID enablePost;
    jmethodID setBrushRawDrawingEnabled;
    jmethodID setEraserRawDrawingEnabled;
    bool eraserHasStyle;
    bool opened;
    bool enabled;
    bool renderEnabled;
    bool eraserEnabled;
    float currentStrokeWidth;
    int currentStrokeStyle;
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

NH_API_RESULT nh_wsi_setAndroidRawDrawingEraserEnabled(
    nh_wsi_AndroidOnyx *Onyx_p, bool enabled
);

void nh_wsi_closeAndroidOnyx(
    nh_wsi_AndroidOnyx *Onyx_p
);

#endif
