#ifndef NH_WSI_PLATFORMS_ANDROID_SUPERNOTE_H
#define NH_WSI_PLATFORMS_ANDROID_SUPERNOTE_H

#include "../../Common/Includes.h"
#include <android/native_window.h>

// EGL omits the e-ink damage metadata supplied by Supernote's software Surface posting path.
NH_API_RESULT nh_wsi_markAndroidSupernoteFrameDirty(ANativeWindow *window);

#endif
