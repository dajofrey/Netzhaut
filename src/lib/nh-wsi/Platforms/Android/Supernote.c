#include "Supernote.h"
#include "Window.h"

#include <jni.h>
#include <dlfcn.h>
#include <pthread.h>
#include <stdbool.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define NH_WSI_SUPERNOTE_TAG "Netzhaut-Supernote"

// Same values Surface::unlockAndPost passes for software-rendered buffers: an empty rect
// covers the whole surface.
#define NH_WSI_SUPERNOTE_DATA_MODE 3
#define NH_WSI_SUPERNOTE_DISP_MODE 3
#define NH_WSI_SUPERNOTE_A2_GATE   0
#define NH_WSI_SUPERNOTE_RSVD0     4
#define NH_WSI_SUPERNOTE_RSVD1     0

typedef jobject (*nh_wsi_toSurface_f)(JNIEnv *env, ANativeWindow *window);

static pthread_mutex_t NH_WSI_SUPERNOTE_MUTEX = PTHREAD_MUTEX_INITIALIZER;
static bool NH_WSI_SUPERNOTE_INITIALIZED = false;
static bool NH_WSI_SUPERNOTE_AVAILABLE = false;
static nh_wsi_toSurface_f NH_WSI_SUPERNOTE_TO_SURFACE = NULL;
static jclass NH_WSI_SUPERNOTE_RECT_CLASS = NULL;
static jmethodID NH_WSI_SUPERNOTE_RECT_INIT = NULL;
static jmethodID NH_WSI_SUPERNOTE_ADD_DIRTY = NULL;
static jmethodID NH_WSI_SUPERNOTE_RELEASE = NULL;

static bool nh_wsi_supernoteClearException(
    JNIEnv *env)
{
    if (!(*env)->ExceptionCheck(env)) {
        return false;
    }
    (*env)->ExceptionClear(env);
    return true;
}

static void nh_wsi_initSupernote(
    JNIEnv *env)
{
    NH_WSI_SUPERNOTE_INITIALIZED = true;

    // Introduced in API 26, minSdk may be lower.
    NH_WSI_SUPERNOTE_TO_SURFACE = (nh_wsi_toSurface_f)dlsym(RTLD_DEFAULT, "ANativeWindow_toSurface");
    if (!NH_WSI_SUPERNOTE_TO_SURFACE) {
        __android_log_print(ANDROID_LOG_INFO, NH_WSI_SUPERNOTE_TAG, "ANativeWindow_toSurface unavailable");
        return;
    }

    jclass surfaceClass = (*env)->FindClass(env, "android/view/Surface");
    if (nh_wsi_supernoteClearException(env) || !surfaceClass) {
        return;
    }
    NH_WSI_SUPERNOTE_ADD_DIRTY = (*env)->GetMethodID(env, surfaceClass, "addDirtyRectFull",
        "(Landroid/graphics/Rect;IIIII)Z");
    if (nh_wsi_supernoteClearException(env) || !NH_WSI_SUPERNOTE_ADD_DIRTY) {
        // Not a Supernote.
        __android_log_print(ANDROID_LOG_INFO, NH_WSI_SUPERNOTE_TAG, "Surface.addDirtyRectFull unavailable");
        NH_WSI_SUPERNOTE_ADD_DIRTY = NULL;
        (*env)->DeleteLocalRef(env, surfaceClass);
        return;
    }
    NH_WSI_SUPERNOTE_RELEASE = (*env)->GetMethodID(env, surfaceClass, "release", "()V");
    (*env)->DeleteLocalRef(env, surfaceClass);
    if (nh_wsi_supernoteClearException(env) || !NH_WSI_SUPERNOTE_RELEASE) {
        return;
    }

    jclass rectClass = (*env)->FindClass(env, "android/graphics/Rect");
    if (nh_wsi_supernoteClearException(env) || !rectClass) {
        return;
    }
    NH_WSI_SUPERNOTE_RECT_INIT = (*env)->GetMethodID(env, rectClass, "<init>", "()V");
    if (nh_wsi_supernoteClearException(env) || !NH_WSI_SUPERNOTE_RECT_INIT) {
        (*env)->DeleteLocalRef(env, rectClass);
        return;
    }
    NH_WSI_SUPERNOTE_RECT_CLASS = (jclass)(*env)->NewGlobalRef(env, rectClass);
    (*env)->DeleteLocalRef(env, rectClass);

    NH_WSI_SUPERNOTE_AVAILABLE = NH_WSI_SUPERNOTE_RECT_CLASS != NULL;
    if (NH_WSI_SUPERNOTE_AVAILABLE) {
        __android_log_print(ANDROID_LOG_INFO, NH_WSI_SUPERNOTE_TAG,
            "Supernote e-ink surface damage available");
    }
}

