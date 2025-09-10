/**
 * Netzhaut - Web Browser 
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

#include "../../lib/nh-api/nh-api.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void printLog(
    char *node_p, char *options_p, char *message_p)
{
    if (strstr(node_p, "nh-ecmascript")) {
        printf("%s %s\n", node_p, message_p);
    }
}

int main(
    int argc, char **argv_pp) 
{
    if (argc < 2 || argc > 3) {
        puts("Invalid argument count. Exiting.");
        return 1;
    }

    if (nh_api_initialize(NULL, argc > 2 ? argv_pp[2] : NULL, argc > 2 ? strlen(argv_pp[2]) : 0) != NH_API_SUCCESS) {
        puts("API initialization failed. Exiting.\n");
        return 1;
    }

    long size = 0;
    void *script_p = nh_api_getFileData(argv_pp[1], &size);

    if (!script_p || !size) {
        script_p = argv_pp[1];
        puts("Getting file data failed.");
    }

//    if (nh_api_addLogCallback(printLog)) {return 1;}

    nh_api_Runtime *Runtime_p = nh_api_startRuntime();
    nh_api_Agent *Agent_p = nh_api_createAgent();

    nh_api_Script *Script_p = nh_api_parseScript(script_p, NULL, 0);
    nh_api_evaluateScript(Script_p);

    nh_api_terminate();
}
