#include "Onyx.h"
#include "Window.h"

#include <jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/system_properties.h>

static pthread_mutex_t NH_WSI_ONYX_POST_MUTEX = PTHREAD_MUTEX_INITIALIZER;

bool nh_wsi_isAndroidOnyxTraceEnabled(void)
{
    char value[PROP_VALUE_MAX];
    return __system_property_get("debug.monodraw.handoff", value) > 0 && !strcmp(value, "1");
}

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
    Onyx_p->applyTransientUpdate = nh_wsi_onyxGetStaticMethod(env, helperClass, "applyTransientUpdate", "(I)V", false);
    Onyx_p->clearTransientUpdate = nh_wsi_onyxGetStaticMethod(env, helperClass, "clearTransientUpdate", "(Z)V", false);
    Onyx_p->refreshScreen        = nh_wsi_onyxGetStaticMethod(env, helperClass, "refreshScreen", "(IIIII)V", false);

    jclass viewClass = (*env)->FindClass(env, "android/view/View");
    if (!nh_wsi_onyxCheckException(env, "FindClass android.view.View") && viewClass) {
        Onyx_p->getLocationOnScreen = (*env)->GetMethodID(env, viewClass, "getLocationOnScreen", "([I)V");
        if (nh_wsi_onyxCheckException(env, "View.getLocationOnScreen")) {
            Onyx_p->getLocationOnScreen = NULL;
        }
        (*env)->DeleteLocalRef(env, viewClass);
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
    char value[PROP_VALUE_MAX];
    if (__system_property_get("debug.monodraw.software", value) > 0 && !strcmp(value, "1"))
        return false;
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

static bool nh_wsi_onyxSetEraser(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env, bool enabled)
{
    // Eraser raw drawing configures eraser input, not the normal pencil tip.
    if (!Onyx_p->setStrokeColor && enabled) {
        NH_WSI_ONYX_ERR("setStrokeColor not available for pencil-tip erasing");
        return false;
    }
    if (Onyx_p->setStrokeColor) {
        (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->setStrokeColor,
            enabled ? (jint)0xFFFFFFFF : (jint)0xFF000000);
        if (nh_wsi_onyxCheckException(env, "setStrokeColor")) return false;
    }
    if (!Onyx_p->setEraserRawDrawingEnabled) {
        return true;
    }
    if (Onyx_p->eraserHasStyle) {
        (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->setEraserRawDrawingEnabled,
            enabled ? JNI_TRUE : JNI_FALSE, (jint)5);
    } else {
        (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->setEraserRawDrawingEnabled,
            enabled ? JNI_TRUE : JNI_FALSE);
    }
    return !nh_wsi_onyxCheckException(env, "setEraserRawDrawingEnabled");
}

static void nh_wsi_onyxApplyStroke(
    nh_wsi_AndroidOnyx *Onyx_p, JNIEnv *env)
{
    (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->setStrokeWidth, (jfloat)Onyx_p->currentStrokeWidth);
    nh_wsi_onyxCheckException(env, "setStrokeWidth");
    nh_wsi_onyxCallInt(Onyx_p, env, Onyx_p->setStrokeStyle, Onyx_p->currentStrokeStyle, "setStrokeStyle");
    nh_wsi_onyxCallInt(Onyx_p, env, Onyx_p->setStrokeColor,
        Onyx_p->eraserEnabled ? (jint)0xFFFFFFFF : (jint)0xFF000000, "setStrokeColor");
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
        __atomic_store_n(&Onyx_p->disablePostAt, 0, __ATOMIC_SEQ_CST);
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
    nh_api_Rect *excludes_p = NULL;
    if (excludeRects_p && excludeCount > 0) {
        excludes_p = malloc(sizeof(*excludes_p) * (size_t)excludeCount);
        if (!excludes_p) {
            NH_WSI_ONYX_ERR("Could not allocate raw drawing exclusion rectangles");
            return NH_API_ERROR_BAD_STATE;
        }
        memcpy(excludes_p, excludeRects_p, sizeof(*excludes_p) * (size_t)excludeCount);
    }
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        free(excludes_p);
        return NH_API_ERROR_BAD_STATE;
    }
    JNIEnv *env = JNI.env;

    // SurfaceFlinger rejects region changes while the pen is in drawing state, so pause around the update.
    bool resume = Onyx_p->opened && Onyx_p->renderEnabled;
    if (resume) {
        nh_wsi_onyxSetPenState(Onyx_p, env, NH_WSI_ONYX_PEN_STATE_PAUSE);
    }

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

    if (resume) {
        nh_wsi_onyxSetPenState(Onyx_p, env, NH_WSI_ONYX_PEN_STATE_DRAWING);
    }

    pthread_mutex_lock(&NH_WSI_ONYX_POST_MUTEX);
    Onyx_p->Limit = Limit;
    Onyx_p->hasLimit = true;
    free(Onyx_p->excludeRects_p);
    Onyx_p->excludeRects_p = excludes_p;
    Onyx_p->excludeCount = excludes_p ? excludeCount : 0;
    pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
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
    if (!(width > 0.0f)) {
        nh_wsi_onyxDetach(&JNI);
        return NH_API_ERROR_BAD_STATE;
    }
    (*JNI.env)->CallStaticVoidMethod(JNI.env, Onyx_p->helperClass, Onyx_p->setStrokeWidth, (jfloat)width);
    bool success = !nh_wsi_onyxCheckException(JNI.env, "setStrokeWidth");
    if (success) Onyx_p->currentStrokeWidth = width;
    nh_wsi_onyxDetach(&JNI);
    return success ? NH_API_SUCCESS : NH_API_ERROR_BAD_STATE;
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
    bool success = nh_wsi_onyxSetEraser(Onyx_p, JNI.env, enabled);
    if (success) Onyx_p->eraserEnabled = enabled;
    nh_wsi_onyxDetach(&JNI);
    return success ? NH_API_SUCCESS : NH_API_ERROR_BAD_STATE;
}

