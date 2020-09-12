// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/URI.h"
#include "../Header/Memory.h"
#include "../Header/String.h"
#include "../Header/File.h"
#include "../Header/Macros.h"
#include "../Header/Config.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include "../../Network/Header/URL.h"

#include "../../CSS/Header/Sheet.h"
#include "../../HTML/Header/Document.h"
#include "../../JavaScript/Header/Script.h"

#include "../../Test/HTML/Index.h"
#include "../../Test/HTML/Background.h"
#include "../../Test/HTML/Border.h"
#include "../../Test/HTML/Canvas.h"
#include "../../Test/HTML/Image.h"
#include "../../Test/HTML/Input.h"
#include "../../Test/HTML/Selector.h"
#include "../../Test/HTML/Text.h"
#include "../../Test/HTML/List.h"

#include "../../Test/Media/JPG.h"

#include "../../CSS/Header/Icons/Expand.h"
#include "../../CSS/Header/Icons/Disc.h"
#include "../../CSS/Header/Icons/Circle.h"
#include "../../CSS/Header/Icons/Square.h"

#include "../../CSS/Header/Sheets/Misc.h"
#include "../../CSS/Header/Sheets/SelectOption.h"
#include "../../CSS/Header/Sheets/Header.h"
#include "../../CSS/Header/Sheets/List.h"

#include "../../JavaScript/Header/Scripts/SelectOption.h"
#include "../../JavaScript/Header/Scripts/Scroll.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// DATA ============================================================================================

const char *syntax_pp[] =
{
    "test:html:index",
    "test:html:border",
    "test:html:image",
    "test:html:input",
    "test:html:canvas",
    "test:html:text",
    "test:html:background",
    "test:html:list",
    "test:media:jpg",
    "icon:expand",
    "icon:disc",
    "icon:circle",
    "icon:square",
    "css:list",
    "css:selectoption",
    "css:misc",
    "css:header",
    "js:selectoption",
    "js:scroll",
    "js:selecttext"
    "win:$",
    "tab:$",
    "win:$:tab:$",
};

// DECLARE =========================================================================================

typedef struct Nh_InternalURN {
    int syntaxIndex;
    int indices_p[10];
} Nh_InternalURN;

typedef struct Nh_InternalURL {
    NH_INTERNAL_URL type;
    void *pointer;
} Nh_InternalURL;

typedef struct Nh_FileURL {
    char *authority_p; /**<Currently always NULL*/ 
    char *path_p;
} Nh_FileURL;

static Nh_URI Nh_createFileURL(
    char *chars_p, char *base_p
);
static Nh_URI Nh_createInternalURN(
    char *chars_p
);
static Nh_URI Nh_createInternalURL(
    void *pointer, NH_INTERNAL_URL type
);

static NH_MEDIA Nh_getInternalURNMediaType(
    int syntaxIndex
);

static char *Nh_getDataFromInternalURN(
    Nh_InternalURN *URN_p, long *size_p
);
static char *Nh_getDataFromInternalURL(
    Nh_InternalURL *URL_p
);

// DEFAULT =========================================================================================

static Nh_URI Nh_getDefaultURI()
{
    Nh_URI URI;
    URI.scheme = -1;
    URI.size = 0;
    URI.data_p = NULL;
    URI.payload_p = NULL;
    return URI;
}

// CREATE ==========================================================================================

Nh_URI Nh_createURI(
    char *chars_p, char *base_p, Nh_URI *Base_p, void *pointer, NH_INTERNAL_URL type)
{
NH_BEGIN()

    if (pointer != NULL) {NH_END(Nh_createInternalURL(pointer, type))}
    if (Base_p != NULL) {printf("URI base not implemented!!!\n");exit(0);}
    if (chars_p == NULL) {NH_END(Nh_getDefaultURI())}

    char cpy_p[2048] = {'\0'};
    strcpy(cpy_p, chars_p);
    chars_p = cpy_p;

    while (chars_p[0] == ' ' || chars_p[0] == '\t') {chars_p = chars_p + 1;} // remove leading spaces or tabs
    while (chars_p[strlen(chars_p) - 1] == ' ' || chars_p[strlen(chars_p) - 1] == '\t') {chars_p[strlen(chars_p) - 1] = '\0';} // remove trailing spaces

    if (chars_p[0] == 'n' && chars_p[1] == 'h' && chars_p[2] == ':') {
        NH_END(Nh_createInternalURN(chars_p + 3))
    }  

    if (Nh_fileExistsOnLocalHost(chars_p, base_p)) {
        NH_END(Nh_createFileURL(chars_p, base_p))
    }

    // default
    Nh_URI URI = Nh_getDefaultURI();
    URI.data_p = Nh_Net_createURL(chars_p, &URI.scheme);
    
NH_END(URI)
}

