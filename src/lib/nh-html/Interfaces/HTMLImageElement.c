// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HTMLImageElement.h"

#include "../Parser/Algorithms.h"
#include "../Common/Macros.h"

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
    NH_BYTE *imageData_p;
    int currentPixelDensity;
} nh_html_ImageRequest;

typedef struct nh_html_Image {
    nh_webidl_Object *dimensionAttributeSource_p;
    nh_html_ImageRequest CurrentRequest;
    nh_html_ImageRequest *PendingRequest_p;
} nh_html_Image;

// INITIALIZE ======================================================================================

static nh_html_ImageRequest nh_html_initImageRequest()
{
NH_HTML_BEGIN()

    nh_html_ImageRequest Request;

    Request.state = NH_HTML_IMAGE_REQUEST_UNAVAILABLE;
    Request.CurrentURL_p = NULL;
    Request.imageData_p = NULL;
    Request.currentPixelDensity = 1;

NH_HTML_END(Request)
}

NH_HTML_RESULT nh_html_initializeHTMLImageElement(
    nh_webidl_Object *Image_p)
{
NH_HTML_BEGIN()

    Image_p->internal_p = nh_core_allocate(sizeof(nh_html_Image));
    NH_HTML_CHECK_NULL(Image_p->internal_p)

    ((nh_html_Image*)Image_p->internal_p)->dimensionAttributeSource_p = Image_p;
    ((nh_html_Image*)Image_p->internal_p)->CurrentRequest = nh_html_initImageRequest();
    ((nh_html_Image*)Image_p->internal_p)->PendingRequest_p = NULL;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// UPDATE ==========================================================================================

// https://html.spec.whatwg.org/#update-the-image-data
void nh_html_updateImageData(
    nh_html_HTMLImageElement *Image_p, NH_BOOL restartAnimations)
{
NH_HTML_BEGIN()

    // TODO

    NH_BYTE *selectedSource_p = NULL;
    int selectedPixelDensity = -1;

    nh_dom_Attr *Attr_p = nh_dom_getAttrByLocalName(
        nh_dom_getElement((nh_webidl_Object*)Image_p), "src"
    );

    nh_webidl_DOMString *Value_p = nh_dom_getAttrValue(Attr_p);

    selectedSource_p = Value_p->p;
    selectedPixelDensity = 1;

    NH_BYTE *lastSelectedSource_p = selectedSource_p;

//    if (selectedSource_p) 
//    {
//        nh_url_URL *URL_p = nh_html_parseURL();
//    printf("%s\n", selectedSource_p);
//    puts("updateImageData");
//    exit(0);
//    }

NH_HTML_SILENT_END()
}

// API =============================================================================================

nh_html_HTMLImageElement *nh_html_getHTMLImageElement(
    nh_webidl_Object *Object_p)
{
NH_HTML_BEGIN()
NH_HTML_END((nh_html_HTMLImageElement*)nh_webidl_getObject(Object_p, "HTML", "HTMLImageElement"))
}

