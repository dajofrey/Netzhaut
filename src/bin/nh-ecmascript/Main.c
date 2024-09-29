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
    if (argc <= 1) {return 1;}

    long size = 0;
    void *script_p = getFileData(argv_pp[1], &size);

    if (!script_p || !size) {return 1;}
    if (nh_api_initialize(NH_LOADER_SCOPE_SYSTEM, NULL, NULL, 0) != NH_API_SUCCESS) {return 1;}

    if (nh_api_addLogCallback(printLog)) {return 1;}

    nh_api_parseScript(script_p, NULL, 0);

    nh_api_terminate();
}