static Nh_URI Nh_createFileURL(
    char *chars_p, char *base_p)
{
NH_BEGIN()

    Nh_URI URI = Nh_getDefaultURI();
    URI.scheme = NH_URI_SCHEME_FILE;
    URI.data_p = Nh_allocate(sizeof(Nh_FileURL));
    if (URI.data_p == NULL) {NH_END(URI)}

    Nh_FileURL *URL_p = URI.data_p;
    URL_p->authority_p = NULL;
    URL_p->path_p = NULL;

    char path_p[2048] = {'\0'};
    if (Nh_getFilePath(chars_p, base_p, path_p) != NH_SUCCESS) {NH_END(URI)}

    URL_p->path_p = Nh_allocateChars(path_p);

NH_END(URI)
}

static Nh_URI Nh_createInternalURN(
    char *chars_p)
{
NH_BEGIN()

    Nh_URI URI = Nh_getDefaultURI();
    URI.scheme = NH_URI_SCHEME_INTERNAL_URN;
    URI.data_p = Nh_allocate(sizeof(Nh_InternalURN));
    if (URI.data_p == NULL) {NH_END(URI)}

    Nh_InternalURN *URN_p = URI.data_p;
    URN_p->syntaxIndex = -1;
    memset(URN_p->indices_p, 0, sizeof(int) * 10);

    for (int i = 0; i < Nh_getConfig()->InternalURNSyntax.count; ++i) 
    {
        bool hit = true;

        char parts_p[512] = {'\0'};
        strcpy(parts_p, chars_p);

        char syntax_p[512] = {'\0'};
        strcpy(syntax_p, Nh_getListItem(&Nh_getConfig()->InternalURNSyntax, i));
        
        char *parts_pp[100] = {NULL};
        char *part_p = strtok(parts_p, ":");

        int index = 0;
        while (part_p != NULL) {
            parts_pp[index++] = part_p;
            part_p = strtok(NULL, ":");
        }

        char *syntaxPart_p = strtok(syntax_p, ":");
        
        index = 0;
        int partIndex = 0;
        while (syntaxPart_p != NULL) 
        {
            if (parts_pp[partIndex] == NULL) {hit = false; break;}
            if (strcmp(syntaxPart_p, parts_pp[partIndex]) != 0) 
            {
                if (syntaxPart_p[0] == '$') 
                {
                    char *end_p;
                    long int value = strtol(parts_pp[partIndex], &end_p, 10);
                    if (end_p == parts_pp[partIndex]) {hit = false; break;}
                    URN_p->indices_p[index++] = value;
                }
                else {hit = false;}
            } 
            partIndex++;
            syntaxPart_p = strtok(NULL, ":");
        }

        if (hit) {
            URN_p->syntaxIndex = i;
            break;
        }
    }

NH_END(URI)
}

static Nh_URI Nh_createInternalURL(
    void *pointer, NH_INTERNAL_URL type)
{
NH_BEGIN()

    Nh_URI URI = Nh_getDefaultURI();

    URI.scheme = NH_URI_SCHEME_INTERNAL_URL;
    URI.data_p = Nh_allocate(sizeof(Nh_InternalURL));
    if (URI.data_p == NULL) {NH_END(URI)}

    Nh_InternalURL *URL_p = URI.data_p;
    URL_p->pointer = pointer;
    URL_p->type = type;

NH_END(URI)
}

// FREE ============================================================================================

void Nh_freeURI(
    Nh_URI URI)
{
NH_BEGIN()

    Nh_freeData(URI);

    switch (URI.scheme)
    {
        case NH_URI_SCHEME_HTTP  :
        case NH_URI_SCHEME_HTTPS : Nh_Net_freeURL(URI); break;
        default                  : Nh_free(URI.data_p); break;
    }

NH_SILENT_END()
}

// SYNTAX ==========================================================================================