void nh_wsi_enableAndroidOnyxPost(
    nh_wsi_AndroidOnyx *Onyx_p)
{
    pthread_mutex_lock(&NH_WSI_ONYX_POST_MUTEX);
    if (Onyx_p) {__atomic_store_n(&Onyx_p->disablePostAt, 0, __ATOMIC_SEQ_CST);}
    if (!Onyx_p || !Onyx_p->opened || !Onyx_p->renderEnabled || !Onyx_p->enablePost ||
        (__atomic_load_n(&Onyx_p->Frame.generation, __ATOMIC_SEQ_CST) & 1)) {
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return;
    }
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return;
    }
    (*JNI.env)->CallStaticVoidMethod(JNI.env, Onyx_p->helperClass, Onyx_p->enablePost, (jint)1);
    if (!nh_wsi_onyxCheckException(JNI.env, "enablePost")) {
        __atomic_store_n(&Onyx_p->Frame.posted,
            __atomic_load_n(&Onyx_p->Frame.generation, __ATOMIC_SEQ_CST), __ATOMIC_SEQ_CST);
    }
    nh_wsi_onyxDetach(&JNI);
    pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
}

static bool nh_wsi_onyxContains(nh_api_Rect rect, float x, float y)
{
    int left = rect.left < rect.right ? rect.left : rect.right;
    int right = rect.left < rect.right ? rect.right : rect.left;
    int top = rect.top < rect.bottom ? rect.top : rect.bottom;
    int bottom = rect.top < rect.bottom ? rect.bottom : rect.top;
    return x >= left && x < right && y >= top && y < bottom;
}

