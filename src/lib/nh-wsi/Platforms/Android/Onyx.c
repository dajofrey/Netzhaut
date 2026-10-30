#include "Onyx.h"
#include "Window.h"

#include <jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <string.h>

#define NH_WSI_ONYX_TAG "Netzhaut-Onyx"
#define NH_WSI_ONYX_LOG(...) __android_log_print(ANDROID_LOG_INFO, NH_WSI_ONYX_TAG, __VA_ARGS__)
#define NH_WSI_ONYX_ERR(...) __android_log_print(ANDROID_LOG_ERROR, NH_WSI_ONYX_TAG, __VA_ARGS__)

// JNI HELPERS =====================================================================================

typedef struct nh_wsi_OnyxJNI {
    JavaVM *vm_p;
    JNIEnv *env;
    bool detach;
} nh_wsi_OnyxJNI;

static bool nh_wsi_onyxAttach(
    nh_wsi_OnyxJNI *JNI_p)
{
    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity || !app_p->activity->vm) {
        return false;
    }
    JNI_p->vm_p = app_p->activity->vm;
    JNI_p->env = NULL;
    JNI_p->detach = false;
    jint status = (*JNI_p->vm_p)->GetEnv(JNI_p->vm_p, (void**)&JNI_p->env, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
        if ((*JNI_p->vm_p)->AttachCurrentThread(JNI_p->vm_p, &JNI_p->env, NULL) != JNI_OK) {
            return false;
        }
        JNI_p->detach = true;
    } else if (status != JNI_OK) {
        return false;
    }
    return true;
}

static void nh_wsi_onyxDetach(
    nh_wsi_OnyxJNI *JNI_p)
{
    if (JNI_p->detach) {
        (*JNI_p->vm_p)->DetachCurrentThread(JNI_p->vm_p);
    }
}

// Returns true and clears the exception if one is pending.
static bool nh_wsi_onyxCheckException(
    JNIEnv *env, const char *what_p)
{
    if (!(*env)->ExceptionCheck(env)) {
        return false;
    }
    if (what_p) {
        NH_WSI_ONYX_ERR("Java exception in %s", what_p);
        (*env)->ExceptionDescribe(env);
    }
    (*env)->ExceptionClear(env);
    return true;
}

static jmethodID nh_wsi_onyxGetStaticMethod(
    JNIEnv *env, jclass class, const char *name_p, const char *signature_p, bool required)
{
    jmethodID method = (*env)->GetStaticMethodID(env, class, name_p, signature_p);
    if (nh_wsi_onyxCheckException(env, NULL) || !method) {
        if (required) {
            NH_WSI_ONYX_ERR("ViewUpdateHelper.%s%s not available", name_p, signature_p);
        }
        return NULL;
    }
    return method;
}

static jobject nh_wsi_onyxGetDecorView(
    JNIEnv *env)
{
    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity || !app_p->activity->clazz) {
        return NULL;
    }

    jobject decorView = NULL;
    jclass activityClass = (*env)->GetObjectClass(env, app_p->activity->clazz);
    jmethodID getWindow = (*env)->GetMethodID(env, activityClass, "getWindow", "()Landroid/view/Window;");
    jobject window = getWindow ? (*env)->CallObjectMethod(env, app_p->activity->clazz, getWindow) : NULL;
    if (!nh_wsi_onyxCheckException(env, "Activity.getWindow") && window) {
        jclass windowClass = (*env)->GetObjectClass(env, window);
        jmethodID getDecorView = (*env)->GetMethodID(env, windowClass, "getDecorView", "()Landroid/view/View;");
        decorView = getDecorView ? (*env)->CallObjectMethod(env, window, getDecorView) : NULL;
        if (nh_wsi_onyxCheckException(env, "Window.getDecorView")) {
            decorView = NULL;
        }
        (*env)->DeleteLocalRef(env, windowClass);
        (*env)->DeleteLocalRef(env, window);
    }
    (*env)->DeleteLocalRef(env, activityClass);
    return decorView;
}

// INIT ============================================================================================

