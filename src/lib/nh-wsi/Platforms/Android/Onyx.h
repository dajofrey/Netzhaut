#ifndef NH_WSI_PLATFORMS_ANDROID_ONYX_H
#define NH_WSI_PLATFORMS_ANDROID_ONYX_H

#include "../../Common/Includes.h"
#include <jni.h>
#include "OnyxFrame.h"

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
    jmethodID applyTransientUpdate;
    jmethodID clearTransientUpdate;
    jmethodID refreshScreen;
    jmethodID getLocationOnScreen;
    bool eraserHasStyle;
    bool opened;
    bool enabled;
    bool renderEnabled;
    bool eraserEnabled;
    float currentStrokeWidth;
    int currentStrokeStyle;
    int updateMode;
    int64_t disablePostAt; // Monotonic ms, 0 if not scheduled.
    nh_wsi_OnyxFrame Frame;
    nh_api_Rect Limit;
    nh_api_Rect *excludeRects_p;
    int excludeCount;
    bool hasLimit;
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

/**
 * The firmware stops posting the app's frames to the panel as soon as the pen touches down in
 * the raw drawing region, so that app rendering doesn't overwrite the hardware strokes. This
 * posts them again and must be called before the app shows content the hardware strokes don't
 * already show, e.g. on panning. Stroke handoff uses nh_wsi_postAndroidOnyxFrame
 * to enable posting immediately before submitting a GPU-completed canvas buffer.
 */
void nh_wsi_enableAndroidOnyxPost(
    nh_wsi_AndroidOnyx *Onyx_p
);

void nh_wsi_beginAndroidOnyxStroke(
    nh_wsi_AndroidOnyx *Onyx_p, float x, float y
);

bool nh_wsi_isAndroidOnyxTraceEnabled(void);

/**
 * Stops posting the app's frames again after delayMs, once the last frame reached the panel.
 * Then the next pen down doesn't have to wait for the firmware. Cancelled by
 * nh_wsi_enableAndroidOnyxPost.
 */
void nh_wsi_scheduleAndroidOnyxPostDisable(
    nh_wsi_AndroidOnyx *Onyx_p, int delayMs
);

NH_API_RESULT nh_wsi_postAndroidOnyxFrame(
    nh_wsi_AndroidOnyx *Onyx_p, uint64_t generation,
    NH_API_RESULT (*post_f)(void *), void *user_p
);

/**
 * Must be called regularly from the event loop to run scheduled work.
 */
void nh_wsi_updateAndroidOnyx(
    nh_wsi_AndroidOnyx *Onyx_p
);

NH_API_RESULT nh_wsi_setAndroidEPaperUpdateMode(
    nh_wsi_AndroidOnyx *Onyx_p, int mode
);

NH_API_RESULT nh_wsi_requestAndroidEPaperRefreshRect(
    nh_wsi_AndroidOnyx *Onyx_p, int left, int top, int right, int bottom, int mode
);

void nh_wsi_closeAndroidOnyx(
    nh_wsi_AndroidOnyx *Onyx_p
);

#endif