void nh_wsi_beginAndroidOnyxStroke(
    nh_wsi_AndroidOnyx *Onyx_p, float x, float y)
{
    pthread_mutex_lock(&NH_WSI_ONYX_POST_MUTEX);
    bool hardware = Onyx_p->opened && Onyx_p->renderEnabled &&
        (!Onyx_p->hasLimit || nh_wsi_onyxContains(Onyx_p->Limit, x, y));
    for (int i = 0; hardware && i < Onyx_p->excludeCount; ++i)
        if (nh_wsi_onyxContains(Onyx_p->excludeRects_p[i], x, y)) hardware = false;
    if (!hardware) {
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return;
    }
    __atomic_store_n(&Onyx_p->disablePostAt, 0, __ATOMIC_SEQ_CST);
    nh_wsi_beginOnyxStroke(&Onyx_p->Frame);
    if (nh_wsi_isAndroidOnyxTraceEnabled())
        NH_WSI_ONYX_LOG("handoff pen-down generation=%llu at %.1f,%.1f",
            (unsigned long long)__atomic_load_n(&Onyx_p->Frame.generation, __ATOMIC_SEQ_CST), x, y);
    if (Onyx_p->opened && Onyx_p->renderEnabled) {
        nh_wsi_OnyxJNI JNI;
        if (Onyx_p->enablePost && nh_wsi_onyxBegin(Onyx_p, &JNI)) {
            (*JNI.env)->CallStaticVoidMethod(JNI.env, Onyx_p->helperClass, Onyx_p->enablePost, (jint)0);
            nh_wsi_onyxCheckException(JNI.env, "enablePost pen down");
            nh_wsi_onyxDetach(&JNI);
        } else {
            NH_WSI_ONYX_ERR("Cannot block app posting on pen down");
        }
    }
    pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
}

static int64_t nh_wsi_onyxNow()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void nh_wsi_scheduleAndroidOnyxPostDisable(
    nh_wsi_AndroidOnyx *Onyx_p, int delayMs)
{
    if (!Onyx_p || !Onyx_p->opened || !Onyx_p->renderEnabled || !Onyx_p->enablePost) {
        return;
    }
    __atomic_store_n(&Onyx_p->disablePostAt,
        nh_wsi_onyxNow() + (delayMs > 0 ? delayMs : 1), __ATOMIC_SEQ_CST);
}