static bool nh_wsi_initAndroidOnyx(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env)
{
    if (Onyx_p->initialized) {
        return Onyx_p->isOnyxDevice;
    }
    Onyx_p->initialized = true;
    Onyx_p->isOnyxDevice = false;

    jclass helperClass = (*env)->FindClass(env, "android/onyx/ViewUpdateHelper");
    if (nh_wsi_onyxCheckException(env, NULL) || !helperClass) {
        NH_WSI_ONYX_LOG("android.onyx.ViewUpdateHelper not found, no Boox raw drawing support");
        return false;
    }

    Onyx_p->setPenState      = nh_wsi_onyxGetStaticMethod(env, helperClass, "setScreenHandWritingPenState", "(I)V", true);
    Onyx_p->setRegionLimit   = nh_wsi_onyxGetStaticMethod(env, helperClass, "setScreenHandWritingRegionLimit", "(Landroid/view/View;[I)V", true);
    Onyx_p->setRegionExclude = nh_wsi_onyxGetStaticMethod(env, helperClass, "setScreenHandWritingRegionExclude", "(Landroid/view/View;[I)V", false);
    Onyx_p->setStrokeWidth   = nh_wsi_onyxGetStaticMethod(env, helperClass, "setStrokeWidth", "(F)V", true);
    Onyx_p->setStrokeStyle   = nh_wsi_onyxGetStaticMethod(env, helperClass, "setStrokeStyle", "(I)V", true);
    Onyx_p->setStrokeColor   = nh_wsi_onyxGetStaticMethod(env, helperClass, "setStrokeColor", "(I)V", false);
    Onyx_p->enablePost       = nh_wsi_onyxGetStaticMethod(env, helperClass, "enablePost", "(I)V", false);
    Onyx_p->setBrushRawDrawingEnabled = nh_wsi_onyxGetStaticMethod(env, helperClass, "setBrushRawDrawingEnabled", "(Z)V", false);
    Onyx_p->setEraserRawDrawingEnabled = nh_wsi_onyxGetStaticMethod(env, helperClass, "setEraserRawDrawingEnabled", "(ZI)V", false);
    Onyx_p->eraserHasStyle = Onyx_p->setEraserRawDrawingEnabled != NULL;
    if (!Onyx_p->setEraserRawDrawingEnabled) {
        Onyx_p->setEraserRawDrawingEnabled = nh_wsi_onyxGetStaticMethod(env, helperClass, "setEraserRawDrawingEnabled", "(Z)V", false);
    }

    if (!Onyx_p->setPenState || !Onyx_p->setRegionLimit || !Onyx_p->setStrokeWidth || !Onyx_p->setStrokeStyle) {
        (*env)->DeleteLocalRef(env, helperClass);
        return false;
    }

    Onyx_p->helperClass = (jclass)(*env)->NewGlobalRef(env, helperClass);
    (*env)->DeleteLocalRef(env, helperClass);

    jobject decorView = nh_wsi_onyxGetDecorView(env);
    if (decorView) {
        Onyx_p->hostView = (*env)->NewGlobalRef(env, decorView);
        (*env)->DeleteLocalRef(env, decorView);
    }

    Onyx_p->currentStrokeWidth = 3.0f;
    Onyx_p->currentStrokeStyle = 0;
    Onyx_p->isOnyxDevice = true;

    NH_WSI_ONYX_LOG("Boox raw drawing available (hostView=%p)", Onyx_p->hostView);
    return true;
}

// Attaches to the VM and lazily initializes. Returns false if raw drawing is unavailable.
static bool nh_wsi_onyxBegin(
    nh_wsi_AndroidOnyx *Onyx_p, nh_wsi_OnyxJNI *JNI_p)
{
    if (!Onyx_p || !nh_wsi_onyxAttach(JNI_p)) {
        return false;
    }
    if (!nh_wsi_initAndroidOnyx(Onyx_p, JNI_p->env)) {
        nh_wsi_onyxDetach(JNI_p);
        return false;
    }
    return true;
}

// PRIMITIVES ======================================================================================

static void nh_wsi_onyxCallInt(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env, jmethodID method, int value, const char *what_p)
{
    if (!method) {return;}
    (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, method, (jint)value);
    nh_wsi_onyxCheckException(env, what_p);
}

static void nh_wsi_onyxCallBool(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env, jmethodID method, bool value, const char *what_p)
{
    if (!method) {return;}
    (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, method, value ? JNI_TRUE : JNI_FALSE);
    nh_wsi_onyxCheckException(env, what_p);
}

static void nh_wsi_onyxSetPenState(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env, NH_WSI_ONYX_PEN_STATE_E state)
{
    nh_wsi_onyxCallInt(Onyx_p, env, Onyx_p->setPenState, state, "setScreenHandWritingPenState");
}