void nh_wsi_markAndroidSupernoteFrameDirty(
    ANativeWindow *NativeWindow_p)
{
    if (!NativeWindow_p) {
        return;
    }
    pthread_mutex_lock(&NH_WSI_SUPERNOTE_MUTEX);
    if (NH_WSI_SUPERNOTE_INITIALIZED && !NH_WSI_SUPERNOTE_AVAILABLE) {
        pthread_mutex_unlock(&NH_WSI_SUPERNOTE_MUTEX);
        return;
    }

    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity || !app_p->activity->vm) {
        pthread_mutex_unlock(&NH_WSI_SUPERNOTE_MUTEX);
        return;
    }
    JavaVM *vm_p = app_p->activity->vm;
    JNIEnv *env = NULL;
    bool detach = false;
    jint status = (*vm_p)->GetEnv(vm_p, (void**)&env, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
        if ((*vm_p)->AttachCurrentThread(vm_p, &env, NULL) != JNI_OK) {
            pthread_mutex_unlock(&NH_WSI_SUPERNOTE_MUTEX);
            return;
        }
        detach = true;
    } else if (status != JNI_OK) {
        pthread_mutex_unlock(&NH_WSI_SUPERNOTE_MUTEX);
        return;
    }

    if (!NH_WSI_SUPERNOTE_INITIALIZED) {
        nh_wsi_initSupernote(env);
    }

    if (NH_WSI_SUPERNOTE_AVAILABLE) {
        // The Java Surface wraps the same native Surface EGL queues to.
        jobject surface = NH_WSI_SUPERNOTE_TO_SURFACE(env, NativeWindow_p);
        jobject rect = surface ? (*env)->NewObject(env, NH_WSI_SUPERNOTE_RECT_CLASS,
            NH_WSI_SUPERNOTE_RECT_INIT) : NULL;
        if (!nh_wsi_supernoteClearException(env) && surface && rect) {
            (*env)->CallBooleanMethod(env, surface, NH_WSI_SUPERNOTE_ADD_DIRTY, rect,
                (jint)NH_WSI_SUPERNOTE_DATA_MODE, (jint)NH_WSI_SUPERNOTE_DISP_MODE,
                (jint)NH_WSI_SUPERNOTE_A2_GATE, (jint)NH_WSI_SUPERNOTE_RSVD0,
                (jint)NH_WSI_SUPERNOTE_RSVD1);
            if (nh_wsi_supernoteClearException(env)) {
                __android_log_print(ANDROID_LOG_ERROR, NH_WSI_SUPERNOTE_TAG,
                    "Surface.addDirtyRectFull failed");
            }
        }
        if (surface) {
            // Drops the Java Surface's reference to the native window right away.
            (*env)->CallVoidMethod(env, surface, NH_WSI_SUPERNOTE_RELEASE);
            nh_wsi_supernoteClearException(env);
            (*env)->DeleteLocalRef(env, surface);
        }
        if (rect) {
            (*env)->DeleteLocalRef(env, rect);
        }
    }

    if (detach) {
        (*vm_p)->DetachCurrentThread(vm_p);
    }
    pthread_mutex_unlock(&NH_WSI_SUPERNOTE_MUTEX);
}