NH_RESULT Nh_createInternalURNSyntax()
{
NH_BEGIN()

    for (int i = 0; i < (sizeof(syntax_pp) / sizeof(syntax_pp[0])); ++i) {
        if (Nh_addInternalURNSyntax((char*)syntax_pp[i]) == -1) {NH_END(NH_ERROR_BAD_STATE)}
    }

NH_END(NH_SUCCESS)
}

int Nh_addInternalURNSyntax(
    char *syntax_p)
{
NH_BEGIN()

    if (Nh_addListItem(&Nh_getConfig()->InternalURNSyntax, Nh_allocateChars(syntax_p)) != NH_SUCCESS) {
        NH_END(-1)
    }
  
NH_END(Nh_getConfig()->InternalURNSyntax.count - 1)
}

// COPY ============================================================================================

Nh_URI Nh_copyURI(
    Nh_URI *URI_p)
{
NH_BEGIN()

    Nh_URI URI = Nh_getDefaultURI();

    switch (URI_p->scheme)
    {
        case NH_URI_SCHEME_INTERNAL_URN : 
        {
            printf("copy");exit(0);
        }
        case NH_URI_SCHEME_INTERNAL_URL : 
        {
            printf("copy");exit(0);
        }
        case NH_URI_SCHEME_FILE         : 
        {
            Nh_FileURL *Old_p = URI_p->data_p;
            Nh_FileURL *New_p = Nh_allocate(sizeof(Nh_FileURL));

            URI.data_p = New_p;
            URI.scheme = NH_URI_SCHEME_FILE;

            New_p->path_p = NULL;
            New_p->authority_p = NULL;

            if (Old_p->path_p != NULL) {
                New_p->path_p = Nh_allocate(sizeof(char) * (strlen(Old_p->path_p) + 1));
                strcpy(New_p->path_p, Old_p->path_p);
            }
            if (Old_p->authority_p != NULL) {
                New_p->authority_p = Nh_allocate(sizeof(char) * (strlen(Old_p->authority_p) + 1));
                strcpy(New_p->authority_p, Old_p->authority_p);
            }
        }
    }

NH_END(URI)
}

// COMPARE =========================================================================================

bool Nh_equivalentURI(
    Nh_URI URI1, Nh_URI URI2)
{
NH_BEGIN()

    if (URI1.scheme != URI2.scheme) {NH_END(false)}

    switch (URI1.scheme)
    {
        case NH_URI_SCHEME_INTERNAL_URN :
        {
            Nh_InternalURN *Tmp1_p = URI1.data_p, *Tmp2_p = URI2.data_p;
            if (Tmp1_p->syntaxIndex != Tmp2_p->syntaxIndex) {NH_END(false)}
            for (int i = 0; i < 10; ++i) {if (Tmp1_p->indices_p[i] != Tmp2_p->indices_p[i]) {NH_END(false)}}
            NH_END(true)
        }
        case NH_URI_SCHEME_FILE :
        {
            Nh_FileURL *URL1_p = URI1.data_p, *URL2_p = URI2.data_p;
            if (URL1_p->path_p == NULL && URL2_p->path_p == NULL) {NH_END(true)}
            if (URL1_p->path_p != NULL && URL2_p->path_p != NULL && !strcmp(URL1_p->path_p, URL2_p->path_p)) {NH_END(true)}
            NH_END(false)
        }
       
        default : break;
    }

NH_END(false)
}

// DATA FORMAT =====================================================================================

NH_MEDIA Nh_getMediaType(
    Nh_URI URI)
{
NH_BEGIN()

    switch (URI.scheme)
    {
        case NH_URI_SCHEME_INTERNAL_URN : 
        {
            Nh_InternalURN *URN_p = URI.data_p;
            NH_END(Nh_getInternalURNMediaType(URN_p->syntaxIndex))
        }
        case NH_URI_SCHEME_INTERNAL_URL : 
        {
            NH_END(-1) 
        }
        case NH_URI_SCHEME_FILE         : 
        {
            Nh_FileURL *URL_p = URI.data_p;
            NH_END(Nh_getMediaTypeFromPath(URL_p->path_p))
        }
    }

NH_END(-1)
}