static void nh_wsi_onyxSetEraser(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env, bool enabled)
{
    if (!Onyx_p->setEraserRawDrawingEnabled) {return;}
    if (Onyx_p->eraserHasStyle) {
        (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->setEraserRawDrawingEnabled,
            enabled ? JNI_TRUE : JNI_FALSE, (jint)5);
    } else {
        (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->setEraserRawDrawingEnabled,
            enabled ? JNI_TRUE : JNI_FALSE);
    }
    nh_wsi_onyxCheckException(env, "setEraserRawDrawingEnabled");
}

static void nh_wsi_onyxApplyStroke(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env)
{
    (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->setStrokeWidth, (jfloat)Onyx_p->currentStrokeWidth);
    nh_wsi_onyxCheckException(env, "setStrokeWidth");
    nh_wsi_onyxCallInt(Onyx_p, env, Onyx_p->setStrokeStyle, Onyx_p->currentStrokeStyle, "setStrokeStyle");
    nh_wsi_onyxCallInt(Onyx_p, env, Onyx_p->setStrokeColor, (int)0xFF000000, "setStrokeColor");
}

static void nh_wsi_onyxOpen(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env)
{
    if (Onyx_p->opened) {return;}
    nh_wsi_onyxApplyStroke(Onyx_p, env);
    nh_wsi_onyxSetPenState(Onyx_p, env, NH_WSI_ONYX_PEN_STATE_START);
    nh_wsi_onyxCallBool(Onyx_p, env, Onyx_p->setBrushRawDrawingEnabled, true, "setBrushRawDrawingEnabled");
    nh_wsi_onyxSetEraser(Onyx_p, env, Onyx_p->eraserEnabled);
    Onyx_p->opened = true;
    Onyx_p->renderEnabled = false;
}

static void nh_wsi_onyxSetRender(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env, bool enabled)
{
    if (enabled) {
        nh_wsi_onyxSetPenState(Onyx_p, env, NH_WSI_ONYX_PEN_STATE_DRAWING);
    } else {
        // Equivalent to the SDK's leaveScribbleMode + pauseDrawing.
        nh_wsi_onyxCallInt(Onyx_p, env, Onyx_p->enablePost, 1, "enablePost");
        nh_wsi_onyxSetPenState(Onyx_p, env, NH_WSI_ONYX_PEN_STATE_PAUSE);
    }
    Onyx_p->renderEnabled = enabled;
}

// API =============================================================================================

NH_API_RESULT nh_wsi_setAndroidRawDrawingEnabled(
    nh_wsi_AndroidOnyx *Onyx_p, bool enabled)
{
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        return NH_API_ERROR_BAD_STATE;
    }

    if (enabled) {
        nh_wsi_onyxOpen(Onyx_p, JNI.env);
        nh_wsi_onyxSetRender(Onyx_p, JNI.env, true);
    } else if (Onyx_p->opened) {
        nh_wsi_onyxSetRender(Onyx_p, JNI.env, false);
    }
    Onyx_p->enabled = enabled;

    nh_wsi_onyxDetach(&JNI);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidRawDrawingRenderEnabled(
    nh_wsi_AndroidOnyx *Onyx_p, bool enabled)
{
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        return NH_API_ERROR_BAD_STATE;
    }

    NH_API_RESULT result = NH_API_SUCCESS;
    if (!Onyx_p->opened) {
        // Like the SDK, render state can only be toggled after raw drawing was opened.
        result = NH_API_ERROR_BAD_STATE;
    } else if (Onyx_p->renderEnabled != enabled) {
        nh_wsi_onyxSetRender(Onyx_p, JNI.env, enabled);
    }

    nh_wsi_onyxDetach(&JNI);
    return result;
}

