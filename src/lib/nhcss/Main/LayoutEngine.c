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
#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"
#include "../../nhcore/Util/List.h"
#include "../../nhcore/Util/Array.h"

#include "../../nhdom/Interfaces/Element.h"
#include "../../nhdom/Interfaces/Node.h"

#include "../../nhhtml/Interfaces/Document.h"
#include "../../nhgfx/Base/Viewport.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
// INIT ============================================================================================

static void *nh_css_initLayoutEngine(
    nh_core_Workload *Workload_p)
{
NH_CSS_BEGIN()

    static NH_BYTE *name_p = "Layout Engine";
    static NH_BYTE *path_p = "nhcss/Main/LayoutEngine.c";
    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_CSS;

    nh_css_LayoutEngine *LayoutEngine_p = nh_core_allocate(sizeof(nh_css_LayoutEngine));
    NH_CSS_CHECK_MEM_2(NULL, LayoutEngine_p)

    LayoutEngine_p->DocumentContext_p = Workload_p->args_p;
    LayoutEngine_p->CanvasTypes = nh_core_initArray(sizeof(nh_css_CanvasType), 2);
    LayoutEngine_p->Layout_p = NULL;

NH_CSS_END(LayoutEngine_p)
}

// RUN =============================================================================================

static nh_css_Canvas *nh_css_getCanvas(
    nh_css_Layout *Layout_p, nh_css_CanvasType *Type_p)
{
NH_CSS_BEGIN()

    nh_css_Canvas *Canvas_p = NULL;
    for (int i = 0; Layout_p->Canvases.size; ++i) {
        Canvas_p = Layout_p->Canvases.pp[i];
        if (Canvas_p->Type.Size.height == Type_p->Size.height && Canvas_p->Type.Size.width == Type_p->Size.width) {
            NH_CSS_END(Canvas_p)
        }
        Canvas_p = NULL;
    }

NH_CSS_END(Canvas_p)
}

static nh_webidl_Object *nh_css_getHTMLElement(
    nh_webidl_Object *DocumentObject_p)
{
NH_CSS_BEGIN()

    nh_List *Children_p = nh_dom_getNodeChildren(nh_dom_getNode(DocumentObject_p));

    for (int i = 0; i < Children_p->size; ++i) {
        nh_webidl_Object *Child_p = Children_p->pp[i];
        if (!strcmp(Child_p->Interface_p->name_p, "HTMLHtmlElement")) {
            NH_CSS_END(Child_p)
        }
    }

NH_CSS_END(NULL)
}

static NH_CSS_RESULT nh_css_initializeLayout(
    nh_css_Layout *Layout_p)
{
NH_CSS_BEGIN()

    Layout_p->Canvases = nh_core_initList(8);
    Layout_p->initialized = NH_TRUE;

NH_CSS_END(NH_CSS_SUCCESS)
}

static NH_CSS_RESULT nh_css_updateLayout(
    nh_css_LayoutEngine *LayoutEngine_p, NH_BOOL *idle_p)
{
NH_CSS_BEGIN()

    nh_webidl_Object *DocumentObject_p = LayoutEngine_p->DocumentContext_p->Document_p;
    if (!DocumentObject_p) {NH_CSS_END(NH_CSS_SUCCESS)}

    nh_css_DocumentObject *Document_p = nh_css_getDocument(DocumentObject_p);
    NH_CSS_CHECK_MEM(Document_p)

    nh_css_Layout *Layout_p = nh_css_getLayout(Document_p);

    if (!Layout_p) {NH_CSS_END(NH_CSS_SUCCESS)}
    if (!Layout_p->initialized) {
//        NH_CSS_CHECK(nh_css_initializeDocument(Document_p))
        NH_CSS_CHECK(nh_css_initializeLayout(Layout_p))
        *idle_p = NH_FALSE;
    }

    for (int j = 0; j < LayoutEngine_p->CanvasTypes.length; ++j) 
    {
        nh_css_CanvasType *Type_p = &((nh_css_CanvasType*)LayoutEngine_p->CanvasTypes.p)[j];
        nh_css_Canvas *Canvas_p = nh_css_getCanvas(Layout_p, Type_p);

        if (!Canvas_p) {
            Canvas_p = nh_css_addCanvas(Layout_p, *Type_p); 
            NH_CSS_CHECK_NULL(Canvas_p)
            NH_CSS_CHECK(nh_css_computeCanvas(
                Canvas_p, nh_css_getHTMLElement(DocumentObject_p), nh_css_getStyleSheetList(Document_p)
            ))
            *idle_p = NH_FALSE;
        }
    }

    LayoutEngine_p->Layout_p = Layout_p;

NH_CSS_END(NH_CSS_SUCCESS)
}

static NH_SIGNAL nh_css_runLayoutEngine(
    void *args_p) 
{
NH_CSS_BEGIN()

    nh_css_LayoutEngine *LayoutEngine_p = args_p;
    NH_CSS_CHECK_NULL_2(NH_SIGNAL_ERROR, LayoutEngine_p)

    NH_BOOL idle = NH_TRUE;
    NH_CSS_CHECK_2(NH_SIGNAL_ERROR, nh_css_updateLayout(LayoutEngine_p, &idle))

NH_CSS_END(idle ? NH_SIGNAL_IDLE : NH_SIGNAL_OK)
}

// CREATE ==========================================================================================

nh_css_LayoutEngine *nh_css_createLayoutEngine(
    nh_html_DocumentContext *DocumentContext_p) 
{
NH_CSS_BEGIN()

    if (DocumentContext_p->LayoutEngine_p) {NH_CSS_END(NULL)}

    nh_css_LayoutEngine *LayoutEngine_p = 
        nh_core_activateWorkload(nh_css_initLayoutEngine, nh_css_runLayoutEngine, NULL, NULL,  DocumentContext_p, NH_TRUE);

    DocumentContext_p->LayoutEngine_p = LayoutEngine_p;

NH_CSS_END(LayoutEngine_p)
}

