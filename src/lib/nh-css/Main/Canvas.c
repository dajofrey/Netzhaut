// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Canvas.h"

#include "../Layout/Inline.h"
#include "../Layout/Block.h"
#include "../Layout/Depth.h"

#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/Array.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-html/Interfaces/Document.h"
#include "../../nh-gfx/Base/Viewport.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
// CREATE ==========================================================================================

static nh_css_Canvas *nh_css_createCanvas(
    nh_api_CanvasType Type)
{
    nh_css_Canvas *Canvas_p = nh_core_allocate(sizeof(nh_css_Canvas));
    NH_CORE_CHECK_MEM_2(NULL, Canvas_p)

    Canvas_p->render = false;
    Canvas_p->FragmentTree.Root_p = NULL;
    Canvas_p->Type = Type;

    return Canvas_p;
}

nh_css_Canvas *nh_css_addCanvas(
    nh_css_Layout *Layout_p, nh_api_CanvasType Type)
{
    for (int i = 0; i < Layout_p->Canvases.size; ++i) {
        nh_css_Canvas *Canvas_p = Layout_p->Canvases.pp[i];
        if (Canvas_p->Type.Size.width == Type.Size.width && Canvas_p->Type.Size.height == Type.Size.height) {
            return NULL;
        }
    }

    nh_css_Canvas *Canvas_p = nh_css_createCanvas(Type);
    NH_CORE_CHECK_NULL_2(NULL, Canvas_p)

    nh_core_appendToList(&Layout_p->Canvases, Canvas_p);

    return Canvas_p;
}

// COMPUTE =========================================================================================

NH_API_RESULT nh_css_computeCanvas(
    nh_css_Canvas *Canvas_p, nh_webidl_Object *HTMLElement_p, nh_webidl_Object *StyleSheets_p)
{
    Canvas_p->_float = false;
    Canvas_p->Floats = nh_core_initList(8);
    Canvas_p->LineBoxes = nh_core_initArray(sizeof(nh_css_LineBox), 8);

    NH_CORE_CHECK(nh_css_createSourceTree(HTMLElement_p, StyleSheets_p, true, Canvas_p, &Canvas_p->SourceTree))
    NH_CORE_CHECK(nh_css_logSourceTree(Canvas_p, Canvas_p->SourceTree.Root_p))

    Canvas_p->BoxTree = nh_css_createBoxTree(Canvas_p->SourceTree.Root_p);
    NH_CORE_CHECK_NULL(Canvas_p->BoxTree.Root_p)
    NH_CORE_CHECK(nh_css_logBoxTree(Canvas_p, Canvas_p->BoxTree))

    Canvas_p->FragmentTree = nh_css_createFragmentTree(Canvas_p->BoxTree.Root_p, &Canvas_p->Type);
    NH_CORE_CHECK_NULL(Canvas_p->FragmentTree.Root_p)

    NH_CORE_CHECK(nh_css_arrangeBlockFormattingContext(Canvas_p, Canvas_p->FragmentTree.Root_p, NULL))
    nh_css_getFragmentDepth(Canvas_p->FragmentTree.Root_p);

    NH_CORE_CHECK(nh_css_logFragmentTree(Canvas_p, Canvas_p->FragmentTree))

    nh_core_freeList(&Canvas_p->Floats, false);
    nh_core_freeArray(&Canvas_p->LineBoxes);

    return NH_API_SUCCESS;
}

// CANVAS TYPE =====================================================================================

nh_api_CanvasType nh_css_createCanvasType(
    nh_api_PixelSize Size)
{
    nh_api_CanvasType Type;
    Type.Size = Size;
    return Type;
}

NH_API_RESULT nh_css_addCanvasType(
    nh_css_LayoutEngine *LayoutEngine_p, nh_api_CanvasType Type)
{
    for (int i = 0; i < LayoutEngine_p->CanvasTypes.length; ++i) {
        nh_api_CanvasType *Type_p = ((nh_api_CanvasType*)LayoutEngine_p->CanvasTypes.p)+i;
        if (Type_p->Size.width == Type.Size.width && Type_p->Size.height == Type.Size.height) {
            return NH_API_SUCCESS;
        }
    }
 
    nh_api_CanvasType *Type_p = nh_core_incrementArray(&LayoutEngine_p->CanvasTypes);
    NH_CORE_CHECK_MEM(Type_p)

    *Type_p = Type;

    return NH_API_SUCCESS;
}