NH_API_RESULT nh_wsi_postAndroidOnyxFrame(
    nh_wsi_AndroidOnyx *Onyx_p, uint64_t generation,
    NH_API_RESULT (*post_f)(void *), void *user_p)
{
    pthread_mutex_lock(&NH_WSI_ONYX_POST_MUTEX);
    bool pending = __atomic_load_n(&Onyx_p->Frame.generation, __ATOMIC_SEQ_CST) !=
        __atomic_load_n(&Onyx_p->Frame.posted, __ATOMIC_SEQ_CST);
    if (Onyx_p->opened && Onyx_p->renderEnabled && pending &&
        !nh_wsi_canPostOnyxFrame(&Onyx_p->Frame, generation)) {
        if (nh_wsi_isAndroidOnyxTraceEnabled())
            NH_WSI_ONYX_LOG("handoff skip buffer=%llu current=%llu",
                (unsigned long long)generation,
                (unsigned long long)__atomic_load_n(&Onyx_p->Frame.generation, __ATOMIC_SEQ_CST));
        // Do not queue partial or stale buffers for a later enablePost to expose.
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return NH_API_SUCCESS;
    }
    if (!Onyx_p->opened || !Onyx_p->renderEnabled ||
        !nh_wsi_canPostOnyxFrame(&Onyx_p->Frame, generation)) {
        NH_API_RESULT result = post_f(user_p);
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return result;
    }
    if (!Onyx_p->enablePost) {
        NH_WSI_ONYX_ERR("Cannot post finalized canvas: enablePost unavailable");
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return NH_API_ERROR_BAD_STATE;
    }
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return NH_API_ERROR_BAD_STATE;
    }
    if (nh_wsi_isAndroidOnyxTraceEnabled())
        NH_WSI_ONYX_LOG("handoff enable-post generation=%llu", (unsigned long long)generation);
    (*JNI.env)->CallStaticVoidMethod(JNI.env, Onyx_p->helperClass, Onyx_p->enablePost, (jint)1);
    bool failed = nh_wsi_onyxCheckException(JNI.env, "enablePost finalized canvas");
    char pause[PROP_VALUE_MAX];
    if (!failed && __system_property_get("debug.monodraw.handoff.pause", pause) > 0 &&
        !strcmp(pause, "1")) {
        NH_WSI_ONYX_LOG("handoff diagnostic pause START generation=%llu (800 ms before swap)",
            (unsigned long long)generation);
        struct timespec remaining = {0, 800000000};
        int sleepResult;
        do {
            sleepResult = nanosleep(&remaining, &remaining);
        } while (sleepResult != 0 && errno == EINTR);
        if (sleepResult != 0)
            NH_WSI_ONYX_ERR("handoff diagnostic pause failed: %s", strerror(errno));
        NH_WSI_ONYX_LOG("handoff diagnostic pause END generation=%llu; submitting canvas now",
            (unsigned long long)generation);
    }
    NH_API_RESULT result = failed ? NH_API_ERROR_BAD_STATE : post_f(user_p);
    if (result == NH_API_SUCCESS) {
        if (nh_wsi_isAndroidOnyxTraceEnabled())
            NH_WSI_ONYX_LOG("handoff swap-complete generation=%llu", (unsigned long long)generation);
        __atomic_store_n(&Onyx_p->Frame.posted, generation, __ATOMIC_SEQ_CST);
        nh_wsi_scheduleAndroidOnyxPostDisable(Onyx_p, 600);
    } else {
        nh_wsi_onyxCallInt(Onyx_p, JNI.env, Onyx_p->enablePost, 0, "enablePost failed handoff");
    }
    nh_wsi_onyxDetach(&JNI);
    pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
    return result;
}

void nh_wsi_updateAndroidOnyx(
    nh_wsi_AndroidOnyx *Onyx_p)
{
    if (!Onyx_p) return;
    int64_t deadline = __atomic_load_n(&Onyx_p->disablePostAt, __ATOMIC_SEQ_CST);
    if (!deadline || nh_wsi_onyxNow() < deadline) {
        return;
    }
    pthread_mutex_lock(&NH_WSI_ONYX_POST_MUTEX);
    if (!__atomic_compare_exchange_n(&Onyx_p->disablePostAt, &deadline, 0,
            false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return;
    }
    if (!Onyx_p->opened || !Onyx_p->renderEnabled || !Onyx_p->enablePost) {
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return;
    }
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
        return;
    }
    nh_wsi_onyxCallInt(Onyx_p, JNI.env, Onyx_p->enablePost, 0, "enablePost");
    nh_wsi_onyxDetach(&JNI);
    pthread_mutex_unlock(&NH_WSI_ONYX_POST_MUTEX);
}

// Values of android.onyx.ViewUpdateHelper.UI_*_MODE.
static int nh_wsi_onyxGetWaveform(
    int mode)
{
    switch (mode)
    {
        case NH_API_EPAPER_UPDATE_MODE_A2 : return 4;  // UI_A2_PERFORMANCE_MODE
        case NH_API_EPAPER_UPDATE_MODE_DU : return 1;  // UI_DU_MODE
        case NH_API_EPAPER_UPDATE_MODE_GU : return 2;  // UI_GU_MODE
        case NH_API_EPAPER_UPDATE_MODE_GC : return 98; // UI_GC_MODE
        default                           : return 5;  // UI_DEFAULT_MODE
    }
}

static bool nh_wsi_onyxIsValidMode(
    int mode)
{
    return mode >= NH_API_EPAPER_UPDATE_MODE_DEFAULT && mode <= NH_API_EPAPER_UPDATE_MODE_GC;
}

