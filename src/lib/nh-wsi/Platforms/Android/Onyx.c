#include "Onyx.h"
#include "Window.h"

#include "../../Window/Event.h"

#include <jni.h>
#include <android/rect.h>
#include <android/log.h>
#include <android_native_app_glue.h>

NH_API_RESULT nh_wsi_setAndroidRawDrawingEnabled(
    nh_wsi_AndroidOnyx *Onyx_p, bool enabled)
{
    if (!Onyx_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity) {
        return NH_API_ERROR_BAD_STATE;
    }

    JNIEnv *env = NULL;
    bool detach = false;
    if ((*app_p->activity->vm)->GetEnv(app_p->activity->vm, (void**)&env, JNI_VERSION_1_6) == JNI_EDETACHED) {
        if ((*app_p->activity->vm)->AttachCurrentThread(app_p->activity->vm, &env, NULL) != JNI_OK) {
            return NH_API_ERROR_BAD_STATE;
        }
        detach = true;
    }

    jobject touchHelper = Onyx_p->touchHelperGlobalRef;
    if (touchHelper) {
        jclass touchHelperClass = (*env)->GetObjectClass(env, touchHelper);
        jmethodID setRawDrawingEnabled = (*env)->GetMethodID(env, touchHelperClass, "setRawDrawingEnabled", "(Z)V");
        if (setRawDrawingEnabled) {
            (*env)->CallVoidMethod(env, touchHelper, setRawDrawingEnabled, enabled ? JNI_TRUE : JNI_FALSE);
        }
    }

    if (detach) {
        (*app_p->activity->vm)->DetachCurrentThread(app_p->activity->vm);
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidRawDrawingRenderEnabled(
    nh_wsi_AndroidOnyx *Onyx_p, bool enabled)
{
    if (!Onyx_p || !Onyx_p->touchHelperGlobalRef) {
        return NH_API_ERROR_BAD_STATE;
    }

    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity) {
        return NH_API_ERROR_BAD_STATE;
    }

    JNIEnv *env = NULL;
    bool detach = false;
    jint status = (*app_p->activity->vm)->GetEnv(app_p->activity->vm, (void**)&env, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
        if ((*app_p->activity->vm)->AttachCurrentThread(app_p->activity->vm, &env, NULL) != JNI_OK) {
            return NH_API_ERROR_BAD_STATE;
        }
        detach = true;
    }

    jobject touchHelper = Onyx_p->touchHelperGlobalRef;
    if (touchHelper) {
        jclass touchHelperClass = (*env)->GetObjectClass(env, touchHelper);
        
        // Onyx TouchHelper.setRawDrawingRenderEnabled typically returns TouchHelper (fluent API)
        jmethodID setRenderEnabled = (*env)->GetMethodID(
            env, touchHelperClass, "setRawDrawingRenderEnabled", "(Z)Lcom/onyx/android/sdk/pen/TouchHelper;"
        );
        
        if (setRenderEnabled) {
            (*env)->CallObjectMethod(env, touchHelper, setRenderEnabled, enabled ? JNI_TRUE : JNI_FALSE);
        } else {
            // Fallback signature in case a different SDK version uses return type void
            jmethodID setRenderEnabledVoid = (*env)->GetMethodID(
                env, touchHelperClass, "setRawDrawingRenderEnabled", "(Z)V"
            );
            if (setRenderEnabledVoid) {
                (*env)->CallVoidMethod(env, touchHelper, setRenderEnabledVoid, enabled ? JNI_TRUE : JNI_FALSE);
            }
        }

        if (touchHelperClass) {
            (*env)->DeleteLocalRef(env, touchHelperClass);
        }
    }

    if (detach) {
        (*app_p->activity->vm)->DetachCurrentThread(app_p->activity->vm);
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidRawDrawingLimitRect(
    nh_wsi_AndroidOnyx *Onyx_p, int left, int top, int right, int bottom,
    const nh_api_Rect *excludeRects_p, int excludeCount)
{
    if (!Onyx_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity) {
        return NH_API_ERROR_BAD_STATE;
    }

    JNIEnv *env = NULL;
    bool detach = false;
    if ((*app_p->activity->vm)->GetEnv(app_p->activity->vm, (void**)&env, JNI_VERSION_1_6) == JNI_EDETACHED) {
        if ((*app_p->activity->vm)->AttachCurrentThread(app_p->activity->vm, &env, NULL) != JNI_OK) {
            return NH_API_ERROR_BAD_STATE;
        }
        detach = true;
    }

    jobject touchHelper = Onyx_p->touchHelperGlobalRef;
    if (touchHelper) {
        jclass touchHelperClass = (*env)->GetObjectClass(env, touchHelper);
        jclass rectClass = (*env)->FindClass(env, "android/graphics/Rect");
        jmethodID rectInit = (*env)->GetMethodID(env, rectClass, "<init>", "(IIII)V");
        
        // 1. Create main canvas limit Rect
        jobject limitRect = (*env)->NewObject(env, rectClass, rectInit, left, top, right, bottom);

        // 2. Build Java ArrayList for UI exclusion zones (toolbars, buttons)
        jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
        jmethodID arrayListInit = (*env)->GetMethodID(env, arrayListClass, "<init>", "(I)V");
        jmethodID arrayListAdd = (*env)->GetMethodID(env, arrayListClass, "add", "(Ljava/lang/Object;)Z");
        jobject excludeList = (*env)->NewObject(env, arrayListClass, arrayListInit, excludeCount);

        for (int i = 0; i < excludeCount; ++i) {
            jobject exRect = (*env)->NewObject(env, rectClass, rectInit, 
                excludeRects_p[i].left, excludeRects_p[i].top, 
                excludeRects_p[i].right, excludeRects_p[i].bottom);
            (*env)->CallBooleanMethod(env, excludeList, arrayListAdd, exRect);
            (*env)->DeleteLocalRef(env, exRect);
        }

        // 3. Call TouchHelper.setLimitRect(Rect, List<Rect>)
        jmethodID setLimitRect = (*env)->GetMethodID(env, touchHelperClass, "setLimitRect", 
            "(Landroid/graphics/Rect;Ljava/util/List;)Lcom/onyx/android/sdk/pen/TouchHelper;");
        if (setLimitRect) {
            (*env)->CallObjectMethod(env, touchHelper, setLimitRect, limitRect, excludeList);
        }

        (*env)->DeleteLocalRef(env, limitRect);
        (*env)->DeleteLocalRef(env, excludeList);
    }

    if (detach) {
        (*app_p->activity->vm)->DetachCurrentThread(app_p->activity->vm);
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidRawDrawingStrokeWidth(
    nh_wsi_AndroidOnyx *Onyx_p, float width)
{
    if (!Onyx_p || !Onyx_p->isOnyxDevice || !Onyx_p->touchHelperGlobalRef) {
        return NH_API_ERROR_BAD_STATE;
    }

    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity) {
        return NH_API_ERROR_BAD_STATE;
    }

    JNIEnv *env = NULL;
    bool detach = false;
    jint status = (*app_p->activity->vm)->GetEnv(app_p->activity->vm, (void**)&env, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
        if ((*app_p->activity->vm)->AttachCurrentThread(app_p->activity->vm, &env, NULL) != JNI_OK) {
            return NH_API_ERROR_BAD_STATE;
        }
        detach = true;
    }

    jclass touchHelperClass = (*env)->GetObjectClass(env, Onyx_p->touchHelperGlobalRef);
    jmethodID setStrokeWidth = (*env)->GetMethodID(env, touchHelperClass, "setStrokeWidth", "(F)Lcom/onyx/android/sdk/pen/TouchHelper;");
    
    if (setStrokeWidth) {
        (*env)->CallObjectMethod(env, Onyx_p->touchHelperGlobalRef, setStrokeWidth, width);
        Onyx_p->currentStrokeWidth = width;
    }

    (*env)->DeleteLocalRef(env, touchHelperClass);
 
    if (detach) {
        (*app_p->activity->vm)->DetachCurrentThread(app_p->activity->vm);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidRawDrawingStrokeStyle(
    nh_wsi_AndroidOnyx *Onyx_p, int style)
{
    if (!Onyx_p || !Onyx_p->isOnyxDevice || !Onyx_p->touchHelperGlobalRef) {
        return NH_API_ERROR_BAD_STATE;
    }

    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity) {
        return NH_API_ERROR_BAD_STATE;
    }

    JNIEnv *env = NULL;
    bool detach = false;
    jint status = (*app_p->activity->vm)->GetEnv(app_p->activity->vm, (void**)&env, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
        if ((*app_p->activity->vm)->AttachCurrentThread(app_p->activity->vm, &env, NULL) != JNI_OK) {
            return NH_API_ERROR_BAD_STATE;
        }
        detach = true;
    }

    jclass touchHelperClass = (*env)->GetObjectClass(env, Onyx_p->touchHelperGlobalRef);
    jmethodID setStrokeStyle = (*env)->GetMethodID(env, touchHelperClass, "setStrokeStyle", "(I)Lcom/onyx/android/sdk/pen/TouchHelper;");
    
    if (setStrokeStyle) {
        (*env)->CallObjectMethod(env, Onyx_p->touchHelperGlobalRef, setStrokeStyle, (jint)style);
        Onyx_p->currentStrokeStyle = style;
    }

    (*env)->DeleteLocalRef(env, touchHelperClass);
    
    if (detach) {
        (*app_p->activity->vm)->DetachCurrentThread(app_p->activity->vm);
    }

    return NH_API_SUCCESS;
}

static void nh_wsi_dispatchOnyxStylusEvent(
    JNIEnv *env, jobject thiz, jlong window_ptr, NH_API_TRIGGER_E trigger, jfloat x, jfloat y, jfloat pressure) 
{
    nh_api_Window *Window_p = (nh_api_Window *)window_ptr;
    if (!Window_p) {
        return;
    }
    nh_wsi_sendStylusEvent(Window_p, trigger, x, y, pressure);
}

JNIEXPORT void JNICALL
Java_com_netzhaut_wsi_OnyxTouchBridge_nativeOnBeginRawDrawing(
    JNIEnv *env, jobject thiz, jlong window_ptr, jfloat x, jfloat y, jfloat pressure) 
{
    nh_wsi_dispatchOnyxStylusEvent(env, thiz, window_ptr, NH_API_TRIGGER_PRESS, x, y, pressure);
}

JNIEXPORT void JNICALL
Java_com_netzhaut_wsi_OnyxTouchBridge_nativeOnRawDrawingPointMove(
    JNIEnv *env, jobject thiz, jlong window_ptr, jfloat x, jfloat y, jfloat pressure) 
{
    nh_wsi_dispatchOnyxStylusEvent(env, thiz, window_ptr, NH_API_TRIGGER_MOVE, x, y, pressure);
}

JNIEXPORT void JNICALL
Java_com_netzhaut_wsi_OnyxTouchBridge_nativeOnEndRawDrawing(
    JNIEnv *env, jobject thiz, jlong window_ptr, jfloat x, jfloat y, jfloat pressure) 
{
    nh_wsi_dispatchOnyxStylusEvent(env, thiz, window_ptr, NH_API_TRIGGER_RELEASE, x, y, pressure);
}
