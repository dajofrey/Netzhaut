#include "Supernote.h"
#include "Window.h"

#include <android/log.h>
#include <android_native_app_glue.h>
#include <dlfcn.h>
#include <jni.h>
#include <pthread.h>

#define TAG "Netzhaut-Supernote"

typedef jobject (*nh_wsi_toSurface_f)(JNIEnv *, ANativeWindow *);
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static bool initialized, available;
static nh_wsi_toSurface_f toSurface;
static jclass rectClass;
static jmethodID rectInit, addDirty, releaseSurface;

static bool exception(JNIEnv *env)
{
    if (!(*env)->ExceptionCheck(env)) return false;
    (*env)->ExceptionDescribe(env);
    (*env)->ExceptionClear(env);
    return true;
}

static bool initialize(JNIEnv *env)
{
    initialized = true;
    toSurface = (nh_wsi_toSurface_f)dlsym(RTLD_DEFAULT, "ANativeWindow_toSurface");
    if (!toSurface) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "ANativeWindow_toSurface unavailable");
        return true;
    }
    jclass surfaceClass = (*env)->FindClass(env, "android/view/Surface");
    if (exception(env) || !surfaceClass) return false;
    addDirty = (*env)->GetMethodID(env, surfaceClass, "addDirtyRectFull",
        "(Landroid/graphics/Rect;IIIII)Z");
    if ((*env)->ExceptionCheck(env) || !addDirty) {
        // A missing vendor method is an expected capability probe on other Android devices.
        (*env)->ExceptionClear(env);
        (*env)->DeleteLocalRef(env, surfaceClass);
        __android_log_print(ANDROID_LOG_INFO, TAG, "Surface.addDirtyRectFull unavailable");
        return true;
    }
    releaseSurface = (*env)->GetMethodID(env, surfaceClass, "release", "()V");
    (*env)->DeleteLocalRef(env, surfaceClass);
    if (exception(env) || !releaseSurface) return false;
    jclass local = (*env)->FindClass(env, "android/graphics/Rect");
    if (exception(env) || !local) return false;
    rectInit = (*env)->GetMethodID(env, local, "<init>", "()V");
    if (!exception(env) && rectInit) rectClass = (*env)->NewGlobalRef(env, local);
    (*env)->DeleteLocalRef(env, local);
    if (exception(env) || !rectClass) return false;
    available = true;
    __android_log_print(ANDROID_LOG_INFO, TAG, "Supernote e-ink surface damage available");
    return true;
}

NH_API_RESULT nh_wsi_markAndroidSupernoteFrameDirty(ANativeWindow *window)
{
    if (!window) return NH_API_ERROR_BAD_STATE;
    pthread_mutex_lock(&lock);
    if (initialized && !available) {
        pthread_mutex_unlock(&lock);
        return NH_API_SUCCESS;
    }
    struct android_app *app = nh_wsi_getAndroidApp();
    JavaVM *vm = app && app->activity ? app->activity->vm : NULL;
    JNIEnv *env = NULL;
    bool detach = false;
    NH_API_RESULT result = NH_API_ERROR_BAD_STATE;
    if (!vm) goto done;
    jint status = (*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
        if ((*vm)->AttachCurrentThread(vm, &env, NULL) != JNI_OK) goto done;
        detach = true;
    } else if (status != JNI_OK) goto done;
    if ((*env)->PushLocalFrame(env, 8) != JNI_OK) {
        exception(env);
        goto done;
    }
    if (!initialized && !initialize(env)) {
        // Initialization errors must not become a successful "unsupported" result next frame.
        initialized = false;
        goto pop;
    }
    if (!available) {
        result = NH_API_SUCCESS;
        goto pop;
    }
    jobject surface = toSurface(env, window);
    bool failed = exception(env);
    jobject rect = !failed && surface ? (*env)->NewObject(env, rectClass, rectInit) : NULL;
    failed |= exception(env);
    if (!failed && surface && rect) {
        // Same full-surface metadata as Supernote's Surface::unlockAndPost (MonoReader's fix).
        jboolean accepted = (*env)->CallBooleanMethod(env, surface, addDirty, rect, 3, 3, 0, 4, 0);
        failed = exception(env);
        if (accepted && !failed) result = NH_API_SUCCESS;
    }
    if (surface) {
        (*env)->CallVoidMethod(env, surface, releaseSurface);
        if (exception(env)) result = NH_API_ERROR_BAD_STATE;
    }
pop:
    (*env)->PopLocalFrame(env, NULL);
done:
    if (result != NH_API_SUCCESS)
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Could not attach e-ink damage to the EGL frame");
    if (detach) (*vm)->DetachCurrentThread(vm);
    pthread_mutex_unlock(&lock);
    return result;
}
