// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/HTMLCanvasElement.h"
#include "../Header/HTMLElement.h"
#include "../Header/CanvasRenderingContext2D.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/Memory.h"

#include <string.h>

#include NH_CUSTOM_CHECK
#include NH_JS_DEBUG_FUNCTION

// API =============================================================================================

Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_HTMLCanvasElement(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_captureStream(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_getContext(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 1 && Arguments_p[0].type == NH_JS_TYPE_STRING) 
    {
        Nh_JS_Object *Object_p = Nh_JS_getObject(Function_p, NH_JS_OBJECT_HTML_CANVAS_ELEMENT);

        if (!strcmp(Arguments_p[0].data_p, "2d") && Object_p != NULL) 
        {
            NH_JS_HTMLCanvasElement *Element_p = Object_p->data_p;
            if (Element_p->Context2D_p == NULL) {
                NH_CHECK(Nh_JS_getNULLResult(), Nh_JS_createCanvasRenderingContext2DObject(Script_p, &Element_p->Context2D_p))
            }
            NH_END(Nh_JS_getResult(NH_JS_TYPE_OBJECT, false, Element_p->Context2D_p))
        }
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_toDataURL(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_toBlob(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_transferControlToOffscreen(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

// INTERNAL ========================================================================================

#include NH_DEFAULT_CHECK
#include NH_JS_DEBUG

NH_RESULT Nh_JS_createHTMLCanvasElementObject(
    Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_HTML_Node *Node_p, Nh_JS_Object *Parent_p)
{
NH_BEGIN()

    NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_HTML_CANVAS_ELEMENT, Object_pp))

    NH_JS_HTMLCanvasElement *Canvas_p = Nh_allocate(sizeof(NH_JS_HTMLCanvasElement));
    NH_CHECK_MEM(Canvas_p)
    Canvas_p->Context2D_p = NULL;

    (*Object_pp)->data_p = Canvas_p;

    Nh_JS_Object *Element_p = Nh_JS_getObject(*Object_pp, NH_JS_OBJECT_HTML_ELEMENT);
    NH_CHECK(Nh_JS_createHTMLElementData(Script_p, Element_p, Node_p, Parent_p))
 
NH_END(NH_SUCCESS)
}

void Nh_JS_destroyHTMLCanvasElementObject(
    Nh_JS_Object *Object_p)
{
NH_BEGIN()

    NH_JS_HTMLCanvasElement *Element_p = Object_p->data_p;

    if (Element_p->Context2D_p != NULL) {
        Nh_JS_destroyCanvasRenderingContext2DObject(Element_p->Context2D_p);
    }

    Nh_free(Element_p->Context2D_p);
    Nh_free(Element_p);

    Nh_JS_Object *HTMLElement_p = Nh_JS_getObject(Object_p, NH_JS_OBJECT_HTML_ELEMENT);
    Nh_JS_destroyHTMLElementData(HTMLElement_p);

    Nh_JS_destroyObject(Object_p);

NH_SILENT_END()
}

