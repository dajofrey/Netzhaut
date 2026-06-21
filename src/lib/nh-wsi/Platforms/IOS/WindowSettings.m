#include "WindowSettings.h"

NH_API_RESULT nh_wsi_setIOSWindowTitle(
    nh_wsi_IOSWindow *Window_p, char *title_p)
{
    (void)Window_p;
    (void)title_p;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setIOSWindowDecorated(
    nh_wsi_IOSWindow *Window_p, bool decorated)
{
    (void)Window_p;
    (void)decorated;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setIOSWindowState(
    nh_wsi_IOSWindow *Window_p, bool *state_p)
{
    (void)Window_p;
    (void)state_p;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setIOSWindowMouseCursor(
    nh_wsi_IOSWindow *Window_p, NH_WSI_CURSOR_E cursor)
{
    (void)Window_p;
    (void)cursor;
    return NH_API_SUCCESS;
}
