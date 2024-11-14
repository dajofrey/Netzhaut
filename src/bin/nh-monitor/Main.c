/**
 * Netzhaut - Web Browser 
 * Copyright (C) 2022  Dajo Frey
 * This file is published under MIT.
 */

#include "nh-api/nh-api.h"
#include "ttyr-api/ttyr-api.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static ttyr_tty_TTY *TTY_p = NULL;

static void handleMonitorInput(
    nh_api_Window *Window_p, nh_api_WSIEvent Event)
{
    switch (Event.type)
    {
        case NH_API_WSI_EVENT_KEYBOARD :
            ttyr_api_sendEvent(TTY_p, Event);
            break;
    }
}

static int openMonitor()
{
    static void *Interface_p = NULL;

    Interface_p = nh_api_createMonitorInterface();
    if (!Interface_p) {
        puts("Creating monitor interface failed.");
        return 1;
    }

    ttyr_api_initialize();

    TTY_p = ttyr_api_openTTY(NULL, Interface_p);
    if (!TTY_p) {
        puts("Opening tty failed");
        return 1;
    }

    if (ttyr_api_claimStandardIO(TTY_p) == 0) {return 0;}

    puts("Opening standard-IO monitor failed. Trying standalone monitor.");

    ttyr_terminal_Terminal *Terminal_p = ttyr_api_openTerminal(NULL, TTY_p); 
    if (!Terminal_p) {
        puts("Opening terminal failed.");
        return 1;
    } 
 
    nh_api_Window *Window_p = 
        nh_api_createWindow(NULL, nh_api_getSurfaceRequirements()); 
    if (!Window_p) {
        puts("Opening window failed.");
        return 1;
    }
 
    nh_api_Surface *Surface_p = nh_api_createSurface(Window_p, NH_API_GRAPHICS_BACKEND_OPENGL); 
    if (!Surface_p) {
        puts("Opening surface failed.");
        return 1;
    }
 
    nh_api_PixelPosition Position = {0};
    nh_api_PixelSize Size; 
    Size.width  = 700; 
    Size.height = 700; 
 
    nh_api_Viewport *Viewport_p = nh_api_createViewport(Surface_p, Position, Size); 
    if (!Viewport_p) {
        puts("Opening viewport failed.");
        return 1;
    } 
 
    if (ttyr_api_setViewport(Terminal_p, Viewport_p)) {
        puts("Setting viewport failed.");
        return 1;
    }

    nh_api_setWindowEventListener(Window_p, handleMonitorInput);

    return 0;
}

int main(
    int argc, char **argv_pp) 
{
    if (nh_api_initialize(NULL, NULL, 0) != NH_API_SUCCESS) {
        puts("API initialization failed. Exiting.");
        return 1;
    }

    if (argc > 1) {
        char config_p[128] = {0};
        sprintf(config_p, "nh-core.monitor.port:%d;", atoi(argv_pp[1]));  
        if (nh_api_loadConfig(config_p, strlen(config_p)) != NH_API_SUCCESS) {
            puts("Setting monitor port failed. Exiting.");
            return 1;
        }
    }

    if (openMonitor()) {
        puts("Opening monitor failed. Exiting.");
        return 1;
    }

    while (1) {
        int result = nh_api_run();
        if (result == -1) {
            puts("Encountered workload error. Exiting.");
            break;
        } else if (result == 0) {
            // idle
            usleep(10000);
        }
        if (!nh_api_keepRunning()) {break;}
    }

    nh_api_terminate();
}
