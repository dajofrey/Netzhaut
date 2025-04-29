// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HTMLImageElement.h"

#include "../Parser/Algorithms.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/Attr.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-webidl/Runtime/Object.h"
#include "../../nh-url/Main/URLParser.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

typedef enum NH_HTML_IMAGE_REQUEST {
    NH_HTML_IMAGE_REQUEST_UNAVAILABLE,
    NH_HTML_IMAGE_REQUEST_PARTIALLY_AVAILABLE,
    NH_HTML_IMAGE_REQUEST_COMPLETELY_AVAILABLE,
    NH_HTML_IMAGE_REQUEST_BROKEN,
} NH_HTML_IMAGE_REQUEST;

typedef struct nh_html_ImageRequest {
    NH_HTML_IMAGE_REQUEST state;
    nh_url_URL *CurrentURL_p;
    char *imageData_p;
    int currentPixelDensity;
} nh_html_ImageRequest;

typedef struct nh_html_Image {
    nh_webidl_Object *dimensionAttributeSource_p;
    nh_html_ImageRequest CurrentRequest;
    nh_html_ImageRequest *PendingRequest_p;
} nh_html_Image;

// FUNCTIONS =======================================================================================

static nh_html_ImageRequest nh_html_initImageRequest()
{
    nh_html_ImageRequest Request;

    Request.state = NH_HTML_IMAGE_REQUEST_UNAVAILABLE;
    Request.CurrentURL_p = NULL;
    Request.imageData_p = NULL;
    Request.currentPixelDensity = 1;

    return Request;
}

NH_API_RESULT nh_html_initializeHTMLImageElement(
    nh_webidl_Object *Image_p)
{
    Image_p->internal_p = nh_core_allocate(sizeof(nh_html_Image));
    NH_CORE_CHECK_NULL(Image_p->internal_p)

    ((nh_html_Image*)Image_p->internal_p)->dimensionAttributeSource_p = Image_p;
    ((nh_html_Image*)Image_p->internal_p)->CurrentRequest = nh_html_initImageRequest();
    ((nh_html_Image*)Image_p->internal_p)->PendingRequest_p = NULL;

    return NH_API_SUCCESS;
}

// https://html.spec.whatwg.org/#update-the-image-data
void nh_html_updateImageData(
    void *Image_p, bool restartAnimations)
{
    // TODO

    char *selectedSource_p = NULL;
    int selectedPixelDensity = -1;

    nh_webidl_Object *Attr_p = nh_dom_getAttrByLocalName(
        NH_WEBIDL_GET_DOM_ELEMENT((nh_webidl_Object*)Image_p), "src"
    );

    nh_webidl_DOMString *Value_p = nh_dom_getAttrValue(Attr_p);

    selectedSource_p = Value_p->p;
    selectedPixelDensity = 1;

    char *lastSelectedSource_p = selectedSource_p;

//    if (selectedSource_p) 
//    {
//        nh_url_URL *URL_p = nh_html_parseURL();
//    printf("%s\n", selectedSource_p);
//    puts("updateImageData");
//    exit(0);
//    }
}