static NH_MEDIA Nh_getInternalURNMediaType(
    int syntaxIndex)
{
NH_BEGIN()

    switch (syntaxIndex)
    {
        case  0:
        case  1:
        case  2: 
        case  3: 
        case  4: 
        case  5: 
        case  6: 
        case  7: NH_END(NH_MEDIA_TEXT_HTML) 
        case  8: NH_END(NH_MEDIA_IMAGE_JPEG)
        case  9: 
        case 10: 
        case 11: 
        case 12: NH_END(NH_MEDIA_IMAGE_PNG)
        case 13: 
        case 14: 
        case 15: 
        case 16: NH_END(NH_MEDIA_TEXT_CSS) 
        case 17: 
        case 18: 
        case 19: NH_END(NH_MEDIA_TEXT_JAVASCRIPT) 
        case 20: break;
        case 21: break;
        case 22: break;
    }

NH_END(-1)
}

// DATA ============================================================================================

void *Nh_getData(
    Nh_URI URI, long *size_p)
{
NH_BEGIN()

    if (URI.payload_p != NULL) {
        if (size_p != NULL) {*size_p = URI.size;}
        NH_END(URI.payload_p)
    }

    long size = 0;
    void *payload_p = NULL;

    switch (URI.scheme)
    {
        case NH_URI_SCHEME_HTTP         : 
        case NH_URI_SCHEME_HTTPS        : 
        case NH_URI_SCHEME_UNDEFINED    : payload_p = Nh_Net_getDataFromURL(URI.data_p, &size, 0);break;
        case NH_URI_SCHEME_INTERNAL_URN : payload_p = Nh_getDataFromInternalURN(URI.data_p, &size); break;
        case NH_URI_SCHEME_INTERNAL_URL : payload_p = Nh_getDataFromInternalURL(URI.data_p); break;
        case NH_URI_SCHEME_FILE         : payload_p = Nh_getFileData(((Nh_FileURL*)URI.data_p)->path_p, &size); break;
        default : break;
    }

    URI.size = size;
    URI.payload_p = payload_p;

    if (size_p != NULL) {*size_p = size;}

NH_END(payload_p)
}

void Nh_freeData(
    Nh_URI URI)
{
NH_BEGIN()

    if (URI.payload_p == NULL) {NH_SILENT_END()}

    switch (URI.scheme)
    {
        case NH_URI_SCHEME_FILE : Nh_free(URI.payload_p); break;
        default : break;
    }

NH_SILENT_END()
}

static char *Nh_getDataFromInternalURN(
    Nh_InternalURN *URN_p, long *size_p)
{
NH_BEGIN()

    switch (URN_p->syntaxIndex)
    {
        case  0: NH_END(IndexHTML)
        case  1: NH_END(BorderHTML)
        case  2: NH_END(ImageHTML) 
        case  3: NH_END(InputHTML) 
        case  4: NH_END(CanvasHTML) 
        case  5: NH_END(TextHTML) 
        case  6: NH_END(BackgroundHTML) 
        case  7: NH_END(ListHTML) 
        case  8: *size_p = JPG_len; NH_END(JPG) 
        case  9: *size_p = ExpandPNG_len; NH_END(ExpandPNG)
        case 10: *size_p = DiscPNG_len; NH_END(DiscPNG)
        case 11: *size_p = CirclePNG_len; NH_END(CirclePNG)
        case 12: *size_p = SquarePNG_len; NH_END(SquarePNG) 
        case 13: NH_END(ListCSS)
        case 14: NH_END(SelectOptionCSS)
        case 15: NH_END(MiscCSS)
        case 16: NH_END(HeaderCSS)
        case 17: NH_END(SelectOptionJS)
        case 18: NH_END(ScrollJS)
        case 19: NH_END(NULL)
        case 20: NH_END(NULL) 
        case 21: NH_END(NULL) 
        case 22: NH_END(NULL) 
    }

NH_END(NULL)
}

static char *Nh_getDataFromInternalURL(
    Nh_InternalURL *URL_p)
{
NH_BEGIN()

    switch (URL_p->type)
    {
        case NH_INTERNAL_URL_HTML_NODE_TEXT            : NH_END( ((Nh_HTML_Node*)URL_p->pointer)->text_p       )
        case NH_INTERNAL_URL_HTML_NODE_ATTRIBUTE_VALUE : NH_END( ((Nh_HTML_Attribute*)URL_p->pointer)->value_p )
    }

NH_END(NULL)
}