static jintArray nh_wsi_onyxNewRectArray(
    JNIEnv *env, const nh_api_Rect *Rects_p, int count)
{
    jintArray array = (*env)->NewIntArray(env, count * 4);
    if (!array) {
        nh_wsi_onyxCheckException(env, "NewIntArray");
        return NULL;
    }
    jint *values_p = (*env)->GetIntArrayElements(env, array, NULL);
    for (int i = 0; i < count; ++i) {
        const nh_api_Rect *Rect_p = &Rects_p[i];
        values_p[i*4+0] = Rect_p->left < Rect_p->right ? Rect_p->left : Rect_p->right;
        values_p[i*4+1] = Rect_p->top < Rect_p->bottom ? Rect_p->top : Rect_p->bottom;
        values_p[i*4+2] = Rect_p->left < Rect_p->right ? Rect_p->right : Rect_p->left;
        values_p[i*4+3] = Rect_p->top < Rect_p->bottom ? Rect_p->bottom : Rect_p->top;
    }
    (*env)->ReleaseIntArrayElements(env, array, values_p, 0);
    return array;
}

NH_API_RESULT nh_wsi_setAndroidRawDrawingLimitRect(
    nh_wsi_AndroidOnyx *Onyx_p, int left, int top, int right, int bottom,
    const nh_api_Rect *excludeRects_p, int excludeCount)
{
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        return NH_API_ERROR_BAD_STATE;
    }
    JNIEnv *env = JNI.env;

    // Coordinates are relative to the host view; ViewUpdateHelper adds the view's screen offset.
    nh_api_Rect Limit = {left, top, right, bottom};
    jintArray limit = nh_wsi_onyxNewRectArray(env, &Limit, 1);
    if (limit) {
        (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->setRegionLimit, Onyx_p->hostView, limit);
        nh_wsi_onyxCheckException(env, "setScreenHandWritingRegionLimit");
        (*env)->DeleteLocalRef(env, limit);
    }

    if (Onyx_p->setRegionExclude && excludeRects_p && excludeCount > 0) {
        jintArray exclude = nh_wsi_onyxNewRectArray(env, excludeRects_p, excludeCount);
        if (exclude) {
            (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->setRegionExclude, Onyx_p->hostView, exclude);
            nh_wsi_onyxCheckException(env, "setScreenHandWritingRegionExclude");
            (*env)->DeleteLocalRef(env, exclude);
        }
    }

    nh_wsi_onyxDetach(&JNI);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidRawDrawingStrokeWidth(
    nh_wsi_AndroidOnyx *Onyx_p, float width)
{
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        return NH_API_ERROR_BAD_STATE;
    }
    Onyx_p->currentStrokeWidth = width;
    (*JNI.env)->CallStaticVoidMethod(JNI.env, Onyx_p->helperClass, Onyx_p->setStrokeWidth, (jfloat)width);
    nh_wsi_onyxCheckException(JNI.env, "setStrokeWidth");
    nh_wsi_onyxDetach(&JNI);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidRawDrawingStrokeStyle(
    nh_wsi_AndroidOnyx *Onyx_p, int style)
{
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        return NH_API_ERROR_BAD_STATE;
    }
    Onyx_p->currentStrokeStyle = style;
    nh_wsi_onyxCallInt(Onyx_p, JNI.env, Onyx_p->setStrokeStyle, style, "setStrokeStyle");
    nh_wsi_onyxDetach(&JNI);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidRawDrawingEraserEnabled(
    nh_wsi_AndroidOnyx *Onyx_p, bool enabled)
{
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        return NH_API_ERROR_BAD_STATE;
    }
    Onyx_p->eraserEnabled = enabled;
    nh_wsi_onyxSetEraser(Onyx_p, JNI.env, enabled);
    nh_wsi_onyxDetach(&JNI);
    return NH_API_SUCCESS;
}

void nh_wsi_closeAndroidOnyx(
    nh_wsi_AndroidOnyx *Onyx_p)
{
    if (!Onyx_p || !Onyx_p->initialized) {
        return;
    }
    nh_wsi_OnyxJNI JNI;
    if (nh_wsi_onyxAttach(&JNI)) {
        JNIEnv *env = JNI.env;
        if (Onyx_p->isOnyxDevice && Onyx_p->opened) {
            nh_wsi_onyxSetRender(Onyx_p, env, false);
            nh_wsi_onyxSetPenState(Onyx_p, env, NH_WSI_ONYX_PEN_STATE_STOP);
        }
        if (Onyx_p->hostView) {
            (*env)->DeleteGlobalRef(env, Onyx_p->hostView);
        }
        if (Onyx_p->helperClass) {
            (*env)->DeleteGlobalRef(env, Onyx_p->helperClass);
        }
        nh_wsi_onyxDetach(&JNI);
    }
    memset(Onyx_p, 0, sizeof(nh_wsi_AndroidOnyx));
}
