/**
 * Netzhaut - Web Browser 
 * Copyright (C) 2022  Dajo Frey
 * This file is published under MIT.
 */

#include "nh-api/nh-api.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static void handleInput( 
    nh_api_Window *Window_p, nh_api_WSIEvent Event) 
{
    switch (Event.type) 
    { 
        case NH_API_WSI_EVENT_MOUSE : 
            break; 
        case NH_API_WSI_EVENT_WINDOW : 
            break; 
    } 
} 

/** 
 * Routine for opening a window.
 */
int main(
    int argc, char **argv_pp) 
{
    if (argc < 1 || argc > 2) {
        puts("Invalid argument count. Exiting.");
        return 1;
    }

    if (nh_api_initialize(NULL, argc > 1 ? argv_pp[1] : NULL, argc > 1 ? strlen(argv_pp[1]) : 0) != NH_API_SUCCESS) {
        puts("API initialization failed. Exiting.\n");
        return 1;
    }

    nh_api_Window *Window_p = nh_api_createWindow(NULL, NULL);
    if (!Window_p) {
        puts("Creating window failed. Exiting.");
        return 1;
    }

    nh_api_setWindowEventListener(Window_p, handleInput);

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
