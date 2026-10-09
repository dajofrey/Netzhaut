#ifndef NH_WSI_PLATFORMS_ANDROID_INIT_H
#define NH_WSI_PLATFORMS_ANDROID_INIT_H

#include "../../Common/Includes.h"

struct android_app;

NH_API_RESULT nh_wsi_initializeAndroid(
);

NH_API_RESULT nh_wsi_setAndroidApp(
    struct android_app *app_p
);

void nh_wsi_terminateAndroid(
);

#endif
