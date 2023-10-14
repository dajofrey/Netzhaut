// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

// INCLUDE =========================================================================================

#include "Version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/types.h> 
#include <sys/stat.h>

// UPDATE VERSIONS =================================================================================

static NH_BYTE *writeVersion(
    NH_BYTE *data_p, NH_BYTE *scope_p, long version)
{
    NH_BYTE *newData_p = malloc(sizeof(NH_BYTE)* (strlen(data_p)+8));
    if (!newData_p) {return NULL;}
    memset(newData_p, 0, strlen(data_p)+8);

    NH_BYTE *n = strstr(data_p, scope_p);
    if (!n) {return NULL;}

    *n = '\0';

    NH_BYTE new_p[255];
    memset(new_p, 0, 255);
    sprintf(new_p, "%s %d", scope_p, version);

    NH_BYTE *end_p = n+1;
    while (*end_p != '\n') {++end_p;}

    sprintf(newData_p, "%s%s%s", data_p, new_p, end_p);

    free(data_p);

    return newData_p;
}

static NH_BYTE *createFileData(
    NH_BYTE *data_p, nh_docs_Context *Context_p, nh_docs_Version Version)
{
    NH_BYTE api_p[255], major_p[255], minor_p[255], patch_p[255];
    NH_BYTE name_p[255];
    memset(name_p, 0, 255);

    strcpy(name_p, Context_p->name_p + 2);
    for (int i = 0; i < strlen(name_p); ++i) {name_p[i] = toupper(name_p[i]);}

    if (Context_p->type == NH_DOCS_CONTEXT_BINARY) {
        sprintf(api_p, "API_VERSION"); 
        sprintf(major_p, "MAJOR_VERSION"); 
        sprintf(minor_p, "MINOR_VERSION");
        sprintf(patch_p, "PATCH_VERSION");
    }
    else if (!strcmp(Context_p->name_p, "netzhaut")) {
        sprintf(api_p, "NETZHAUT_API_VERSION"); 
        sprintf(major_p, "NETZHAUT_MAJOR_VERSION"); 
        sprintf(minor_p, "NETZHAUT_MINOR_VERSION");
        sprintf(patch_p, "NETZHAUT_PATCH_VERSION");
    }
    else {
        sprintf(api_p, "NH_%s_API_VERSION", name_p); 
        sprintf(major_p, "NH_%s_MAJOR_VERSION", name_p); 
        sprintf(minor_p, "NH_%s_MINOR_VERSION", name_p);
        sprintf(patch_p, "NH_%s_PATCH_VERSION", name_p);
    }

    NH_BYTE *newData_p = NULL;

    newData_p = writeVersion(data_p, api_p, Version.api);
    newData_p = writeVersion(newData_p, major_p, Version.major);
    newData_p = writeVersion(newData_p, minor_p, Version.minor);
    newData_p = writeVersion(newData_p, patch_p, Version.patch);

    return newData_p;
}

int generateVersionInAboutFile(
    nh_docs_Context *Context_p, nh_docs_Version Version)
{
    long size = 0;
    NH_BYTE path_p[255];
    memset(path_p, 0, 255);

    if (!strcmp(Context_p->name_p, "netzhaut") && Context_p->type != NH_DOCS_CONTEXT_BINARY) {
        sprintf(path_p, "src/lib/netzhaut/netzhaut.h");
    }
    else {
        if (Context_p->type == NH_DOCS_CONTEXT_BINARY) {
            sprintf(path_p, "src/bin/%s/About.h", Context_p->name_p);
        }
        else {
            sprintf(path_p, "src/lib/%s/Common/About.h", Context_p->name_p);
        }
    }

    NH_BYTE *data_p = nh_api_getFileData(path_p, &size);
    if (!data_p) {return 1;}

    data_p = createFileData(data_p, Context_p, Version);
    if (nh_api_writeBytesToFile(path_p, data_p)) {return 1;}
    free(data_p);

    return 0;
}

