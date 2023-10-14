// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

// INCLUDE =========================================================================================

#include "Header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#include <sys/types.h> 
#include <sys/stat.h>

// HELPER ==========================================================================================

//static long *getLatestDate(
//    long date1_p[3], long date2_p[3], long date3_p[3])
//{
//    NH_BOOL ignoreDate2 = NH_FALSE, ignoreDate3 = NH_FALSE;
//
//    if (date1_p[0] > date2_p[0] && date1_p[0] > date3_p[0]) {
//        return date1_p;
//    }
//    else if (date1_p[0] < date2_p[0] || date1_p[0] < date3_p[0]) {
//        return NULL;
//    }
//    if (date1_p[0] > date2_p[0]) {ignoreDate2 = NH_TRUE;}
//    if (date1_p[0] > date3_p[0]) {ignoreDate3 = NH_TRUE;}
//
//    if ((ignoreDate2 || date1_p[1] > date2_p[1]) && (ignoreDate3 || date1_p[1] > date3_p[1])) {
//        return date1_p;
//    }
//    else if (date1_p[1] < date2_p[1] || date1_p[1] < date3_p[1]) {
//        return NULL;
//    }
//    if (date1_p[1] > date2_p[1]) {ignoreDate2 = NH_TRUE;}
//    if (date1_p[1] > date3_p[1]) {ignoreDate3 = NH_TRUE;}
//
//
//    if ((ignoreDate2 || date1_p[2] > date2_p[2]) && (ignoreDate3 || date1_p[2] > date3_p[2])) {
//        return date1_p;
//    }
//    else if (date1_p[2] < date2_p[2] || date1_p[2] < date3_p[2]) {
//        return NULL;
//    }
//
//    return NULL;
//}

// INSERT ==========================================================================================

//static NH_BYTE *insertVersion(
//    nh_make_Runtime *Runtime_p, NH_BYTE *data_p, size_t size, NH_BOOL ver)
//{
//    int offset = 0;
//
//    while (ver ? strstr(data_p + offset, "NH_MAKE_INSERT_VER_BEGIN") : strstr(data_p + offset, "NH_MAKE_INSERT_REV_BEGIN"))
//    {
//        NH_BYTE *new_p = malloc(size + 10000);
//        memset(new_p, 0, size + 10000);
//
//        NH_BYTE *before_p = data_p;
//        NH_BYTE *after_p = ver ? strstr(before_p + offset, "NH_MAKE_INSERT_VER_BEGIN") : 
//            strstr(before_p + offset, "NH_MAKE_INSERT_REV_BEGIN");
//
//        if (!after_p) {return NULL;}
//
//        after_p += 28;
//        *after_p = 0;
//
//        sprintf(new_p, before_p);
//        offset = strlen(new_p);
//
//        NH_BYTE version_p[255];
//        if (ver) {getVersion(Runtime_p, version_p);}
//        else {getRevision(Runtime_p, version_p);}
//        sprintf(new_p + strlen(new_p), "\n<b>%s</b>", version_p);
//
//        after_p = ver ? strstr(after_p + 1, "<!-- NH_MAKE_INSERT_VER_END") : 
//            strstr(after_p + 1, "<!-- NH_MAKE_INSERT_REV_END"); 
//
//        if (!after_p) {return NULL;}
//
//        sprintf(new_p + strlen(new_p), "\n%s", after_p);
//
//        NH_BOOL next = ver ? strstr(after_p, "NH_MAKE_INSERT_VER_BEGIN") != NULL : 
//            strstr(after_p, "NH_MAKE_INSERT_REV_BEGIN") != NULL;
//
//        free(data_p);
//        data_p = new_p;
//    }
//
//    return data_p;
//}
//
//static NH_BYTE *insertChangelogs(
//    nh_make_SourceContextArray *Array_p, NH_BYTE *data_p, size_t size)
//{
//    NH_BYTE *new_p = malloc(size + 10000);
//    memset(new_p, 0, size + 10000);
//
//    NH_BYTE *before_p = data_p;
//    NH_BYTE *after_p = strstr(data_p, "NH_MAKE_INSERT_CHANGELOGS_BEGIN"); 
//    if (!after_p) {return NULL;}
//    
//    after_p += 35;
//    *after_p = 0;
//
//    sprintf(new_p, before_p);
//
//    for (int i = 0; i < Array_p->length; ++i) 
//    {
//        nh_make_SourceContext *Context_p = &Array_p->SourceContexts_p[i];
//        
//        long *date_p = getLatestDate(Context_p->majorDate_p, Context_p->minorDate_p, Context_p->patchDate_p);
//        if (!date_p) {date_p = getLatestDate(Context_p->minorDate_p, Context_p->majorDate_p, Context_p->patchDate_p);}
//        if (!date_p) {date_p = getLatestDate(Context_p->patchDate_p, Context_p->majorDate_p, Context_p->minorDate_p);}
//        if (!date_p) {date_p = Context_p->majorDate_p;} 
//        
//        NH_BYTE line_p[512];
//
//        sprintf(line_p, "\n%d-%02d-%02d", date_p[0], date_p[1], date_p[2]);
//        sprintf(line_p+strlen(line_p), Context_p->type == NH_MAKE_SOURCE_CONTEXT_BINARY ? " bin " : " lib ");
//        sprintf(line_p+strlen(line_p), Context_p->type == NH_MAKE_SOURCE_CONTEXT_BINARY ? 
//            "<a href=\"dev/html/group__bin__%s__changelog.html" 
//         :  "<a href=\"dev/html/group__lib__%s__changelog.html", Context_p->name_p);
//        sprintf(line_p+strlen(line_p), "#v%d.%d.%d.%d\">%s v%d.%d.%d.%d</a><br>", 
//            Context_p->api, Context_p->major, Context_p->minor, Context_p->patch, Context_p->name_p, 
//            Context_p->api, Context_p->major, Context_p->minor, Context_p->patch);
//
//        sprintf(new_p+strlen(new_p), line_p);
//    }
//
//    after_p = strstr(after_p + 1, "<!-- NH_MAKE_INSERT_CHANGELOGS_END"); 
//    if (!after_p) {return NULL;}
//
//    sprintf(new_p + strlen(new_p), "\n%s", after_p);
//
//    free(data_p);
//
//    return new_p;
//}

