#ifndef NH_WSI_PLATFORMS_ANDROID_SUPERNOTE_H
#define NH_WSI_PLATFORMS_ANDROID_SUPERNOTE_H

#include "../../Common/Includes.h"
#include <android/native_window.h>

// Supernote (Ratta/htfyun) e-paper firmware.
// The HWC only refreshes the panel for buffers queued with an e-ink dirty region, which the
// firmware attaches in Surface.unlockCanvasAndPost and the patched ViewRootImpl, but not for
// buffers queued by EGL. Without it, GL frames reach SurfaceFlinger but not the panel.

/**
 * Marks the next buffer queued to NativeWindow_p as dirty for the whole surface.
 * Must be called right before eglSwapBuffers. Does nothing on other devices.
 * Returns an error if attaching the metadata fails on a supported device.
 */
NH_API_RESULT nh_wsi_markAndroidSupernoteFrameDirty(
    ANativeWindow *NativeWindow_p
);

#endif
