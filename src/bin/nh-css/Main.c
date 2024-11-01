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
 * Utility function for getting file data. 
 */
static void *getFileData(
    char *path_p, long *size_p) 
{
    FILE *fh = fopen(path_p, "rb");
    if (fh == NULL) {return NULL;}
    
    if (fseek(fh, 0L, SEEK_END) != 0) {return NULL;}

    long size = ftell(fh);

    if (fseek(fh, 0L, SEEK_SET) != 0) {return NULL;}

    if(size <= 0) {
        fclose(fh);
        return NULL;
    }

    char *data_p = (char*)malloc(((size_t)size) + 1); 
    if (!data_p) {return NULL;}
    
    fread(data_p, 1, size, fh);
    fclose(fh);

    data_p[size] = '\0';
    if (size_p) {*size_p = size;}

    return data_p;
}

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
    void *data_p = getFileData(argv_pp[1], &size);

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