static int getVersionString(
    nh_docs_Changelog *Changelog_p, nh_docs_Context *Context_p, nh_docs_Commit *LastCommit_p, NH_BYTE *version_p)
{
    nh_docs_Version Version = nh_docs_getVersion(Changelog_p, Context_p, LastCommit_p);
    sprintf(version_p, "ver.%ld.%ld.%ld.%ld", Version.api, Version.major, Version.minor, Version.patch);
    return 0;
}

static NH_BYTE *insertAPIVersion(
    nh_docs_Changelog *Changelog_p, NH_BYTE *data_p, size_t size)
{
    int offset = 0;

    while (strstr(data_p + offset, "NH_MAKE_INSERT_API_VERSION_BEGIN"))
    {
        NH_BYTE *new_p = malloc(size + 10000);
        memset(new_p, 0, size + 10000);
    
        NH_BYTE *before_p = data_p;
        NH_BYTE *after_p = strstr(data_p + offset, "NH_MAKE_INSERT_API_VERSION_BEGIN"); 
        if (!after_p) {return NULL;}
        
        after_p += 36;
        *after_p = 0;
    
        sprintf(new_p, before_p);
        offset = strlen(new_p);

        NH_BYTE version_p[255];
        memset(version_p, 0, 255);
        nh_docs_ContextArray *ContextArray_p = nh_docs_getContextArray(Changelog_p);
        getVersionString(Changelog_p, nh_docs_getContextFromDescription(ContextArray_p, "netzhaut", NH_DOCS_CONTEXT_LIBRARY), NULL, version_p);
        nh_docs_freeContextArray(ContextArray_p);

        sprintf(new_p + strlen(new_p), "\n");
        sprintf(new_p + strlen(new_p), version_p);
    
        after_p = strstr(after_p + 1, "<!-- NH_MAKE_INSERT_API_VERSION_END"); 
        if (!after_p) {return NULL;}
   
        sprintf(new_p + strlen(new_p), "\n%s", after_p);
    
        free(data_p);
        data_p = new_p;
    }

    return data_p;
}

static int getRevisionString(
    nh_docs_Changelog *Changelog_p, nh_docs_Commit *LastCommit_p,  NH_BYTE *revision_p)
{
    nh_docs_Version Revision = nh_docs_getRevision(Changelog_p, LastCommit_p);
    sprintf(revision_p, "rev.%ld.%ld.%ld.%ld", Revision.api, Revision.major, Revision.minor, Revision.patch);
    return 0;
}

static NH_BYTE *insertRevision(
    nh_docs_Changelog *Changelog_p, NH_BYTE *data_p, size_t size)
{
    int offset = 0;

    while (strstr(data_p + offset, "NH_MAKE_INSERT_REVISION_BEGIN"))
    {
        NH_BYTE *new_p = malloc(size + 10000);
        memset(new_p, 0, size + 10000);
    
        NH_BYTE *before_p = data_p;
        NH_BYTE *after_p = strstr(data_p + offset, "NH_MAKE_INSERT_REVISION_BEGIN"); 
        if (!after_p) {return NULL;}
        
        after_p += 33;
        *after_p = 0;
    
        sprintf(new_p, before_p);
        offset = strlen(new_p);

        NH_BYTE revision_p[255];
        memset(revision_p, 0, 255);
        getRevisionString(Changelog_p, NULL, revision_p);

        sprintf(new_p + strlen(new_p), "\n");
        sprintf(new_p + strlen(new_p), revision_p);

        after_p = strstr(after_p + 1, "<!-- NH_MAKE_INSERT_REVISION_END"); 
        if (!after_p) {return NULL;}
    
        sprintf(new_p + strlen(new_p), "\n%s", after_p);
    
        free(data_p);
        data_p = new_p;
    }

    return data_p;
}

// GENERATE ========================================================================================

int generateHeader(
    nh_docs_Changelog *Changelog_p, NH_BYTE *path_p)
{
    long size;
    NH_BYTE *data_p = nh_api_getFileData(path_p, &size);
    if (!data_p) {return 1;}

    data_p = insertAPIVersion(Changelog_p, data_p, size);
    if (!data_p) {return 1;}

    data_p = insertRevision(Changelog_p, data_p, size);
    if (!data_p) {return 1;}

    nh_api_writeBytesToFile(path_p, data_p);
    free(data_p);

    return 0;
}

