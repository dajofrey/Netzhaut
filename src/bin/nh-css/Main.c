/**
 * Netzhaut - Web Browser 
 * Copyright (C) 2022  Dajo Frey
 * This file is published under MIT.
 */

#include "nh-api/nh-api.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/** 
 * Parse a style sheet and dump the result.
 */
int main(
    int argc, char **argv_pp) 
{
    if (argc < 2) {
        puts("Path expected. Exiting.");
        return 1;
    }

    if (nh_api_initialize(NULL, NULL, 0) != NH_API_SUCCESS) {
        puts("API initialization failed. Exiting.\n");
        return 1;
    }

    long size;
    void *data_p = nh_api_getFileData(argv_pp[1], &size);

    if (!data_p || !size) {
        puts("Getting file data failed. Exiting.");
        return 1;
    }

    nh_api_StyleSheet *StyleSheet_p = nh_api_parseStyleSheet(data_p, size);
    if (!StyleSheet_p) {
        puts("Creating stylesheet failed. Exiting.");
        return 1;
    }

    if (argc == 3) {
        char node_p[255] = {'\0'}; 
        sprintf(node_p, "nh-css:Sheet:%p:%s", StyleSheet_p, argv_pp[2]);
        nh_api_dump(node_p);
    }

    nh_api_terminate();
}
