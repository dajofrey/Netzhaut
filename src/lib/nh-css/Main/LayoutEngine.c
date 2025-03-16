// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "LayoutEngine.h"
#include "SourceTree.h"
#include "BoxTree.h"
#include "FragmentTree.h"
#include "Canvas.h"

#include "../Properties/Properties.h"
#include "../Properties/Values.h"
#include "../Properties/Compute.h"

#include "../Interfaces/Document.h"
#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/System/Thread.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/Array.h"

#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/Node.h"

#include "../../nh-html/Interfaces/Document.h"
#include "../../nh-gfx/Base/Viewport.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
// FUNCTIONS =======================================================================================

static void *nh_css_initLayoutEngine(
    nh_core_Workload *Workload_p)
{
    static char *name_p = "Layout Engine";
    static char *path_p = "nh-css/Main/LayoutEngine.c";
    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_CSS;

    nh_css_LayoutEngine *LayoutEngine_p = nh_core_allocate(sizeof(nh_css_LayoutEngine));
    NH_CORE_CHECK_MEM_2(NULL, LayoutEngine_p)

    LayoutEngine_p->DocumentContext_p = Workload_p->args_p;
    LayoutEngine_p->CanvasTypes = nh_core_initArray(sizeof(nh_api_CanvasType), 2);
    LayoutEngine_p->Layout_p = NULL;

    return LayoutEngine_p;
}

static nh_css_Canvas *nh_css_getCanvas(
    nh_css_Layout *Layout_p, nh_api_CanvasType *Type_p)
{
    nh_css_Canvas *Canvas_p = NULL;
    for (int i = 0; Layout_p->Canvases.size; ++i) {
        Canvas_p = Layout_p->Canvases.pp[i];
        if (Canvas_p->Type.Size.height == Type_p->Size.height && Canvas_p->Type.Size.width == Type_p->Size.width) {
            return Canvas_p;
        }
        Canvas_p = NULL;
    }

    return Canvas_p;
}

static nh_webidl_Object *nh_css_getHTMLElement(
    nh_webidl_Object *DocumentObject_p)
{
    nh_core_List *Children_p = nh_dom_getNodeChildren(NH_WEBIDL_GET_DOM_NODE(DocumentObject_p));

    for (int i = 0; i < Children_p->size; ++i) {
        nh_webidl_Object *Child_p = Children_p->pp[i];
        if (!strcmp(Child_p->Interface_p->name_p, "HTMLHtmlElement")) {
            return Child_p;
        }
    }

    return NULL;
}

static NH_API_RESULT nh_css_initializeLayout(
    nh_css_Layout *Layout_p)
{
    Layout_p->Canvases = nh_core_initList(8);
    Layout_p->initialized = true;

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_updateLayout(
    nh_css_LayoutEngine *LayoutEngine_p, bool *idle_p)
{
    nh_webidl_Object *DocumentObject_p = LayoutEngine_p->DocumentContext_p->Document_p;
    if (!DocumentObject_p) {return NH_API_SUCCESS;}

    nh_webidl_Object *Document_p = nh_css_getDocument(DocumentObject_p);
    NH_CORE_CHECK_MEM(Document_p)

    nh_css_Layout *Layout_p = nh_css_getLayout(Document_p);

    if (!Layout_p) {return NH_API_SUCCESS;}
    if (!Layout_p->initialized) {
//        NH_CORE_CHECK(nh_css_initializeDocument(Document_p))
        NH_CORE_CHECK(nh_css_initializeLayout(Layout_p))
        *idle_p = false;
    }

    for (int j = 0; j < LayoutEngine_p->CanvasTypes.length; ++j) 
    {
        nh_api_CanvasType *Type_p = &((nh_api_CanvasType*)LayoutEngine_p->CanvasTypes.p)[j];
        nh_css_Canvas *Canvas_p = nh_css_getCanvas(Layout_p, Type_p);

        if (!Canvas_p) {
            Canvas_p = nh_css_addCanvas(Layout_p, *Type_p); 
            NH_CORE_CHECK_NULL(Canvas_p)
            NH_CORE_CHECK(nh_css_computeCanvas(
                Canvas_p, nh_css_getHTMLElement(DocumentObject_p), nh_css_getStyleSheetList(Document_p)
            ))
            *idle_p = false;
        }
    }

    LayoutEngine_p->Layout_p = Layout_p;

    return NH_API_SUCCESS;
}

static NH_SIGNAL nh_css_runLayoutEngine(
    void *args_p) 
{
    nh_css_LayoutEngine *LayoutEngine_p = args_p;
    NH_CORE_CHECK_NULL_2(NH_SIGNAL_ERROR, LayoutEngine_p)

    bool idle = true;
    NH_CORE_CHECK_2(NH_SIGNAL_ERROR, nh_css_updateLayout(LayoutEngine_p, &idle))

    return idle ? NH_SIGNAL_IDLE : NH_SIGNAL_OK;
}

nh_css_LayoutEngine *nh_css_createLayoutEngine(
    nh_html_DocumentContext *DocumentContext_p) 
{
    if (DocumentContext_p->LayoutEngine_p) {return NULL;}

    nh_css_LayoutEngine *LayoutEngine_p = 
        nh_core_activateWorkload(nh_css_initLayoutEngine, nh_css_runLayoutEngine, NULL, NULL,  DocumentContext_p, true);

    DocumentContext_p->LayoutEngine_p = LayoutEngine_p;

    return LayoutEngine_p;
}
