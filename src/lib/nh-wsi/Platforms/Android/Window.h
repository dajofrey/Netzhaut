#ifndef NH_WSI_PLATFORMS_ANDROID_WINDOW_H
#define NH_WSI_PLATFORMS_ANDROID_WINDOW_H

#include "../../Common/Config.h"
#include "../../Common/Includes.h"
#include "../../../nh-gfx/Base/SurfaceRequirements.h"

#include <android/native_window.h>
#include <stdint.h>

typedef struct nh_wsi_Window nh_wsi_Window;
struct android_app;

typedef struct nh_wsi_AndroidWindow {
    ANativeWindow *Handle;
    uint32_t generation;
    int32_t touchPointerId;
    float touchScrollX;
    float touchScrollY;
    float touchLastX;
    float touchLastY;
    bool touchActive;
    bool touchScrolling;
} nh_wsi_AndroidWindow;

NH_API_RESULT nh_wsi_createAndroidWindow(
    nh_wsi_Window *Window_p, nh_wsi_WindowConfig Config,
    nh_gfx_SurfaceRequirements *Requirements_p
);

NH_API_RESULT nh_wsi_destroyAndroidWindow(
    nh_wsi_AndroidWindow *Window_p
);

NH_API_RESULT nh_wsi_moveAndroidWindow(
    nh_wsi_AndroidWindow *Window_p
);

NH_API_RESULT nh_wsi_getAndroidWindowSize(
    nh_wsi_Window *Window_p, int *width_p, int *height_p
);

NH_API_RESULT nh_wsi_showAndroidKeyboard(
    nh_wsi_AndroidWindow *Window_p
);

NH_API_RESULT nh_wsi_hideAndroidKeyboard(
    nh_wsi_AndroidWindow *Window_p
);

NH_API_RESULT nh_wsi_getAndroidInput(
    nh_wsi_Window *Window_p, bool *idle_p
);

NH_API_RESULT nh_wsi_setAndroidWindowDecorated(
    nh_wsi_AndroidWindow *Window_p, bool decorated
);

NH_API_RESULT nh_wsi_setAndroidWindowState(
    nh_wsi_AndroidWindow *Window_p, bool *state_p
);

NH_API_RESULT nh_wsi_setAndroidWindowTitle(
    nh_wsi_AndroidWindow *Window_p, const char *title_p
);

NH_API_RESULT nh_wsi_setAndroidApp(
    struct android_app *app_p
);

ANativeWindow *nh_wsi_acquireAndroidNativeWindow(
    nh_wsi_Window *Window_p, uint32_t *generation_p
);

void nh_wsi_releaseAndroidNativeWindow(
    ANativeWindow *Window_p
);

struct android_app *nh_wsi_getAndroidApp(
);

#endif
