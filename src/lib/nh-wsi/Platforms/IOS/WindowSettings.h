#ifndef NH_WSI_IOS_WINDOW_SETTINGS_H
#define NH_WSI_IOS_WINDOW_SETTINGS_H

#include "Window.h"
#include "../../Window/Window.h"
#include "../../Common/Includes.h"

NH_API_RESULT nh_wsi_setIOSWindowTitle(
    nh_wsi_IOSWindow *Window_p, char *title_p
);

NH_API_RESULT nh_wsi_setIOSWindowDecorated(
    nh_wsi_IOSWindow *Window_p, bool decorated
);

NH_API_RESULT nh_wsi_setIOSWindowState(
    nh_wsi_IOSWindow *Window_p, bool *state_p
);

NH_API_RESULT nh_wsi_setIOSWindowMouseCursor(
    nh_wsi_IOSWindow *Window_p, NH_WSI_CURSOR_E cursor
);

#endif