NH_API_RESULT nh_wsi_setAndroidEPaperUpdateMode(
    nh_wsi_AndroidOnyx *Onyx_p, int mode)
{
    if (!nh_wsi_onyxIsValidMode(mode)) {
        return NH_API_ERROR_BAD_STATE;
    }
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        return NH_API_ERROR_BAD_STATE;
    }

    NH_API_RESULT result = NH_API_SUCCESS;
    if (!Onyx_p->applyTransientUpdate || !Onyx_p->clearTransientUpdate) {
        result = NH_API_ERROR_BAD_STATE;
    } else if (mode != Onyx_p->updateMode) {
        if (mode == NH_API_EPAPER_UPDATE_MODE_DEFAULT) {
            (*JNI.env)->CallStaticVoidMethod(JNI.env, Onyx_p->helperClass, Onyx_p->clearTransientUpdate, JNI_FALSE);
            nh_wsi_onyxCheckException(JNI.env, "clearTransientUpdate");
        } else {
            nh_wsi_onyxCallInt(Onyx_p, JNI.env, Onyx_p->applyTransientUpdate, nh_wsi_onyxGetWaveform(mode), "applyTransientUpdate");
        }
        Onyx_p->updateMode = mode;
    }

    nh_wsi_onyxDetach(&JNI);
    return result;
}

NH_API_RESULT nh_wsi_requestAndroidEPaperRefreshRect(
    nh_wsi_AndroidOnyx *Onyx_p, int left, int top, int right, int bottom, int mode)
{
    if (!nh_wsi_onyxIsValidMode(mode) || right <= left || bottom <= top) {
        return NH_API_ERROR_BAD_STATE;
    }
    nh_wsi_OnyxJNI JNI;
    if (!nh_wsi_onyxBegin(Onyx_p, &JNI)) {
        return NH_API_ERROR_BAD_STATE;
    }
    JNIEnv *env = JNI.env;

    NH_API_RESULT result = NH_API_SUCCESS;
    if (!Onyx_p->refreshScreen) {
        result = NH_API_ERROR_BAD_STATE;
    } else {
        // refreshScreen expects screen coordinates, the rect is relative to the host view.
        int offsetX = 0, offsetY = 0;
        if (Onyx_p->hostView && Onyx_p->getLocationOnScreen) {
            jintArray location = (*env)->NewIntArray(env, 2);
            if (location) {
                (*env)->CallVoidMethod(env, Onyx_p->hostView, Onyx_p->getLocationOnScreen, location);
                if (!nh_wsi_onyxCheckException(env, "View.getLocationOnScreen")) {
                    jint values[2] = {0};
                    (*env)->GetIntArrayRegion(env, location, 0, 2, values);
                    offsetX = values[0];
                    offsetY = values[1];
                }
                (*env)->DeleteLocalRef(env, location);
            } else {
                nh_wsi_onyxCheckException(env, "NewIntArray");
            }
        }
        (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->refreshScreen,
            (jint)(left + offsetX), (jint)(top + offsetY), (jint)(right - left), (jint)(bottom - top),
            (jint)nh_wsi_onyxGetWaveform(mode));
        nh_wsi_onyxCheckException(env, "refreshScreen");
    }

    nh_wsi_onyxDetach(&JNI);
    return result;
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
        if (Onyx_p->isOnyxDevice && Onyx_p->updateMode != NH_API_EPAPER_UPDATE_MODE_DEFAULT && Onyx_p->clearTransientUpdate) {
            (*env)->CallStaticVoidMethod(env, Onyx_p->helperClass, Onyx_p->clearTransientUpdate, JNI_FALSE);
            nh_wsi_onyxCheckException(env, "clearTransientUpdate");
        }
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
    free(Onyx_p->excludeRects_p);
    memset(Onyx_p, 0, sizeof(nh_wsi_AndroidOnyx));
}
